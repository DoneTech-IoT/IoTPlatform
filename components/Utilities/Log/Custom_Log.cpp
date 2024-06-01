#include "Custom_Log.h"
#include "string.h"
void RamOccupyFunction(bool TimingFlag, char *Component, char *ProgramStage)
{
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
    ServiceMangerRamStatus("OAth", ProgramStage);
#endif
#ifdef CONFIG_DONE_GUI_LOG_RAM
    ServiceMangerRamStatus("GUI", ProgramStage);
#endif
#ifdef CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
    ServiceMangerRamStatus("Matter_Interface", ProgramStage);
#endif
#ifdef CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
    ServiceMangerRamStatus("Main", ProgramStage);
#endif
}

void RamOccupy(bool TimingFlag, char *ProgramStage)
{
#ifdef CONFIG_DONE_OATH_LOG_RAM
    ServiceMangerRamOccupy(TimingFlag, "OAth", ProgramStage);
#endif
#ifdef CONFIG_DONE_GUI_LOG_RAM
    ServiceMangerRamOccupy(TimingFlag, "GUI", ProgramStage);
#endif
#ifdef CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
    ServiceMangerRamOccupy(TimingFlag, "Matter_Interface", ProgramStage);
#endif
#ifdef CONFIG_DONE_MATTER_INTERFACE_LOG_RAM
    ServiceMangerRamOccupy(TimingFlag, "Main", ProgramStage);
#endif
}