#include "tcp.h"

LOCAL struct espconn esp_conn;
LOCAL esp_tcp        esptcp;
LOCAL os_timer_t     waiting_for_wifi_timer;
handler              *tcp_input_handler;

void ICACHE_FLASH_ATTR tcpserver_init( uint32 port )
{
    esp_conn.type                  = ESPCONN_TCP;
    esp_conn.state                 = ESPCONN_NONE;
    esp_conn.proto.tcp             = &esptcp;
    esp_conn.proto.tcp->local_port = port;

    espconn_regist_connectcb( &esp_conn, tcp_server_listen );
    espconn_accept( &esp_conn );
}

void ICACHE_FLASH_ATTR set_input_handler( handler cb )
{
    tcp_input_handler = cb;
}

LOCAL void ICACHE_FLASH_ATTR tcp_server_recv_cb( void *arg, char *received, unsigned short length )
{
    tcp_input_handler( received );
}

LOCAL void ICACHE_FLASH_ATTR tcp_server_listen( void *espconn )
{
    espconn_regist_recvcb( espconn, tcp_server_recv_cb );
    tcp_server_multi_send( "Connected!\n" );
}

LOCAL void tcp_server_multi_send( char *message )
{
    struct espconn *connection = &esp_conn;

    remot_info *remote = NULL;

    if ( espconn_get_connection_info( connection, &remote, 0 ) == ESPCONN_OK )
    {
        for ( uint8 i = 0; i < connection->link_cnt; i++ )
        {
            connection->proto.tcp->remote_port = remote[ i ].remote_port;

            os_memcpy(
                    connection->proto.tcp->remote_ip,
                    remote[ i ].remote_ip,
                    sizeof( connection->proto.tcp->remote_ip )
            );

            espconn_sent( connection, message, os_strlen( message ));
        }
    }
}

void ICACHE_FLASH_ATTR tcp_send( char *string )
{
    tcp_server_multi_send( string );
}

void ICACHE_FLASH_ATTR start_tcp_server( uint32 port )
{
    os_timer_disarm( &waiting_for_wifi_timer );

    uint8 status = wifi_station_get_connect_status();

    if ( status != STATION_GOT_IP )
    {
        switch ( status )
        {
            case STATION_NO_AP_FOUND:
                DEBUG( "Connection Failed: SSID Not Found\n" );
                return;

            case STATION_WRONG_PASSWORD:
                DEBUG( "Connection Failed: Wrong Password\n" );
                return;

            case STATION_CONNECT_FAIL:
                DEBUG( "Connection failed for no reason\n" );
                return;

            default:
                os_timer_setfn( &waiting_for_wifi_timer, (os_timer_func_t *) start_tcp_server, port );
                os_timer_arm( &waiting_for_wifi_timer, 100, 0 );
                return;
        }
    }

    tcpserver_init( port );
}
