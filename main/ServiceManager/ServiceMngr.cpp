#include "nvsFlash.h"
#include <cstring>
#include "esp_heap_caps.h"

#include "ServiceMngr.hpp"
#include "Singleton.hpp"

static const char* TAG = "ServiceMngr";
TaskHandle_t ServiceMngr::SrvMngHandle = nullptr;
ServiceMngr::ServiceParams_t ServiceMngr::mServiceParams[SharedBus::ServiceID::MAX_ID];
#ifdef CONFIG_DONE_COMPONENT_LVGL
TaskHandle_t ServiceMngr::LVGLHandle = nullptr;
std::shared_ptr<UICoffeeMaker> ServiceMngr::uiCoffeeMaker;
#endif  
#ifdef CONFIG_DONE_COMPONENT_MATTER
TaskHandle_t ServiceMngr::MatterHandle = nullptr;
std::shared_ptr<MatterCoffeeMaker> ServiceMngr::matterCoffeeMaker;
#endif
#ifdef CONFIG_DONE_COMPONENT_MQTT
TaskHandle_t ServiceMngr::MQTTHandle = nullptr;
#endif

#include "esp_heap_caps.h"

ServiceMngr::ServiceMngr(
    const char *TaskName,
    const SharedBus::ServiceID &ServiceID) :
    ServiceBase(TaskName, ServiceID)
{
    esp_err_t err;    

    nvsFlashInit();

    SharedBus sharedBus;
    if(sharedBus.Init() == ESP_OK)
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

    err = TaskInit(
            &SrvMngHandle,
            tskIDLE_PRIORITY + 1,
            mServiceStackSize[SharedBus::ServiceID::SERVICE_MANAGER]);
    
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG,"%s service was created.",
            mServiceName[SharedBus::ServiceID::SERVICE_MANAGER]);
    }     
    else 
    {
        ESP_LOGE(TAG,"%s service was not created.",
            mServiceName[SharedBus::ServiceID::SERVICE_MANAGER]);
    }        
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
        ESP_LOGE(TAG, "Failed to create %s!", mServiceName[serviceParams.id]);
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
    // ESP_LOGI(TAG, "%s service was Deleted !", mServiceParams[ServiceID].name);
    // mServiceParams[ServiceID].taskKiller();
}

esp_err_t ServiceMngr::OnMachineStateStart()
{
    esp_err_t err = ESP_OK;
    
#ifdef CONFIG_DONE_COMPONENT_LVGL
    uiCoffeeMaker = Singleton<UICoffeeMaker, const char*, SharedBus::ServiceID>::
                        GetInstance(static_cast<const char*>
                        (mServiceName[SharedBus::ServiceID::UI]),
                        SharedBus::ServiceID::UI);    

    uiCoffeeMaker->OnSetupSrv(
        uiCoffeeMaker->OnSetup);

    uiCoffeeMaker->OnSharedbusReceivedSrv(
        uiCoffeeMaker->OnSharedBusReceivedAppp);
            
    err = uiCoffeeMaker->TaskInit(
        &LVGLHandle,
        tskIDLE_PRIORITY + 1,
        mServiceStackSize[SharedBus::ServiceID::UI]);

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG,"%s service was created.",
            mServiceName[SharedBus::ServiceID::UI]);
    }
    else
    {
        ESP_LOGE(TAG,"%s service was not created.",
            mServiceName[SharedBus::ServiceID::UI]);
    }    
#endif //CONFIG_DONE_COMPONENT_LVGL

#ifdef CONFIG_DONE_COMPONENT_MATTER 
    matterCoffeeMaker = Singleton<MatterCoffeeMaker, const char*, SharedBus::ServiceID>::
                            GetInstance(static_cast<const char*>
                            (mServiceName[SharedBus::ServiceID::MATTER]), 
                            SharedBus::ServiceID::MATTER);    
    
    err = matterCoffeeMaker->TaskInit(
            &MatterHandle,
            tskIDLE_PRIORITY + 1,
            mServiceStackSize[SharedBus::ServiceID::MATTER]);
    
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG,"%s service was created.",
            mServiceName[SharedBus::ServiceID::MATTER]);
    }     
    else
    {
        ESP_LOGE(TAG,"%s service was not created.",
            mServiceName[SharedBus::ServiceID::MATTER]);
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