// hack to make ccls work
#ifdef __clang__
#define __ATTR_PROGMEM__
#endif

#include QMK_KEYBOARD_H


#define _QWERTY 0
#define _SYM 1
#define _FN 2

enum custom_keycodes {
    CYCLE_RGB = SAFE_RANGE,
    LIG_INC,
    LIG_DEC,
    NORMMODE,
    WINCMD,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
    LT(_FN,KC_ESC),KC_A,KC_S,   KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LALT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LGUI,          LT(1,KC_ESC),KC_N,KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_EQL,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_BSPC, KC_LCTL, KC_LSFT,                   KC_SPC, OSL(_SYM),KC_ENT
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_SYM] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            KC_CIRC, KC_AMPR, KC_ASTR, XXXXXXX, XXXXXXX, KC_UNDS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, KC_LCBR, KC_RCBR, XXXXXXX, XXXXXXX, KC_PIPE,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            KC_LBRC, KC_LPRN, KC_RPRN, KC_RBRC, KC_COLN, KC_DQUO,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,          XXXXXXX, WINCMD,  NORMMODE,KC_LT,   KC_GT,   KC_QUES, KC_PLUS,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    XXXXXXX, _______, _______,                   _______, KC_ESC,  _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_FN] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, XXXXXXX, XXXXXXX, KC_END,  XXXXXXX, RESET,                              RESET,   XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR, KC_F12,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, KC_HOME, XXXXXXX, XXXXXXX, KC_PGDN, XXXXXXX,                            KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, LIG_DEC, CYCLE_RGB,LIG_INC,XXXXXXX, KC_PGUP, _______,          XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_DEL,  XXXXXXX, _______,                   _______, XXXXXXX, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),
};

#if 0

  [_FN] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),
#endif

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

#define NUM_COLOURS 12

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
#define FLAG_BOTHROWS(i0, i1, i2, i3, i4, i5) {i2, i1, i0, i5, i4, i3, i5, i4, i3, i2, i1, i0}

Colour colour_patterns[][NUM_COLOURS] = {
    FLAG_BOTHROWS(CLR(197, 100), CLR(348, 100), CLR(0, 0), CLR(0, 0), CLR(348, 100), CLR(197, 100)),      /* trans flag */
    FLAG_BOTHROWS(CLR(57, 100), CLR(0, 0), CLR(0, 0), CLR(273, 100), CLR(273, 100), CLR(244, 100)),  /* nonbinary flag */

    FLAG_BOTHROWS(CLR(267, 100), CLR(267, 100), CLR(0, 0), CLR(0, 0), CLR(85, 100), CLR(85, 100)),        /* queer flag */

    FLAG_BOTHROWS(CLR(13, 100), CLR(24, 86), CLR(0, 0), CLR(0, 0), CLR(325, 74), CLR(324, 100)),        /* lesbian flag */
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

// On wake from suspend, refresh the RGB state
void suspend_wakeup_init_user(void) {
    keyboard_post_init_user();
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
        case NORMMODE:
            SEND_STRING(SS_LCTL("\\n"));
            break;
        case WINCMD:
            SEND_STRING(SS_LCTL("\\nw"));
            break;
    }
    return true;
}

