#ifndef COMBO_MULTI_H
#define COMBO_MULTI_H

#include <combo/types.h>

typedef struct PlayState PlayState;

void    Multi_DrawWisps(PlayState* play);
void    Multi_ResetWisps(void);
void    Multi_Update(PlayState* play);

void    Multi_SendItem(u8 to, s16 gi, s16 flags, u32 key);
void    Multi_SendSelfItem(s16 gi, s16 flags, u32 key);

#endif
