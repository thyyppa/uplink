#include <osapi.h>
#include "input_handler.h"

LOCAL os_timer_t serial_timer;
LOCAL os_timer_t action_timer;
LOCAL os_timer_t display_timeout;
unsigned char    bits_remaining = 20;
uint32           bit_sequence   = 0;
char             enable_display = 0;

void input_handler( char *input )
{
    DEBUG( "input: %s\n", input );

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
        case 'b':
            reboot();
            break;
        case 'h':
            help();
            break;
        case 0x1b:
            arrow_key( input );
            break;
        case 'x':
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

void reboot()
{
    INFO( "Rebooting!" );
    tcp_disconnect();
    system_restart();
}

void help()
{
    INFO( HELP_MSG );
}

void arrow_key( char *input )
{
    if ( input[ 1 ] != '[' )
    {
        INFO( "not arrow key?" );
        return;
    }

    switch ( input[ 2 ] )
    {
        case 'A':
            move_up();
            break;
        case 'B':
            move_down();
            break;
        default:
            INFO( "Unknown key!" );

    }
}

void move_up()
{
    pin_high( GPIO_UP );
    pin_high( GPIO_LED_ACT );
    INFO( "Moving Up!" );
    startActionTimeout();
}

void move_down()
{
    pin_high( GPIO_DOWN );
    pin_high( GPIO_LED_ACT );
    INFO( "Moving Down!" );
    startActionTimeout();
}

void stop()
{
    pin_low( GPIO_DOWN );
    pin_low( GPIO_UP );
    pin_low( GPIO_LED_ACT );
    INFO( "Stopping motion!" );
    os_timer_disarm( &action_timer );
}

void display( char *data )
{
    // pop off the initial command character
    os_memmove( data, data + 1, strlen( data ));

    int num = atoi( data );

    bit_sequence = (( num << 1 ) << 10 ) | ( 257 << 1 );
    INFO( "Setting display!" );

    overrideDisplay();
    displaySerial( bit_sequence );
}

void overrideDisplay()
{
    INFO( "Enable Display!" );
    pin_high( GPIO_ENDISP );
    enable_display = 1;

    os_timer_disarm( &display_timeout );
    os_timer_setfn( &display_timeout, (os_timer_func_t *) releaseDisplay );
    os_timer_arm( &display_timeout, DISPLAY_OVERRIDE_DURATION, 0 );
}

void releaseDisplay()
{
    INFO( "Releasing Display!" );
    pin_low( GPIO_ENDISP );
    enable_display = 0;
}

void displaySerial( uint32 data )
{
    os_timer_disarm( &serial_timer );

    switch ( data & 0x1 )
    {
        case 0:
            DEBUG( "0" );
            pin_low( GPIO_SIGDISP );
            break;
        case 1:
            DEBUG( "1" );
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
            DEBUG( "display not enabled. stop sending serial" );
            return;
        }
    }

    os_timer_setfn( &serial_timer, (os_timer_func_t *) displaySerial, data );
    ets_timer_arm_new( &serial_timer, 104, 0, 0 );
}