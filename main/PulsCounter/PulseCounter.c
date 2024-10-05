

#include "PulseCounter.h"

static const char *TAG = "Pulse COunter";

#define PCNT_GPIO_A 2                 // gpio
#define PCNT_GLITCHE_FILTER_TIME 1000 // nano sec
#define PCNT_COUNTING_TIME 1000       // mil sec
#define PCNT_FREQUENCY 100
#define PCNT_COUNT_HIGH_LIMIT 10000 // reach point
#define PCNT_TIMER_ID 0

static TimerHandle_t PulsCounterTimer;
static pcnt_unit_handle_t pcntUnitHandler = NULL;
static int PulseCounter = 0;

void UserCallBack(bool status)
{
    ESP_LOGI(TAG, "User call back status= %d", (int)status);
}
void PulseCounterStopTimer(TimerHandle_t *pulsCounterTimer)
{
    if (xTimerStop(*pulsCounterTimer, 0) != pdPASS)
    {
        ESP_LOGE(TAG, "Timer unsuccessfully stopped please restart this task.\n");
        return;
    }
    ESP_LOGI(TAG, "Timer successfully stopped.");
    PulseCounter = 0;
}
void PulseCounterTimerCallBack(TimerHandle_t pulsCounterTimer)
{
    ESP_ERROR_CHECK(pcnt_unit_get_count(pcntUnitHandler, &PulseCounter));
    ESP_LOGI(TAG, "PulseCounter = %d", PulseCounter);
    if (PulseCounter > (PCNT_FREQUENCY - PCNT_FREQUENCY * 0.05) &&
        PulseCounter < (PCNT_FREQUENCY + PCNT_FREQUENCY * 0.05))
    {
        PulseCounter = 0;
        ESP_ERROR_CHECK(pcnt_unit_clear_count(pcntUnitHandler));
        return;
    }
    PulseCounter = 0;
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcntUnitHandler));
    UserCallBack(true);
}
void PulseCounterUnitConfig()
{
    static pcnt_unit_config_t pcntUnitConfig;
    pcntUnitConfig.high_limit = PCNT_COUNT_HIGH_LIMIT;
    pcntUnitConfig.low_limit = ((-1) * PCNT_COUNT_HIGH_LIMIT);

    ESP_ERROR_CHECK(pcnt_new_unit(&pcntUnitConfig, &pcntUnitHandler));

    static pcnt_glitch_filter_config_t pcntFilterConfig;
    pcntFilterConfig.max_glitch_ns = PCNT_GLITCHE_FILTER_TIME;
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcntUnitHandler, &pcntFilterConfig));

    static pcnt_chan_config_t pcntChanelConfig;
    pcntChanelConfig.edge_gpio_num = PCNT_GPIO_A;
    pcntChanelConfig.level_gpio_num = -1;

    static pcnt_channel_handle_t pcntChanelHandler = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcntUnitHandler, &pcntChanelConfig, &pcntChanelHandler));

    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcntChanelHandler, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD));
    ESP_ERROR_CHECK(pcnt_unit_enable(pcntUnitHandler));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcntUnitHandler));
    ESP_ERROR_CHECK(pcnt_unit_start(pcntUnitHandler));

    PulsCounterTimer = xTimerCreate("Pulse Counter Timer",
                                    pdMS_TO_TICKS(PCNT_COUNTING_TIME),
                                    pdTRUE, (void *)PCNT_TIMER_ID,
                                    PulseCounterTimerCallBack);
    if (xTimerStart(PulsCounterTimer, (TickType_t)0) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to start the timer\n");
    }
}