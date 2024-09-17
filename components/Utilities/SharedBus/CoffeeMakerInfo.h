#ifndef COFFEE_MAKER_IFO_H
#define COFFEE_MAKER_IFO_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
POWER_KEY = 0,
GRINDER_KEY = 1,
COFFEE_KEY = 2,
TEA_KEY = 3,
CUP_COUNTER_KEY = 4,
MICRO_SWITCH_KEY = 5
} Keys_t;

typedef enum
{
DONT_CARE = 0,
GRINDER_MODE = 1,
COFFEE_MODE = 2,
TEA_MODE = 3
} CookingMode_t;

typedef enum
{
STANDBY_MODE = 1,
ON_MODE = 2,
PAUSE_MODE = 3
} PowerKeyMode_t;

#ifdef __cplusplus
}
#endif

#endif //COFFEE_MAKER_IFO_H