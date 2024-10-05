idf_component_register(
SRCS       "app_main.cpp"  "ServiceManager/ServiceManager.cpp" "GPIO/Setup_GPIO.c" "CoffeeMaker/CoffeeMakerApp.cpp" "PulsCounter/PulseCounter.c"


INCLUDE_DIRS          
                      "GPIO"
                      "ServiceManager"
                      "CoffeeMaker"
                      "PulsCounter"
                      )                      
                     

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")