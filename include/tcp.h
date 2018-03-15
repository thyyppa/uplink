#ifndef tcp_h
#define tcp_h

#include <ide_defs.h>
#include <user_config.h>
#include <user_interface.h>
#include <espconn.h>
#include <osapi.h>
#include "../user/input_handler.h"
#include "../user/user_config.h"

typedef void (handler)( char * );

/**
 * Start the TCP server.
 * This function continuously checks for a wifi connection
 * When the connection is successful it triggers the tcpserver_init
 * method.
 * @param port
 */
void ICACHE_FLASH_ATTR start_tcp_server( uint32 port );

/**
 * Sets up the TCP server and registers callbacks to handle input.
 * @param port
 */
void ICACHE_FLASH_ATTR tcpserver_init( uint32 port );

/**
 * Set a callback function that will handle parsing through the input
 * and executing commands.
 * @param cb
 */
void ICACHE_FLASH_ATTR set_input_handler( handler cb );

/**
 * Send string to all connected clients
 * Basically a wrapper for tcp_server_multi_send
 * Either works
 * @param message
 */
void ICACHE_FLASH_ATTR tcp_send( char *message );
LOCAL void tcp_server_multi_send( char *message );

/**
 * Kick all clients from the server and stop server
 */
void ICACHE_FLASH_ATTR tcp_disconnect();

/**
 * Sets a callback for each connected client to handle input
 * @param espconn
 */
LOCAL void ICACHE_FLASH_ATTR tcp_server_listen( void *espconn );

/**
 * This function is where the TCP input is finally sent off to the input handler
 * @param arg
 * @param received
 * @param length
 */
LOCAL void ICACHE_FLASH_ATTR tcp_server_recv_cb( void *arg, char *received, unsigned short length );

#endif //tcp_h
