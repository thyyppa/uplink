#include "pins.h"

void pin_high( uint32 pin )
{
    GPIO_OUTPUT_SET( GPIO_ID_PIN( pin ), 1 );
}

void pin_low( uint32 pin )
{
    GPIO_OUTPUT_SET( GPIO_ID_PIN( pin ), 0 );
}

uint32 pin_read( uint32 pin )
{
    // todo: untested!
    return GPIO_REG_READ( GPIO_OUT_ADDRESS ) & ( 1 << pin );
}

void pin_function( int pin, uint32 mux, uint32 function )
{
    PIN_FUNC_SELECT( PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12 );
}