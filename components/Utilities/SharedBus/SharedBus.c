#include <esp_err.h>
#include "SharedBus.h"
#include "Custom_Log.h"

#define SHAREDBUS_QUEUE_CAPACITY 1000

#define BIT_23	( 1 << 23 ) //prevent overwriting bus for send 
#define BIT_22	( 1 << 22 ) //notify the end of task run permission

#define UI_DAEMON_ID        ( 1 << UI_INTERFACE_ID )
#define MATTER_DAEMON_ID    ( 1 << MATTER_INTERFACE_ID ) 
#define MQTT_DAEMON_ID      ( 1 << MQTT_INTERFACE_ID )
#define SERVICE_MANAGER_DAEMON_ID   ( 1 << SERVICE_MANAGER_INTERFACE_ID )

#define COUNT_ONES(x) __builtin_popcount(x)

static const char *TAG = "SharedBus";

static EventBits_t EventBits;
static EventGroupHandle_t EventGroupHandleLocal;
static uint8_t TaskID = 1;//start by service manager id
static uint8_t TaskReceiveCnt = 0;
static uint32_t AllDaemonIDs = 0;
QueueHandle_t QueueHandle;

static EventBits_t DaemonEventBits;
static EventGroupHandle_t DaemonEventGroupHandle;

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
    AllDaemonIDs |= SERVICE_MANAGER_DAEMON_ID;
#ifdef CONFIG_DONE_COMPONENT_LVGL
    AllDaemonIDs |= UI_DAEMON_ID;
#endif    
#ifdef CONFIG_DONE_COMPONENT_MATTER
    AllDaemonIDs |= MATTER_DAEMON_ID;
#endif
// #ifdef CONFIG_DONE_COMPONENT_MQTT  
//     AllDaemonIDs |= MQTT_DAEMON_ID;
// #endif

    EventGroupHandleLocal = xEventGroupCreate();  
    EventBits = xEventGroupClearBits(
                    EventGroupHandleLocal,
                    AllDaemonIDs);      

    QueueHandle = xQueueCreate(1, sizeof(SharedBusPacket_t));     
    SharedBusPacket.data =  malloc(SHAREDBUS_QUEUE_CAPACITY);
    
    DaemonEventGroupHandle = xEventGroupCreate();  
    DaemonEventBits = xEventGroupClearBits(
                            DaemonEventGroupHandle,
                            AllDaemonIDs);      
    
    ReceiveEventGroupHandle = xEventGroupCreate();  
    ReceiveEventBits = xEventGroupClearBits(
                            ReceiveEventGroupHandle,
                            AllDaemonIDs);      

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

    ReceiveEventBits = xEventGroupClearBits(
                            ReceiveEventGroupHandle,
                            AllDaemonIDs);      

    EventBits = xEventGroupSetBits(
                    EventGroupHandleLocal, /* The event group being updated. */
                    BIT_23);               /* The bits being set. */

    xQueueOverwrite(QueueHandle, &SharedBusPacket);                
    
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
    if(TaskReceiveCnt == COUNT_ONES(AllDaemonIDs))
    {
        TaskReceiveCnt = 0; 
        EventBits = xEventGroupClearBits(
            EventGroupHandleLocal,
            BIT_23);
    }

    ReceiveEventBits = xEventGroupGetBits(ReceiveEventGroupHandle);
    if((ReceiveEventBits & (1 << interfaceID)) == (1 << interfaceID))
    {         
        TaskReceiveCnt++;           
        return false;
    }    
    
    xQueuePeek(QueueHandle, SharedBusPacket, 1);    

    ReceiveEventBits = xEventGroupSetBits(
                            ReceiveEventGroupHandle, /* The event group being updated. */
                            (1 << interfaceID));                        
                           
    return true;
}

/**
 * @brief Check all deamon run and confirm
 * @param interfaceID The ID of tasks.
 * @return True if task daemon were ran
 */
esp_err_t SharedBusTaskDaemonRunsConfirmed(    
    TaskInterfaceID_t interfaceID)
{       
    DaemonEventBits = xEventGroupGetBits(DaemonEventGroupHandle);
    if((DaemonEventBits & AllDaemonIDs) != AllDaemonIDs)
    {
        DaemonEventBits = xEventGroupSetBits(
                            DaemonEventGroupHandle, /* The event group being updated. */
                            (1 << interfaceID));          
        return false;                
    }

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
                BIT_22, /* The bits within the event group to wait for. */
                pdFALSE,/* BIT 20 should NOT be cleared before returning. */
                pdTRUE, /* Wait for 20 bit, either bit will do. */
                1);     /* Wait a maximum of 1ms for either bit to be set.*/                    

    if((EventBits & BIT_22) == BIT_22)
    {        
        return NOT_ANY_ID;
    }        
    
    return TaskID;                            
}

/**
 * @brief confirm the running of task body
 * @return nothing
 */
void SharedBusTaskContinuousConfirm()
{        
    TaskID++;      
    if(TaskID > COUNT_ONES(AllDaemonIDs))
    {        
        TaskID = 0;
        EventBits = xEventGroupSetBits(
            EventGroupHandleLocal,
            BIT_22);           
    }
}