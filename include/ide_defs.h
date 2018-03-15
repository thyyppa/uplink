#ifndef ide_defs_h
#define ide_defs_h

// Fake some nonsense to appease ide

#include <os_type.h>
#include "c_types.h"

void os_printf( char *str, ... );
void os_printf_plus( char *str, ... );
void uart_div_modify( uint32 a, uint32 b );
void ets_timer_setfn( os_timer_t *timer, os_timer_func_t *function, ... );
void ets_timer_arm_new( os_timer_t *timer, uint32 delay, uint32 a, uint32 b );
void ets_timer_disarm( os_timer_t *timer );
void ets_memset( void *data, int a, int length );
void ets_memcpy( void *to, void *data, int length );
void ets_memmove( void *to, void *data, int length );
uint16 ets_strlen( char *string );
uint16 ets_sprintf( char *string, ... );

typedef uint32 uint32_t;

#endif //ide_defs_h