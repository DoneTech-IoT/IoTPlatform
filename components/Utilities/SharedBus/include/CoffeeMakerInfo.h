#ifndef COFFEE_MAKER_INFO_H
#define COFFEE_MAKER_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif

//#include ""

typedef enum
{
    INCREMENT_MODE = 0,
    DECREMENT_MODE = 1,
    EXPLICIT_MODE = 2
} LevelControlCurrentValueMode;

typedef enum
{
    POWER_KEY = 0,
    GRINDER_KEY = 1,
    COFFEE_KEY = 2,
    TEA_KEY = 3,
    CUP_COUNTER_KEY = 4,
    MICRO_SWITCH_KEY = 5
} DeviceKeys;

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
    MICRO_SWITCH = 0,
    AUTO_TURN_OFF = 1,
    TIMER_COUNT
} TimerID;

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