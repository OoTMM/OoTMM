#ifndef COMBO_MM_DARK_LINK_H
#define COMBO_MM_DARK_LINK_H

#include <combo.h>

#define MM_DARK_LINK_ACTOR_ID 0xc33

int comboMmDarkLinkPerch(Actor* owner, PlayState* play, int hold);
void comboMmDarkLinkUpdatePlayer(Player* player, PlayState* play);

#endif
