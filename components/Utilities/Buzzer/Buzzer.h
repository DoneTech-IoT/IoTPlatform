#ifndef BUZZER_H
#define BUZZER_H

#ifdef __cplusplus
extern "C"
{    
#endif

typedef enum {
    NO_EFFECT = 0,
    ONE_BIZ = 1,
    DOUBLE_BIZ = 2,
    TRIPLE_BIZ = 3,
} BuzzerEffect;

void BuzzerInit(int gpio_num, uint32_t freq, uint32_t duration);
/** 
 * @brief play Buzzer with an effect
 * @param[in] effectNo effect that define in BuzzerEffect_t
 */
void BuzzerPlay(BuzzerEffect effectNo);

#ifdef __cplusplus
}
#endif
#endif //BUZZER_H