
#pragma once

#include <lvgl.h>

#ifdef __cplusplus
extern "C"
{
#endif
#define DIRECT_CURRENT_BACKLIGHT
#define CONFIG_SPI_MISO GPIO_NUM_7
#define CONFIG_SPI_MOSI GPIO_NUM_18
#define CONFIG_SPI_CLOCK GPIO_NUM_47
#define CONFIG_TFT_CS_PIN GPIO_NUM_3
#define CONFIG_TFT_RESET_PIN GPIO_NUM_48
#define CONFIG_TFT_DC_PIN GPIO_NUM_12
#define CONFIG_DISPLAY_COLOR_MODE COLOR_RGB_ELEMENT_ORDER_RGB
#define CONFIG_LV_HOR_RES_MAX 480
#define CONFIG_LV_VER_RES_MAX 320
#define CONFIG_DISPLAY_COMMAND_BITS 8
#define CONFIG_DISPLAY_PARAMETER_BITS 8
#define CONFIG_SPI_TFT_CLOCK_SPEED_HZ 40 * 1000 * 1000
#define CONFIG_DISPLAY_SPI_QUEUE_LEN 50
#define CONFIG_SPI_MAX_TRANSFER_SIZE 4096

    void disp_driver_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
    void lvgl_driver_init();
#ifdef __cplusplus
}
#endif
