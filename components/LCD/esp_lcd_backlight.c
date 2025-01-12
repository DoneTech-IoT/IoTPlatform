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
#ifdef CONFIG_TFT_BACKLIGHT_ENABLE
// Backlight configuration based on kconfig options
static const ledc_mode_t BACKLIGHT_LEDC_MODE = LEDC_LOW_SPEED_MODE;


static const ledc_channel_t backLight_LEDC_Channel = BACKLIGHT_LEDC_CHANNEL;
static const ledc_timer_t  backLight_LEDC_Timer = BACKLIGHT_LEDC_TIMER;
static const ledc_timer_bit_t  backLight_LEDC_TimResolution = BACKLIGHT_LEDC_TIMER_RESOLUTION;
#endif

void display_brightness_init(void)
{
    const ledc_channel_config_t LCD_backlight_channel =
        {
            .gpio_num = (gpio_num_t)CONFIG_TFT_BACKLIGHT_PIN,
            .speed_mode = BACKLIGHT_LEDC_MODE,
            .channel =  backLight_LEDC_Channel,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel =  backLight_LEDC_Timer,
            .duty = 0,
            .hpoint = 0,
            .flags =
                {
                    .output_invert = 0}};
    const ledc_timer_config_t LCD_backlight_timer =
        {
            .speed_mode = BACKLIGHT_LEDC_MODE,
            .duty_resolution =  backLight_LEDC_TimResolution,
            .timer_num =  backLight_LEDC_Timer,
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
    ESP_ERROR_CHECK(ledc_set_duty(BACKLIGHT_LEDC_MODE,  backLight_LEDC_Channel, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(BACKLIGHT_LEDC_MODE,  backLight_LEDC_Channel));
}