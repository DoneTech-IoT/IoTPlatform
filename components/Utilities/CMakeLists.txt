idf_component_register(
    SRCS
    
"nvsFlash/nvsFlash.c"
"nvsFlash/SpiffsManger.c"
"JsonUtility/JsonUtility.c"
"Log/Custom_Log.c"
 INCLUDE_DIRS 

"JsonUtility/include"
"Log"
"nvsFlash/include"
     REQUIRES spiffs  nvs_flash esp_timer GlobalInit driver json Utilities) 