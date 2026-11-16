#ifndef COMBO_ENEMY_GOLD_H
#define COMBO_ENEMY_GOLD_H

#include <combo.h>

void comboEnemyGoldWrap(Actor*, PlayState*, u16);
int comboEnemyGoldSpawning(void);
int comboEnemyGoldIsTarget(Actor*);
#if defined(GAME_MM)
u16 comboEnemyLilyPrepare(PlayState*, u16);
void comboEnemyLilyWrap(Actor*, PlayState*, u16);
int comboEnemyLilySpawning(void);
#endif

#endif
