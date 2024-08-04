/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"

void setup_scr_screen(lv_ui *ui)
{
	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen, 480, 272);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_7
	ui->screen_img_7 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_7, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_7, &_icon_number_cups_alpha_56x56);
	lv_img_set_pivot(ui->screen_img_7, 50,50);
	lv_img_set_angle(ui->screen_img_7, 0);
	lv_obj_set_pos(ui->screen_img_7, 405, 211);
	lv_obj_set_size(ui->screen_img_7, 56, 56);

	//Write style for screen_img_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_1
	ui->screen_label_1 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_1, "2x");
	lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_1, 350, 224);
	lv_obj_set_size(ui->screen_label_1, 65, 45);

	//Write style for screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_1, lv_color_hex(0x106308), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_1, &lv_font_arial_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_2
	ui->screen_label_2 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_2, "Ready to start - estimated time: 10:00");
	lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_2, 10, 231);
	lv_obj_set_size(ui->screen_label_2, 350, 30);

	//Write style for screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_2, lv_color_hex(0x106308), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_2, &lv_font_arial_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_2, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_8
	ui->screen_img_8 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_8, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_8, &_frame_alpha_360x100);
	lv_img_set_pivot(ui->screen_img_8, 50,50);
	lv_img_set_angle(ui->screen_img_8, 0);
	lv_obj_set_pos(ui->screen_img_8, 111, 4);
	lv_obj_set_size(ui->screen_img_8, 360, 100);

	//Write style for screen_img_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_9
	ui->screen_img_9 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_9, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_9, &_icon_coffee_beans_alpha_96x96);
	lv_img_set_pivot(ui->screen_img_9, 50,50);
	lv_img_set_angle(ui->screen_img_9, 0);
	lv_obj_set_pos(ui->screen_img_9, 126, 5);
	lv_obj_set_size(ui->screen_img_9, 96, 96);

	//Write style for screen_img_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_10
	ui->screen_img_10 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_10, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_10, &_icon_coffee_powder_alpha_96x96);
	lv_img_set_pivot(ui->screen_img_10, 50,50);
	lv_img_set_angle(ui->screen_img_10, 0);
	lv_obj_set_pos(ui->screen_img_10, 250, 7);
	lv_obj_set_size(ui->screen_img_10, 96, 96);

	//Write style for screen_img_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_10, 167, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_11
	ui->screen_img_11 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_11, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_11, &_icon_tea_leaf_alpha_96x96);
	lv_img_set_pivot(ui->screen_img_11, 50,50);
	lv_img_set_angle(ui->screen_img_11, 0);
	lv_obj_set_pos(ui->screen_img_11, 364, 7);
	lv_obj_set_size(ui->screen_img_11, 96, 96);

	//Write style for screen_img_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_11, 167, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_12
	ui->screen_img_12 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_12, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_12, &_frame_alpha_360x100);
	lv_img_set_pivot(ui->screen_img_12, 50,50);
	lv_img_set_angle(ui->screen_img_12, 0);
	lv_obj_set_pos(ui->screen_img_12, 113, 110);
	lv_obj_set_size(ui->screen_img_12, 360, 100);

	//Write style for screen_img_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_13
	ui->screen_img_13 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_13, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_13, &_icon_coffee_grind_lvl_1_alpha_90x90);
	lv_img_set_pivot(ui->screen_img_13, 50,50);
	lv_img_set_angle(ui->screen_img_13, 0);
	lv_obj_set_pos(ui->screen_img_13, 116, 115);
	lv_obj_set_size(ui->screen_img_13, 90, 90);

	//Write style for screen_img_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_13, 167, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_14
	ui->screen_img_14 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_14, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_14, &_icon_coffee_grind_lvl_2_alpha_90x90);
	lv_img_set_pivot(ui->screen_img_14, 50,50);
	lv_img_set_angle(ui->screen_img_14, 0);
	lv_obj_set_pos(ui->screen_img_14, 237, 114);
	lv_obj_set_size(ui->screen_img_14, 90, 90);

	//Write style for screen_img_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_15
	ui->screen_img_15 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_15, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_15, &_icon_coffee_grind_lvl_3_alpha_90x90);
	lv_img_set_pivot(ui->screen_img_15, 50,50);
	lv_img_set_angle(ui->screen_img_15, 0);
	lv_obj_set_pos(ui->screen_img_15, 360, 115);
	lv_obj_set_size(ui->screen_img_15, 90, 90);

	//Write style for screen_img_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_15, 167, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_16
	ui->screen_img_16 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_16, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_16, &_select_grind_size_alpha_100x100);
	lv_img_set_pivot(ui->screen_img_16, 50,50);
	lv_img_set_angle(ui->screen_img_16, 0);
	lv_obj_set_pos(ui->screen_img_16, 6, 110);
	lv_obj_set_size(ui->screen_img_16, 100, 100);

	//Write style for screen_img_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_17
	ui->screen_img_17 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_17, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_17, &_select_your_drink_alpha_100x100);
	lv_img_set_pivot(ui->screen_img_17, 50,50);
	lv_img_set_angle(ui->screen_img_17, 0);
	lv_obj_set_pos(ui->screen_img_17, 6, 3);
	lv_obj_set_size(ui->screen_img_17, 100, 100);

	//Write style for screen_img_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_17, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen);

}
