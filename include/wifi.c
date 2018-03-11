#include "wifi.h"

void ICACHE_FLASH_ATTR connect_wifi( char *ssid, char *password )
{
    struct station_config station;

    station.bssid_set = 0;
    os_memset( station.ssid, 0, 32 );
    os_memset( station.password, 0, 64 );

    os_memcpy( &station.ssid, ssid, 32 );
    os_memcpy( &station.password, password, 64 );

    wifi_set_opmode( STATIONAP_MODE );
    wifi_station_set_config( &station );
}