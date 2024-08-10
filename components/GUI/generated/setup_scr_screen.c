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
#include "custom.h"



void setup_scr_screen(lv_ui *ui)
{
	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen, 536, 240);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_11
	ui->screen_img_11 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_11, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_11, &_frame_alpha_400x90);
	lv_img_set_pivot(ui->screen_img_11, 50,50);
	lv_img_set_angle(ui->screen_img_11, 0);
	lv_obj_set_pos(ui->screen_img_11, 116, 8);
	lv_obj_set_size(ui->screen_img_11, 400, 90);

	//Write style for screen_img_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_img_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_img_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_img_11, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_10
	ui->screen_img_10 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_10, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_10, &_frame_alpha_400x90);
	lv_img_set_pivot(ui->screen_img_10, 50,50);
	lv_img_set_angle(ui->screen_img_10, 0);
	lv_obj_set_pos(ui->screen_img_10, 116, 103);
	lv_obj_set_size(ui->screen_img_10, 400, 90);

	//Write style for screen_img_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_img_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_img_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_img_10, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_SmallGrind
	ui->screen_SmallGrind = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_SmallGrind, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_SmallGrind, &_icon_coffee_grind_lvl_3_alpha_80x80);
	lv_img_set_pivot(ui->screen_SmallGrind, 50,50);
	lv_img_set_angle(ui->screen_SmallGrind, 0);
	lv_obj_set_pos(ui->screen_SmallGrind, 396, 105);
	lv_obj_set_size(ui->screen_SmallGrind, 80, 80);

	//Write style for screen_SmallGrind, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_SmallGrind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_SmallGrind, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_SmallGrind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_SmallGrind, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_tea
	ui->screen_tea = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_tea, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_tea, &_icon_tea_leaf_alpha_80x80);
	lv_img_set_pivot(ui->screen_tea, 50,50);
	lv_img_set_angle(ui->screen_tea, 0);
	lv_obj_set_pos(ui->screen_tea, 400, 11);
	lv_obj_set_size(ui->screen_tea, 80, 80);

	//Write style for screen_tea, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_tea, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_tea, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_tea, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_tea, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_scop
	ui->screen_scop = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_scop, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_scop, &_icon_coffee_powder_alpha_80x80);
	lv_img_set_pivot(ui->screen_scop, 50,50);
	lv_img_set_angle(ui->screen_scop, 0);
	lv_obj_set_pos(ui->screen_scop, 273, 13);
	lv_obj_set_size(ui->screen_scop, 80, 80);

	//Write style for screen_scop, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_scop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_scop, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_scop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_scop, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_MediumGrind
	ui->screen_MediumGrind = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_MediumGrind, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_MediumGrind, &_icon_coffee_grind_lvl_2_alpha_80x80);
	lv_img_set_pivot(ui->screen_MediumGrind, 50,50);
	lv_img_set_angle(ui->screen_MediumGrind, 0);
	lv_obj_set_pos(ui->screen_MediumGrind, 265, 108);
	lv_obj_set_size(ui->screen_MediumGrind, 80, 80);

	//Write style for screen_MediumGrind, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_MediumGrind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_MediumGrind, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_MediumGrind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_MediumGrind, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_Coffee
	ui->screen_Coffee = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_Coffee, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_Coffee, &_icon_coffee_beans_alpha_80x80);
	lv_img_set_pivot(ui->screen_Coffee, 50,50);
	lv_img_set_angle(ui->screen_Coffee, 0);
	lv_obj_set_pos(ui->screen_Coffee, 146, 13);
	lv_obj_set_size(ui->screen_Coffee, 80, 80);

	//Write style for screen_Coffee, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_Coffee, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_Coffee, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_Coffee, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_Coffee, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_longGrind
	ui->screen_longGrind = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_longGrind, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_longGrind, &_icon_coffee_grind_lvl_1_alpha_80x80);
	lv_img_set_pivot(ui->screen_longGrind, 50,50);
	lv_img_set_angle(ui->screen_longGrind, 0);
	lv_obj_set_pos(ui->screen_longGrind, 137, 108);
	lv_obj_set_size(ui->screen_longGrind, 80, 80);

	//Write style for screen_longGrind, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_longGrind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_longGrind, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_longGrind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_longGrind, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_7
	ui->screen_img_7 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_7, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_7, &_select_your_drink_alpha_90x90);
	lv_img_set_pivot(ui->screen_img_7, 50,50);
	lv_img_set_angle(ui->screen_img_7, 0);
	lv_obj_set_pos(ui->screen_img_7, 14, 8);
	lv_obj_set_size(ui->screen_img_7, 90, 90);

	//Write style for screen_img_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_img_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_img_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_img_7, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_8
	ui->screen_img_8 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_8, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_8, &_select_grind_size_alpha_90x90);
	lv_img_set_pivot(ui->screen_img_8, 50,50);
	lv_img_set_angle(ui->screen_img_8, 0);
	lv_obj_set_pos(ui->screen_img_8, 14, 105);
	lv_obj_set_size(ui->screen_img_8, 90, 90);

	//Write style for screen_img_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_img_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_img_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_img_8, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_9
	ui->screen_img_9 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_img_9, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_9, &_coffee_alpha_50x50);
	lv_img_set_pivot(ui->screen_img_9, 50,50);
	lv_img_set_angle(ui->screen_img_9, 0);
	lv_obj_set_pos(ui->screen_img_9, 457, 188);
	lv_obj_set_size(ui->screen_img_9, 50, 50);

	//Write style for screen_img_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_radius(ui->screen_img_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_img_9, true, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(ui->screen_img_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_img_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_1
	ui->screen_label_1 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_1, "Ready to start - estimated time: ");
	lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_1, 7, 212);
	lv_obj_set_size(ui->screen_label_1, 349, 22);

	//Write style for screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_2
	ui->screen_label_2 = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_label_2, "X");
	lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_2, 416, 209);
	lv_obj_set_size(ui->screen_label_2, 60, 60);

	//Write style for screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_2, &lv_font_montserratMedium_22, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_Timer
	ui->screen_Timer = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_Timer, "10:00");
	lv_label_set_long_mode(ui->screen_Timer, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_Timer, 320, 209);
	lv_obj_set_size(ui->screen_Timer, 100, 32);

	//Write style for screen_Timer, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_Timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_Timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_Timer, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_Timer, &lv_font_montserratMedium_19, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_Timer, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_Timer, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_Timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_Timer, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_Timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_Timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_Timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_Timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_Timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_Timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_CountOfCup
	ui->screen_CountOfCup = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_CountOfCup, "2");
	lv_label_set_long_mode(ui->screen_CountOfCup, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_CountOfCup, 416, 209);
	lv_obj_set_size(ui->screen_CountOfCup, 23, 25);

	//Write style for screen_CountOfCup, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_CountOfCup, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_CountOfCup, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_CountOfCup, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_CountOfCup, &lv_font_montserratMedium_22, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_CountOfCup, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_CountOfCup, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_CountOfCup, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_CountOfCup, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_CountOfCup, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_CountOfCup, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_CountOfCup, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_CountOfCup, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_CountOfCup, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_CountOfCup, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen);

	//Init events for screen.
	events_init_screen(ui);
}
