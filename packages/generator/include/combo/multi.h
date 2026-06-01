#ifndef COMBO_MULTI_H
#define COMBO_MULTI_H

#include <combo/types.h>

typedef struct PlayState PlayState;

void    Multi_DrawWisps(PlayState* play);
void    Multi_ResetWisps(void);
void    Multi_Update(PlayState* play);

#endif
