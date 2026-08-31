#ifndef COMBO_AGE_H
#define COMBO_AGE_H

#define AGE_ADULT 0
#define AGE_CHILD 1

typedef struct PlayState PlayState;

int Age_GetStarting(void);
void Age_SetOot(PlayState* play, int age);
void Age_SwapOot(PlayState* play);
void Age_SetMm(PlayState* play, int age);
void Age_SwapMm(PlayState* play);

#endif
