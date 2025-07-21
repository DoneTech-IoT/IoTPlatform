file(GLOB SOURCES)
file(GLOB INCLUDES)
 
file(GLOB_RECURSE C_SOURCES "*.c")
file(GLOB_RECURSE CPP_SOURCES "app_main.cpp" "ServiceManager/ServiceMngr.cpp")
list(APPEND SOURCES ${C_SOURCES} ${CPP_SOURCES})
list(APPEND INCLUDES "GPIO" "ServiceManager")
                                      
idf_component_register(
    SRCS ${SOURCES} 
    INCLUDE_DIRS ${INCLUDES})

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")