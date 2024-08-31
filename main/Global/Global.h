#ifndef GLOBAL_VARIABLE_H_
#define GLOBAL_VARIABLE_H_
#include <stdint.h>
#include <sys/param.h>

typedef struct CoffeeMakerAppMatterVariable
{
    uint8_t GrinderLevel;
    uint8_t Cups;
    uint8_t TeaFlag;
    uint8_t CoffeeFlag;
    uint16_t UpdateTime;
} CoffeeMakerMatter_str;


#endif