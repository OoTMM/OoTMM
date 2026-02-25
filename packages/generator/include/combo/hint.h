#ifndef COMBO_HINT_H
#define COMBO_HINT_H

#include <combo/types.h>

#define HINT_GOSSIP         0x00
#define HINT_GOSSIP_GROTTO  0x01

#define HINT_TYPE_PATH          0x00
#define HINT_TYPE_FOOLISH       0x01
#define HINT_TYPE_ITEM_EXACT    0x02
#define HINT_TYPE_ITEM_REGION   0x03
#define HINT_TYPE_JUNK          0x04

typedef struct PlayState PlayState;

void Hint_Init(void);
void Hint_Display(PlayState* play, u8 key);
void Hint_DisplayJunk(PlayState* play, u32 key);
void Hint_DisplayJunkRandom(PlayState* play);

#endif
