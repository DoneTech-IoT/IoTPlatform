#include <esp_err.h>
#include "SharedBus.h"

#define UI_EVENT_FLAG	( 1 << UI_INTERFACE_ID )
#define UI_EVENT_FLAG	( 1 << UI_INTERFACE_ID )

#define BIT_23	( 1 << 23 )

static EventBits_t EventBits;
static EventGroupHandle_t *EventGroupHandleLocal;

const TickType_t xTicksToWait = 1 ;// portTICK_PERIOD_MS;

esp_err_t SharedBusInit(void)    
{
    EventGroupHandleLocal = EventGroupHandle;
    *EventGroupHandleLocal = xEventGroupCreate();            
}

esp_err_t SharedBusSend(        
    QueueHandle_t *QueueHandle,
    SharedBusPacket_t *SharedBusPacket)    
{    
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_23         /* The bits within the event group to wait for. */
                    pdTRUE,        /* BIT_23 should be cleared before returning. */
                    pdTRUE,        /* Wait for 23th bit, either bit will do. */
                    portMAX_DELAY);/* Wait a maximum of 100ms for either bit to be set. */

    EventBits = xEventGroupSetBits(
                    EventGroupHandleLocal, /* The event group being updated. */
                    BIT_23);               /* The bits being set. */

    *QueueHandle = xQueueCreate(1, sizeof(SharedBusPacket_t));    

    xQueueSend(*QueueHandle, SharedBusPacket, 1); 
    
    // TODO: manage all sync bits
    // EventBits = xEventGroupWaitBits(
    //                 EventGroupHandleLocal,   /* The event group being tested. */
    //                 ALL_SYNC_BITS  /* The bits within the event group to wait for. */
    //                 pdTRUE,        /* BIT_23 should be cleared before returning. */
    //                 pdTRUE,        /* Wait for 23th bit, either bit will do. */
    //                 portMAX_DELAY);/                       
}

esp_err_t SharedBusRecieve(    
    QueueHandle_t *QueueHandle)
{
    SharedBusPacket_t *recievedPacket;
    
    xQueuePeek(QueueHandle, recievedPacket, 1);

    if (recievedPacket->SourceID == currentTaskID)
    {
        EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_23         /* The bits within the event group to wait for. */
                    pdTRUE,        /* BIT_23 should be cleared before returning. */
                    pdTRUE,        /* Wait for 23th bit, either bit will do. */
                    portMAX_DELAY);/* Wait a maximum of 100ms for either bit to be set. */

        xQueueReceive(QueueHandle, recievedPacket, 1);
        return 0;
    }

    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_23         /* The bits within the event group to wait for. */
                    pdTRUE,        /* BIT_23 should be cleared before returning. */
                    pdTRUE,        /* Wait for 23th bit, either bit will do. */
                    portMAX_DELAY);/* Wait a maximum of 100ms for either bit to be set. */
    
    return 0;
}  

// esp_err_t SharedBusTransaction(    
//     QueueHandle_t *QueueHandle)
// {
//     EventBits = xEventGroupWaitBits(
//                     *EventGroupHandleLocal,   /* The event group being tested. */
//                     BIT_23, /* The bits within the event group to wait for. */
//                     pdTRUE,        /* BIT_23 should be cleared before returning. */
//                     pdFALSE,       /* Don't wait for both bits, either bit will do. */
//                     1); /* Wait a maximum of 100ms for either bit to be set. */
// } 

