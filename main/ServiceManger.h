#ifndef TASK_MANGER_H_
#define TASK_MANGER_H_
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>

#include <stdint.h>

#define LVGL_STACK 2500
typedef enum
{
    PSRAM_,
    SRAM_
} RAM_Types;

typedef void (*TaskKillerPtr)(TaskHandle_t * );
typedef struct
{
    char name[32];        // Task name
    UBaseType_t priority; // Priority of the task
    uint32_t startupRAM;  // Amount of RAM needed at startup time
    TaskHandle_t taskHandler;
    TaskKillerPtr TaskKiller; // Pointer to the task handler function
    RAM_Types ramType;         // RAM type where task occupies (PSRAM or SRAM)
    uint32_t taskStack;
    uint32_t maximumRAM_Needed; // Maximum SRAM needed by any task
} Task;

typedef struct
{
    Task tasks[10]; // Array of tasks (assuming a maximum of 10 tasks)
} ServiceManger;
void testServiceManger();

#endif