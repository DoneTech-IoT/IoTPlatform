#include "lvglGui.h"
#include "GlobalInit.h"
#include "nvsFlash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Setup_GPIO.h"
#include "ServiceManger.h"
#include "Custom_Log.h"

extern "C" void app_main()
{

    Log_RamOccupy("main", "service manager");

    ServiceMangerTaskInit();
    vTaskDelay(pdMS_TO_TICKS(1 * SEC));

    GlobalInit();
    nvsFlashInit();
}