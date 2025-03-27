#pragma once 
#include "sdkconfig.h"
#ifdef CONFIG_DONE_COMPONENT_LVGL
//#include "GUIService.hpp"
//#include "coffeeMaker_GUI.h"
#endif
#ifdef CONFIG_DONE_COMPONENT_MATTER
#include "MatterCoffeeMaker.hpp"
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

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"

#include <esp_event.h>
#include "esp_log.h"
#include "esp_system.h"
#include <sys/param.h>

#include "ServiceBase.hpp"
#include "SharedBus.hpp"

class ServiceMngr : public ServiceBase
{
public:
    static constexpr char* mServiceName[SharedBus::ServiceID::MAX_ID] =
    {
        "",             //NO_ID
        "SRV_MNGR",     //Service Manager
        "UI",
        "MATTER",
        "MQTT",
        "LOG",      
    };    

    static constexpr uint32_t mServiceStackSize[SharedBus::ServiceID::MAX_ID] =
    {
        0 ,             //NO_ID
        50  * 1024,     //Service Manager
        100 * 1024,     //UI
        50  * 1024,     //MATTER
        50  * 1024,     //MQTT
        0               //LOG
    };
    
    explicit ServiceMngr(
        const char *TaskName,
        const SharedBus::ServiceID &ServiceID);

    ~ServiceMngr();

private:    
    static TaskHandle_t SrvMngHandle;
#ifdef CONFIG_DONE_COMPONENT_LVGL
    static TaskHandle_t LVGLHandle;    
#endif  
#ifdef CONFIG_DONE_COMPONENT_MATTER
    static TaskHandle_t MatterHandle;
    static MatterCoffeeMaker *matterCoffeeMaker;
#endif
#ifdef CONFIG_DONE_COMPONENT_MQTT
    static TaskHandle_t MQTTHandle;
#endif
    

    typedef void (*TaskKillerPtr)(void);
    typedef esp_err_t (*TaskInitPtr)(
                            TaskHandle_t *taskHandler,
                            UBaseType_t taskPriority,
                            uint32_t taskStackSize);
    typedef struct
    {                   
        SharedBus::ServiceID id;  
        TaskHandle_t taskHandler; 
        TaskInitPtr taskInit;  
        TaskKillerPtr taskKiller;      
        UBaseType_t priority;
        uint32_t taskStackSize;        
    } ServiceParams_t;

    static ServiceParams_t mServiceParams[SharedBus::ServiceID::MAX_ID];    

    /**
    * @brief run given service.
    * This function runs the given service by initializing the service parameters and creating the task.
    * @param[in] serviceParams Service parameters    
    * @retval ESP_OK if the service is run successfully, otherwise ESP_FAIL
    */ 
    esp_err_t RunService(ServiceParams_t serviceParams);    

    /**
    * @brief Deletes a task.
    * This function deletes the specified task.
    * @param ServiceID The service ID to be deleted.
    * @return void
    */
    void KillService(const SharedBus::ServiceID &ServiceID);

    /**
     * @brief Handles the transition to the machine's start state.
     * This function is called when the state machine enters the start state. 
     * It is responsible for initializing necessary components, starting tasks, 
     * or performing any setup required for this state. 
     * @return ESP_OK on successful execution.
     * @return Appropriate error code if the state transition fails.
     */
    esp_err_t OnMachineStateStart() override;             
};