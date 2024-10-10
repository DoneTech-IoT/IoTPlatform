#ifndef TASK_MANGER_H_
#define TASK_MANGER_H_
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

#include "NetworkChecker.h"
#include "nvsFlash.h"
#ifdef CONFIG_DONE_COMPONENT_LVGL
#include "GUI.h"
#endif
#ifdef CONFIG_DONE_COMPONENT_MATTER
#include "MatterInterface.h"
#else
#include "esp_netif.h"
#include "protocol_examples_common.h"
#endif
#ifdef CONFIG_DONE_LOG
#include "Custom_Log.h"
#endif

#ifdef CONFIG_DONE_COMPONENT_MQTT
#include "MQTT_Interface.h"
#endif

#define LVGL_STACK 100 * 1000 // in word not byte
#define SERVICE_MANGER_STACK 30 * 1000
#define NUMBER_OF_COSTUME_TASK 10
#define SERVICE_MANAGER_SEC 1000
#define MATTER_STACK_SIZE 1024 * 4
#define MQTT_STACK 30 * 1000

typedef enum
{
    GUI_Task = 0,
    MatterTask = 1,
    MQTTTask = 2,
} ServiceID;
typedef enum
{
    PSRAM_,
    SRAM_
} RAM_Types;

typedef void (*TaskCreatorPtr)(void);
typedef void (*TaskKillerPtr)(TaskHandle_t *);
typedef esp_err_t (*TaskInitPtr)(void*, 
                            TaskHandle_t *taskHandler,
                            UBaseType_t taskPriority,
                            uint32_t taskStack);
typedef struct
{
    char name[32];            // Task name
    void *interfaceHandler;                             // Interface handler
    UBaseType_t priority;     // Priority of the task
    uint32_t startupRAM;      // Amount of RAM needed at startup time
    TaskHandle_t taskHandler; // Pointer to the task handler function
    TaskCreatorPtr TaskCreator;
    TaskKillerPtr TaskKiller;
    TaskInitPtr TaskInit;
    RAM_Types ramType; // RAM type where task occupies (PSRAM or SRAM)
    uint32_t taskStack;
    uint32_t maximumRAM_Needed; // Maximum SRAM needed by any task
} ServiceParams_t;

typedef struct
{
    ServiceParams_t Services[NUMBER_OF_COSTUME_TASK];   // Array of tasks (assuming a maximum of 10 tasks)
    // uint8_t numberOfTasks;                       // Number of tasks
} ServiceManger_t;

/**
 * @brief Initializes the Service Manager task.
 * This function initializes the Service Manager task by allocating memory and creating the task.
 * @return void
 */
void ServiceManger_Init();

/* 
    * @brief run given service.
    * This function runs the given service by initializing the service parameters and creating the task.
    * @param[in] serviceParams Service parameters
    * @param[in] service Service to run
    * @retval ESP_OK if the service is run successfully, otherwise ESP_FAIL
*/ 
esp_err_t ServiceManager_RunService(ServiceParams_t serviceParams);
#endif