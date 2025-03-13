#ifdef CONFIG_DONE_COMPONENT_LVGL
//#include "GUIService.hpp"
//#include "coffeeMaker_GUI.h"
#endif
#ifdef CONFIG_DONE_COMPONENT_MATTER
#include "MatterService.hpp"
#else
#include "esp_netif.h"
#include "protocol_examples_common.h"
#endif
#ifdef CONFIG_DONE_LOG
#include "Custom_Log.h"
#endif
#ifdef CONFIG_DONE_COMPONENT_MQTT
//#include "MQTTSErvise.hpp"
#endif

#include "ServiceMngr.hpp"
#include "SharedBus.hpp"
#include "Singleton.hpp"
#include "nvsFlash.h"
#include <cstring>

static const char* TAG = "ServiceMngr";
ServiceMngr::ServiceParams_t ServiceMngr::mServiceParams[SharedBus::ServiceID::MAX_ID];

#ifdef CONFIG_DONE_COMPONENT_LVGL
    TaskHandle_t ServiceMngr::LVGLHandle = NULL;
#endif  //CONFIG_DONE_COMPONENT_LVGL

#ifdef CONFIG_DONE_COMPONENT_MATTER
    TaskHandle_t ServiceMngr::MatterHandle = NULL;
#endif  //CONFIG_DONE_COMPONENT_MATTER

#ifdef CONFIG_DONE_COMPONENT_MQTT
    TaskHandle_t ServiceMngr::MQTTHandle = NULL;
#endif  //CONFIG_DONE_COMPONENT_MQTT

ServiceMngr::ServiceMngr(
    const char *TaskName,
    const SharedBus::ServiceID &ServiceID) :
    ServiceBase(TaskName, ServiceID)
{
    nvsFlashInit();

    SharedBus sharedBus;
    if(sharedBus.Init())
    {
        ESP_LOGI(TAG, "Initialized SharedBus successfully");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to Initialize SharedBus.");
    }                

#ifdef MONITORING
// char pcTaskList[TASK_LIST_BUFFER_SIZE];
#endif    
}

ServiceMngr::~ServiceMngr()
{        
}

/**
* @brief run given service.
* This function runs the given service by initializing the service parameters and creating the task.
* @param[in] serviceParams Service parameters    
* @retval ESP_OK if the service is run successfully, otherwise ESP_FAIL
*/ 
esp_err_t ServiceMngr::RunService(ServiceParams_t serviceParams)
{
    esp_err_t err = ESP_OK;

    // Call the function pointer (init_func) with proper arguments
    TaskInitPtr init_func = serviceParams.taskInit;
    err = init_func(&serviceParams.taskHandler, 
                    serviceParams.priority, 
                    serviceParams.taskStackSize);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to create %s!", serviceParams.name);
        return err;
    }

    return ESP_OK;    
}

/**
* @brief Deletes a task.
* This function deletes the specified task.
* @param ServiceID The service ID to be deleted.
* @return void
*/
void ServiceMngr::KillService(const SharedBus::ServiceID &ServiceID)
{    
    ESP_LOGI(TAG, "%s service was Deleted !", mServiceParams[ServiceID].name);
    mServiceParams[ServiceID].taskKiller(&mServiceParams[ServiceID].taskHandler);
}

/**
 * @brief Starts all registered daemon services.
 * This function iterates through all registered services and calls their `Start()` method.
 * Each service is expected to be a subclass of `ServiceBase` and implement its own startup logic.
 */
void ServiceMngr::RunAllDaemons()
{
    esp_err_t err;

// #ifdef CONFIG_DONE_COMPONENT_LVGL    
//     ServiceParams_t GUIParams;    
//     strcpy(GUIParams.name, "GUI");    
//     GUIParams.TaskKiller = GUI;
//     GUIParams.taskStack = LVGL_STACK;
//     GUIParams.priority = tskIDLE_PRIORITY + 1;  
//     GUIParams.taskHandler = LVGLHandle;  
//     GUIParams.TaskInit = GUI_TaskInit;
//     GUI_OnInitElements(GUI_Init);
//     GUI_OnSharedBusReceived(GUI_ProcessSharedBusMsg);
//     err = ServiceManager_RunService(GUIParams);
//     if (err)
//     {
//         ESP_LOGE(TAG, "Failed to create GUI!");
//     }
//     else
//     {
//         ESP_LOGI(TAG, "GUI Daemon Created !");        
//     }    
// #endif //CONFIG_DONE_COMPONENT_LVGL

#ifdef CONFIG_DONE_COMPONENT_MATTER    
    ServiceParams_t MatterParams;
    strcpy(MatterParams.name, "Matter");    
    MatterParams.id = SharedBus::ServiceID::MATTER;
    MatterParams.taskHandler = MatterHandle;
    MatterParams.taskInit = MatterService::TaskInit;
    MatterParams.taskKiller =  MatterService::~MatterService;
    MatterParams.taskStackSize = mServiceStackSize[MatterParams.id];
    MatterParams.priority = tskIDLE_PRIORITY + 1;
        
    err = RunService(MatterParams);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to create Matter !");
    }
    else 
    {
        ESP_LOGI(TAG, "Matter Daemon Created !");
    }
