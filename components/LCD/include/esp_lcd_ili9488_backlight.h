
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define DIRECT_CURRENT_BACKLIGHT



#ifdef PWM_BACKLIGHT
#define BACKLIGHT_LEDC_CHANNEL LEDC_CHANNEL_0
#define BACKLIGHT_LEDC_TIMER LEDC_TIMER_2
#define BACKLIGHT_LEDC_TIMER_RESOLUTION LEDC_TIMER_12_BIT
#endif
#ifdef DIRECT_CURRENT_BACKLIGHT
#define CONFIG_TFT_BACKLIGHT_PIN GPIO_NUM_12
#endif

    void display_brightness_set(int brightness_percentage);
    void display_brightness_init(void);

#ifdef __cplusplus
}
#endif