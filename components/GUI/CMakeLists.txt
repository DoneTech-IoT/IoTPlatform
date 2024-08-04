idf_component_register(
    SRCS
    
"srcs/lvglTimer.c"
"srcs/lvglGui.c"
"generated/widgets_init.c"
"generated/events_init.c"
"generated/guider_fonts/lv_font_arial_32.c"
"generated/guider_fonts/lv_font_arial_18.c"
"generated/guider_fonts/lv_font_montserratMedium_16.c"
"generated/setup_scr_screen.c"
"generated/gui_guider.c"
"generated/images/_select_your_drink_alpha_100x100.c"
"generated/images/_icon_coffee_grind_lvl_1_alpha_90x90.c"
"generated/images/_icon_coffee_grind_lvl_3_alpha_90x90.c"
"generated/images/_icon_coffee_grind_lvl_2_alpha_90x90.c"
"generated/images/_icon_coffee_beans_alpha_96x96.c"
"generated/images/_icon_number_cups_alpha_56x56.c"
"generated/images/_icon_coffee_powder_alpha_96x96.c"
"generated/images/_frame_alpha_360x100.c"
"generated/images/_select_grind_size_alpha_100x100.c"
"generated/images/_icon_tea_leaf_alpha_96x96.c"
 INCLUDE_DIRS 

"generated"
"generated/guider_customer_fonts"
"include"
     REQUIRES lvgl__lvgl lvgl_esp32_drivers  driver esp_timer GlobalInit driver espressif__button) 
