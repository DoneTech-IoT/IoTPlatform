#include "Custom_Log.h"
#include "string.h"

const char *TAG = "Heap log";
#ifdef CONFIG_DONE_LOG
static Log log2;
static Log log;
#endif
uint8_t Log_FindComponentLocationInPool(Log *Log, char *Component);
uint8_t Log_EmptyPlaceInComponentPool(Log *Log);
uint8_t Log_FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName);
uint8_t Log_EmptyPlaceInEventPool(Log *Log, int ComponentNumber);
uint8_t Log_IsEventExist(Log *Log, int ComponentNumber, char *EventName);
uint8_t Log_IsComponentExist(Log *Log, char *ComponentName);
uint8_t Log_NumberSavedEvent(Log *Log, int ComponentNumber);
void Log_RamStatus(char *ComponentName, char *EventName);
void Log_RecordStatus(Log *Log, int ComponentNumber, int EventNumber, int FistTimeFlag);

/**
 * @brief Occupies RAM for a component and event.
 * This function manages the RAM occupation for a specified component and event,
 * creating new entries or updating existing ones as necessary.
 * @param Component Name of the component.
 * @param EventName Name of the event.
 * @return void
 */
void Log_RamOccupy(char *Component, char *EventName)
{
#ifdef CONFIG_DONE_LOG
    int componentNumber;
    int eventNumber;
    if (Log_IsComponentExist(&log, Component) == false)
    {
        componentNumber = 0;
        eventNumber = 0;
        strncpy(log.Component[componentNumber].Name, Component, STRING_LEN);
        strncpy(log.Component[componentNumber].Event[eventNumber].Name,
                EventName, STRING_LEN);
        Log_RecordStatus(&log, componentNumber, eventNumber, LogStart);
        return;
    }
    componentNumber = Log_FindComponentLocationInPool(&log, Component);
    if (componentNumber == ERROR_CODE)
    {
        ESP_LOGE(TAG, " there is not any space in Component pool for creating new component");
        return;
    }
    uint8_t isEventExist = Log_IsEventExist(&log, componentNumber, EventName);
    if (isEventExist == 1)
    {
        eventNumber = Log_FindEventInEventPool(&log, componentNumber, EventName);
        strncpy(log.Component[componentNumber].Event[eventNumber].Name,
                EventName, STRING_LEN);
        if (log.Component[componentNumber].Event[eventNumber].Counter == 2)
            Log_RecordStatus(&log, componentNumber, eventNumber, LogStart); // add log
        else
            Log_RecordStatus(&log, componentNumber, eventNumber, LogEnd); // add log
    }
    else
    {
        eventNumber = Log_EmptyPlaceInEventPool(&log, componentNumber);
        printf("State 8:ComponentNumber: %d, EventNumber: %d, EventName: %s\n", componentNumber, eventNumber, log.Component[componentNumber].Event[eventNumber].Name);
        if (eventNumber == ERROR_CODE)
        {
            ESP_LOGE(TAG, "  there is not any space in Event pool for creating new event");
            return;
        }
        strncpy(log.Component[componentNumber].Event[eventNumber].Name, EventName, STRING_LEN);
        Log_RecordStatus(&log, componentNumber, eventNumber, LogStart); // add log
    }
#endif
}

/**
 * @brief Records the status of an event.
 * This function logs the current RAM usage for a given event and component.
 * @param Log Pointer to the Log structure.
 * @param ComponentNumber Index of the component.
 * @param EventNumber Index of the event.
 * @param FirstTimeFlag Indicates if this is the first time logging the event (LogStart or LogEnd).
 * @return void
 */
