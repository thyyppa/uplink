#include "pins.h"

void pin_high( uint32 pin )
{
    gpio_output_set( 0, ( 1 << pin ), 0, 0 );
}

void pin_low( uint32 pin )
{
    gpio_output_set(( 1 << pin ), 0, 0, 0 );
}

uint32 pin_read( uint32 pin )
{
    return GPIO_REG_READ( GPIO_OUT_ADDRESS ) & ( 1 << pin );
}

void pin_disable( uint32 pin )
{
    gpio_output_set( 0, 0, 0, ( 1 << pin ));
}

void pin_enable( uint32 pin )
{
    gpio_output_set( 0, 0, ( 1 << pin ), 0 );
}

void pin_function( uint32 mux, uint32 function )
{
    PIN_FUNC_SELECT( mux, function );
}