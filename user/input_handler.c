#include <pins.h>
#include "input_handler.h"

LOCAL os_timer_t action_timer;
bool             enable_display = false;

void input_handler( char *input )
{
    os_printf( "Input! %s\n", input );

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
            overrideDisplay( input );
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
    // pop off the first character (s)
    os_memmove( data, data + 1, strlen( data ));

    int num = atoi( data );
    os_printf( "Set display to: %d\n", num );
    tcp_send( "Setting display!\n" );

    overrideDisplay( "o1" );
}

void overrideDisplay( char *data )
{
    // pop off the initial command character
    os_memmove( data, data + 1, strlen( data ));

    switch ( data[ 0 ] )
    {
        case '1':
            pin_high( GPIO_ENDISP );
            enable_display = true;
            break;
        default:
            enable_display = false;
            pin_low( GPIO_ENDISP );
    }

}
