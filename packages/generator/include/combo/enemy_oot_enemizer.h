#ifndef COMBO_ENEMY_OOT_ENEMIZER_H
#define COMBO_ENEMY_OOT_ENEMIZER_H

#include <combo.h>

int comboOotEnemizerCanSpawn(PlayState*, const ActorEntry*);
int comboEnemyActorPlacementKey(Actor*, PlayState*, u32*);
void comboOotEnemizerResetLizalfos(void);
void comboOotEnemizerDefeated(PlayState*, u32 placementKey);
int comboEnemyOotCombatRoomTimer(PlayState*, s32 room);

#endif
