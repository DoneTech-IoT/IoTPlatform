#include "ServiceManager.h"
#include "CoffeeMakerApp.hpp"
#include "SharedBus.h"

static const char *TAG = "Service_Manager";

#ifdef CONFIG_DONE_COMPONENT_MATTER
MatterInterfaceHandler_t MatterInterfaceHandler;
static QueueHandle_t MatterBufQueue;
static SemaphoreHandle_t MatterSemaphore = NULL;
static TaskHandle_t MatterHandle = NULL;
static UBaseType_t MatterPriority = tskIDLE_PRIORITY + 1;
#endif  //CONFIG_DONE_COMPONENT_MATTER

#ifdef CONFIG_DONE_COMPONENT_MQTT
MQTT_InterfaceHandler_t MQTT_InterfaceHandler;
static QueueHandle_t MQTTDataFromBrokerQueue;
static SemaphoreHandle_t MQTTConnectedSemaphore;
static SemaphoreHandle_t MQTTErrorOrDisconnectSemaphore;
static TaskHandle_t MQTTHandle = NULL;
static UBaseType_t MQTTPriority = tskIDLE_PRIORITY + 1;

#endif  //CONFIG_DONE_COMPONENT_MQTT

#define TASK_LIST_BUFFER_SIZE 512
// #define MONITORING
/**
 * @brief Task function for the Service Manager task.
 * This task initializes and manages other tasks.
 * @param pvParameter Pointer to task parameters.
 * @return void
 */
void ServiceMangerTask(void *pvParameter);

// Global instance of Service Manager
ServiceManger_t ServiceManger;

#ifdef CONFIG_DONE_COMPONENT_MATTER
void MatterAttributeUpdateCBMain(callback_type_t type,
                                 uint16_t endpoint_id, uint32_t cluster_id,
                                 uint32_t attribute_id, esp_matter_attr_val_t *val,
                                 void *priv_data)
{
    printf("callback_type_t: %u\n", type);
    printf("endpoint_id: %u\n", endpoint_id);
    printf("cluster_id: %lu\n", cluster_id);
    printf("attribute_id: %lu\n", attribute_id);
    printf("val: %p\n", val);
    printf("priv_data: %pGlobalInitGlobalInitGlobalInit\n", priv_data);
}
#endif

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

/**
 * @brief Initializes the Service Manager task.
 * This function initializes the Service Manager task by allocating memory and creating the task.
 * @return void
 */
void ServiceManger_Init()
{
    StaticTask_t *xTaskServiceMangerBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    StackType_t *xServiceMangerStack = (StackType_t *)malloc(SERVICE_MANGER_STACK * sizeof(StackType_t));
    xTaskCreateStatic(
        ServiceMangerTask,         // Task function
        "ServiceMangerTask",       // Task name (for debugging)
        SERVICE_MANGER_STACK,      // Stack size (in words)
        NULL,                      // Task parameters (passed to the task function)
        tskIDLE_PRIORITY + 1,      // Task priority (adjust as needed)
        xServiceMangerStack,       // Stack buffer
        xTaskServiceMangerBuffer); // Task control block
}

/* 
    * @brief run given service.
    * This function runs the given service by initializing the service parameters and creating the task.
    * @param[in] serviceParams Service parameters
    * @param[in] id Service to run
    * @retval ESP_OK if the service is run successfully, otherwise ESP_FAIL
*/ 
esp_err_t ServiceManager_RunService(ServiceParams_t serviceParams, ServiceID id)
{
    ServiceManger.Services[id] = serviceParams;
    
    switch (id)
    {
        case GUI_Task:
            GUI_TaskInit(&ServiceManger.Services[id].taskHandler, 
                        ServiceManger.Services[id].priority, 
                        ServiceManger.Services[id].taskStack);

            ESP_LOGI(TAG, "GUI Created !");
            vTaskDelay(SERVICE_MANAGER_SEC * 4);
            break;

        case MatterTask:
        Matter_TaskInit(&MatterInterfaceHandler, 
                        &MatterHandle,
                        MatterPriority,
                        MATTER_STACK_SIZE);        
            ESP_LOGI(TAG, "Matter interface called !");
            break;

        case MQTTTask:            
            MQTT_TaskInit(&MQTT_InterfaceHandler, 
                        &MQTTHandle,
                        MQTTPriority,
                        MQTT_STACK); 
            vTaskDelay(SERVICE_MANAGER_SEC);
            MQTT_Start();
            break;
        default:
            break;

    }
    return ESP_OK;
}