void Log_RecordStatus(Log *Log, int ComponentNumber, int EventNumber,
                      int FistTimeFlag)
{

    if (FistTimeFlag == 1)
    {
        Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram =
            (esp_get_free_heap_size() / 1000);
        Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram =
            (xPortGetFreeHeapSize() / 1000);
        Log->Component[ComponentNumber].Event[EventNumber].Counter = 1;
    }
    else if (FistTimeFlag == 0)
    {
        size_t psramSize =
            Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
        size_t sramSize =
            Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
        psramSize = psramSize - (esp_get_free_heap_size() / 1000);
        sramSize = sramSize - (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
#ifdef CONFIG_DONE_LOG_PRINT
        ESP_LOGE(TAG, "Event name :%s SRAM: %u K bytes PSRAM occupy: %u K bytes occupy at %u millis",
                 Log->Component[ComponentNumber].Event[EventNumber].Name, sramSize,
                 psramSize, TimeFromBootUp);
#endif
        Log->Component[ComponentNumber].Event[EventNumber].Counter = 2;
    }
}

/**
 * @brief Checks if a component exists in the log.
 * This function checks if a component with the specified name exists within the log.
 * @param Log Pointer to the Log structure.
 * @param ComponentName Name of the component to find.
 * @return true if the component exists, false otherwise.
 */
uint8_t Log_IsComponentExist(Log *Log, char *ComponentName)
{
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (0 == (strcmp(Log->Component[i].Name, ComponentName)))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if an event exists within a component.
 * This function checks if an event with the specified name exists within a given component.
 * @param Log Pointer to the Log structure.
 * @param ComponentNumber Index of the component to check.
 * @param EventName Name of the event to find.
 * @return true if the event exists, false otherwise.
 */
uint8_t Log_IsEventExist(Log *Log, int ComponentNumber, char *EventName)
{
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (0 ==
            (strcmp(Log->Component[ComponentNumber].Event[i].Name, EventName)))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Finds the index of an event within a component.
 * This function returns the index of an event with the specified name within a given component.
 * @param Log Pointer to the Log structure.
 * @param ComponentNumber Index of the component to check.
 * @param EventName Name of the event to find.
 * @return Index of the event if found, ERROR_CODE otherwise.
 */
uint8_t Log_FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName)
{
    uint8_t eventNumber = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (0 == (strcmp(Log->Component[ComponentNumber].Event[i].Name, EventName)))
        {
            eventNumber = i;
            break;
        }
    }
    return eventNumber;
}

/**
 * @brief Finds the location of a component in the pool.
 * This function returns the index of a component with the specified name within the log.
 * @param Log Pointer to the Log structure.
 * @param Component Name of the component to find.
 * @return Index of the component if found, ERROR_CODE otherwise.
 */
uint8_t Log_FindComponentLocationInPool(Log *Log, char *Component)
{
    uint8_t componentNumber = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (0 == (strcmp(Log->Component[i].Name, Component)))
        {
            componentNumber = i;
        }
    }
    return componentNumber;
}

/**
 * @brief Finds an empty place in the component pool.
 * This function returns the index of the first empty component slot in the log.
 * @param Log Pointer to the Log structure.
 * @return Index of the empty component slot if found, ERROR_CODE otherwise.
 */
uint8_t Log_EmptyPlaceInComponentPool(Log *Log)
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

/**
 * @brief Finds an empty place in the event pool of a component.
 * This function returns the index of the first empty event slot in a given component.
 * @param Log Pointer to the Log structure.
 * @param ComponentNumber Index of the component.
 * @return Index of the empty event slot if found, ERROR_CODE otherwise.
 */
uint8_t Log_EmptyPlaceInEventPool(Log *Log, int ComponentNumber)
{
    uint8_t emptyPlace = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (strlen(Log->Component[ComponentNumber].Event[i].Name) == 0)
        {
            emptyPlace = i;
            break;
        }
    }
    return emptyPlace;
}

/**
 * @brief Finds the number of saved events in a component.
 * This function returns the number of non-empty event slots in a given component.
 * @param Log Pointer to the Log structure.
 * @param ComponentName Name of the component.
 * @return Number of saved events in the component.
 */
uint8_t Log_NumberSavedEvent(Log *Log, int ComponentNumber)
{
    uint8_t numberOfEvent = 0;
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (strlen(Log->Component[ComponentNumber].Event[i].Name) != 0)
            numberOfEvent++;
    }
    return numberOfEvent;
}

/**
 * @brief Logs the status of RAM usage for a component and event.
 * This function logs the current RAM usage and timestamp for a specified component and event.
 * @param ComponentName Name of the component.
 * @param EventName Name of the event.
 * @return void
 */
