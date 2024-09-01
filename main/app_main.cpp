#include "nvsFlash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Setup_GPIO.h"
#include "ServiceManger.h"
#include "Custom_Log.h"
#include "SharedBus.h"
#include "MQTT_Interface.h"
//#include "MatterInterface.h"
//#include "DoneCoffeeMaker.h"

#define CONFIG_DONE_COMPONENT_MQTT
#define TIMER_TIME pdMS_TO_TICKS(500) // in millis
#define TaskStack 5000

QueueHandle_t QueueHandle;
EventGroupHandle_t EventGroupHandle;

char *msg = "HI every one. I'm blackboard";
 SharedBusPacket_t recievePacket; //= {
//     .SourceID = LOG_INTERFACE_ID,
//     .PacketID = 1,
//     .data = msg
// };
// QueueHandle_t MatterBufQueue;
// SemaphoreHandle_t MatterSemaphore = NULL;
// MatterInterfaceHandler_t MatterInterfaceHandler;
// ****************************** GLobal Variables ****************************** //
static const char *TAG = "Main";   
// ****************************** GLobal Functions ****************************** //

// void MatterAttributeUpdateCBMain(callback_type_t type,
//                                 uint16_t endpoint_id, uint32_t cluster_id,
//                                 uint32_t attribute_id, esp_matter_attr_val_t *val,
//                                 void *priv_data)
// {
//     // printf("callback_type_t: %u\n", type);
//     // printf("endpoint_id: %u\n", endpoint_id);
//     // printf("cluster_id: %lu\n", cluster_id);
//     // printf("attribute_id: %lu\n", attribute_id);
//     // printf("val: %p\n", val);
//     // printf("priv_data: %pGlobalInitGlobalInitGlobalInit\n", priv_data);
// }

void MatterNetworkConnected()
{
    ESP_LOGI(TAG, "Matter Network Connected\n");
}

/**
 * @brief Function to change colors based on a timer callback
 */

void vTaskCode1( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, MATTER_INTERFACE_ID))
            ESP_LOGE(TAG, "task1-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTaskCode2( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, MQTT_INTERFACE_ID))
            ESP_LOGE(TAG, "task2-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTaskCode3( void * pvParameters )
{    
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    for( ;; )
    {
        /* TODO recieve here. */
        if(SharedBusRecieve(QueueHandle, recievePacket, LOG_INTERFACE_ID))
            ESP_LOGE(TAG, "task3-%s", (char*) recievePacket.data);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void app_main()
{
    Log_RamOccupy("main", "service manager");
    ServiceMangerTaskInit();
    nvsFlashInit();
    Log_RamOccupy("main", "service manager");

    BaseType_t xReturned;
    TaskHandle_t xHandle1 = NULL;
    TaskHandle_t xHandle2 = NULL;
    TaskHandle_t xHandle3 = NULL;

    SharedBusInit(&EventGroupHandle, &QueueHandle);

vTaskDelay (pdMS_TO_TICKS(5000));

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    vTaskCode1,       /* Function that implements the task. */
                    "num1",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle1 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode2,       /* Function that implements the task. */
                    "num2",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle2 );      /* Used to pass out the created task's handle. */
    xReturned = xTaskCreate(
                    vTaskCode3,       /* Function that implements the task. */
                    "num3",          /* Text name for the task. */
                    TaskStack,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle3 );      /* Used to pass out the created task's handle. */

    recievePacket.SourceID = LOG_INTERFACE_ID;
    recievePacket.PacketID = 1;
    recievePacket.data = msg;

    SharedBusSend(QueueHandle, recievePacket);

    // Log_RamOccupy("main", "Matter usage");
    // MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
    // MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
    // MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    // MatterInterfaceHandler.ConnectToMatterNetwork = MatterNetworkConnected;
    // Matter_TaskInit(&MatterInterfaceHandler);

    //KeyStatePair_t pKeyStatePair;

    while (true)
    {
        // if(xQueueReceive(
        //     *(MatterInterfaceHandler.SharedBufQueue), 
        //     &pKeyStatePair, pdMS_TO_TICKS(1)) == pdTRUE)
        // {
        //     ESP_LOGW(TAG, "pKeyStatePair-> Key: %d, State: %d\n", 
        //     pKeyStatePair.Key, pKeyStatePair.State);        
        // }
        vTaskDelay(100);
    }        
}
