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
typedef enum
{
    LogEnd = 0,
    LogStart
} LogFlag;

void RamOStatus(char *ProgramStage);
void RamOccupy(bool TimingFlag, char *ProgramStage);

#endif