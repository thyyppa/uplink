#ifndef input_handler_h
#define input_handler_h

#include <stdlib.h>

#include "ide_defs.h"
#include "user_config.h"
#include "osapi.h"
#include <tcp.h>
#include <pins.h>

#define INPUT_HANDLER input_handler

typedef void (handler)( char * );

void input_handler( char *input );
void move_up();
void move_down();
void stop();
void display( char *data );
void startActionTimeout();
void displaySerial( uint32 data );
void overrideDisplay();
void releaseDisplay();
void reboot();
void help();
void arrow_key( char *input );

#endif //input_handler_h
