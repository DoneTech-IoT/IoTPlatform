#include <esp_err.h>
#include "SharedBus.h"
#include "Custom_Log.h"

#define BIT_23	( 1 << 23 ) //prevent overwriting bus for send 
#define BIT_22	( 1 << 22 ) //permission for all components to peek from SharedBus 
#define BIT_21	( 1 << 21 ) //check if received the packet one time
#define BIT_20	( 1 << 20 ) //task continuous permission
#define BIT_19	( 1 << 19 ) //task continuous confirm

#define UI_DAEMON_ID        ( 1 << UI_INTERFACE_ID )
#define MATTER_DAEMON_ID    ( 1 << MATTER_INTERFACE_ID ) 
#define MQTT_DAEMON_ID      ( 1 << MQTT_INTERFACE_ID )
#define SERVICE_MANAGER_DAEMON_ID   ( 1 << SERVICE_MANAGER_INTERFACE_ID )
#define ALL_DAEMON_IDs ( UI_DAEMON_ID    | MATTER_DAEMON_ID |\ 
                        /*MQTT_DAEMON_ID  |*/ SERVICE_MANAGER_DAEMON_ID )
                        
static const char *TAG = "SharedBus";

static EventBits_t EventBits;
static EventGroupHandle_t EventGroupHandleLocal;
static uint8_t TaskID = 1;//start by service manager id
QueueHandle_t QueueHandle;

static EventBits_t DaemonEventBits;
static EventGroupHandle_t DeamonEventGroupHandle;

static EventBits_t DaemonEventBits;
static EventGroupHandle_t DeamonEventGroupHandle;

static EventBits_t ReceiveEventBits;
static EventGroupHandle_t ReceiveEventGroupHandle;

SharedBusPacket_t SharedBusPacket;

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
    SharedBusPacket.data =  malloc(1000);

    DaemonEventBits = 0;
    DeamonEventGroupHandle = xEventGroupCreate();  

    ReceiveEventBits = 0;
    ReceiveEventGroupHandle = xEventGroupCreate();  

    return true;         
}

/**
 * @brief Prepare needed Bits and send the Packet.
 * @param SharedBusPacket The Packet to publish on Bus.
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

    EventBits = xEventGroupSetBits(
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
esp_err_t SharedBusReceive(
    SharedBusPacket_t *SharedBusPacket,
    TaskInterfaceID_t interfaceID)
{ 
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal,   /* The event group being tested. */
                    BIT_22 | BIT_21,         /* The bits within the event group to wait for. */
                    pdFALSE,        /* BIT 21 or 22 should NOT be cleared before returning. */
                    pdTRUE,         /* Wait for 21 or 22th bit, either bit will do. */
                    1);             /* Wait a maximum of 1ms for either bit to be set. */    

    //every task listen to sharedbus before send
    //this case always return before send
    //return false if permission is not granted
    if((EventBits & BIT_22) == 0) //default state
    {             
        //ESP_LOGE(TAG, "1, %d", interfaceID);        
        return false;
    }   
    
    // return false if queue is empty
    if(xQueuePeek(QueueHandle, SharedBusPacket, 1) != pdTRUE)
    {         
        ESP_LOGE(TAG, "2, %d", interfaceID);
        return false; 
    } 

    if((EventBits & BIT_21) == 0)
    {
        //permission to itself
        EventBits = xEventGroupClearBits(
                EventGroupHandleLocal, /* The event group being updated. */
                BIT_22);           

        EventBits = xEventGroupClearBits(
            EventGroupHandleLocal, /* The event group being updated. */
            BIT_23);

        ReceiveEventBits = 0;
        
        ESP_LOGE(TAG, "3, %d", interfaceID);             
        return false;
    }    

    // return false if receiver and transmitter was the same component
    if (SharedBusPacket->SourceID == interfaceID)
    {                
        EventBits = xEventGroupClearBits(
            EventGroupHandleLocal, /* The event group being updated. */
            BIT_21);  

        ESP_LOGE(TAG, "4, %d", interfaceID);
        return false;
    }    

    ReceiveEventBits = xEventGroupGetBits(ReceiveEventGroupHandle);
    if((ReceiveEventBits & (1 << interfaceID)) == (1 << interfaceID))
    {                
        ESP_LOGE(TAG, "5, %d", interfaceID);
        ESP_LOGE(TAG, "6, %d", (uint8_t) ReceiveEventBits);                    
        return false;
    }

    ESP_LOGE(TAG, "7, %d", interfaceID);             
    return true;
}

