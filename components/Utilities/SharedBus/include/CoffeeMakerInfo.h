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
    HEATER_STATE = 3,
    READY_STATE = 4,
    WARMER_STATE = 5, 
    EOF_COOKING_STATE = 6
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

typedef struct 
{
    bool PowerKey;
    PowerKeyMode PowerKeyMode;
    CookingMode CookingMode;
    MicroSwitchMode MicroSwitchMode;
    uint8_t GrinderLevel;
    uint8_t CupCounter;
} CoffeeMakerPacket;

typedef struct 
{
    CookingState CookingState;    
} CoffeeMakerEventPacket;

#ifdef __cplusplus
}
#endif

#endif //COFFEE_MAKER_IFO_H