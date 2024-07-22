#ifndef BUZZER_H
#define BUZZER_H

#ifdef __cplusplus
extern "C"
{    
typedef enum {
    NO_EFFECT = 0,
    ON_BIZ = 1,
    DOUBLE_BIZ = 2,
    TRIPLE_BIZ = 3,
}BuzzerEffect_t;

void BuzzerPlay(BuzzerEffect_t effectNo);

#ifdef __cplusplus
}
#endif
#endif //BUZZER_H