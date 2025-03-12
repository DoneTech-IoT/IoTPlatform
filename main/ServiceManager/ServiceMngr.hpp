#pragma once 
#include <stdint.h>
#include <stdio.h>
#include "string.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"

#include <esp_event.h>
#include "esp_log.h"
#include "esp_system.h"
#include <sys/param.h>

#define NUMBER_OF_COSTUME_TASK 10
#define LVGL_STACK 100 * 1024 // in word not byte
#define SERVICE_MANGER_STACK 20 * 1024
#define MATTER_STACK_SIZE 50 * 1024
#define MQTT_STACK 50 * 1024

#include "ServiceBase.hpp"
#include "SharedBus.hpp"

class ServiceMngr : public ServiceBase
{
public:
    explicit ServiceMngr(
        const char *TaskName,
        const SharedBus::ServiceID &ServiceID);

    ~ServiceMngr() {}

private:
    static const char *pTAG;  
    static constexpr uint32_t mServiceStackSize[SharedBus::ServiceID::MAX_ID] =
    [
        0 ,             //NO_ID
        20  * 1024,     //SERVICE_MANAGER
        100 * 1024,     //UI
        50  * 1024,     //MATTER
        50  * 1024,     //MQTT
        0               //LOG
    ];

    typedef void (*TaskCreatorPtr)(void);
    typedef void (*TaskKillerPtr)(TaskHandle_t *);
    typedef esp_err_t (*TaskInitPtr)(
                            TaskHandle_t *taskHandler,
                            UBaseType_t taskPriority,
                            uint32_t taskStackSize);
    typedef struct
    {        
        UBaseType_t priority;     
        TaskHandle_t taskHandler; 
        TaskCreatorPtr taskCreator;        
        TaskKillerPtr taskKiller;
        TaskInitPtr taskInit;        
        uint32_t taskStackSize;        
    } ServiceParams;
};