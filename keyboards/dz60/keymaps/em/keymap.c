#include QMK_KEYBOARD_H
#include "tmk_core/common/wait.h"

enum custom_keycodes {
    CYCLE_RGB = SAFE_RANGE,  // cycle through the pride flags
    LIG_DEC,                 // decrease luminance (value) of the LEDs
    LIG_INC,                 // increase luminance (value) of the LEDs
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    LAYOUT_60_ansi(
        KC_GRV, KC_1,     KC_2,    KC_3,      KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,           KC_BSPC,
        KC_TAB,           KC_Q,    KC_W,      KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        LT(1, KC_ESC),    KC_A,    KC_S,      KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,      KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,
        KC_LCTL, KC_LGUI,          KC_LALT,                     KC_SPC,                             KC_RALT, KC_RGUI,          MO(1),   KC_RCTL),

    LAYOUT_60_ansi(
        KC_TRNS, KC_F1,   KC_F2,   KC_F3,     KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_DEL,
        KC_TRNS,          KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RESET,
        KC_TRNS,          KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,          LIG_DEC, CYCLE_RGB, LIG_INC, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
        KC_TRNS, KC_TRNS,          KC_TRNS,                   KC_TRNS,                            KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS)
};

typedef union {
    uint32_t raw;
    struct {
        uint8_t colour_pattern_idx;
        uint8_t val;
    };
} user_config_t;

user_config_t user_config;

/**
 * Back glow LED geometry:
 *  USB
 *  0  1  2  3  4  5  6  7
 *  15 14 13 12 11 10 9  8
 */

/**
 * Set hue to -1 to turn off the LED
 */
typedef struct {
    int16_t hue;
    uint8_t sat;
} Colour;

#define NUM_COLOURS 16

#define OFF {-1, 0}
// Convert colours from degrees and 0-100 sat as is used in most image editors
// to byte distance around a circle and 0-255 sat
#define CLR(hue, sat) {(uint8_t) (hue / 360.0 * 255.0), (uint8_t) (sat / 100.0 * 255.0)}

// discard the last element in a three element list
// If desired you can use stuff like HSV_RED by CLR2(HSV_RED)
#define CLR2_INNER(hue, sat, _) {hue, sat}
#define CLR2(params) CLR2_INNER(params)


// Create a flag which is the same on both rows of LEDs. Note that the second
// row needs to be in reverse order.
#define FLAG_BOTHROWS(i0, i1, i2, i3, i4, i5, i6, i7) {i0, i1, i2, i3, i4, i5, i6, i7, i7, i6, i5, i4, i3, i2, i1, i0}

Colour colour_patterns[][NUM_COLOURS] = {
    FLAG_BOTHROWS(OFF, CLR(197, 100), CLR(348, 100), CLR(0, 0), CLR(0, 0), CLR(348, 100), CLR(197, 100), OFF),      /* trans flag */

    FLAG_BOTHROWS(CLR(57, 100), CLR(57, 100), CLR(0, 0), CLR(0, 0), CLR(273, 100), CLR(273, 100), CLR(244, 100), CLR(244, 100)),  /* nonbinary flag */

    FLAG_BOTHROWS(OFF, CLR(267, 100), CLR(267, 100), CLR(0, 0), CLR(0, 0), CLR(85, 100), CLR(85, 100), OFF),        /* queer flag */

    FLAG_BOTHROWS(OFF, CLR(13, 100), CLR(24, 100), CLR(0, 0), CLR(0, 0), CLR(325, 100), CLR(324, 100), OFF),        /* lesbian flag */
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
    set_colour_pattern(colour_patterns[user_config.colour_pattern_idx]);
}

void save_user_config(void) {
    eeconfig_update_user(user_config.raw);
}

void cycle_colour_pattern(void) {
    uint8_t const num_patterns = sizeof (colour_patterns) / sizeof (Colour [NUM_COLOURS]);
    user_config.colour_pattern_idx = (user_config.colour_pattern_idx + 1) % num_patterns;
    refresh_colour_pattern();
}

#define VAL_STEP 10

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    switch (keycode) {
        case CYCLE_RGB:
            cycle_colour_pattern();
            save_user_config();
            break;
        case LIG_DEC:
            decrease_val();
            refresh_colour_pattern();
            save_user_config();
            break;
        case LIG_INC:
            increase_val();
            refresh_colour_pattern();
            save_user_config();
            break;
    }
    return true;
}