/**
 * @brief Task function for the Service Manager task.
 * This task initializes and manages other tasks.
 * @param pvParameter Pointer to task parameters.
 * @return void
 */
void ServiceMangerTask(void *pvParameter)
{
#ifdef CONFIG_DONE_COMPONENT_MQTT
    esp_err_t err;


    nvsFlashInit();
    
    if (SharedBusInit())
    {
        ESP_LOGI(TAG, "initialized SharedBus successfully");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to Initialize SharedBus.");
    }

    ServiceParams_t GUIParams;
    GUIParams.maximumRAM_Needed = LVGL_STACK * 2;
    strcpy(GUIParams.name, "GUI");
    GUIParams.ramType = PSRAM_;
    GUIParams.TaskKiller = GUI_TaskKill;
    GUIParams.taskStack = LVGL_STACK;
    GUIParams.priority = tskIDLE_PRIORITY + 1;
    GUIParams.taskHandler = NULL;
    err = ServiceManager_RunService(GUIParams, GUI_Task);
    if (err)
    {
        ESP_LOGI(TAG, "GUI Created !");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to create GUI!");
    }
#endif

#ifdef CONFIG_DONE_COMPONENT_MATTER
    // Config and Run Matter
    MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
    MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
    MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    ServiceParams_t MatterParams;
    MatterParams.maximumRAM_Needed = 0;
    strcpy(MatterParams.name, "Matter");
    MatterParams.ramType = SRAM_;
    MatterParams.TaskKiller = Matter_TaskKill;
    MatterParams.taskStack = MATTER_STACK_SIZE;
    MatterParams.priority = tskIDLE_PRIORITY + 1;
    MatterParams.taskHandler = MatterHandle;
    err = ServiceManager_RunService(MatterParams, MatterTask);
    if (err)
    {
        ESP_LOGI(TAG, "Matter Created !");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to create Matter !");
    }
#endif

#ifdef CONFIG_DONE_COMPONENT_MQTT
    // Config and Run MQTT
    MQTT_InterfaceHandler.ErrorDisconnectSemaphore = &MQTTErrorOrDisconnectSemaphore;
    MQTT_InterfaceHandler.IsConnectedSemaphore = &MQTTConnectedSemaphore;
    MQTT_InterfaceHandler.BrokerIncomingDataQueue = &MQTTDataFromBrokerQueue;

    ServiceParams_t MQTTParams;
    MQTTParams.maximumRAM_Needed = 0;
    strcpy(MQTTParams.name, "MQTT");
    MQTTParams.ramType = SRAM_;
    MQTTParams.TaskKiller = MQTT_TaskKill;
    MQTTParams.taskStack = MQTT_STACK;
    MQTTParams.priority = tskIDLE_PRIORITY + 1;
    MQTTParams.taskHandler = MQTTHandle;
    err = ServiceManager_RunService (MQTTParams, MQTTTask);
    if (err)
    {
        ESP_LOGI(TAG, "MQTT Created !");
        vTaskDelay(pdMS_TO_TICKS(500));
        MQTT_Start();
    }
    else
    {
        ESP_LOGE(TAG, "Failed to create MQTT !");
    }
#endif  //CONFIG_DONE_COMPONENT_MQTT

    CoffeeMakerApplication(&MQTTDataFromBrokerQueue, &MQTTConnectedSemaphore, &MQTTErrorOrDisconnectSemaphore);
    char pcTaskList[TASK_LIST_BUFFER_SIZE];
    while (true)
    {
#ifdef MONITORING
// vTaskList(pcTaskList);
// ESP_LOGI(TAG, "Task List:\n%s\n", pcTaskList);
#endif
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
