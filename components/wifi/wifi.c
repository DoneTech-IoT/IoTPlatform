#include "string.h"
#include "wifi.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void configure_wifi (char *ssid, char *password)
{
    wifi_config_t wifi_configuration = {
    .sta = {
        .ssid = "",
        .password = "",

            }
    };
    strcpy((char *)wifi_configuration.sta.ssid, ssid);
    strcpy((char *)wifi_configuration.sta.password, password);
    // esp_log_write(ESP_LOG_INFO, "Kconfig", "SSID=%s, PASS=%s", ssid, pass);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
}

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static int retry_num = 0;

    if (event_id == WIFI_EVENT_STA_START)
    {
        printf("WIFI CONNECTING....\n");
    }
    else if (event_id == WIFI_EVENT_STA_CONNECTED)
    {
        printf("WiFi CONNECTED\n");
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        printf("WiFi connection failed\n");
        if (retry_num < 5)
        {
            esp_wifi_connect();
            retry_num++;
            printf("Retrying to Connect...#%d\n ", retry_num);
        }
        else
        {
            printf("Please check SSID and Password and try again\n\n");
        }
    }
    else if (event_id == IP_EVENT_STA_GOT_IP)
    {
        printf("WiFi successfully connected...\n\n");       
    }
}

void connect_wifi()
{
    // Wi-Fi Configuration Phase
    esp_netif_init();
    esp_event_loop_create_default();     // event loop                    
    esp_netif_create_default_wifi_sta(); // WiFi station                    
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); //
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    // Wi-Fi Start Phase
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    
    // Wi-Fi Connect Phase
    esp_wifi_connect();
}