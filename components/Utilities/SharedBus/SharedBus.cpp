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
    EventBits = BIT_23;
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
                    pdTRUE,        /* Wait for 23th bit, either bit will do. */
                    pdMS_TO_TICKS(500));/* Wait a maximum of 100ms for either bit to be set. */
    EventBits = xEventGroupSetBits(
                    EventGroupHandleLocal, /* The event group being updated. */
                    BIT_23);               /* The bits being set. */

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
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_22,         /* The bits within the event group to wait for. */
                    pdTRUE,        /* BIT_23 should be cleared before returning. */
                    pdTRUE,        /* Wait for 23th bit, either bit will do. */
                    1);/* Wait a maximum of 100ms for either bit to be set. */

        xQueuePeek(QueueHandle, &SharedBusPacket, 1);//) //TODO err-handling

    if (SharedBusPacket.SourceID == LOG_INTERFACE_ID)
    {
        // EventBits = xEventGroupWaitBits(
        //             EventGroupHandleLocal,   /* The event group being tested. */
        //             BIT_23,         /* The bits within the event group to wait for. */
        //             pdTRUE,        /* BIT_23 should be cleared before returning. */
        //             pdTRUE,        /* Wait for 23th bit, either bit will do. */
        //             portMAX_DELAY);/* Wait a maximum of 100ms for either bit to be set. */

        xQueueReceive(QueueHandle, &SharedBusPacket, 1);
        EventBits = xEventGroupSetBits(
                    EventGroupHandleLocal, /* The event group being updated. */
                    BIT_22);               
        return 1;
    }

    if (BIT_22 != NULL){
        xQueueReceive(QueueHandle, &SharedBusPacket, 1);
        return 1;
    }
    // EventBits = xEventGroupWaitBits(
    //                 EventGroupHandleLocal,   /* The event group being tested. */
    //                 BIT_23,         /* The bits within the event group to wait for. */
    //                 pdTRUE,        /* BIT_23 should be cleared before returning. */
    //                 pdTRUE,        /* Wait for 23th bit, either bit will do. */
    //                 portMAX_DELAY);/* Wait a maximum of 100ms for either bit to be set. */
    return 0;
}  