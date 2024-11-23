#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "esp_system.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "Buzzer.h"

static const char *TAG = "BUZZER";

void BuzzerInit(int gpio_num, uint32_t freq, uint32_t duration)
{
	// ledc_timer_config_t timer_conf;
	// timer_conf.speed_mode = LEDC_LOW_SPEED_MODE;	
	// timer_conf.timer_num = LEDC_TIMER_0;
	// timer_conf.duty_resolution = LEDC_TIMER_13_BIT;
	// timer_conf.freq_hz = freq;
	// timer_conf.clk_cfg = LEDC_AUTO_CLK;
	// ledc_timer_config(&timer_conf);

	// ledc_channel_config_t ledc_conf;
	// ledc_conf.speed_mode = LEDC_LOW_SPEED_MODE;
	// ledc_conf.gpio_num = gpio_num;	
	// ledc_conf.channel = LEDC_CHANNEL_0;
	// ledc_conf.intr_type = LEDC_INTR_DISABLE;
	// ledc_conf.timer_sel = LEDC_TIMER_0;
	// ledc_conf.duty = 0x0; 
	// ledc_conf.hpoint = 0x00;						  
	// ledc_channel_config(&ledc_conf);

	// ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
	// ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

/**
 * @brief buzzer driver with esp-ledc pwm-driver.
 * @param[in] gpio_num pin number of pwm to drive buzzer
 * @param[in] freq frequency of biz
 * @param[in] duration time between biz cycles
 * @
 */
static void BuzzerMakeSound(uint32_t duration)
{	
	// start
	// ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 819); // 10% duty - play here for your 
	// ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
	// vTaskDelay(duration / portTICK_PERIOD_MS);
	// // stop
	// ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
	// ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

/**
 * @brief make biz with constant frequency.
 * @param[in] count number of biz of buzzer
 */
static void BuzzerBiz(uint8_t count)
{
	// for (size_t i = 0; i < count; i++)
	// {
	// 	BuzzerMakeSound(CONFIG_DONE_BUZZER_DEFAULT_PWM_DURATION);
	// 	vTaskDelay(CONFIG_DONE_BUZZER_DEFAULT_DELAY_MS / portTICK_PERIOD_MS);
	// }
}

/**
 * @brief play Buzzer with an effect
 * @param[in] effectNo effect that define in BuzzerEffect_t
 */
void BuzzerPlay(BuzzerEffect effectNo)
{
	// switch (effectNo)
	// {
	// 	case ONE_BIZ:
	// 		BuzzerBiz(ONE_BIZ);
	// 		break;
	// 	case DOUBLE_BIZ:
	// 		BuzzerBiz(DOUBLE_BIZ);
	// 		break;
	// 	case TRIPLE_BIZ:
	// 		BuzzerBiz(TRIPLE_BIZ);
	// 		break;
	// 	case NO_EFFECT:
	// 	default:
	// 		break;
	// }
}