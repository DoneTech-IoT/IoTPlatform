#include "nvsFlash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Setup_GPIO.h"
#include "ServiceManager.h"
#include "Custom_Log.h"
#include "CoffeeMakerApp.hpp"
#include "MatterInterface.h"

#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#define TIMER_TIME pdMS_TO_TICKS(500) // in millis

QueueHandle_t MatterBufQueue;
SemaphoreHandle_t MatterSemaphore = NULL;
MatterInterfaceHandler_t MatterInterfaceHandler;
// ****************************** GLobal Variables ****************************** //
static const char *TAG = "Main";
// ****************************** GLobal Functions ****************************** //

// void MatterAttributeUpdateCBMain(callback_type_t type,
//                                 uint16_t endpoint_id, uint32_t cluster_id,
//                                 uint32_t attribute_id, esp_matter_attr_val_t *val,
//                                 void *priv_data)
// {
//     printf("callback_type_t: %u\n", type);
//     printf("endpoint_id: %u\n", endpoint_id);
//     printf("cluster_id: %lu\n", cluster_id);
//     printf("attribute_id: %lu\n", attribute_id);
//     printf("val: %p\n", val);
//     printf("priv_data: %pGlobalInitGlobalInitGlobalInit\n", priv_data);
// }

void MatterNetworkConnected()
{
    ESP_LOGI(TAG, "Matter Network Connected\n");
}

/**
 * @brief Function to change colors based on a timer callback
 */
extern "C" void app_main()
{
    nvsFlashInit();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());
    Log_RamOccupy("main", "service manager");

    ServiceMangerTaskInit();

    Log_RamOccupy("main", "service manager");

    // Log_RamOccupy("main", "Matter usage");
    // MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
    // MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
    // MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    // MatterInterfaceHandler.ConnectToMatterNetwork = MatterNetworkConnected;
    // Matter_TaskInit(&MatterInterfaceHandler);
}