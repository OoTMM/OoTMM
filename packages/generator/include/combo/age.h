#ifndef COMBO_AGE_H
#define COMBO_AGE_H

#define AGE_ADULT 0
#define AGE_CHILD 1

typedef struct PlayState PlayState;

void Age_SetOot(PlayState* play, int age);
void Age_SwapOot(PlayState* play);

#endif
