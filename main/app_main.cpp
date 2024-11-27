#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ServiceManager.h"
#include "Custom_Log.h"
#include "driver/gpio.h"

#define HEARTBEAT_GPIO GPIO_NUM_21

static const char *TAG = "Main";

/**
 * @brief Function to change colors based on a timer callback
 */
extern "C" void app_main()
{

    Log_RamOccupy("main", "service manager");
    ServiceManger_TaskInit();
    Log_RamOccupy("main", "service manager");

    gpio_config_t heartBeatConf;
    heartBeatConf.intr_type = GPIO_INTR_DISABLE;
    heartBeatConf.mode = GPIO_MODE_OUTPUT;
    heartBeatConf.pin_bit_mask = (1ULL << HEARTBEAT_GPIO);
    heartBeatConf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    heartBeatConf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&heartBeatConf);

    while (true)
    {
        gpio_set_level(HEARTBEAT_GPIO, 1);  
        vTaskDelay(pdMS_TO_TICKS(500));    
        gpio_set_level(HEARTBEAT_GPIO, 0); 
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}