
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef CONFIG_TFT_BACKLIGHT_ENABLE


#if CONFIG_BACKLIGHT_LEDC_CHANNEL_0
#define BACKLIGHT_LEDC_CHANNEL   LEDC_CHANNEL_0 
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_1
#define BACKLIGHT_LEDC_CHANNEL   LEDC_CHANNEL_1 
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_2
#define BACKLIGHT_LEDC_CHANNEL   LEDC_CHANNEL_2 
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_3
#define BACKLIGHT_LEDC_CHANNEL   LEDC_CHANNEL_3 
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_4
#define BACKLIGHT_LEDC_CHANNEL   LEDC_CHANNEL_4 
#elif CONFIG_BACKLIGHT_LEDC_CHANNEL_5
#define BACKLIGHT_LEDC_CHANNEL   LEDC_CHANNEL_5 
#else
#error "No valid LEDC channel selected"
#endif
#if CONFIG_BACKLIGHT_LEDC_TIMER_0
 #define BACKLIGHT_LEDC_TIMER   LEDC_TIMER_0 
#elif CONFIG_BACKLIGHT_LEDC_TIMER_1
 #define BACKLIGHT_LEDC_TIMER   LEDC_TIMER_1 
#elif CONFIG_BACKLIGHT_LEDC_TIMER_2
 #define BACKLIGHT_LEDC_TIMER   LEDC_TIMER_2 
#else
#error "No valid LEDC timer selected"
#endif

#if CONFIG_BACKLIGHT_LEDC_TIMER_10_BIT
 #define BACKLIGHT_LEDC_TIMER_RESOLUTION   LEDC_TIMER_10_BIT 
#elif CONFIG_BACKLIGHT_LEDC_TIMER_12_BIT
 #define BACKLIGHT_LEDC_TIMER_RESOLUTION   LEDC_TIMER_12_BIT 
#else
#error "No valid LEDC timer resolution selected"
#endif
#endif


#ifndef CONFIG_TFT_BACKLIGHT_ENABLE
#warning "backlight isn't enable"



void display_brightness_set(int brightness_percentage);
void display_brightness_init(void); 





#endif



#ifdef __cplusplus
}
#endif

