
    
idf_component_register(SRCS  "esp_lcd_ili9488.c" "esp_lcd_ili9488_backlight"     
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_lcd esp_timer lvgl__lvgl) 
