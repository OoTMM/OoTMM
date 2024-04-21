#ifndef COMBO_PLAYER_H
#define COMBO_PLAYER_H

#include <combo/types.h>

#if defined(GAME_OOT)
# include <combo/oot/player.h>
#endif

#if defined(GAME_MM)
# include <combo/mm/player.h>
#endif

typedef struct GameState_Play GameState_Play;
typedef struct Actor_Player Actor_Player;

void FreezePlayer(GameState_Play* play);
void UnfreezePlayer(GameState_Play* play);
void Player_UseItem(GameState_Play* play, Actor_Player* link, s16 itemId);

#endif
