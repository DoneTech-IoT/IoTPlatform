#ifndef PULSE__
#define PULSE__
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdio.h>
#include "string.h"

#include "esp_sleep.h"
#include "esp_err.h"
#include "driver/pulse_cnt.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

    void PulseCounterUnitConfig();
#ifdef __cplusplus
}
#endif
#endif