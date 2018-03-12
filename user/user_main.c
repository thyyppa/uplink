#include "user_main.h"

void ICACHE_FLASH_ATTR user_init()
{
    enable_microsecond_timer();
    gpio_init();
    set_pins_to_output();
    set_uart_baud( ESPBAUD );
    connect_wifi( WIFI_SSID, WIFI_PASSWORD );
    set_input_handler( INPUT_HANDLER );
    start_tcp_server( TCP_PORT );
}

void enable_microsecond_timer()
{
    system_timer_reinit();
}

void set_pins_to_output()
{
    //
}

void set_uart_baud( uint32 baud )
{
    uart_div_modify( 0, UART_CLK_FREQ / baud );
}