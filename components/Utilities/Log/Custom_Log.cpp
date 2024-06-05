#include "Custom_Log.h"
#include "string.h"

#define LOG_MAX_EVENT 20
#define LOG_MAX_COMPONENT 3
#define STRING_LEN 20
#define ERROR_CODE 100
typedef struct
{
    size_t Psram;
    size_t Sram;
} Ram_srt;
typedef struct
{
    Ram_srt RAM;
    char Name[STRING_LEN];
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
uint8_t FindComponentLocationInPool(Log *Log, char *Component);
uint8_t EmptyPlaceInComponentPool(Log *Log);
uint8_t FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName);
void RecordStatus(Log *Log, int ComponentNumber, int EventNumber, int FistTimeFlag);
uint8_t IsEventExist(Log *Log, int ComponentNumber, char *EventName);
void RamOccupyFunction(char *Component, char *EventName)
{
    static Log Log;
    int componentNumber;
    int eventNumber;
    componentNumber = FindComponentLocationInPool(&Log, Component);
    if (componentNumber == 0) // this is new component
    {
        componentNumber = EmptyPlaceInComponentPool(&Log);
        strncpy(Log.Component[componentNumber].Name, Component, strlen(Component));
        // add event without finding space because new component don't any event
        strncpy(Log.Component[componentNumber].Event[0].Name, EventName, strlen(EventName));
        RecordStatus(&Log, componentNumber, 0, LogStart);
    }
    else if (componentNumber == ERROR_CODE)
    {
        ESP_LOGE(Component, "there is not any espace in Component pool for creating new component");
        return;
    }
    else if (componentNumber != ERROR_CODE || componentNumber != 0) // existence component
    {
        if (IsEventExist(&Log, componentNumber, EventName))
        {
            eventNumber = FindEventInEventPool(&Log, componentNumber, EventName);
            strncpy(Log.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
            RecordStatus(&Log, componentNumber, eventNumber, LogEnd); // add log
        }
        else
        {
            eventNumber = FindEventInEventPool(&Log, componentNumber, EventName);
            if (eventNumber == ERROR_CODE)
            {
                ESP_LOGE(Component, "there is not any espace in Event pool creating new event");
                return;
            }
            else
            {
                strncpy(Log.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
                RecordStatus(&Log, componentNumber, eventNumber, LogStart); // add log
            }
        }
    }
}
void RecordStatus(Log *Log, int ComponentNumber, int EventNumber, int FistTimeFlag)
{
    if (FistTimeFlag == 1)
    {
        Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram = (esp_get_free_heap_size() / 1000);
        Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram = (xPortGetFreeHeapSize() / 1000);
    }
    else if (FistTimeFlag == 0)
    {
        size_t PsramSize = Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
        size_t SramSize = Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
        PsramSize = PsramSize - Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
        SramSize = SramSize - Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        ESP_LOGE(Log->Component[ComponentNumber].Name, "Event name :%s SRAM: %u K bytes PSRAM occupy: %u K bytes occupy at %u millis", Log->Component[ComponentNumber].Event[EventNumber].Name, SramSize, PsramSize, TimeFromBootUp);
    }
}
uint8_t IsEventExist(Log *Log, int ComponentNumber, char *EventName)
{
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (!(strcmp(Log->Component[ComponentNumber].Event[i].Name, EventName)))
        {
            return true;
        }
    }
    return false;
}
uint8_t FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName)
{
    uint8_t eventNumber = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (!(strcmp(Log->Component[ComponentNumber].Event[i].Name, EventName)))
        {
            eventNumber = i;
            break;
        }
    }
    return eventNumber;
}
uint8_t FindComponentLocationInPool(Log *Log, char *Component)
{
    uint8_t componentNumber = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (!(strcmp(Log->Component[i].Name, Component)))
        {
            componentNumber = i;
        }
    }
    return componentNumber;
}
uint8_t EmptyPlaceInComponentPool(Log *Log)
{
    uint8_t emptyPlace = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (strlen(Log->Component[i].Name) == 0)
        {
            emptyPlace = i;
            break;
        }
    }
    return emptyPlace;
}
// void RamOccupyFunction(bool TimingFlag, char *Component, char *ProgramStage)
// {
//     Log Log;
// #ifdef CONFIG_DONE_LOG_RAM
//     static size_t PsramSize;
//     static size_t SramSize;
//     static char LastLogTag[20];
//     if (TimingFlag == 1)
//     {
//         PsramSize = 0;
//         SramSize = 0;
//         PsramSize = (esp_get_free_heap_size() / 1000);
//         SramSize = (xPortGetFreeHeapSize() / 1000);
//         strcpy(LastLogTag, Component);
//     }
//     else if (TimingFlag == 0 && !(strcmp(LastLogTag, Component)))
//     {
//         char TAG[100];
//         ESP_LOGE(Component, "SRAM: %u K bytes PSRAM: %u K bytes  at ", SramSize, PsramSize);
//         PsramSize = PsramSize - (esp_get_free_heap_size() / 1000);
//         SramSize = SramSize - (xPortGetFreeHeapSize() / 1000);
//         size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
//         strcpy(TAG, Component);
//         strcat(TAG, " , ");
//         strcat(TAG, ProgramStage);
//         ESP_LOGE(Component, "SRAM: %u K bytes PSRAM occupy: %u K bytes occupy at %u millis", SramSize, PsramSize, TimeFromBootUp);
//     }
// #endif
// }

