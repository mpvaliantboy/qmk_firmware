#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"

// If you want a pause between initial key registration and spam
// For fast double press, my average interval between each 2 DOWN events is about 125 ms
// However, for lazy press, it usually takes about around 125 ms between DOWN and UP events :(
#define SPAM_INITIAL 12
#define SPAM_INTERVAL 12

#define ____________ KC_TRNS
#define SPAM(original_keycode) (original_keycode + SAFER_RANGE)
#define SWAP(original_keycode) (original_keycode + SAFER_RANGE)

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here

  LOL_ON,
  LOL_OFF,
  CHAT_TG,

  SAFER_RANGE
};

enum layers {
  BASE,

  LOL_SPM,
  LOL_F2,
  LOL_F3,
  LOL_F4,
  LOL_F5,
  LOL_Q,
  LOL_W,
  LOL_E,
  LOL_R,
  LOL_D,
  LOL_F,
  LOL_CHT,

  FNCTN,
};

static uint8_t counter[36];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap BASE: Basic layer
 * ,----------------------------------------------------------.       ,----------------------------------------------------------.
 * |          |       |   2@  |   3#  |   4$  |   5%  |  Mute |       |  PScr |   6^  |   7&  |   8*  |   9(  |   =+  |   ]}     |
 * |----------+-------+-------+-------+-------+---------------|       |-------+-------+-------+-------+-------+-------+----------|
 * |      `~  |   1!  |   W   |   E   |   R   |   T   |  VolU |       | FNCTN |   Y   |   U   |   I   |   O   |   0)  |   -_     |
 * |----------+-------+-------+-------+-------+-------|       |       |       |-------+-------+-------+-------+-------+----------|
 * |     LGui |   Q   |   S   |   D   |   F   |   G   |-------|       |-------|   H   |   J   |   K   |   L   |   P   |   [{     |
 * |----------+-------+-------+-------+-------+-------|  VolD |       |LOL_ON |-------+-------+-------+-------+-------+----------|
 * |     LSft |   A   |   X   |   C   |   V   |   B   |       |       |       |   N   |   M   |   ,<  |   .>  |   ;:  |   '"     |
 * `----------+-------+-------+-------+-------+---------------'       `---------------+-------+-------+-------+-------+----------'
 *    |  LCtl |   Z   |  LAlt |  Left |  Rght |                                       |   Up  |  Down |   \|  |   /?  |  Caps |
 *    `---------------------------------------'                                       `---------------------------------------'
 *                                            ,---------------.       ,---------------.
 *                                            |  Esc  |  LGui |       |  Ins  |  Del  |
 *                                    ,-------|-------|-------|       |-------+-------+-------.
 *                                    |       |       |  Home |       |  PgUp |       |       |
 *                                    |  Spc  |  Tab  |-------|       |-------|  BkSp |  Ent  |
 *                                    |       |       |  End  |       |  PgDn |       |       |
 *                                    `-----------------------'       `-----------------------'
 * Keymap BASE: Basic layer
 * ,----------------------------------------------------------.       ,----------------------------------------------------------.
 * |      `~  |   1!  |   2@  |   3#  |   4$  |   5%  |  Mute |       |  PScr |   6^  |   7&  |   8*  |   9(  |   0)  |   -_     |
 * |----------+-------+-------+-------+-------+---------------|       |-------+-------+-------+-------+-------+-------+----------|
 * |    FNCTN |   Q   |   W   |   E   |   R   |   T   |  VolU |       | FNCTN |   Y   |   U   |   I   |   O   |   P   |   =+     |
 * |----------+-------+-------+-------+-------+-------|       |       |       |-------+-------+-------+-------+-------+----------|
 * |     Caps |   A   |   S   |   D   |   F   |   G   |-------|       |-------|   H   |   J   |   K   |   L   |   ;:  |   '"     |
 * |----------+-------+-------+-------+-------+-------|  VolD |       |LOL_ON |-------+-------+-------+-------+-------+----------|
 * |     LSft |   Z   |   X   |   C   |   V   |   B   |       |       |       |   N   |   M   |   ,<  |   .>  |   /?  |  RSft    |
 * `----------+-------+-------+-------+-------+---------------'       `---------------+-------+-------+-------+-------+----------'
 *    |  LCtl |  LGui |  LAlt |  Left |  Rght |                                       |   Up  |  Down |   [{  |   ]}  |   \|  |
 *    `---------------------------------------'                                       `---------------------------------------'
 *                                            ,---------------.       ,---------------.
 *                                            |  Esc  |  LSft |       |  Ins  |  Del  |
 *                                    ,-------|-------|-------|       |-------+-------+-------.
 *                                    |       |       |  Home |       |  PgUp |       |       |
 *                                    |  Spc  |  Tab  |-------|       |-------|  BkSp |  Ent  |
 *                                    |       |       |  End  |       |  PgDn |       |       |
 *                                    `-----------------------'       `-----------------------'
 * Keymap LOL_SPM: League layer
 * ,----------------------------------------------------------.       ,----------------------------------------------------------.
 * |          |   F1  |   F2  |   F3  |   F4  |   F5  |       |       |       |SWAP(F1|SWAP(F2|SWAP(F3|SWAP(F4|SWAP(F5|          |
 * |----------+-------+-------+-------+-------+---------------|       |-------+-------+-------+-------+-------+-------+----------|
 * |          |SPAM(1)|SPAM(2)|SPAM(3)|SPAM(4)|SPAM(5)|       |       |       |       |       |       |       |       |          |
 * |----------+-------+-------+-------+-------+-------|       |       |       |-------+-------+-------+-------+-------+----------|
 * |          |   S   |SPAM(Q)|SPAM(W)|SPAM(E)|SPAM(R)|-------|       |-------|       | LOL_Q | LOL_W | LOL_E | LOL_R |          |
 * |----------+-------+-------+-------+-------+-------|       |       |LOL_OFF|-------+-------+-------+-------+-------+----------|
 * |     LAlt |SPAM(D)|SPAM(6)|SPAM(7)|SPAM(F)|   T   |       |       |       |       | LOL_F |       |       | LOL_D |          |
 * `----------+-------+-------+-------+-------+---------------'       `---------------+-------+-------+-------+-------+----------'
 *    |  LCtl |  LSft |       |   B   |   P   |                                       |       |       |       |       |       |
 *    `---------------------------------------'                                       `---------------------------------------'
 *                                            ,---------------.       ,---------------.
 *                                            |       |       |       |       |       |
 *                                    ,-------|-------|-------|       |-------+-------+-------.
 *                                    |       |       |   C   |       |       |       |       |
 *                                    |       |       |-------|       |-------|       |CHAT_TG|
 *                                    |       |       |   Z   |       |       |       |       |
 *                                    `-----------------------'       `-----------------------'
 * Keymap FNCTN: Function layer
 * ,----------------------------------------------------------.       ,----------------------------------------------------------.
 * |          |   F1  |   F2  |   F3  |   F4  |   F5  |       |       | RESET |   F6  |   F7  |   F8  |   F9  |  F10  |  F11     |
 * |----------+-------+-------+-------+-------+---------------|       |-------+-------+-------+-------+-------+-------+----------|
 * |          |       |       |   Up  |       |       |       |       |       |       |       |   Up  |       |       |  F12     |
 * |----------+-------+-------+-------+-------+-------|       |       |       |-------+-------+-------+-------+-------+----------|
 * |          |       |  Left |  Down |  Rght |       |-------|       |-------|       |  Left |  Down |  Rght |       |          |
 * |----------+-------+-------+-------+-------+-------|       |       |       |-------+-------+-------+-------+-------+----------|
 * |          |  LCtl |       |       |       |       |       |       |       |       |       |       |       |  LCtl |          |
 * `----------+-------+-------+-------+-------+---------------'       `---------------+-------+-------+-------+-------+----------'
 *    |       |       |       |       |       |                                       |       |       |       |       |       |
 *    `---------------------------------------'                                       `---------------------------------------'
 *                                            ,---------------.       ,---------------.
 *                                            |       |       |       |       |       |
 *                                    ,-------|-------|-------|       |-------+-------+-------.
 *                                    |       |       |       |       |       |       |       |
 *                                    |       |       |-------|       |-------|       |  Spc  |
 *                                    |       |       |       |       |       |       |       |
 *                                    `-----------------------'       `-----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
//  [BASE] = LAYOUT_ergodox(  // layer 0 : default
//    // left hand
//    KC_LSFT,      ____________, KC_2,         KC_3,         KC_4,         KC_5,         KC_MUTE,
//    KC_GRV,       KC_1,         KC_W,         KC_E,         KC_R,         KC_T,         KC_VOLU,
//    KC_LGUI,      KC_Q,         KC_S,         KC_D,         KC_F,         KC_G,
//    OSM(MOD_LSFT),KC_A,         KC_X,         KC_C,         KC_V,         KC_B,         KC_VOLD,
//    KC_LCTL,      KC_Z,         KC_LALT,      KC_LEFT,      KC_RGHT,
//
//                                                                          KC_ESC,       ____________,
//                                                                                        KC_HOME,
//                                                            KC_SPC,       KC_TAB,       KC_END,
//    // right hand
//    KC_PSCR,      KC_6,         KC_7,         KC_8,         KC_9,         KC_EQL,       KC_RBRC,
//    TG(FNCTN),    KC_Y,         KC_U,         KC_I,         KC_O,         KC_0,         KC_MINS,
//                  KC_H,         KC_J,         KC_K,         KC_L,         KC_P,         KC_LBRC,
//    LOL_ON,       KC_N,         KC_M,         KC_COMM,      KC_DOT,       KC_SCLN,      KC_QUOT,
//                                KC_UP,        KC_DOWN,      KC_BSLS,      KC_SLSH,      KC_CAPS,
//
//    KC_INS,       KC_DEL,
//    KC_PGUP,
//    KC_PGDN,      KC_BSPC,      KC_ENT
//  ),
  [BASE] = LAYOUT_ergodox(  // layer 0 : default
    // left hand
    KC_GRV,       KC_1,         KC_2,         KC_3,         KC_4,         KC_5,         KC_MUTE,
    TG(FNCTN),    KC_Q,         KC_W,         KC_E,         KC_R,         KC_T,         KC_VOLU,
    KC_CAPS,      KC_A,         KC_S,         KC_D,         KC_F,         KC_G,
    KC_LSFT,      KC_Z,         KC_X,         KC_C,         KC_V,         KC_B,         KC_VOLD,
    KC_LCTL,      KC_LGUI,      KC_LALT,      KC_LEFT,      KC_RGHT,

                                                                          KC_ESC,       KC_LSFT,
                                                                                        KC_HOME,
                                                            KC_SPC,       KC_TAB,       KC_END,
    // right hand
    KC_PSCR,      KC_6,         KC_7,         KC_8,         KC_9,         KC_0,         KC_MINS,
    MO(FNCTN),    KC_Y,         KC_U,         KC_I,         KC_O,         KC_P,         KC_EQL,
                  KC_H,         KC_J,         KC_K,         KC_L,         KC_SCLN,      KC_QUOT,
    LOL_ON,       KC_N,         KC_M,         KC_COMM,      KC_DOT,       KC_SLSH,      KC_RSFT,
                                KC_UP,        KC_DOWN,      KC_LBRC,      KC_RBRC,      KC_BSLS,

    KC_INS,       KC_DEL,
    KC_PGUP,
    KC_PGDN,      KC_BSPC,      KC_ENT
  ),
  [LOL_SPM] = LAYOUT_ergodox(
    // left hand
    ____________, KC_F1,        KC_F2,        KC_F3,        KC_F4,        KC_F5,        ____________,
    ____________, SPAM(KC_1),   SPAM(KC_2),   SPAM(KC_3),   SPAM(KC_4),   SPAM(KC_5),   ____________,
    ____________, KC_S,         SPAM(KC_Q),   SPAM(KC_W),   SPAM(KC_E),   SPAM(KC_R),
    KC_LALT,      SPAM(KC_D),   SPAM(KC_6),   SPAM(KC_7),   SPAM(KC_F),   KC_T,         ____________,
    ____________, KC_LSFT,      ____________, KC_B,         KC_P,

                                                                          ____________, ____________,
                                                                                        KC_C,
                                                            ____________, ____________, KC_Z,
    // right hand
    ____________, SWAP(KC_F1),  SWAP(KC_F2),  SWAP(KC_F3),  SWAP(KC_F4),  SWAP(KC_F5),  ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, TG(LOL_Q),    TG(LOL_W),    TG(LOL_E),    TG(LOL_R),    ____________,
    LOL_OFF     , ____________, TG(LOL_F),    ____________, ____________, TG(LOL_D),    ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, CHAT_TG
  ),
  [LOL_F2] = LAYOUT_ergodox(
    // left hand
    ____________, KC_F2,        KC_F1,        KC_F3,        KC_F4,        KC_F5,        ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_F3] = LAYOUT_ergodox(
    // left hand
    ____________, KC_F2,        KC_F3,        KC_F1,        KC_F4,        KC_F5,        ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_F4] = LAYOUT_ergodox(
    // left hand
    ____________, KC_F2,        KC_F3,        KC_F4,        KC_F1,        KC_F5,        ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_F5] = LAYOUT_ergodox(
    // left hand
    ____________, KC_F2,        KC_F3,        KC_F4,        KC_F5,        KC_F1,        ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_Q] = LAYOUT_ergodox(
    // left hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, KC_Q,         ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_W] = LAYOUT_ergodox(
    // left hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, KC_W,         ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_E] = LAYOUT_ergodox(
    // left hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, KC_E,         ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_R] = LAYOUT_ergodox(
    // left hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, KC_R,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_D] = LAYOUT_ergodox(
    // left hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, KC_D,         ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_F] = LAYOUT_ergodox(
    // left hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, KC_F,         ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_F] = LAYOUT_ergodox(
    // left hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, KC_F,         ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [LOL_CHT] = LAYOUT_ergodox(  // layer 0 : default
    // left hand
    ____________, KC_1,         KC_2,         KC_3,         KC_4,         KC_5,         ____________,
    ____________, KC_Q,         KC_W,         KC_E,         KC_R,         KC_T,         ____________,
    ____________, KC_A,         KC_S,         KC_D,         KC_F,         KC_G,
    KC_LSFT,      KC_Z,         KC_X,         KC_C,         KC_V,         KC_B,         ____________,
    ____________, KC_LGUI,      ____________, KC_LEFT,      KC_RGHT,

                                                                          ____________, ____________,
                                                                                        KC_HOME,
                                                            ____________, ____________, KC_END,
    // right hand
    ____________, KC_6,         KC_7,         KC_8,         KC_9,         KC_0,         ____________,
    ____________, ____________, ____________, ____________, ____________, ____________, ____________,
                  ____________, KC_J,         KC_K,         KC_L,         KC_SCLN,      ____________,
    ____________, ____________, KC_M,         ____________, ____________, KC_SLSH,      ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, ____________
  ),
  [FNCTN] = LAYOUT_ergodox(
    // left hand
    ____________, KC_F1,        KC_F2,        KC_F3,        KC_F4,        KC_F5,        ____________,
    ____________, ____________, ____________, KC_UP,        ____________, ____________, ____________,
    ____________, ____________, KC_LEFT,      KC_DOWN,      KC_RGHT,      ____________,
    ____________, KC_LCTL,      ____________, ____________, ____________, ____________, ____________,
    ____________, ____________, ____________, ____________, ____________,

                                                                          ____________, ____________,
                                                                                        ____________,
                                                            ____________, ____________, ____________,
    // right hand
    RESET,        KC_F6,        KC_F7,        KC_F8,        KC_F9,        KC_F10,       KC_F11,
    ____________, ____________, ____________, KC_UP,        ____________, ____________, KC_F12,
                  ____________, KC_LEFT,      KC_DOWN,      KC_RGHT,      ____________, ____________,
    ____________, ____________, ____________, ____________, ____________, KC_LCTL,      ____________,
                                ____________, ____________, ____________, ____________, ____________,

    ____________, ____________,
    ____________,
    ____________, ____________, KC_SPC
  ),
};

uint8_t get_original_keycode(uint16_t safer_keycode) {
  return safer_keycode - SAFER_RANGE;
}

uint32_t convert_layer_to_state(uint8_t layer) {
  return 1UL<<layer;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LOL_ON:
      if (record->event.pressed) {
        layer_on(LOL_SPM);
      }
      return false;
    case LOL_OFF:
      if (record->event.pressed) {
        uint32_t state_mask = 0;
        state_mask |= convert_layer_to_state(LOL_SPM);
        state_mask |= convert_layer_to_state(LOL_F2);
        state_mask |= convert_layer_to_state(LOL_F3);
        state_mask |= convert_layer_to_state(LOL_F4);
        state_mask |= convert_layer_to_state(LOL_F5);
        state_mask |= convert_layer_to_state(LOL_Q);
        state_mask |= convert_layer_to_state(LOL_W);
        state_mask |= convert_layer_to_state(LOL_E);
        state_mask |= convert_layer_to_state(LOL_R);
        state_mask |= convert_layer_to_state(LOL_D);
        state_mask |= convert_layer_to_state(LOL_F);
        state_mask |= convert_layer_to_state(LOL_CHT);
        layer_and(~state_mask);
      }
      return false;
    case CHAT_TG:
      //uint8_t alted = get_mods() & MOD_BIT(KC_LALT);
      if (record->event.pressed) {
        if (has_anymod()) {
          clear_mods();
          register_code(KC_LSFT);
        }
        register_code(KC_ENT);
      } else {
        unregister_code(KC_ENT);
        if (has_anymod()) {
          unregister_code(KC_LSFT);
        }
        layer_invert(LOL_CHT);
      }
      return false;
    default:
      if (keycode >= SAFER_RANGE) {
        // F1 to F5
        if (keycode >= SAFER_RANGE + KC_F1) {
          uint8_t original_keycode = get_original_keycode(keycode);
          uint8_t offset = original_keycode - KC_F1;

          if (record->event.pressed) {
            uint32_t state_mask = 0;
            state_mask |= convert_layer_to_state(LOL_F2);
            state_mask |= convert_layer_to_state(LOL_F3);
            state_mask |= convert_layer_to_state(LOL_F4);
            state_mask |= convert_layer_to_state(LOL_F5);
            layer_and(~state_mask);
            layer_on(LOL_SPM + offset);
          }
          return false;
        }
        // A to 0
        uint8_t original_keycode = get_original_keycode(keycode);
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
  }
  return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
};

void check_spam(void) {
  for (int offset = 0; offset < 36; offset++) {
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

  if (layer_state_cmp(state, FNCTN)) {
    ergodox_right_led_1_on();
  }
  if (layer_state_cmp(state, LOL_SPM)) {
    ergodox_right_led_2_on();
  }
  if (layer_state_cmp(state, LOL_Q) ||
      layer_state_cmp(state, LOL_W) ||
      layer_state_cmp(state, LOL_E) ||
      layer_state_cmp(state, LOL_R) ||
      layer_state_cmp(state, LOL_D) ||
      layer_state_cmp(state, LOL_F)) {
    ergodox_right_led_3_on();
  }

  return state;
};