idf_component_register(
SRCS       "app_main.cpp"  "ServiceManager/ServiceManager.cpp"


INCLUDE_DIRS          "ServiceManager"
                      )                      

spiffs_create_partition_image(
    storage 
    ../Data
    FLASH_IN_PROJECT)                      

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")