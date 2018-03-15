#ifndef input_handler_h
#define input_handler_h

#include <stdlib.h>
#include <user_config.h>
#include <tcp.h>
#include <pins.h>
#include <ide_defs.h>
#include <osapi.h>

/**
 * Define a function that will be used to handle input
 * the "input_handler" function is a fine choice.
 */
#define INPUT_HANDLER input_handler
typedef void (handler)( char * );

/**
 * This is where the magic happens.
 * In this function is a switch statement
 * that runs the commands sent over tcp.
 * @param input
 */
void input_handler( char *input );
void arrow_key( char *input );
void move_up();
void move_down();
void stop();
void reboot();
void help();

/**
 * Send a three digit number to the 7 segment LCD
 *
 * todo: This does not work properly yet!
 * @param data
 */
void display( char *data );
void displaySerial( uint32 data );

/**
 * Whenever an action is performed be sure to
 * cancel it after a period of time. Holding down
 * the "down" button indefinitely wouldn't be good
 * at all.
 */
void startActionTimeout();

/**
 * Cut off the LCD signal from the original controller
 * so that we can inject our own.
 *
 * todo: These don't work properly yet!
 */
void overrideDisplay();
void releaseDisplay();

#endif //input_handler_h
