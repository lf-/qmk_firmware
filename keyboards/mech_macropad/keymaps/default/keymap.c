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

enum my_layers { _NUMPAD, _MACROS };

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes { QMKBEST = SAFE_RANGE, QMKURL };

#define NOTIFY_CURR_TRACK LCTL(KC_F23)
#define LIKE_CURR_TRACK LCTL(KC_F24)
#define DISLIKE_CURR_TRACK LCTL(KC_F22)

#define MIC_MUTE LCTL(KC_F17)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_NUMPAD] = LAYOUT(
        TG(_MACROS),	QMKURL,	KC_NO,	KC_NO,
        KC_NO,	KC_NO,	KC_NO,	KC_NO,

        KC_NUMLOCK,	KC_KP_SLASH,	KC_KP_ASTERISK,	KC_KP_MINUS,
        KC_KP_7,	KC_KP_8,	KC_KP_9,	KC_KP_PLUS,
        KC_KP_4,	KC_KP_5,	KC_KP_6,	KC_KP_PLUS,
        KC_KP_1,	KC_KP_2,	KC_KP_3,	KC_KP_ENTER,
        KC_KP_0,	KC_KP_0,	KC_KP_DOT,	KC_KP_ENTER
    ),
    [_MACROS] = LAYOUT(
        KC_TRNS,	QMKBEST,	RGB_VAI,	RESET,
        KC_TRNS,	KC_TRNS,	RGB_VAD,	KC_TRNS,

        MIC_MUTE,	KC_NO,	KC_NO,	KC_NO,
        KC_NO,	KC_NO,	KC_NO,	KC_NO,
        KC_NO,	KC_NO,	KC_NO,	NOTIFY_CURR_TRACK,
        KC_VOLD,	KC_MUTE,	KC_VOLU,	LIKE_CURR_TRACK,
        KC_MPRV,	KC_MPLY,	KC_MNXT,	DISLIKE_CURR_TRACK
    ),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QMKBEST:
      if (record->event.pressed) {
        // when keycode QMKBEST is pressed
        SEND_STRING("QMK is the best thing ever!");
      } else {
        // when keycode QMKBEST is released
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

void matrix_scan_user(void) {}

void led_set_user(uint8_t usb_led) {}

uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case _NUMPAD:
      rgb_seths(53, 255);
      break;
    case _MACROS:
      rgb_seths(0, 255);
      break;
    default:
      rgb_seths(0, 0);
      break;
  }
  return state;
}
