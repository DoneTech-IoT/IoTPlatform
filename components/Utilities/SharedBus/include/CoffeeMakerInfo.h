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
    AUTO_MODE = 0,
    MANUAL_MODE = 1    
} AttributeUpdateMode;

typedef struct 
{
    bool PowerKey;
    PowerKeyMode busPowerKeyMode;
    CookingMode busCookingMode;
    MicroSwitchMode busMicroSwitchMode;
    uint8_t GrinderLevel;
    uint8_t CupCounter;
} CoffeeMakerPacket;

#ifdef __cplusplus
}
#endif

#endif //COFFEE_MAKER_IFO_H