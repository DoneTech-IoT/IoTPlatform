/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

static void Drink_CoffeeBean_handler(lv_event_t *e)
{

}

static void Drink_CoffeePowder_handler(lv_event_t *e)
{

}

static void Drink_Tea_handler(lv_event_t *e)
{

}

static void CoffeeBeanSize_L_handler(lv_event_t *e)
{

}

static void CoffeeBeanSize_M_handler(lv_event_t *e)
{

}

static void CoffeeBeanSize_S_handler(lv_event_t *e)
{

}

void events_init(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_img_5, Drink_CoffeeBean_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_img_3, Drink_CoffeePowder_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_img_2, Drink_Tea_handler, LV_EVENT_ALL, NULL);

    lv_obj_add_event_cb(ui->screen_img_6, CoffeeBeanSize_L_handler, LV_EVENT_ALL, NULL);lv_obj_add_event_cb(ui->screen_img_4, CoffeeBeanSize_M_handler, LV_EVENT_ALL, NULL);lv_obj_add_event_cb(ui->screen_img_1, CoffeeBeanSize_S_handler, LV_EVENT_ALL, NULL);
}