/**
 * @brief confirm the running of task body
 * @return nothing
 */
void SharedBusReceiveConfirmed(
    TaskInterfaceID_t interfaceID)
{    
    ReceiveEventBits = xEventGroupSetBits(
            ReceiveEventGroupHandle, /* The event group being updated. */
            (1 << interfaceID));      
}

/**
 * @brief Check all deamon run and confirm
 * @param interfaceID The ID of tasks.
 * @return True if task daemon were ran
 */
esp_err_t SharedBusTaskDaemonRunsConfirmed(    
    TaskInterfaceID_t interfaceID)
{
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal, /* The event group being tested. */
                    BIT_20,  /* The bits within the event group to wait for. */
                    pdFALSE, /* BIT 20 should NOT be cleared before returning. */
                    pdTRUE, /* Wait for 20 bit, either bit will do. */
                    1);      /* Wait a maximum of 1ms for either bit to be set. */    

    if((EventBits & BIT_20) == BIT_20)
    {        
        return false;
    }

    DaemonEventBits = xEventGroupSync( 
                        DeamonEventGroupHandle,
                        (1 << interfaceID),
                        ALL_DAEMON_IDs,
                        1); //tick to wait

    if((DaemonEventBits & ALL_DAEMON_IDs) != ALL_DAEMON_IDs)
    {                
        // ESP_LOGE(TAG, "1, %d", interfaceID);
        // ESP_LOGE(TAG, "3, %d", (uint8_t) DaemonEventBits);                    
        return false;
    }
    
    EventBits = xEventGroupSetBits(
                        EventGroupHandleLocal, /* The event group being updated. */
                        BIT_20);               /* The bits being set. */
        // ESP_LOGE(TAG, "2, %d", interfaceID);
        // ESP_LOGE(TAG, "4--------------------------, %d", (uint8_t) DaemonEventBits);                    
    return true;        
}

/**
 * @brief get permission all task to continue if its daemon was ran before 
 * @return True for permission
 */
uint8_t SharedBusTaskContinuousPermission()
{
    EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal, /* The event group being tested. */
                    BIT_20,  /* The bits within the event group to wait for. */
                    pdFALSE, /* BIT 20 should NOT be cleared before returning. */
                    pdTRUE, /* Wait for 20 bit, either bit will do. */
                    1);      /* Wait a maximum of 1ms for either bit to be set. */    

    if((EventBits & BIT_20) != BIT_20)
    {        
        return NOT_ANY_ID;
    }    
    else
    {
        EventBits = xEventGroupWaitBits(
                    EventGroupHandleLocal, /* The event group being tested. */
                    BIT_19, /* The bits within the event group to wait for. */
                    pdFALSE,/* BIT 19 should NOT be cleared before returning. */
                    pdTRUE, /* Wait for 19 bit, either bit will do. */
                    1);     /* Wait a maximum of 1ms for either bit to be set.*/                    

        //ESP_LOGE(TAG, "6, %d", TaskID);    
        return TaskID;                        
    }
}

/**
 * @brief confirm the running of task body
 * @return nothing
 */
void SharedBusTaskContinuousConfirm()
{    
    EventBits = xEventGroupSetBits(
            EventGroupHandleLocal, /* The event group being updated. */
            BIT_19);  

    TaskID++;      
    if(TaskID > MAX_TASK_ID - 3)//not NOT_ANY_ID nor mqtt or log
        TaskID = MAX_TASK_ID;              
}