#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ServiceManager.h"
#include "Custom_Log.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_system.h"
#include "esp_log.h"

#include "ServiceMngr.hpp"

void check_memory() {
    ESP_LOGI("MEM", "Free heap memory:     %lu bytes", 
        (uint32_t)esp_get_free_heap_size());
    ESP_LOGI("MEM", "Free internal memory: %lu bytes", 
        (uint32_t)heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    ESP_LOGI("MEM", "Free PSRAM:           %lu bytes", 
        (uint32_t)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
}

#define HEARTBEAT_GPIO GPIO_NUM_21

// Define the heartbeat pattern in milliseconds
const int HeartbeatPattern[] = {
    200, // First "lub" (on time)
    100, // Pause between "lub" and "dub"
    200, // Second "dub" (on time)
    1000 // Rest time before the next heartbeat
};
const int HeartbeatPatternLength = sizeof(HeartbeatPattern) / sizeof(HeartbeatPattern[0]);
static const char *TAG = "Main";

/**
 * @brief Function to change colors based on a timer callback
 */
extern "C" void app_main()
{
    // Log_RamOccupy("main", "service manager");
    // ServiceManger_TaskInit();
    // Log_RamOccupy("main", "service manager");
    check_memory();
    ServiceMngr serviceMngr(
        ServiceMngr::mServiceName[static_cast<uint8_t>(SharedBus::ServiceID::SERVICE_MANAGER)],
        SharedBus::ServiceID::SERVICE_MANAGER);
    check_memory();

    gpio_config_t heartBeatConf;
    heartBeatConf.intr_type = GPIO_INTR_DISABLE;
    heartBeatConf.mode = GPIO_MODE_OUTPUT;
    heartBeatConf.pin_bit_mask = (1ULL << HEARTBEAT_GPIO);
    heartBeatConf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    heartBeatConf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&heartBeatConf);    

    while (true)
    {
        for (int i = 0; i < HeartbeatPatternLength; i++) 
        {            
            gpio_set_level(HEARTBEAT_GPIO, i % 2);            
            vTaskDelay(pdMS_TO_TICKS(HeartbeatPattern[i]));
        }
    }
}