// void RamStatusFunction(char *Component, char *ProgramStage)
// {
// #ifdef CONFIG_DONE_LOG_RAM
//     char TAG[100];
//     size_t PsramSize;
//     size_t SramSize;
//     PsramSize = (esp_get_free_heap_size() / 1000);
//     SramSize = (xPortGetFreeHeapSize() / 1000);
//     int TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
//     strcpy(TAG, Component);
//     strcat(TAG, " , ");
//     strcat(TAG, ProgramStage);
//     ESP_LOGE(TAG, "SRAM: %u K bytes , PSRAM: %u K bytes at %d millis", SramSize, PsramSize, TimeFromBootUp);
// #endif
// }

// void RamOStatus(char *ProgramStage)
// {
// #ifdef CONFIG_DONE_OATH_LOG_RAM
//     RamStatusFunction("OAth", ProgramStage);
// #endif
// #ifdef CONFIG_DONE_GUI_LOG_RAM
//     RamStatusFunction("GUI", ProgramStage);
// #endif
// #ifdef CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
//     RamStatusFunction("Matter_Interface", ProgramStage);
// #endif
// #ifdef CONFIG_DONE_MAIN_LOG_RAM
//     RamStatusFunction("Main", ProgramStage);
// #endif
// }

// void RamOccupy(bool TimingFlag, char *ProgramStage)
// {
// #ifdef CONFIG_DONE_OATH_LOG_RAM
//     RamOccupyFunction(TimingFlag, "OAth", ProgramStage);
// #endif
// #ifdef CONFIG_DONE_GUI_LOG_RAM
//     RamOccupyFunction(TimingFlag, "GUI", ProgramStage);
// #endif
// #ifdef CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
//     RamOccupyFunction(TimingFlag, "Matter_Interface", ProgramStage);
// #endif
// #ifdef CONFIG_DONE_MAIN_LOG_RAM
//     RamOccupyFunction(TimingFlag, "Main", ProgramStage);
// #endif
// }

#ifdef _TEST_
// Test function for EmptyPlaceInComponentPool
void test_WhereIsEmptyInComponentPool()
{
    Log log = {0}; // Initialize the log with empty components

    // Initially, all components are empty
    uint8_t result = EmptyPlaceInComponentPool(&log);
    printf("Test 1: Expecting 0, got %d\n", result);

    // Fill the first component
    strncpy(log.Component[0].Name, "Component1", STRING_LEN);
    result = EmptyPlaceInComponentPool(&log);
    printf("Test 2: Expecting 1, got %d\n", result);

    // Fill the second component
    strncpy(log.Component[1].Name, "Component2", STRING_LEN);
    result = EmptyPlaceInComponentPool(&log);
    printf("Test 3: Expecting 2, got %d\n", result);

    // Fill all components
    strncpy(log.Component[2].Name, "Component3", STRING_LEN);
    result = EmptyPlaceInComponentPool(&log);
    printf("Test 4: Expecting %d (ERROR_CODE), got %d\n", ERROR_CODE, result);
}

// Test function for FindEventInEventPool
void test_FindEventInEventPool()
{
    Log log = {0}; // Initialize the log with empty components and events

    // Fill the first component with some events
    strncpy(log.Component[0].Name, "Component1", STRING_LEN);
    strncpy(log.Component[0].Event[0].Name, "Event1", STRING_LEN);
    strncpy(log.Component[0].Event[1].Name, "Event2", STRING_LEN);
    strncpy(log.Component[0].Event[2].Name, "Event3", STRING_LEN);

    // Test for an existing event
    uint8_t result = FindEventInEventPool(&log, 0, "Event2");
    printf("Test 1: Expecting 1, got %d\n", result);

    // Test for a non-existing event
    result = FindEventInEventPool(&log, 0, "Event4");
    printf("Test 2: Expecting %d (ERROR_CODE), got %d\n", ERROR_CODE, result);

    // Test for an event in a non-existing component
    result = FindEventInEventPool(&log, 0, "Event1");
    printf("Test 3: Expecting %d , got %d\n", result);

    // Test for an existing event in another component
    strncpy(log.Component[1].Name, "Component2", STRING_LEN);
    strncpy(log.Component[1].Event[0].Name, "EventA", STRING_LEN);
    result = FindEventInEventPool(&log, 1, "EventA");
    printf("Test 4: Expecting 0, got %d\n", result);
}

// Test function for FindComponentLocationInPool
void test_FindComponentLocationInPool()
{
    Log log = {0}; // Initialize the log with empty components

    // Fill the first component
    strncpy(log.Component[0].Name, "Component1", STRING_LEN);

    // Test for an existing component
    uint8_t result = FindComponentLocationInPool(&log, "Component1");
    printf("Test 1: Expecting 0, got %d\n", result);

    // Test for a non-existing component
    result = FindComponentLocationInPool(&log, "Component2");
    printf("Test 2: Expecting %d (ERROR_CODE), got %d\n", ERROR_CODE, result);

    // Fill more components
    strncpy(log.Component[1].Name, "Component2", STRING_LEN);
    strncpy(log.Component[2].Name, "Component3", STRING_LEN);

    // Test for another existing component
    result = FindComponentLocationInPool(&log, "Component3");
    printf("Test 3: Expecting 2, got %d\n", result);

    // Test for a non-existing component after filling
    result = FindComponentLocationInPool(&log, "Component4");
    printf("Test 4: Expecting %d (ERROR_CODE), got %d\n", ERROR_CODE, result);
}
#endif