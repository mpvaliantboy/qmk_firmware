#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

// this makes it possible to do rolling combos (zx) with keys that convert to other keys on hold
// (z becomes ctrl when you hold it, and when this option isn't enabled, z rapidly followed by x actually sends Ctrl-x. That's bad.)
#undef IGNORE_MOD_TAP_INTERRUPT

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#undef LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#undef LOCKING_RESYNC_ENABLE

/* key combination for command */
#undef IS_COMMAND

/* number of backlight levels */
#undef BACKLIGHT_LEVELS

/* ws2812 RGB LED */
#undef RGB_DI_PIN
#undef RGBLIGHT_ANIMATIONS
#undef RGBLED_NUM  // Number of LEDs
#undef RGBLIGHT_HUE_STEP
#undef RGBLIGHT_SAT_STEP
#undef RGBLIGHT_VAL_STEP

// #define RGB_MIDI
#undef RGBW_BB_TWI

#undef RGBW

/* fix space cadet rollover issue */
#undef DISABLE_SPACE_CADET_ROLLOVER

//#undef PREVENT_STUCK_MODIFIERS

#undef DEBOUNCE
#define DEBOUNCE    3

#undef LED_BRIGHTNESS_DEFAULT
#define LED_BRIGHTNESS_DEFAULT (0)

// NO_DEBUG and NO_PRINT are defined, just in case
#ifndef NO_DEBUG
#define NO_DEBUG
#endif
#ifndef NO_PRINT
#define NO_PRINT
#endif

/* disable action features */
//#define NO_ACTION_LAYER
// the following 2 have to be disabled for one shot
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
#define ONESHOT_TAP_TOGGLE 3  /* Tapping this number of times holds the key until tapped this number of times again. */
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

//#define QMK_KEYS_PER_SCAN 4

#endif
