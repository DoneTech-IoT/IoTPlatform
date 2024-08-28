#ifndef COFFEE_MAKER_APP_H_
#define COFFEE_MAKER_APP_H_
#include <stdint.h>
#include <sys/param.h>
#include "cJSON.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"
#include <esp_event.h>
#include <esp_system.h>
#include "esp_log.h"

#include "MQTT_Interface.h"

#define COFFEE_TIME 120
#define COFFEE_MAKER_APP_SEC 1000
typedef struct CoffeeMakerApp
{
    uint8_t GinderLevel;
    uint8_t Cups;
    uint8_t TeaFlag;
    uint8_t CoffeeFlag;
    uint8_t Temp;
    uint16_t UpdateTime;
    char DeviceMACAddress[18];
    char Pass[18];
    char State[18];
} CoffeeMakerJson_str;

/**
 * @brief Main function for the coffee maker application.
 * This function initializes the coffee maker timer, handles MQTT communication, and processes coffee maker settings.
 * @return void
 */
void CoffeeMakerApplication();

#ifdef COFFEE_MAKER_APP_TEST
void PublishJsonForTest(char *CoffeeMakerJsonOutPut);
void JSON_TEST(CoffeeMakerJson_str *CoffeeMakerJson);
void parserTEST(char *temp);
#endif

#endif
