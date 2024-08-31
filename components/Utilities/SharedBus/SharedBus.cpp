#include <esp_err.h>
#include "SharedBus.h"
#include "Custom_Log.h"

#define UI_EVENT_FLAG	( 1 << UI_INTERFACE_ID )
#define UI_EVENT_FLAG	( 1 << UI_INTERFACE_ID )

#define BIT_23	( 1 << 23 )
#define BIT_22	( 1 << 22 )

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
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_23,         /* The bits within the event group to wait for. */
                    pdTRUE,        /* BIT_23 should be cleared before returning. */
                    pdFALSE,        /* Wait for 23th bit, either bit will do. */
                    1);/* Wait a maximum of 100ms for either bit to be set. */
    EventBits = xEventGroupSetBits(
                    EventGroupHandleLocal, /* The event group being updated. */
                    BIT_23);               /* The bits being set. */

    xQueueSend(QueueHandle, &SharedBusPacket, 1);
    return 0;    
}

esp_err_t SharedBusRecieve(    
    QueueHandle_t QueueHandle, 
    SharedBusPacket_t SharedBusPacket,
    TaskInterfaceID_t interfaceID)
{   
    esp_err_t ret = false;
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_22,         /* The bits within the event group to wait for. */
                    pdFALSE,        /* BIT_22 should NOT be cleared before returning. */
                    pdTRUE,         /* Wait for 22th bit, either bit will do. */
                    1);/*           Wait a maximum of 100ms for either bit to be set. */
    
    if((EventBits & BIT_22))
    {
        if(xQueueReceive(QueueHandle, &SharedBusPacket, 1) == pdTRUE)
            ESP_LOGE(TAG, "Recieved-%d", SharedBusPacket.SourceID);
        ret = false;
    }        
    else //default state
    {
        if(xQueuePeek(QueueHandle, &SharedBusPacket, 1) == pdTRUE) //TODO err-handling   
            ESP_LOGE(TAG, "Peek-%d", SharedBusPacket.SourceID);

        if (SharedBusPacket.SourceID == interfaceID)
        {                
            EventBits = xEventGroupSetBits(
                        EventGroupHandleLocal, /* The event group being updated. */
                        BIT_22);                    
        }
        ret = true;
    }    
        
    return ret;
}  