#ifndef DONE_COFFEE_MAKER_H
#define DONE_COFFEE_MAKER_H

#include <esp_err.h>
#include <esp_matter.h>

#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_ID 0x1001
#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_VERSION 1

typedef enum {
    INCREMENT_MODE = 0,
    DECREMENT_MODE = 1,
    EXPLICIT_MODE = 2    
}LevelControlCurrentValueMode_t;

typedef enum {
    DONT_CARE = 0,
    GRINDER_MODE = 1,
    COFFEE_MODE = 2,
    TEA_MODE = 3    
}CookingMode_t;

typedef enum {
    OFF_MODE = 0,
    ON_MODE = 1,
    PAUSE_MODE = 2,
    RESUME_MODE = 3    
}PowerKeyMode_t;

typedef enum {
    ERROR_MODE = 0,        
    NORMAL_MODE = 1    
}MicroSwitchMode_t;

esp_err_t create_DoneCoffeeMaker(esp_matter::node_t* node);

#endif //DONE_COFFEE_MAKER_H