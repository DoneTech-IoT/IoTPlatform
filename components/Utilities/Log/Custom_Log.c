#include "Custom_Log.h"
#include "string.h"




const char *TAG = "Heap log";
static Log Log2;

uint8_t FindComponentLocationInPool(Log *Log, char *Component);
uint8_t EmptyPlaceInComponentPool(Log *Log);
uint8_t FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName);
void RecordStatus(Log *Log, int ComponentNumber, int EventNumber, int FistTimeFlag);
uint8_t IsEventExist(Log *Log, int ComponentNumber, char *EventName);
uint8_t IsComponentExist(Log *Log, char *ComponentName);
void RamStatusFunction(char *ComponentName, char *EventName);
uint8_t NumberSavedEvent(Log *Log, int ComponentNumber);
void ReportComponentRamUsed(char *ComponentName);
void ReportComponentRamStatus(char *ComponentName);
uint8_t EmptyPlaceInEventPool(Log *Log, int ComponentNumber);
void RamOccupyFunction(char *Component, char *EventName)
{
    static Log log;
    int componentNumber;
    int eventNumber;
    if (IsComponentExist(&log, Component) == false)
    {
        componentNumber = 0;
        eventNumber = 0;
        strncpy(log.Component[componentNumber].Name, Component, STRING_LEN);
        strncpy(log.Component[componentNumber].Event[eventNumber].Name,
                EventName, STRING_LEN);
        RecordStatus(&log, componentNumber, eventNumber, LogStart);
    }
    else
    {
        componentNumber = FindComponentLocationInPool(&log, Component);
        if (componentNumber == ERROR_CODE)
        {
            ESP_LOGE(TAG, " there is not any space in Component pool for creating new component");
            return;
        }
        else
        {
            uint8_t isEventExist =
                IsEventExist(&log, componentNumber, EventName);
            if (isEventExist == 1)
            {
                eventNumber =
                    FindEventInEventPool(&log, componentNumber, EventName);
                strncpy(log.Component[componentNumber].Event[eventNumber].Name,
                        EventName, STRING_LEN);
                if (log.Component[componentNumber].Event[eventNumber].Counter == 2)
                    RecordStatus(&log, componentNumber, eventNumber, LogStart); // add log
                else
                    RecordStatus(&log, componentNumber, eventNumber, LogEnd); // add log
            }
            else if (isEventExist == 0)
            {
                eventNumber = EmptyPlaceInEventPool(&log, componentNumber);
                if (eventNumber == ERROR_CODE)
                {
                    ESP_LOGE(TAG, "  there is not any space in Event pool for creating new event");
                    return;
                }
                else
                {
                    strncpy(log.Component[componentNumber].Event[eventNumber].Name, EventName, STRING_LEN);
                    RecordStatus(&log, componentNumber, eventNumber, LogStart); // add log
                }
            }
        }
    }
}

void RecordStatus(Log *Log, int ComponentNumber, int EventNumber,
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
        psramSize =
            psramSize -
            Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
        sramSize =
            sramSize -
            Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        ESP_LOGE(TAG, "Event name :%s SRAM: %u K bytes PSRAM occupy: %u K bytes occupy at %u millis",
                 Log->Component[ComponentNumber].Event[EventNumber].Name, sramSize,
                 psramSize, TimeFromBootUp);
        Log->Component[ComponentNumber].Event[EventNumber].Counter = 2;
    }
}

uint8_t IsComponentExist(Log *Log, char *ComponentName)
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

uint8_t IsEventExist(Log *Log, int ComponentNumber, char *EventName)
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

uint8_t FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName)
{
    uint8_t eventNumber = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (0 ==
            (strcmp(Log->Component[ComponentNumber].Event[i].Name, EventName)))
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
        if (0 == (strcmp(Log->Component[i].Name, Component)))
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

uint8_t NumberSavedEvent(Log *Log, int ComponentNumber)
{
    uint8_t numberOfEvent = 0;
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (strlen(Log->Component[ComponentNumber].Event[i].Name) != 0)
            numberOfEvent++;
    }
    return numberOfEvent;
}

void RamStatusFunction(char *ComponentName, char *EventName)
{
    int componentNumber;
    int eventNumber;
    size_t psramSize;
    size_t sramSize;
    if (IsComponentExist(&Log2, ComponentName) == false)
    {
        componentNumber = 0;
        eventNumber = 0;
        psramSize = (esp_get_free_heap_size() / 1000);
        sramSize = (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        strncpy(Log2.Component[componentNumber].Name, ComponentName,
                strlen(ComponentName));
        strncpy(Log2.Component[componentNumber].Event[eventNumber].Name,
                EventName, strlen(EventName));
        Log2.Component[componentNumber].Event[eventNumber].RAM.Psram =
            psramSize;
        Log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
        Log2.Component[componentNumber].Event[eventNumber].TimeStamp =
            TimeFromBootUp;
        ESP_LOGE(TAG, "component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
                 ComponentName, EventName, sramSize, psramSize, TimeFromBootUp);
    }
    else
    {
        psramSize = (esp_get_free_heap_size() / 1000);
        sramSize = (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        componentNumber = FindComponentLocationInPool(&Log2, ComponentName);
        eventNumber = EmptyPlaceInEventPool(&Log2, componentNumber);
        if (NumberSavedEvent(&Log2, componentNumber) >= (LOG_MAX_EVENT - 1))
            memset(&Log2.Component[componentNumber], 0,
                   sizeof(Log2.Component[componentNumber]));
        strncpy(Log2.Component[componentNumber].Name, ComponentName,
                strlen(ComponentName));
        strncpy(Log2.Component[componentNumber].Event[eventNumber].Name,
                EventName, strlen(EventName));
        Log2.Component[componentNumber].Event[eventNumber].RAM.Psram =
            psramSize;
        Log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
        Log2.Component[componentNumber].Event[eventNumber].TimeStamp =
            TimeFromBootUp;
        ESP_LOGE(TAG, "component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
                 ComponentName, EventName, sramSize, psramSize, TimeFromBootUp);
    }
}

void ReportComponentRamStatus(char *ComponentName)
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
                ESP_LOGE(TAG, "in the report component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
                         ComponentName, Log2.Component[componentNumber].Event[i].Name,
                         Log2.Component[componentNumber].Event[i].RAM.Sram,
                         Log2.Component[componentNumber].Event[i].RAM.Psram,
                         Log2.Component[componentNumber].Event[i].TimeStamp);
            }
            else
                break;
        }
    }
}

void ReportComponentRamUsed(char *ComponentName)
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
                ESP_LOGE(TAG, "component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is occupy",
                         ComponentName, Log2.Component[componentNumber].Event[i].Name,
                         Log2.Component[componentNumber].Event[i].RAM.Sram,
                         Log2.Component[componentNumber].Event[i].RAM.Psram,
                         Log2.Component[componentNumber].Event[i].TimeStamp);
            }
            else
                break;
        }
    }
}
