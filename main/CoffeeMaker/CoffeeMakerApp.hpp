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

void RunMQTTAndTestJson();
#endif
