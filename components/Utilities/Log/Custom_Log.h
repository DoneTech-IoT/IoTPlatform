#ifndef _LOG_H_
#define _LOG_H_
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_system.h"
#include <esp_event.h>
#include <sys/param.h>
#include <stdint.h>

#define CONFIG_DONE_LOG_RAM
#define CONFIG_DONE_OATH_LOG_RAM
#define CONFIG_DONE_GUI_LOG_RAM
#define CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
#define CONFIG_DONE_MAIN_LOG_RAM

#define LOG_MAX_EVENT 40
#define LOG_MAX_COMPONENT 3
#define STRING_LEN 20
#define ERROR_CODE 100
typedef enum
{
    LogEnd = 0,
    LogStart
} LogFlag;
typedef struct
{
    size_t Psram;
    size_t Sram;
} Ram_srt;
typedef struct
{
    Ram_srt RAM;
    char Name[STRING_LEN];
    size_t TimeStamp;
    uint8_t Counter;
} Event_str;
typedef struct
{
    Event_str Event[LOG_MAX_EVENT];
    char Name[STRING_LEN];

} Component_str;
typedef struct
{
    Component_str Component[LOG_MAX_COMPONENT];
} Log;
void RamOccupyFunction(char *Component, char *EventName);
void RamStatusFunction(char *Component, char *EventName);
void ReportRamStatus(char *ComponentName);
#endif