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
    void disp_driver_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
    void lvgl_driver_init();
#ifdef __cplusplus
}
#endif