#endif //CONFIG_DONE_COMPONENT_MATTER
    
// #ifdef CONFIG_DONE_COMPONENT_MQTT    
//     ServiceParams_t MQTTParams;
//     strcpy(MQTTParams.name, "MQTT");
//     MQTTParams.maximumRAM_Needed = 0;                
//     MQTTParams.ramType = SRAM_;
//     MQTTParams.TaskKiller = MQTT_TaskKill;
//     MQTTParams.taskStack = MQTT_STACK;
//     MQTTParams.priority = tskIDLE_PRIORITY + 1;
//     MQTTParams.taskHandler = MQTTHandle;
//     MQTTParams.TaskInit = MQTT_TaskInit;
//     err = ServiceManager_RunService (MQTTParams);
//     if (err)
//     {
//         ESP_LOGE(TAG, "Failed to create MQTT !");
//     }    
//     else 
//     {
//         ESP_LOGI(TAG, "MQTT Daemon Created !");
//     }
// #endif //CONFIG_DONE_COMPONENT_MQTT    
}

esp_err_t OnMachineStateStart()
{
    esp_err_t err = ESP_OK;

// #ifdef CONFIG_DONE_COMPONENT_LVGL    
//     ServiceParams_t GUIParams;    
//     strcpy(GUIParams.name, "GUI");    
//     GUIParams.TaskKiller = GUI;
//     GUIParams.taskStack = LVGL_STACK;
//     GUIParams.priority = tskIDLE_PRIORITY + 1;  
//     GUIParams.taskHandler = LVGLHandle;  
//     GUIParams.TaskInit = GUI_TaskInit;
//     GUI_OnInitElements(GUI_Init);
//     GUI_OnSharedBusReceived(GUI_ProcessSharedBusMsg);
//     err = ServiceManager_RunService(GUIParams);
//     if (err)
//     {
//         ESP_LOGE(TAG, "Failed to create GUI!");
//     }
//     else
//     {
//         ESP_LOGI(TAG, "GUI Daemon Created !");        
//     }    
// #endif //CONFIG_DONE_COMPONENT_LVGL

#ifdef CONFIG_DONE_COMPONENT_MATTER    
    ServiceParams_t MatterParams;
    strcpy(MatterParams.name, "Matter");    
    MatterParams.id = SharedBus::ServiceID::MATTER;
    MatterParams.taskHandler = MatterHandle;
    MatterParams.taskInit = MatterService::TaskInit;
    MatterParams.taskKiller =  MatterService::~MatterService;
    MatterParams.taskStackSize = mServiceStackSize[MatterParams.id];
    MatterParams.priority = tskIDLE_PRIORITY + 1;
        
    err = RunService(MatterParams);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to create Matter !");
    }
    else 
    {
        ESP_LOGI(TAG, "Matter Daemon Created !");
    }
#endif //CONFIG_DONE_COMPONENT_MATTER
    
// #ifdef CONFIG_DONE_COMPONENT_MQTT    
//     ServiceParams_t MQTTParams;
//     strcpy(MQTTParams.name, "MQTT");
//     MQTTParams.maximumRAM_Needed = 0;                
//     MQTTParams.ramType = SRAM_;
//     MQTTParams.TaskKiller = MQTT_TaskKill;
//     MQTTParams.taskStack = MQTT_STACK;
//     MQTTParams.priority = tskIDLE_PRIORITY + 1;
//     MQTTParams.taskHandler = MQTTHandle;
//     MQTTParams.TaskInit = MQTT_TaskInit;
//     err = ServiceManager_RunService (MQTTParams);
//     if (err)
//     {
//         ESP_LOGE(TAG, "Failed to create MQTT !");
//     }    
//     else 
//     {
//         ESP_LOGI(TAG, "MQTT Daemon Created !");
//     }
// #endif //CONFIG_DONE_COMPONENT_MQTT    

    return err;        
}