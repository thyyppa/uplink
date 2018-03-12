#ifndef tcp_h
#define tcp_h

#include "ide_defs.h"
#include "user_config.h"
#include <user_interface.h>
#include <espconn.h>
#include "osapi.h"
#include "../user/input_handler.h"

typedef void (handler)( char * );

void ICACHE_FLASH_ATTR start_tcp_server( uint32 port );
void ICACHE_FLASH_ATTR tcpserver_init( uint32 port );
void ICACHE_FLASH_ATTR set_input_handler( handler cb );
void ICACHE_FLASH_ATTR tcp_send( char *string );
void ICACHE_FLASH_ATTR tcp_disconnect();
LOCAL void ICACHE_FLASH_ATTR tcp_server_listen( void *espconn );
LOCAL void tcp_server_multi_send( char *message );
LOCAL void ICACHE_FLASH_ATTR tcp_server_recv_cb( void *arg, char *received, unsigned short length );

#endif //tcp_h
