#ifndef COFFEE_MAKER_INFO_H
#define COFFEE_MAKER_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    DONT_CARE = 0,
    GRINDER_MODE = 1,
    COFFEE_MODE = 2,
    TEA_MODE = 3
} CookingMode;

typedef enum
{    
    COFFEE_BEAN_STATE =1,
    GRINDER_STATE = 2,
    COFFEE_STATE = 3,
    TEA_STATE = 4,
    HEATER_STATE = 5,
    READY_STATE = 6,
    WARMER_STATE = 7, 
    EOF_COOKING_STATE = 8
} CookingState;

typedef enum
{
    STANDBY_MODE = 1,
    ON_MODE = 2,
    PAUSE_MODE = 3
} PowerKeyMode;

typedef enum
{
    ERROR_MODE = 0,
    NORMAL_MODE = 1
} MicroSwitchMode;

typedef enum 
{
    POWER_KEY_INDEX = 0,
    POWER_KEY_MODE_INDEX,
    COOKING_MODE_INDEX,
    MICRO_SWITCH_MODE_INDEX,
    GRINDER_LEVEL_INDEX,
    CUP_COUNTER_INDEX,
} CoffeeMakerEventIndex;

typedef struct 
{
    bool PowerKey;
    PowerKeyMode PowerKeyMode;
    CookingMode CookingMode;
    MicroSwitchMode MicroSwitchMode;
    uint8_t GrinderLevel;
    uint8_t CupCounter;
    CoffeeMakerEventIndex CoffeeMakerEventIndex;
} CoffeeMakerPacket;

typedef struct 
{
    CookingState CookingState;    
} CoffeeMakerEventPacket;

#ifdef __cplusplus
}
#endif

#endif //COFFEE_MAKER_IFO_H