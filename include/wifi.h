#ifndef wifi_h
#define wifi_h

#include <user_interface.h>
#include <osapi.h>
#include "ide_defs.h"

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif

#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif

void ICACHE_FLASH_ATTR connect_wifi( char *ssid, char *password );

#endif //wifi_h
