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




TaskHandle_t xTaskGUIHandler;
#include <stdint.h>

// Define enum for RAM type
// typedef enum {
//     PSRAM,
//     SRAM
// } RAM_Type;

// // Define struct for Task
// typedef struct {
//     char name[32];        // Task name
//     uint8_t priority;     // Priority of the task
//     uint32_t startupRAM;  // Amount of RAM needed at startup time
//     void (*taskHandler)(void*); // Pointer to the task handler function
//     RAM_Type ramType;     // RAM type where task occupies (PSRAM or SRAM)
// } Task;

// // Define struct for Task Manager
// typedef struct {
//     Task tasks[10];       // Array of tasks (assuming a maximum of 10 tasks)
//     uint32_t maxSRAM;     // Maximum SRAM needed by any task
// } ServiceManger;
// void testServiceManger();

#endif