#ifndef user_config_h
#define user_config_h

#define ACTION_DURATION 1000
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

#endif //user_config_h
