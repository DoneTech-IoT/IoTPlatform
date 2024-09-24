#include <esp_err.h>
#include "SharedBus.h"
#include "Custom_Log.h"

#define BIT_23	( 1 << 23 ) //prevent overwriting bus for send 
#define BIT_22	( 1 << 22 ) //permisson for all components to peek from SharedBus 
#define BIT_21	( 1 << 21 ) //check if received the packet ones

static const char *TAG = "SharedBus";

static EventBits_t EventBits;
static EventGroupHandle_t EventGroupHandleLocal;
QueueHandle_t QueueHandle;

/**
 * @brief Initialize the SharedBus.
 * @param void This function have no parameter.
 * @return Always true.
 */
esp_err_t SharedBusInit(void)    
{
    EventBits = 0;
    EventGroupHandleLocal = xEventGroupCreate();  
    QueueHandle = xQueueCreate(1, sizeof(SharedBusPacket_t));     
    return true;         
}

/**
 * @brief Prepare needed Bits and send the Packet.
 * @param SahredBusPacket The Packet to publish on Bus.
 * @return True if queue is available, False if queue is busy.
 */
esp_err_t SharedBusSend(SharedBusPacket_t SharedBusPacket)    
{    
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_23,         /* The bits within the event group to wait for. */
                    pdTRUE,        /* BIT_23 should be cleared before returning. */
                    pdFALSE,        /* Wait for 23th bit, either bit will do. */
                    1);/* Wait a maximum of 1ms for either bit to be set. */
    if((EventBits & BIT_23))
    {
        return false;
    }
    
    EventBits = xEventGroupSetBits(
                    EventGroupHandleLocal, /* The event group being updated. */
                    BIT_23);               /* The bits being set. */

    xQueueOverwrite(QueueHandle, &SharedBusPacket);        

    EventBits = xEventGroupClearBits(
                    EventGroupHandleLocal,
                    BIT_21);         

    //permission to all task to peek
    EventBits = xEventGroupSetBits(
                    EventGroupHandleLocal, /* The event group being updated. */
                    BIT_22);

    return true;                                   
}

/**
 * @brief Check needed Bits to receive the Packet.
 * @param SharedBusPacket The received Packet.
 * @param interfaceID The ID of receiver component.
 * @return True if successfully received, false if receiver and sender are the same component.
 */
esp_err_t SharedBusRecieve(
    SharedBusPacket_t *SharedBusPacket,
    TaskInterfaceID_t interfaceID)
{ 

    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_22 | BIT_21,         /* The bits within the event group to wait for. */
                    pdFALSE,        /* BIT 21 or 22 should NOT be cleared before returning. */
                    pdFALSE,         /* Wait for 21 or 22th bit, either bit will do. */
                    1);             /* Wait a maximum of 1ms for either bit to be set. */    

    if((EventBits & BIT_22) == 0) //default state
    {     
        return false;
    }   
 ESP_LOGE(TAG, "data size before peek = %d", sizeof(SharedBusPacket->data));
    if(xQueuePeek(QueueHandle, SharedBusPacket, 1) != pdTRUE)
    { 
        return false; 
    } 
 ESP_LOGE(TAG, "data size after peek = %d", sizeof(SharedBusPacket->data));
    if (SharedBusPacket->SourceID != interfaceID)
    {
        return true;
    }

    if(!(EventBits & BIT_21))
    {
        //permission to itself
        EventBits = xEventGroupSetBits(
                EventGroupHandleLocal, /* The event group being updated. */
                BIT_21);
        return false;
    }

    EventBits = xEventGroupClearBits(
            EventGroupHandleLocal, /* The event group being updated. */
            BIT_22);  

    EventBits = xEventGroupClearBits(
            EventGroupHandleLocal, /* The event group being updated. */
            BIT_23);

    return false;
}