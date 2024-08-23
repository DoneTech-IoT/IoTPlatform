#include "nvsFlash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ServiceManager.h"
#include "Custom_Log.h"

#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#define TIMER_TIME pdMS_TO_TICKS(500) // in millis

// ****************************** GLobal Variables ****************************** //
static const char *TAG = "Main";
// ****************************** GLobal Functions ****************************** //

/**
 * @brief Function to change colors based on a timer callback
 */
extern "C" void app_main()
{

    Log_RamOccupy("main", "service manager");
    nvsFlashInit();
    ServiceMangerTaskInit();

    Log_RamOccupy("main", "service manager");

}