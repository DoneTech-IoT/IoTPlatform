#include "ServiceManager.h"
#include "CoffeeMakerApp.hpp"
#include "SharedBus.h"
#include "GUI.h"

// ****************************** Local Variables
typedef enum 
{
    IDLE = 0,
    INIT,
    START,    
}ServiceManagerDaemonState;
static ServiceManagerDaemonState State;

static const char *TAG = "Service_Manager";

#ifdef CONFIG_DONE_COMPONENT_MATTER
static TaskHandle_t MatterHandle = NULL;
#endif  //CONFIG_DONE_COMPONENT_MATTER

#ifdef CONFIG_DONE_COMPONENT_MQTT
static QueueHandle_t MQTTDataFromBrokerQueue;
static SemaphoreHandle_t MQTTConnectedSemaphore;
static SemaphoreHandle_t MQTTErrorOrDisconnectSemaphore;
static TaskHandle_t MQTTHandle = NULL;

extern SharedBusPacket_t SharedBusPacket;
static MatterEventPacket *MatterEventPacketToSend;
#endif  //CONFIG_DONE_COMPONENT_MQTT

#define TASK_LIST_BUFFER_SIZE 512
// #define MONITORING

// Global instance of Service Manager
static ServiceManger_t ServiceManger;

/**
 * @brief Deletes a task.
 * This function deletes the specified task.
 * @param TaskNumber The number of the task to be deleted.
 * @return void
 */
void TaskKiller(int TaskNumber)
{
    ServiceManger.Services[TaskNumber].TaskKiller(&ServiceManger.Services[TaskNumber].taskHandler);
    ESP_LOGI(TAG, "Task %d Deleted !", TaskNumber);
}

/* 
    * @brief run given service.
    * This function runs the given service by initializing the service parameters and creating the task.
    * @param[in] serviceParams Service parameters
    * @param[in] id Service to run
    * @retval ESP_OK if the service is run successfully, otherwise ESP_FAIL
*/ 
esp_err_t ServiceManager_RunService(ServiceParams_t serviceParams)
{
    esp_err_t err = ESP_OK;

    // Call the function pointer (init_func) with proper arguments
    TaskInitPtr init_func = serviceParams.TaskInit;
    err = init_func(&serviceParams.taskHandler, 
                    serviceParams.priority, 
                    serviceParams.taskStack);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to create MQTT!");
        return err;
    }

    return ESP_OK;
}

static void ServiceManger_RunAllDaemons()
{    
    esp_err_t err;

    ServiceParams_t GUIParams;
    GUIParams.maximumRAM_Needed = LVGL_STACK * 2;
    strcpy(GUIParams.name, "GUI");
    GUIParams.ramType = PSRAM_;
    GUIParams.TaskKiller = GUI_TaskKill;
    GUIParams.taskStack = LVGL_STACK;
    GUIParams.priority = tskIDLE_PRIORITY + 1;    
    GUIParams.TaskInit = GUI_TaskInit;
    err = ServiceManager_RunService(GUIParams);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to create GUI!");
    }
    else
    {
        ESP_LOGI(TAG, "GUI Daemon Created !");        
    }    

#ifdef CONFIG_DONE_COMPONENT_MATTER
    // Config and Run Matter        
    ServiceParams_t MatterParams;
    strcpy(MatterParams.name, "Matter");    
    MatterParams.maximumRAM_Needed = 0;
    MatterParams.ramType = SRAM_;
    MatterParams.TaskKiller = Matter_TaskKill;
    MatterParams.taskStack = MATTER_STACK_SIZE;
    MatterParams.priority = tskIDLE_PRIORITY + 1;
    MatterParams.taskHandler = MatterHandle;
    MatterParams.TaskInit = Matter_TaskInit;
    err = ServiceManager_RunService(MatterParams);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to create Matter !");
    }
    else 
    {
        ESP_LOGI(TAG, "Matter Daemon Created !");
    }
#endif
    
