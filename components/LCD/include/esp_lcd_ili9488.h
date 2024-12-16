/*
 * SPDX-FileCopyrightText: 2022 atanisoft (github.com/atanisoft)
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once


#include <lvgl.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 480
#define DISPLAY_COMMAND_BITS 8
#define DISPLAY_PARAMETER_BITS 8
#define SPI_TFT_CLOCK_SPEED_HZ 40000000
#define DISPLAY_SPI_QUEUE_LEN 50
#define SPI_MAX_TRANSFER_SIZE 4000
#define CONFIG_TFT_CS_PIN 3
#define CONFIG_TFT_RESET_PIN 48
#define CONFIG_TFT_DC_PIN 8
#define CONFIG_TFT_BACKLIGHT_PIN 12

    void disp_driver_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
    void lvgl_driver_init();

#ifdef __cplusplus
}
#endif
