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

#define CON_CLEAR "\e[0m"
#define CON_GREEN "\e[32m"
#define CON_CYAN "\e[36m"
#define CON_YELLOW "\e[93m"

#ifdef INFO_MSGS
#define INFO( msg ) { os_printf( "info: " msg "\n" ); tcp_send( CON_GREEN "info: " CON_CLEAR msg "\n" ); }
#else
#define INFO( ... )
#endif

#ifdef DEBUG_MSGS
#define DEBUG(...) os_printf( "debug: " __VA_ARGS__ );
#else
#define DEBUG( ... )
#endif

#define HELP_MSG \
"\n\n----------\n\n" \
CON_CYAN "u" CON_CLEAR ": Move up\n" \
CON_CYAN "d" CON_CLEAR ": Move down\n" \
CON_CYAN "s[height]" CON_CLEAR ": Set display to height\n" \
CON_CYAN "o" CON_CLEAR ": Override display\n" \
CON_CYAN "r" CON_CLEAR ": Release control of display\n" \
CON_CYAN "x" CON_CLEAR ": Stop\n" \
CON_CYAN "b" CON_CLEAR ": Reboot\n" \
CON_CYAN "h" CON_CLEAR ": Help\n" \
"\n----------\n\n" \

#endif //user_config_h
