# MPvaliantboy

## rules.mk

`CONSOLE_ENABLE     = no  # Console for debug`

  - This allows you to print messages that can be read using [hid_listen](https://www.pjrc.com/teensy/hid_listen.html).

  - By default, all debug (dprint) print (print, xprintf), and user print (uprint) messages will be enabled. This will eat up a significant portion of the flash and may make the keyboard .hex file too big to program.

  - To disable debug messages (dprint) and reduce the .hex file size, include #define NO_DEBUG in your config.h file.
  
  - To disable print messages (print, xprintf) and user print messages (uprint) and reduce the .hex file size, include #define NO_PRINT in your config.h file.

  - To disable print messages (print, xprintf) and KEEP user print messages (uprint), include #define USER_PRINT in your config.h file.

  - To see the text, open hid_listen and enjoy looking at your printed messages.

  - NOTE: Do not include uprint messages in anything other than your keymap code. It must not be used within the QMK system framework. Otherwise, you will bloat other people's .hex files.

  - Consumes about 400 bytes.
  
`COMMAND_ENABLE     = no  # Commands for debug and configuration`

  - This enables magic commands, typically fired with the default magic key combo LSHIFT+RSHIFT+KEY.
  - Magic commands include turning on debugging messages (MAGIC+D) or temporarily toggling NKRO (MAGIC+N).

`VARIABLE_TRACE     = no`

  - Use this to debug changes to variable values, see the tracing variables section of the [Unit Testing] page for more information.
  - https://docs.qmk.fm/reference/unit-testing#tracing-variables
