#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ServiceManager.h"
#include "Custom_Log.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"

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

    ESP_LOGW(
        TAG,
        "\n"
        "  Free Heap:           %u  bytes\n"
        "  MALLOC_CAP_8BIT      %7zu bytes\n"
        "  MALLOC_CAP_DMA       %7zu bytes\n"
        "  MALLOC_CAP_SPIRAM    %7zu bytes\n"
        "  MALLOC_CAP_INTERNAL  %7zu bytes\n"
        "  MALLOC_CAP_DEFAULT   %7zu bytes\n"
        "  MALLOC_CAP_IRAM_8BIT %7zu bytes\n"
        "  MALLOC_CAP_RETENTION %7zu bytes\n",
        xPortGetFreeHeapSize(),
        heap_caps_get_free_size(MALLOC_CAP_8BIT),
        heap_caps_get_free_size(MALLOC_CAP_DMA),
        heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        heap_caps_get_free_size(MALLOC_CAP_DEFAULT),
        heap_caps_get_free_size(MALLOC_CAP_IRAM_8BIT),
        heap_caps_get_free_size(MALLOC_CAP_RETENTION)
    );

    while (true)
    {
        for (int i = 0; i < HeartbeatPatternLength; i++) 
        {
            // Toggle GPIO state (ON -> OFF -> ON -> ...)
            gpio_set_level(HEARTBEAT_GPIO, i % 2);
            // Wait for the current pattern duration
            vTaskDelay(pdMS_TO_TICKS(HeartbeatPattern[i]));
        }
    }
}