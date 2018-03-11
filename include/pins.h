#ifndef pins_h
#define pins_h

#include "ide_defs.h"
#include "c_types.h"
#include "gpio.h"
#include "eagle_soc.h"

void pin_high( uint32 pin );
void pin_low( uint32 pin );
uint32 pin_read( uint32 pin );
void pin_disable( uint32 pin );
void pin_enable( uint32 pin );
void pin_function( uint32 mux, uint32 function );

#endif //pins_h
