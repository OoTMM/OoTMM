#ifndef COMBO_MM_BOOMERANG_H
#define COMBO_MM_BOOMERANG_H

#include <combo/types.h>

typedef struct PlayState PlayState;
typedef struct Player Player;

s32 Player_InitItemAction_CustomBoomerang(Player* player, PlayState* play);
s32 Player_InitItemAction_CustomBoomerang_Upper(Player* player, PlayState* play);

#endif