file(GLOB LCD_SOURCES)
    if(CONFIG_TFT_BACKLIGHT_ENABLE)
        list(APPEND LCD_SOURCES "esp_lcd_backlight.c")
    endif()
    list(APPEND LCD_SOURCES "esp_lcd_ili9488.c")



    
idf_component_register(SRCS  ${LCD_SOURCES}
                        
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_lcd esp_timer lvgl__lvgl) 
