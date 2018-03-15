#ifndef user_config_h
#define user_config_h

#define ACTION_DURATION 750
#define DISPLAY_OVERRIDE_DURATION 10000

#define USE_US_TIMER

#define GPIO_UP 12
#define GPIO_DOWN 13
#define GPIO_ENDISP 16
#define GPIO_SIGDISP 14
#define GPIO_LED_ACT 5
#define GPIO_LED_CONN 4

#ifndef ESPBAUD
#define ESPBAUD 115200
#endif

#ifndef TCP_PORT
#define TCP_PORT 1112
#endif

#ifdef DEBUG_MSGS
#define DEBUG(...) os_printf( __VA_ARGS__ );
#else
#define DEBUG(...)
#endif

#define HELP_MSG \
"\n\n\n----------\n\n" \
"u: Move up\n" \
"d: Move down\n" \
"s[height]: Set display to height\n" \
"o: Override display\n" \
"r: Release control of display\n" \
"x: Stop\n" \
"b: Reboot\n" \
"h: Help\n" \
"\n----------\n\n\n" \

#endif //user_config_h
