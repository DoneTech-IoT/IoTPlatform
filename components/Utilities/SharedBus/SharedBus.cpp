#include <esp_err.h>
#include "SharedBus.h"
#include "Custom_Log.h"

#define UI_EVENT_FLAG	( 1 << UI_INTERFACE_ID )
#define UI_EVENT_FLAG	( 1 << UI_INTERFACE_ID )

#define BIT_23	( 1 << 1 )
#define BIT_22	( 1 << 2 )

static const char *TAG = "SharedBus";

static EventBits_t EventBits;
static EventGroupHandle_t EventGroupHandleLocal;

esp_err_t SharedBusInit(EventGroupHandle_t *EventGroupHandle,QueueHandle_t *QueueHandle)    
{
    EventBits = 0;
    EventGroupHandleLocal = *EventGroupHandle;
    EventGroupHandleLocal = xEventGroupCreate();  
    *QueueHandle = xQueueCreate(1, sizeof(SharedBusPacket_t));     
    return 0;         
}

esp_err_t SharedBusSend(        
    QueueHandle_t QueueHandle,
    SharedBusPacket_t SharedBusPacket)    
{    
    // EventBits = xEventGroupWaitBits(
    //                 EventGroupHandleLocal,   /* The event group being tested. */
    //                 BIT_23,         /* The bits within the event group to wait for. */
    //                 pdTRUE,        /* BIT_23 should be cleared before returning. */
    //                 pdFALSE,        /* Wait for 23th bit, either bit will do. */
    //                 pdMS_TO_TICKS(1));/* Wait a maximum of 100ms for either bit to be set. */
    // EventBits = xEventGroupSetBits(
    //                 EventGroupHandleLocal, /* The event group being updated. */
    //                 BIT_23);               /* The bits being set. */

    xQueueSend(QueueHandle, &SharedBusPacket, 1);
    return 0;
    // TODO: manage all sync bits
    // EventBits = xEventGroupWaitBits(
    //                 EventGroupHandleLocal,   /* The event group being tested. */
    //                 ALL_SYNC_BITS  /* The bits within the event group to wait for. */
    //                 pdTRUE,        /* BIT_23 should be cleared before returning. */
    //                 pdTRUE,        /* Wait for 23th bit, either bit will do. */
    //                 portMAX_DELAY);/                       
}

esp_err_t SharedBusRecieve(    
    QueueHandle_t QueueHandle, SharedBusPacket_t SharedBusPacket)
{   
    esp_err_t ret = false;
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_22,         /* The bits within the event group to wait for. */
                    pdFALSE,        /* BIT_22 should be cleared before returning. */
                    pdTRUE,        /* Wait for 23th bit, either bit will do. */
                    1);/* Wait a maximum of 100ms for either bit to be set. */
ESP_LOGE(TAG, "EventBits-%ld", EventBits);
    if((EventBits & BIT_22))
    {
        xQueueReceive(QueueHandle, &SharedBusPacket, 1);
        ESP_LOGE(TAG, "Recieved22-%d", SharedBusPacket.SourceID);
        ret = true;
    }        
    else //default state
    {
        xQueuePeek(QueueHandle, &SharedBusPacket, 1);//) //TODO err-handling   
        ESP_LOGE(TAG, "Peek-%d", SharedBusPacket.SourceID);
        ret = false;
    }
ESP_LOGE(TAG, "SharedBusPacket.SourceID-%d", SharedBusPacket.SourceID);    
    if (SharedBusPacket.SourceID == LOG_INTERFACE_ID)
    {        
        xQueueReceive(QueueHandle, &SharedBusPacket, 1);

        ESP_LOGE(TAG, "Recieved-%d", SharedBusPacket.SourceID);
        EventBits = xEventGroupSetBits(
                    EventGroupHandleLocal, /* The event group being updated. */
                    BIT_22);               
        ret = true;
    }
        
    return ret;
}  