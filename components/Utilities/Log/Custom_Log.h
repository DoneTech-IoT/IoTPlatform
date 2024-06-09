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

// #define CONFIG_DONE_LOG_RAM
// #define CONFIG_DONE_OATH_LOG_RAM
// #define CONFIG_DONE_GUI_LOG_RAM
// #define CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
// #define CONFIG_DONE_MAIN_LOG_RAM

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
void RamStatusFunction(char *ComponentName, char *EventName);
#endif











// /**
//  * @brief Finds the location of a component in the pool.
//  * This function returns the index of a component with the specified name within the log.
//  * @param Log Pointer to the Log structure.
//  * @param Component Name of the component to find.
//  * @return Index of the component if found, ERROR_CODE otherwise.
//  */
// uint8_t FindComponentLocationInPool(Log *Log, char *Component);

// /**
//  * @brief Finds an empty place in the component pool.
//  * This function returns the index of the first empty component slot in the log.
//  * @param Log Pointer to the Log structure.
//  * @return Index of the empty component slot if found, ERROR_CODE otherwise.
//  */
// uint8_t EmptyPlaceInComponentPool(Log *Log);

// /**
//  * @brief Finds the index of an event within a component.
//  * This function returns the index of an event with the specified name within a given component.
//  * @param Log Pointer to the Log structure.
//  * @param ComponentNumber Index of the component to check.
//  * @param EventName Name of the event to find.
//  * @return Index of the event if found, ERROR_CODE otherwise.
//  */
// uint8_t FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName);

// /**
//  * @brief Records the status of an event.
//  * This function logs the current RAM usage for a given event and component.
//  * @param Log Pointer to the Log structure.
//  * @param ComponentNumber Index of the component.
//  * @param EventNumber Index of the event.
//  * @param FistTimeFlag Indicates if this is the first time logging the event (LogStart or LogEnd).
//  * @return void
//  */
// void RecordStatus(Log *Log, int ComponentNumber, int EventNumber, int FistTimeFlag);

// /**
//  * @brief Checks if an event exists within a component.
//  * This function checks if an event with the specified name exists within a given component.
//  * @param Log Pointer to the Log structure.
//  * @param ComponentNumber Index of the component to check.
//  * @param EventName Name of the event to find.
//  * @return true if the event exists, false otherwise.
//  */
// uint8_t IsEventExist(Log *Log, int ComponentNumber, char *EventName);

// /**
//  * @brief Checks if a component exists in the log.
//  * This function checks if a component with the specified name exists within the log.
//  * @param Log Pointer to the Log structure.
//  * @param ComponentName Name of the component to find.
//  * @return true if the component exists, false otherwise.
//  */
// uint8_t IsComponentExist(Log *Log, char *ComponentName);

// /**
//  * @brief Handles RAM occupation for a component and event.
//  * This function manages RAM occupation by logging the status of a specified component and event,
//  * including whether the event is new or existing.
//  * @param Component Name of the component.
//  * @param EventName Name of the event.
//  * @return void
//  */
// void RamOccupyFunction(char *Component, char *EventName);

// /**
//  * @brief Logs the status of RAM usage for a component and event.
//  * This function logs the current RAM usage and timestamp for a specified component and event.
//  * @param ComponentName Name of the component.
//  * @param EventName Name of the event.
//  * @return void
//  */
// void RamStatusFunction(char *ComponentName, char *EventName);

// /**
//  * @brief Finds the number of saved events in a component.
//  * This function returns the number of non-empty event slots in a given component.
//  * @param Log Pointer to the Log structure.
//  * @param ComponentName Name of the component.
//  * @return Number of saved events in the component.
//  */
// uint8_t NumberSavedEvent(Log *Log, char *ComponentName);

// /**
//  * @brief Reports the RAM usage of a component.
//  * This function prints the RAM usage for all events within a specified component.
//  * @param ComponentName Name of the component to report.
//  * @return void
//  */
// void ReportComponentRamUsed(char *ComponentName);

// /**
//  * @brief Reports the RAM status of a component.
//  * This function prints the RAM usage status for all events within a specified component.
//  * @param ComponentName Name of the component to report.
//  * @return void
//  */
// void ReportComponentRamStatus(char *ComponentName);

// /**
//  * @brief Finds an empty place in the event pool of a component.
//  * This function returns the index of the first empty event slot in a given component.
//  * @param Log Pointer to the Log structure.
//  * @param ComponentNumber Index of the component.
//  * @return Index of the empty event slot if found, ERROR_CODE otherwise.
//  */
// uint8_t EmptyPlaceInEventPool(Log *Log, int ComponentNumber);
