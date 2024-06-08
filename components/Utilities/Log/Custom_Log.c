#include "Custom_Log.h"
#include "string.h"
const char *TAG = "Heap log";
static Log Log2;
static Log log;

uint8_t FindComponentLocationInPool(Log *Log, char *Component);
uint8_t EmptyPlaceInComponentPool(Log *Log);
uint8_t FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName);
void RecordStatus(Log *Log, int ComponentNumber, int EventNumber, int FistTimeFlag);
uint8_t IsEventExist(Log *Log, int ComponentNumber, char *EventName);
uint8_t IsComponentExist(Log *Log, char *ComponentName);
void RamOccupyFunction(char *Component, char *EventName)
{
    ESP_LOGW("log test", "state  0");
    int componentNumber;
    int eventNumber;
    if (IsComponentExist(&log, Component) == false)
    {
        componentNumber = 0;
        eventNumber = 0;
        ESP_LOGW("log test", "state  1 there is not any component ");
        strncpy(log.Component[componentNumber].Name, Component, strlen(Component));
        strncpy(log.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
        ESP_LOGW("log test", "state  2 ,componentNumber=%d , event name = %s", componentNumber, log.Component[componentNumber].Event[eventNumber].Name);
        RecordStatus(&log, componentNumber, eventNumber, 1);
    }
    else
    {
        componentNumber = FindComponentLocationInPool(&log, Component);
        if (componentNumber == 0)
        {
            eventNumber = 0;
            ESP_LOGW("log test", "state  6");
            strncpy(log.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
            RecordStatus(&log, componentNumber, eventNumber, 0); // add log
        }
        else if (componentNumber == ERROR_CODE)
        {
            ESP_LOGE(Component, "there is not any espace in Component pool for creating new component");
            return;
        }
        else if (componentNumber != ERROR_CODE || componentNumber != 0) // existence component
        {
            if (IsEventExist(&log, componentNumber, EventName))
            {
                ESP_LOGW("log test", "state  4 IsEventExist=true");
                eventNumber = FindEventInEventPool(&log, componentNumber, EventName);
                strncpy(log.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
                RecordStatus(&log, componentNumber, eventNumber, LogEnd); // add log
            }
            else
            {
                ESP_LOGW("log test", "state  4 IsEventExist=flase");
                eventNumber = FindEventInEventPool(&log, componentNumber, EventName);
                ESP_LOGW("log test", "state  5 FindEventInEventPool");
                if (eventNumber == ERROR_CODE)
                {
                    ESP_LOGE(Component, "there is not any espace in Event pool creating new event");
                    return;
                }
                else
                {
                    ESP_LOGW("log test", "state  6");
                    strncpy(log.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
                    RecordStatus(&log, componentNumber, eventNumber, 0); // add log
                }
            }
        }
    }
}
void RecordStatus(Log *Log, int ComponentNumber, int EventNumber, int FistTimeFlag)
{

    if (FistTimeFlag == 1 || (Log->Component[ComponentNumber].Event[EventNumber].Counter == 2))
    {

        Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram = (esp_get_free_heap_size() / 1000);
        Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram = (xPortGetFreeHeapSize() / 1000);
        ESP_LOGW("log test", "state  3 ,SRAM: %u K bytes PSRAM K bytes occupy: %u", Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram, Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram);
        Log->Component[ComponentNumber].Event[EventNumber].Counter = 1;
    }
    else if (FistTimeFlag == 0)
    {
        size_t psramSize = Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
        size_t sramSize = Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
        psramSize = psramSize - Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
        sramSize = sramSize - Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        ESP_LOGE(Log->Component[ComponentNumber].Name, "Event name :%s SRAM: %u K bytes PSRAM occupy: %u K bytes occupy at %u millis", Log->Component[ComponentNumber].Event[EventNumber].Name, sramSize, psramSize, TimeFromBootUp);
        Log->Component[ComponentNumber].Event[EventNumber].Counter = 2;
    }
}
uint8_t IsComponentExist(Log *Log, char *ComponentName)
{
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (!(strcmp(Log->Component[i].Name, ComponentName)))
        {
            return true;
        }
    }
    return false;
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
uint8_t EmptyPlaceInEventPool(Log *Log, int ComponentNumber)
{
    uint8_t emptyPlace = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (strlen(Log->Component[ComponentNumber].Event[i].Name) == 0)
        {
            emptyPlace = i;
            break;
        }
    }
    return emptyPlace;
}
void RamStatusFunction(char *ComponentName, char *EventName)
{
    int componentNumber;
    int eventNumber;
    size_t psramSize;
    size_t sramSize;
    if (Log2.Component[componentNumber].Event[eventNumber].Counter == (LOG_MAX_EVENT - 1))
        Log2 = {0};
    if (IsComponentExist(&Log2, ComponentName) == false)
    {
        componentNumber = 0;
        eventNumber = 0;
        psramSize = (esp_get_free_heap_size() / 1000);
        sramSize = (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        strncpy(Log2.Component[componentNumber].Name, ComponentName, strlen(ComponentName));
        strncpy(Log2.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
        Log2.Component[componentNumber].Event[eventNumber].RAM.Psram = psramSize;
        Log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
        Log2.Component[componentNumber].Event[eventNumber].TimeStamp = TimeFromBootUp;
        Log2.Component[componentNumber].Event[eventNumber].Counter++;
        ESP_LOGE(TAG, "component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE", ComponentName, EventName, sramSize, psramSize, TimeFromBootUp);
    }
    else
    {
        psramSize = (esp_get_free_heap_size() / 1000);
        sramSize = (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        componentNumber = FindComponentLocationInPool(&Log2, ComponentName);
        eventNumber = EmptyPlaceInEventPool(&Log2, componentNumber);
        strncpy(Log2.Component[componentNumber].Name, ComponentName, strlen(ComponentName));
        strncpy(Log2.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
        Log2.Component[componentNumber].Event[eventNumber].RAM.Psram = psramSize;
        Log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
        Log2.Component[componentNumber].Event[eventNumber].TimeStamp = TimeFromBootUp;
        Log2.Component[componentNumber].Event[eventNumber].Counter++;
        ESP_LOGE(TAG, "component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE", ComponentName, EventName, sramSize, psramSize, TimeFromBootUp);
    }
}
void ReportRamStatus(char *ComponentName)
{
    int componentNumber;
    if (IsComponentExist(&Log2, ComponentName) == false)
    {
        ESP_LOGE(TAG, "there is not any Log for Component %s", ComponentName);
    }
    else
    {
        componentNumber = FindComponentLocationInPool(&Log2, ComponentName);
        for (int i = 0; i < LOG_MAX_EVENT; i++)
        {
            if (strlen(Log2.Component[componentNumber].Event[i].Name) != 0)
            {
                ESP_LOGE(TAG, "component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
                         ComponentName,
                         Log2.Component[componentNumber].Event[i].Name,
                         Log2.Component[componentNumber].Event[i].RAM.Sram,
                         Log2.Component[componentNumber].Event[i].RAM.Psram,
                         Log2.Component[componentNumber].Event[i].TimeStamp);
            }
            else
                break;
        }
    }
}
// void RamOccupyFunction(bool TimingFlag, char *Component, char *ProgramStage)
// {
//     Log Log;
// #ifdef CONFIG_DONE_LOG_RAM
//     static size_t psramSize;
//     static size_t sramSize;
//     static char LastLogTag[20];
//     if (TimingFlag == 1)
//     {
//         psramSize = 0;
//         sramSize = 0;
//         psramSize = (esp_get_free_heap_size() / 1000);
//         sramSize = (xPortGetFreeHeapSize() / 1000);
//         strcpy(LastLogTag, Component);
//     }
//     else if (TimingFlag == 0 && !(strcmp(LastLogTag, Component)))
//     {
//         char TAG[100];
//         ESP_LOGE(Component, "SRAM: %u K bytes PSRAM: %u K bytes  at ", sramSize, psramSize);
//         psramSize = psramSize - (esp_get_free_heap_size() / 1000);
//         sramSize = sramSize - (xPortGetFreeHeapSize() / 1000);
//         size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
//         strcpy(TAG, Component);
//         strcat(TAG, " , ");
//         strcat(TAG, ProgramStage);
//         ESP_LOGE(Component, "SRAM: %u K bytes PSRAM occupy: %u K bytes occupy at %u millis", sramSize, psramSize, TimeFromBootUp);
//     }
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