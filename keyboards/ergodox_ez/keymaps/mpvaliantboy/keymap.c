#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"

#define _______ KC_TRNS

#define BASE 0 // default layer
#define FN 1
#define LOL 2
#define TF 3

// if you want a pause between initial key registration and spam
#define SPAM_INITIAL 6
#define SPAM_INTERVAL 6

#define SPM(original_keycode) (original_keycode + SAFER_RANGE)
#define GET_ORIGINAL_KEYCODE(spam_keycode) (spam_keycode - SAFER_RANGE)

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here

  SAFER_RANGE
};

static uint8_t counter[26];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `~   |  1!  |  2@  |  3#  |  4$  |  5%  | Mute |           |  F1  |  6^  |  7&  |  8*  |  9(  |  0)  |   -_   |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |   L1   |  Q   |  W   |  E   |  R   |  T   | VolD |           |  F4  |  Y   |  U   |  I   |  O   |  P   |   \+   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |  Caps  |  A   |  S   |  D   |  F   |  G   |------|           |------|  H   |  J   |  K   |  L   |  ;:  |   '"   |
 * |--------+------+------+------+------+------| VolU |           |  F6  |------+------+------+------+------+--------|
 * |  LSft  |  Z   |  X   |  C   |  V   |  B   |      |           |      |  N   |  M   |  ,<  |  .>  |  /?  |  RSft  |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | LCtl | LAlt | LGui | Left | Rght |                                       |  Up  | Down |  [{  |  ]}  |  \|  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | Esc  | PScr |       | Ins  | Del  |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       | PgUp |      |      |
 *                                 | Spc  | Tab  |------|       |------| BkSp | Ent  |
 *                                 |      |      | End  |       | PgDn |      |      |
 *                                 `--------------------'       `--------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
  [BASE] = LAYOUT_ergodox(  // layer 0 : default
    // left hand
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_MUTE,
    TG(FN),   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_VOLD,
    KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_VOLU,
    KC_LCTL,  KC_LALT,  KC_LGUI,  KC_LEFT,  KC_RGHT,

                                                      KC_ESC,   KC_PSCR,
                                                                KC_HOME,
                                            KC_SPC,   KC_TAB,   KC_END,
    // right hand
    KC_F4,    KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,
    TG(TF),   KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_EQL,
              KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
    TG(LOL),  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,
                        KC_UP,    KC_DOWN,  KC_LBRC,  KC_RBRC,  KC_BSLS,

    KC_INS,   KC_DEL,
    KC_PGUP,
    KC_PGDN,  KC_BSPC,  KC_ENT
  ),
  [FN] = LAYOUT_ergodox(
    // left hand
    _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,

                                                      _______,  _______,
                                                                _______,
                                            _______,  _______,  _______,
    // right hand
    RESET,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
    _______,  _______,  _______,  _______,  _______,  _______,  KC_F12,
              _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
                        _______,  _______,  _______,  _______,  _______,

    _______,  _______,
    _______,
    _______,  _______,  _______
  ),
  [LOL] = LAYOUT_ergodox(
    // left hand
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  SPM(KC_Q),SPM(KC_W),SPM(KC_E),SPM(KC_R),SPM(KC_T),_______,
    _______,  _______,  SPM(KC_S),SPM(KC_D),SPM(KC_F),SPM(KC_G),
    _______,  SPM(KC_Z),SPM(KC_X),SPM(KC_C),SPM(KC_V),_______,  _______,
    _______,  _______,  _______,  _______,  _______,

                                                      _______,  _______,
                                                                _______,
                                            _______,  _______,  _______,
    // right hand
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
              _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
                        _______,  _______,  _______,  _______,  _______,

    _______,  _______,
    _______,
    _______,  _______,  _______
  ),
  [TF] = LAYOUT_ergodox(
    // left hand
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  KC_D,     _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,

                                                      _______,  _______,
                                                                _______,
                                            _______,  _______,  _______,
    // right hand
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
              _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,
                        _______,  _______,  _______,  _______,  _______,

    _______,  _______,
    _______,
    _______,  _______,  _______
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (keycode >= SAFER_RANGE) {
    uint8_t original_keycode = GET_ORIGINAL_KEYCODE(keycode);
    uint8_t offset = original_keycode - KC_A;

    if (record->event.pressed) {
      register_code(original_keycode);
      if (has_anymod()) {
        return true;
      } else {
        unregister_code(original_keycode);
        counter[offset] = 1;
      }
    } else {
      counter[offset] = 0;
      unregister_code(original_keycode);
    }
    return false;
  }
  return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
};

void check_spam(void) {
  for (int offset = 0; offset < 26; offset++) {
    if (counter[offset] && counter[offset]++ >= SPAM_INITIAL) {
      counter[offset] = SPAM_INITIAL - SPAM_INTERVAL + 1;
      uint8_t original_keycode = offset + KC_A;

      register_code(original_keycode);
      unregister_code(original_keycode);
    }
  }
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  check_spam();
};

// Runs whenever there is a layer state change.
uint32_t layer_state_set_user(uint32_t state) {
  ergodox_led_all_off();

  if (layer_state_cmp(state, FN)) {
    ergodox_right_led_1_on();
  }
  if (layer_state_cmp(state, LOL)) {
    ergodox_right_led_2_on();
  }
  if (layer_state_cmp(state, TF)) {
    ergodox_right_led_3_on();
  }

  return state;
};
