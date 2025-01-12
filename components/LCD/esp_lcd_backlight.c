#include <driver/gpio.h>
#include <driver/ledc.h>

#include <esp_log.h>
#include <esp_rom_gpio.h>
#include <esp_check.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "esp_lcd_backlight.h"

static const char *TAG = "LCD_backlight";

#define BACKLIGHT_LEDC_FRQUENCY 5000

// Backlight configuration based on kconfig options
static const ledc_mode_t BACKLIGHT_LEDC_MODE = LEDC_LOW_SPEED_MODE;

#if CONFIG_BACKLIGHT_LEDC_CHANNEL_0
static const ledc_channel_t BACKLIGHT_LEDC_CHANNEL = LEDC_CHANNEL_0;
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_1
static const ledc_channel_t BACKLIGHT_LEDC_CHANNEL = LEDC_CHANNEL_1;
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_2
static const ledc_channel_t BACKLIGHT_LEDC_CHANNEL = LEDC_CHANNEL_2;
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_3
static const ledc_channel_t BACKLIGHT_LEDC_CHANNEL = LEDC_CHANNEL_3;
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_4
static const ledc_channel_t BACKLIGHT_LEDC_CHANNEL = LEDC_CHANNEL_4;
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_5
static const ledc_channel_t BACKLIGHT_LEDC_CHANNEL = LEDC_CHANNEL_5;
#else
#error "No valid LEDC channel selected"
#endif

#if CONFIG_BACKLIGHT_LEDC_TIMER_0
static const ledc_timer_t BACKLIGHT_LEDC_TIMER = LEDC_TIMER_0;
#elif CONFIG_BACKLIGHT_LEDC_TIMER_1
static const ledc_timer_t BACKLIGHT_LEDC_TIMER = LEDC_TIMER_1;
#elif CONFIG_BACKLIGHT_LEDC_TIMER_2
static const ledc_timer_t BACKLIGHT_LEDC_TIMER = LEDC_TIMER_2;
#else
#error "No valid LEDC timer selected"
#endif

#if CONFIG_BACKLIGHT_LEDC_TIMER_10_BIT
static const ledc_timer_bit_t BACKLIGHT_LEDC_TIMER_RESOLUTION = LEDC_TIMER_10_BIT;
#elif CONFIG_BACKLIGHT_LEDC_TIMER_12_BIT
static const ledc_timer_bit_t BACKLIGHT_LEDC_TIMER_RESOLUTION = LEDC_TIMER_12_BIT;
#else
#error "No valid LEDC timer resolution selected"
#endif

void display_brightness_init(void)
{
    const ledc_channel_config_t LCD_backlight_channel =
        {
            .gpio_num = (gpio_num_t)CONFIG_TFT_BACKLIGHT_PIN,
            .speed_mode = BACKLIGHT_LEDC_MODE,
            .channel = BACKLIGHT_LEDC_CHANNEL,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = BACKLIGHT_LEDC_TIMER,
            .duty = 0,
            .hpoint = 0,
            .flags =
                {
                    .output_invert = 0}};
    const ledc_timer_config_t LCD_backlight_timer =
        {
            .speed_mode = BACKLIGHT_LEDC_MODE,
            .duty_resolution = BACKLIGHT_LEDC_TIMER_RESOLUTION,
            .timer_num = BACKLIGHT_LEDC_TIMER,
            .freq_hz = BACKLIGHT_LEDC_FRQUENCY,
            .clk_cfg = LEDC_AUTO_CLK};
    ESP_LOGI(TAG, "Initializing LEDC for backlight pin: %d", CONFIG_TFT_BACKLIGHT_PIN);

    ESP_ERROR_CHECK(ledc_timer_config(&LCD_backlight_timer));
    ESP_ERROR_CHECK(ledc_channel_config(&LCD_backlight_channel));
}

void display_brightness_set(int brightness_percentage)
{
    if (brightness_percentage > 100)
    {
        brightness_percentage = 100;
    }
    else if (brightness_percentage < 0)
    {
        brightness_percentage = 0;
    }
    ESP_LOGI(TAG, "Setting backlight to %d%%", brightness_percentage);

    // LEDC resolution set to 10bits, thus: 100% = 1023
    uint32_t duty_cycle = (1023 * brightness_percentage) / 100;
    ESP_ERROR_CHECK(ledc_set_duty(BACKLIGHT_LEDC_MODE, BACKLIGHT_LEDC_CHANNEL, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(BACKLIGHT_LEDC_MODE, BACKLIGHT_LEDC_CHANNEL));
}