#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lvgl.h"
#include "rm67162.h"
#include "wifi.h"

// To Do: ask user through console to provide SSID and Password
char *pass = WIFI_PASSWORD; 
char *ssid = WIFI_SSID;

void app_main(void)
{
    rm67162_init();
    lv_init();

    nvs_flash_init();
    configure_wifi(ssid, pass);
    connect_wifi();

    lcd_fill(100, 100, 536, 240, 0x1221);
}
