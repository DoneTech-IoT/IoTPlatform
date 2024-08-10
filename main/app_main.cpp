#include "lvglGui.h"
#include "GlobalInit.h"
#include "nvsFlash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Setup_GPIO.h"
#include "ServiceManger.h"
#include "Custom_Log.h"
#define TIMER_TIME pdMS_TO_TICKS(500) // in millis
// ****************************** GLobal Variables ****************************** //
static const char *TAG = "Main";

extern "C" void app_main()
{

    // Log_RamOccupy("main", "service manager");

    ServiceMangerTaskInit();
    vTaskDelay(pdMS_TO_TICKS(1 * SEC));

    GlobalInit();
    nvsFlashInit();
    SpiffsGlobalConfig();
    // GUItest();
}
