idf_component_register(SRC_DIRS          "."
                      PRIV_INCLUDE_DIRS  ".")                      

spiffs_create_partition_image(
    storage 
    ../Data
    FLASH_IN_PROJECT)                      
    set(BUILD_DIR ${CMAKE_BINARY_DIR})
    set(SDKCONFIG "${BUILD_DIR}/sdkconfig")
    set(SDKCONFIG_DEFAULTS "sdkconfig.defaults")
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")