#ifdef CONFIG_DONE_COMPONENT_MQTT
    // Config and Run MQTT
    // MQTT_InterfaceHandler.ErrorDisconnectSemaphore = &MQTTErrorOrDisconnectSemaphore;
    // MQTT_InterfaceHandler.IsConnectedSemaphore = &MQTTConnectedSemaphore;
    // MQTT_InterfaceHandler.BrokerIncomingDataQueue = &MQTTDataFromBrokerQueue;

    // ServiceParams_t MQTTParams;
    // strcpy(MQTTParams.name, "MQTT");
    // MQTTParams.maximumRAM_Needed = 0;                
    // MQTTParams.ramType = SRAM_;
    // MQTTParams.TaskKiller = MQTT_TaskKill;
    // MQTTParams.taskStack = MQTT_STACK;
    // MQTTParams.priority = tskIDLE_PRIORITY + 1;
    // MQTTParams.taskHandler = MQTTHandle;
    // MQTTParams.TaskInit = MQTT_TaskInit;
    // err = ServiceManager_RunService (MQTTParams);
    // if (err)
    // {
    //     ESP_LOGE(TAG, "Failed to create MQTT !");
    // }
    // else
    // {
    //     ESP_LOGI(TAG, "MQTT Daemon Created !");
    //     vTaskDelay(pdMS_TO_TICKS(500));
    //     MQTT_Start();
    //     vTaskDelay(pdMS_TO_TICKS(500));   
    // }
    // CoffeeMakerApplication(&MQTTDataFromBrokerQueue, &MQTTConnectedSemaphore, &MQTTErrorOrDisconnectSemaphore);
#endif  //CONFIG_DONE_COMPONENT_MQTT
}

/**
 * @brief Task function for the Service Manager task.
 * This task initializes and manages other tasks.
 * @param pvParameter Pointer to task parameters.
 * @return void
 */
static void ServiceManger_MainTask(void *pvParameter)
{    
    nvsFlashInit();
    
    if (SharedBusInit())
    {
        ESP_LOGI(TAG, "initialized SharedBus successfully");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to Initialize SharedBus.");
    }                

#ifdef MONITORING
// char pcTaskList[TASK_LIST_BUFFER_SIZE];
#endif                
    
    State = ServiceManagerDaemonState::INIT;
    while (true)
    {        
        if(SharedBusReceive(&SharedBusPacket, SERVICE_MANAGER_INTERFACE_ID))        
        {                             
            switch (SharedBusPacket.PacketID)
            {
                case MATTER_EVENT_PACKET_ID:                    
                    MatterEventPacketToSend = (MatterEventPacket*) SharedBusPacket.data;
                    if(MatterEventPacketToSend->PublicEventTypes == kInterfaceIpAddressChanged)
                    {
                        
                    }
                    break;
            
                default:
                    break;
            }
        }
                
        switch (State)
        {
            case ServiceManagerDaemonState::IDLE:
                break;
            case ServiceManagerDaemonState::INIT:    
                SharedBusTaskDaemonRunsConfirmed(SERVICE_MANAGER_INTERFACE_ID);
                State = ServiceManagerDaemonState::START;
                break;

            case ServiceManagerDaemonState::START:        
                ESP_LOGI(TAG, "Service Manager Daemon Created !");            
                ServiceManger_RunAllDaemons();              
                SharedBusTaskContinuousConfirm();
                State = ServiceManagerDaemonState::IDLE;
                break;

            default:
                break;
        }                                                     

        vTaskDelay(pdMS_TO_TICKS(1));

#ifdef MONITORING
// vTaskList(pcTaskList);
// ESP_LOGI(TAG, "Task List:\n%s\n", pcTaskList);
#endif                
    }
}

/**
 * @brief Initializes the Service Manager task.
 * This function initializes the Service Manager task by allocating memory and creating the task.
 * @return void
 */
void ServiceManger_TaskInit()
{
    StaticTask_t *xTaskServiceMangerBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    StackType_t *xServiceMangerStack = (StackType_t *)malloc(SERVICE_MANGER_STACK * sizeof(StackType_t));
    xTaskCreateStatic(
        ServiceManger_MainTask,         // Task function
        "ServiceMangerTask",       // Task name (for debugging)
        SERVICE_MANGER_STACK,      // Stack size (in words)
        NULL,                      // Task parameters (passed to the task function)
        tskIDLE_PRIORITY + 1,      // Task priority (adjust as needed)
        xServiceMangerStack,       // Stack buffer
        xTaskServiceMangerBuffer); // Task control block
}