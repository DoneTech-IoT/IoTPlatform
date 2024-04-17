idf_component_register(
    SRCS
    
"nvsFlash/nvsFlash.c"
"nvsFlash/SpiffsManger.c"
"JsonUtility/JsonUtility.c"
 INCLUDE_DIRS 

"JsonUtility/include"
"nvsFlash/include"
REQUIRES driver esp_timer GlobalInit  espressif__button nvs_flash  spiffs  json) 