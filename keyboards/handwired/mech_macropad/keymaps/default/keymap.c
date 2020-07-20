/* Copyright 2019 lf <software@lfcode.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "tmk_core/common/timer.h"

enum my_layers { _NUMPAD, _MACROS, _SUDOKU };

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes { AUTOCLICKER = SAFE_RANGE, QMKURL };
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
        TO(_MACROS),	BRIGHTUP,	RGB_VAI,	KC_NO,
        SCREENOFF,	BRIGHTDN,	RGB_VAD,	AUTOCLICKER,

        KC_NUMLOCK,	KC_KP_SLASH,	KC_KP_ASTERISK,	KC_KP_MINUS,
        KC_KP_7,	KC_KP_8,	KC_KP_9,	KC_KP_PLUS,
        KC_KP_4,	KC_KP_5,	KC_KP_6,	KC_KP_PLUS,
        KC_KP_1,	KC_KP_2,	KC_KP_3,	KC_KP_ENTER,
        KC_KP_0,	KC_KP_0,	KC_KP_DOT,	KC_KP_ENTER
    ),
    [_MACROS] = LAYOUT(
        TO(_SUDOKU),	BRIGHTUP,	RGB_VAI,	RESET,
        SCREENOFF,	BRIGHTDN,		RGB_VAD,	AUTOCLICKER,

        MIC_MUTE,	KC_NO,		KC_NO,		KC_NO,
        OBS1,		OBS2,		OBS3,		OBS4,
        KC_NO,		KC_NO,		KC_NO,		NOTIFY_CURR_TRACK,
        KC_VOLD,	KC_MUTE,	KC_VOLU,	LIKE_CURR_TRACK,
        KC_MPRV,	KC_MPLY,	KC_MNXT,	DISLIKE_CURR_TRACK
    ),
    [_SUDOKU] = LAYOUT(
        TO(_NUMPAD),	BRIGHTUP,	RGB_VAI,	KC_NO,
        SCREENOFF,	BRIGHTDN,	RGB_VAD,	AUTOCLICKER,

        KC_LEFT,	KC_DOWN,	KC_UP,		KC_RIGHT,
        KC_7,		KC_8,		KC_9,		KC_SPACE,
        KC_4,		KC_5,		KC_6,		KC_DEL,
        KC_1,		KC_2,		KC_3,	 	KC_ESC,
        OSM(MOD_LSFT),	OSM(MOD_LCTL),	LCTL(KC_Z),	LCTL(KC_Y)
    ),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case AUTOCLICKER:
      if (record->event.pressed) {
        // on press
        autoclicker_on = !autoclicker_on;
	layer_state_set_user(layer_state);
      } else {
        // on release
      }
      break;
    case QMKURL:
      if (record->event.pressed) {
        // when keycode QMKURL is pressed
        SEND_STRING("https://qmk.fm/" SS_TAP(X_ENTER));
      } else {
        // when keycode QMKURL is released
      }
      break;
  }
  return true;
}

void matrix_init_user(void) {}

void rgb_seths(uint16_t hue, uint8_t sat) { rgblight_sethsv_noeeprom(hue, sat, rgblight_get_val()); }

void keyboard_post_init_user(void) {
  rgblight_init();
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
  layer_state_set_user(layer_state);
  debug_enable = true;
}

void matrix_scan_user(void) {
  static uint16_t repeat_click = 0;
  static uint16_t delay_click = 0;
  if (!autoclicker_on) {
    repeat_click = delay_click = 0;
    return;
  }
  
  if (delay_click && timer_elapsed(delay_click) > 16) {
    unregister_code(KC_MS_BTN1);
    delay_click = 0;
  } else if (timer_elapsed(repeat_click) > 50) {
    register_code(KC_MS_BTN1);
    delay_click = repeat_click = timer_read();
  }
}

void led_set_user(uint8_t usb_led) {}

uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case _NUMPAD:
      rgb_seths(53, 255);
      break;
    case _MACROS:
      rgb_seths(0, 255);
      break;
    case _SUDOKU:
      rgb_seths(224, 63);
      break;
    default:
      rgb_seths(0, 0);
      break;
  }
  if (autoclicker_on) {
    rgblight_setrgb_at(255, 255, 255, 4);
  } else {
    rgb_seths(rgblight_get_hue(), rgblight_get_sat());
  }
  return state;
}
