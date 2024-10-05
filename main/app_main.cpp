#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ServiceManager.h"
#include "Custom_Log.h"
#include "PulseCounter.h"

/**
 * @brief Function to change colors based on a timer callback
 */
extern "C" void app_main()
{
    vTaskDelay(100);
    PulseCounterUnitConfig();
}