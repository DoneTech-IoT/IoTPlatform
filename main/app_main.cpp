#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Setup_GPIO.h"
#include "ServiceManager.h"
#include "Custom_Log.h"

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
    ServiceMangerTaskInit();
    Log_RamOccupy("main", "service manager");
}