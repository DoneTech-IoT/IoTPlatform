#pragma once 
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
    explicit ServiceMngr(
        const char *TaskName,
        const SharedBus::ServiceID &ServiceID);

    ~ServiceMngr();

private:
    static const char *pTAG;  

#ifdef CONFIG_DONE_COMPONENT_LVGL
    static TaskHandle_t LVGLHandle;
#endif  //CONFIG_DONE_COMPONENT_LVGL

#ifdef CONFIG_DONE_COMPONENT_MATTER
    static TaskHandle_t MatterHandle;
#endif  //CONFIG_DONE_COMPONENT_MATTER

#ifdef CONFIG_DONE_COMPONENT_MQTT
    static TaskHandle_t MQTTHandle;
#endif  //CONFIG_DONE_COMPONENT_MQTT

    static constexpr uint32_t mServiceStackSize[SharedBus::ServiceID::MAX_ID] =
    {
        0 ,             //NO_ID
        20  * 1024,     //SERVICE_MANAGER
        100 * 1024,     //UI
        50  * 1024,     //MATTER
        50  * 1024,     //MQTT
        0               //LOG
    };

    typedef void (*TaskCreatorPtr)(void);
    typedef void (*TaskKillerPtr)(TaskHandle_t *);
    typedef esp_err_t (*TaskInitPtr)(
                            TaskHandle_t *taskHandler,
                            UBaseType_t taskPriority,
                            uint32_t taskStackSize);
    typedef struct
    {           
        char name[32];  //Service name    
        uint8_t id;     //Service if from SharedBus::ServiceID
        UBaseType_t priority;     //Priority of the task
        TaskHandle_t taskHandler; //Pointer to the task handler function
        TaskCreatorPtr taskCreator;        
        TaskKillerPtr taskKiller;
        TaskInitPtr taskInit;        
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
     * @brief Starts all registered daemon services.
     * This function iterates through all registered services and calls their `Start()` method.
     * Each service is expected to be a subclass of `ServiceBase` and implement its own startup logic.     
     */
    void RunAllDaemons();

    /**
    * @brief Deletes a task.
    * This function deletes the specified task.
    * @param ServiceID The service ID to be deleted.
    * @return void
    */
    void KillService(const SharedBus::ServiceID &ServiceID);
};