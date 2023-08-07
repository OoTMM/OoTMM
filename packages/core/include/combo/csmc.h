#ifndef COMBO_CSMC_H
#define COMBO_CSMC_H

#include <combo/types.h>

#define CSMC_NORMAL     0x00
#define CSMC_BOSS_KEY   0x01
#define CSMC_MAJOR      0x02
#define CSMC_KEY        0x03
#define CSMC_SPIDER     0x04
#define CSMC_FAIRY      0x05
#define CSMC_HEART      0x06
#define CSMC_SOUL       0x07

typedef struct GameState_Play GameState_Play;
typedef struct Actor Actor;

/* Generic */
void*   csmcLoadTexture(u32 addr, int fmt, int bpp, int w, int h, int mirror);
int     csmcFromItem(s16 gi);
int     csmcEnabled(void);

/* Chest */
void    csmcChestInit(Actor* this, GameState_Play* play, s16 gi);
void    csmcChestPreDraw(Actor* this, GameState_Play* play, s16 gi);
int     csmcChestLarge(s16 gi);

#endif
