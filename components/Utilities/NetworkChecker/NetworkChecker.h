#ifdef __cplusplus
extern "C"
{
#endif
#ifndef NETWORK_CHECKER_
#define NETWORK_CHECKER_

#include "esp_wifi.h"
#include "esp_log.h"
#include <esp_event.h>
#include "esp_system.h"
#include <sys/param.h>
#include "esp_err.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
    bool CheckInternetConnection();
    bool CheckWifiStatus();
#endif
#ifdef __cplusplus
}
#endif
