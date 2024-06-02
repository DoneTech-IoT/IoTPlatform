#include "Custom_Log.h"
#include "string.h"

#define LOG_MAX_EVENT 20
#define LOG_MAX_COMPONENT 3
#define STRING_LEN 20
typedef struct
{
    int Psram[10];
    int Sram[10];
} Ram_str;
typedef struct
{
    Ram_str RAM;
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

uint8_t FindComponent(Log *Log, char *Component);
uint8_t WhereIsEmpty(Log *Log);

void tset(bool TimingFlag, char *Component, char *EventName)
{
    static Log Log;
    int componentNumber;
    int eventNumber;
    if (FindComponent(&Log, Component) == 0) // this is new component
    {
        componentNumber = WhereIsEmpty(&Log);
        strncpy(Log.Component[componentNumber].name, Component, strlen(Component));
        // add event without finding space because new component don't any event
        strncpy(Log.Component[componentNumber].Event[0].Name, EventName, strlen(EventName));
    }
    else // existence component
    {
        eventNumber = FindEventInComponent(&Log, Component, EventName);
            strncpy(Log.Component[componentNumber].Event[eventNumber].Name,EventName);
    }
}
uint8_t FindEventInComponent(Log *Log, int ComponentNumber, char *EventName)
{
    uint8_t eventNumber = 0;
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (!(strcmp(Log->Component[ComponentNumber].Event[i].Name, EventName)))
        {
            eventNumber++;
            break;
        }
    }
    return eventNumber;
}
uint8_t FindComponent(Log *Log, char *Component)
{
    uint8_t componentNumber = 0;
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (!(strcmp(Log->ComponentComponent[i].Name, Component)))
        {
            componentNumber = i;
        }
    }
    return componentNumber;
}
uint8_t WhereIsEmpty(Log *Log)
{
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (strlen(Log->ComponentComponent[i].Name) == 0)
            return i;
    }
}
void RamOccupyFunction(bool TimingFlag, char *Component, char *ProgramStage)
{
    Log Log;
#ifdef CONFIG_DONE_LOG_RAM
    static size_t PsramSize;
    static size_t SramSize;
    static char LastLogTag[20];
    if (TimingFlag == 1)
    {
        PsramSize = 0;
        SramSize = 0;
        PsramSize = (esp_get_free_heap_size() / 1000);
        SramSize = (xPortGetFreeHeapSize() / 1000);
        strcpy(LastLogTag, Component);
    }
    else if (TimingFlag == 0 && !(strcmp(LastLogTag, Component)))
    {
        char TAG[100];
        ESP_LOGE(Component, "SRAM: %u K bytes PSRAM: %u K bytes  at ", SramSize, PsramSize);
        PsramSize = PsramSize - (esp_get_free_heap_size() / 1000);
        SramSize = SramSize - (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        strcpy(TAG, Component);
        strcat(TAG, " , ");
        strcat(TAG, ProgramStage);
        ESP_LOGE(Component, "SRAM: %u K bytes PSRAM occupy: %u K bytes occupy at %u millis", SramSize, PsramSize, TimeFromBootUp);
    }
#endif
}

void RamStatusFunction(char *Component, char *ProgramStage)
{
#ifdef CONFIG_DONE_LOG_RAM
    char TAG[100];
    size_t PsramSize;
    size_t SramSize;
    PsramSize = (esp_get_free_heap_size() / 1000);
    SramSize = (xPortGetFreeHeapSize() / 1000);
    int TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
    strcpy(TAG, Component);
    strcat(TAG, " , ");
    strcat(TAG, ProgramStage);
    ESP_LOGE(TAG, "SRAM: %u K bytes , PSRAM: %u K bytes at %d millis", SramSize, PsramSize, TimeFromBootUp);
#endif
}

void RamOStatus(char *ProgramStage)
{
#ifdef CONFIG_DONE_OATH_LOG_RAM
    RamStatusFunction("OAth", ProgramStage);
#endif
#ifdef CONFIG_DONE_GUI_LOG_RAM
    RamStatusFunction("GUI", ProgramStage);
#endif
#ifdef CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
    RamStatusFunction("Matter_Interface", ProgramStage);
#endif
#ifdef CONFIG_DONE_MAIN_LOG_RAM
    RamStatusFunction("Main", ProgramStage);
#endif
}

void RamOccupy(bool TimingFlag, char *ProgramStage)
{
#ifdef CONFIG_DONE_OATH_LOG_RAM
    RamOccupyFunction(TimingFlag, "OAth", ProgramStage);
#endif
#ifdef CONFIG_DONE_GUI_LOG_RAM
    RamOccupyFunction(TimingFlag, "GUI", ProgramStage);
#endif
#ifdef CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
    RamOccupyFunction(TimingFlag, "Matter_Interface", ProgramStage);
#endif
#ifdef CONFIG_DONE_MAIN_LOG_RAM
    RamOccupyFunction(TimingFlag, "Main", ProgramStage);
#endif
}