#include <pins.h>
#include "input_handler.h"

LOCAL os_timer_t serial_timer;
LOCAL os_timer_t action_timer;
LOCAL os_timer_t display_timeout;
unsigned char    bits_remaining = 20;
uint32           bit_sequence   = 0;
bool             enable_display = false;

void input_handler( char *input )
{
    os_printf( "input: %s\n", input );

    switch ( input[ 0 ] )
    {
        case 'u':
            move_up();
            break;
        case 'd':
            move_down();
            break;
        case 's':
            display( input );
            break;
        case 'o':
            overrideDisplay();
            break;
        case 'r':
            releaseDisplay();
            break;
        default:
            stop();
    }
}

void startActionTimeout()
{
    os_timer_disarm( &action_timer );
    os_timer_setfn( &action_timer, (os_timer_func_t *) stop );
    os_timer_arm( &action_timer, ACTION_DURATION, 0 );
}

void move_up()
{
    pin_high( GPIO_UP );
    os_printf( "Up!\n" );
    tcp_send( "Moving up!\n" );
    startActionTimeout();
}

void move_down()
{
    pin_high( GPIO_DOWN );
    os_printf( "Down!\n" );
    tcp_send( "Moving down!\n" );
    startActionTimeout();
}

void stop()
{
    pin_low( GPIO_DOWN );
    pin_low( GPIO_UP );
    os_printf( "Stop!\n" );
    tcp_send( "Stopping motion!\n" );
    os_timer_disarm( &action_timer );
}

void display( char *data )
{
    // pop off the initial command character
    os_memmove( data, data + 1, strlen( data ));

    int num = atoi( data );

    bit_sequence = (( num << 1 ) << 10 ) | ( 257 << 1 );
    tcp_send( "Setting display!\n" );

    overrideDisplay();
    displaySerial( bit_sequence );
}

void overrideDisplay()
{
    os_printf( "enable display\n" );
    pin_high( GPIO_ENDISP );
    enable_display = true;

    os_timer_disarm( &display_timeout );
    os_timer_setfn( &display_timeout, (os_timer_func_t *) releaseDisplay );
    os_timer_arm( &display_timeout, DISPLAY_OVERRIDE_DURATION, 0 );
}

void releaseDisplay()
{
    os_printf( "release display\n" );
    pin_low( GPIO_ENDISP );
    enable_display = false;
}

void displaySerial( uint32 data )
{
    os_timer_disarm( &serial_timer );

    switch ( data & 0x1 )
    {
        case 0:
            os_printf( "0" );
            pin_low( GPIO_SIGDISP );
            break;
        case 1:
            os_printf( "1" );
            pin_high( GPIO_SIGDISP );
            break;
    }

    data >>= 1;
    bits_remaining -= 1;

    if ( bits_remaining <= 0 )
    {
        bits_remaining = 20;

        data = bit_sequence;
        if ( !enable_display )
        {
            os_printf( "display not enabled. stop sending serial\n" );
            return;
        }
    }

    os_timer_setfn( &serial_timer, (os_timer_func_t *) displaySerial, data );
    os_timer_arm_us( &serial_timer, 104, 0 );
}