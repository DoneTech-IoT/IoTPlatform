/*
 * SPDX-FileCopyrightText: 2022 atanisoft (github.com/atanisoft)
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/spi_master.h>

#include <esp_lcd_panel_interface.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_commands.h>

#include <esp_log.h>
#include <esp_rom_gpio.h>
#include <esp_check.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <memory.h>
#include <stdlib.h>
#include <sys/cdefs.h>
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
#define SPI_MAX_TRANSFER_SIZE 4096
#define CONFIG_TFT_CS_PIN 3
#define CONFIG_TFT_RESET_PIN 48
#define CONFIG_TFT_DC_PIN 8
#define CONFIG_TFT_BACKLIGHT_PIN 12

    void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
    void display_init();

#ifdef __cplusplus
}
#endif