void Log_RamStatus(char *ComponentName, char *EventName)
{
#ifdef CONFIG_DONE_LOG
    int componentNumber;
    int eventNumber;
    size_t psramSize;
    size_t sramSize;
    if (Log_IsComponentExist(&log2, ComponentName) == false)
    {
        componentNumber = 0;
        eventNumber = 0;
        psramSize = (esp_get_free_heap_size() / 1000);
        sramSize = (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        strncpy(log2.Component[componentNumber].Name, ComponentName,
                strlen(ComponentName));
        strncpy(log2.Component[componentNumber].Event[eventNumber].Name,
                EventName, strlen(EventName));
        log2.Component[componentNumber].Event[eventNumber].RAM.Psram =
            psramSize;
        log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
        log2.Component[componentNumber].Event[eventNumber].TimeStamp =
            TimeFromBootUp;
#ifdef CONFIG_DONE_LOG_PRINT
        ESP_LOGE(TAG, "component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
                 ComponentName, EventName, sramSize, psramSize, TimeFromBootUp);
#endif
    }
    else
    {
        psramSize = (esp_get_free_heap_size() / 1000);
        sramSize = (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        componentNumber = Log_FindComponentLocationInPool(&log2, ComponentName);
        eventNumber = Log_EmptyPlaceInEventPool(&log2, componentNumber);
        if (Log_NumberSavedEvent(&log2, componentNumber) >= (LOG_MAX_EVENT - 1))
            memset(&log2.Component[componentNumber], 0,
                   sizeof(log2.Component[componentNumber]));
        strncpy(log2.Component[componentNumber].Name, ComponentName,
                strlen(ComponentName));
        strncpy(log2.Component[componentNumber].Event[eventNumber].Name,
                EventName, strlen(EventName));
        log2.Component[componentNumber].Event[eventNumber].RAM.Psram =
            psramSize;
        log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
        log2.Component[componentNumber].Event[eventNumber].TimeStamp =
            TimeFromBootUp;
#ifdef CONFIG_DONE_LOG_PRINT
        ESP_LOGE(TAG, "component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
                 ComponentName, EventName, sramSize, psramSize, TimeFromBootUp);
#endif
    }
#endif
}

/**
 * @brief Reports the RAM status of a component.
 * This function prints the RAM usage status for all events within a specified component.
 * @param ComponentName Name of the component to report.
 * @return void
 */
void Log_ReportComponentRamStatus(char *ComponentName)
{
#ifdef CONFIG_DONE_LOG
    int componentNumber;
    if (Log_IsComponentExist(&log2, ComponentName) == false)
    {
        ESP_LOGE(TAG, "there is not any Log for Component %s", ComponentName);
    }
    else
    {
        componentNumber = Log_FindComponentLocationInPool(&log2, ComponentName);
        for (int i = 0; i < LOG_MAX_EVENT; i++)
        {
            if (strlen(log2.Component[componentNumber].Event[i].Name) != 0)
            {
                ESP_LOGE(TAG, "in the report component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
                         ComponentName, log2.Component[componentNumber].Event[i].Name,
                         log2.Component[componentNumber].Event[i].RAM.Sram,
                         log2.Component[componentNumber].Event[i].RAM.Psram,
                         log2.Component[componentNumber].Event[i].TimeStamp);
            }
            else
                break;
        }
    }
#endif
}

/**
 * @brief Reports the RAM usage of a component.
 * This function prints the RAM usage for all events within a specified component.
 * @param ComponentName Name of the component to report.
 * @return void
 */
void Log_ReportComponentRamUsed(char *ComponentName)
{
#ifdef CONFIG_DONE_LOG
    int componentNumber;
    if (Log_IsComponentExist(&log, ComponentName) == false)
    {
        ESP_LOGE(TAG, "there is not any Log for Component %s", ComponentName);
    }
    else
    {
        componentNumber = Log_FindComponentLocationInPool(&log, ComponentName);
        for (int i = 0; i < LOG_MAX_EVENT; i++)
        {
            if (strlen(log.Component[componentNumber].Event[i].Name) != 0)
            {
                ESP_LOGE(TAG, "component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is occupy",
                         ComponentName, log.Component[componentNumber].Event[i].Name,
                         log.Component[componentNumber].Event[i].RAM.Sram,
                         log.Component[componentNumber].Event[i].RAM.Psram,
                         log.Component[componentNumber].Event[i].TimeStamp);
            }
            else
                break;
        }
    }
#endif
}
