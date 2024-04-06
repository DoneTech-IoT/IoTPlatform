idf_component_register(
    SRCS
    
"nvsFlash/nvsFlash.c"
"nvsFlash/SpiffsManger.c"
"GPIO/Setup_GPIO.c"
"JsonUtility/JsonUtility.c"
 INCLUDE_DIRS 

"GPIO/include"
"JsonUtility/include"
"nvsFlash/include"
     REQUIRES driver esp_timer GlobalInit  espressif__button nvs_flash  spiffs  json) 