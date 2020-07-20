/* Copyright 2019 lf <software@lfcode.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.    If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "tmk_core/common/timer.h"

enum my_layers { _NUMPAD, _MACROS, _SUDOKU };

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes { AUTOCLICKER = SAFE_RANGE, RGB_BRIGHTER, RGB_DIMMER };
static bool autoclicker_on = false;

#define NOTIFY_CURR_TRACK LCTL(KC_F23)
#define LIKE_CURR_TRACK LCTL(KC_F24)
#define DISLIKE_CURR_TRACK LCTL(KC_F22)

#define MIC_MUTE LCTL(KC_F17)
#define OBS1 LCTL(KC_F13)
#define OBS2 LCTL(KC_F14)
#define OBS3 LCTL(KC_F15)
#define OBS4 LCTL(KC_F16)
#define SCREENOFF LWIN(KC_F24)
#define BRIGHTUP LCTL(LGUI(KC_F24))
#define BRIGHTDN LCTL(LSFT(LGUI(KC_F24)))

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_NUMPAD] = LAYOUT(
		TO(_MACROS),	BRIGHTUP,	RGB_BRIGHTER,	KC_NO,
		SCREENOFF,	BRIGHTDN,	RGB_DIMMER,	AUTOCLICKER,

		KC_NUMLOCK,	KC_KP_SLASH,	KC_KP_ASTERISK,	KC_KP_MINUS,
		KC_KP_7,	KC_KP_8,	KC_KP_9,	KC_KP_PLUS,
		KC_KP_4,	KC_KP_5,	KC_KP_6,	KC_KP_PLUS,
		KC_KP_1,	KC_KP_2,	KC_KP_3,	KC_KP_ENTER,
		KC_KP_0,	KC_KP_0,	KC_KP_DOT,	KC_KP_ENTER
	),
	[_MACROS] = LAYOUT(
		TO(_SUDOKU),	BRIGHTUP,	RGB_BRIGHTER,	RESET,
		SCREENOFF,	BRIGHTDN,	RGB_DIMMER,	AUTOCLICKER,

		MIC_MUTE,	KC_NO,		KC_NO,		KC_NO,
		OBS1,		OBS2,		OBS3,		OBS4,
		KC_NO,		KC_NO,		KC_NO,		NOTIFY_CURR_TRACK,
		KC_VOLD,	KC_MUTE,	KC_VOLU,	LIKE_CURR_TRACK,
		KC_MPRV,	KC_MPLY,	KC_MNXT,	DISLIKE_CURR_TRACK
	),
	[_SUDOKU] = LAYOUT(
		TO(_NUMPAD),	BRIGHTUP,	RGB_BRIGHTER,	KC_NO,
		SCREENOFF,	BRIGHTDN,	RGB_DIMMER,	AUTOCLICKER,

		KC_LEFT,	KC_DOWN,	KC_UP,		KC_RIGHT,
		KC_7,		KC_8,		KC_9,		KC_SPACE,
		KC_4,		KC_5,		KC_6,		KC_DEL,
		KC_1,		KC_2,		KC_3,		KC_ESC,
		OSM(MOD_LSFT),	OSM(MOD_LCTL),	LCTL(KC_Z),	LCTL(KC_Y)
	),
};
// clang-format on

void led_set_user(uint8_t usb_led) {}

/**
 * Set hue to -1 to turn off the LED
 */
typedef struct {
    int16_t hue;
    uint8_t sat;
} Colour;

typedef union {
    uint32_t raw;
    struct {
        uint8_t val;
    };
} user_config_t;

user_config_t user_config;

#define NUM_COLOURS 5

#define VAL_STEP 10
#define OFF {-1, 0}
// Convert colours from degrees and 0-100 sat as is used in most image editors
// to byte distance around a circle and 0-255 sat
#define CLR(hue, sat) {(uint8_t) (hue / 360.0 * 255.0), (uint8_t) (sat / 100.0 * 255.0)}

Colour colour_patterns[][NUM_COLOURS] = {
    {CLR(197, 100), CLR(348, 100), CLR(0, 0), CLR(348, 100), CLR(197, 100)},    /* trans flag */
    {CLR(57, 100), CLR(0, 0), CLR(273, 100), CLR(244, 100), OFF},         /* nonbinary flag */
    {CLR(267, 100), CLR(0, 0), CLR(85, 100), OFF, OFF},             /* queer flag */
};

void set_colour_pattern(Colour pattern[NUM_COLOURS]) {
    for (uint8_t i = 0; i < NUM_COLOURS; ++i) {
        Colour col = pattern[i];
        // turn off colours with -1 hue (sentinel value)
        if (col.hue == -1) {
            setrgb(0, 0, 0, &led[i]);
            continue;
        }

        uint8_t val = user_config.val;
        sethsv(col.hue, col.sat, val, &led[i]);
    }
    rgblight_set();
}

void refresh_colour_pattern(void) {
    layer_state_set_user(layer_state);
}

void save_user_config(void) {
    eeconfig_update_user(user_config.raw);
}

void decrease_val(void) {
    uint8_t new_val = user_config.val - VAL_STEP;
    if (new_val > user_config.val) {
        // overflowed, unsigned overflow is defined
        new_val = 0;
    }
    user_config.val = new_val;
}

void increase_val(void) {
    uint8_t new_val = user_config.val + VAL_STEP;
    if (new_val < user_config.val) {
        // overflowed, unsigned overflow is defined
        new_val = 255;
    }
    user_config.val = new_val;
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    refresh_colour_pattern();
}

void matrix_scan_user(void) {
    static uint16_t repeat_click = 0;
    static uint16_t delay_click = 0;
    if (!autoclicker_on) {
        repeat_click = delay_click = 0;
        return;
    }

    if (delay_click && timer_elapsed(delay_click) > 16) {
        unregister_code(KC_MS_BTN2);
        delay_click = 0;
    } else if (timer_elapsed(repeat_click) > 50) {
        register_code(KC_MS_BTN2);
        delay_click = repeat_click = timer_read();
    }
}

uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
        case _NUMPAD:
            set_colour_pattern(colour_patterns[0]);
            //rgb_seths(53, 255);
            break;
        case _MACROS:
            set_colour_pattern(colour_patterns[1]);
            //rgb_seths(0, 255);
            break;
        case _SUDOKU:
            set_colour_pattern(colour_patterns[2]);
            //rgb_seths(224, 63);
            break;
        default:
            sethsv(0, 0, 100, &led[0]);
            break;
    }
    if (autoclicker_on) {
        setrgb(255, 255, 255, &led[4]);
    }
    rgblight_set();
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case AUTOCLICKER:
            if (record->event.pressed) {
                // on press
                autoclicker_on = !autoclicker_on;
                refresh_colour_pattern();
            } else {
                // on release
            }
            break;
        case RGB_BRIGHTER:
            if (record->event.pressed) {
                increase_val();
                refresh_colour_pattern();
                save_user_config();
            }
            break;
        case RGB_DIMMER:
            if (record->event.pressed) {
                decrease_val();
                refresh_colour_pattern();
                save_user_config();
            }
            break;
    }
    return true;
}

