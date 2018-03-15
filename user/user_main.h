#ifndef user_main_h
#define user_main_h

#include <c_types.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>
#include <user_interface.h>
#include <eagle_soc.h>

#include "ide_defs.h"
#include "pins.h"
#include "user_config.h"
#include "tcp.h"
#include "wifi.h"
#include "input_handler.h"

void set_uart_baud( uint32 baud );
void set_pins_to_output();
void enable_microsecond_timer();

#endif //user_main_h
