idf_component_register(SRC_DIRS          "." "ServiceManger.cpp"  "Setup_GPIO.c" "CoffeeMakerApp.cpp"
                      PRIV_INCLUDE_DIRS  "."
                      INCLUDE_DIRS 

                      "GPIO"
                      "ServiceManager"
                      "CoffeeMaker"
                      )                      

spiffs_create_partition_image(
    storage 
    ../Data
    FLASH_IN_PROJECT)                      

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")