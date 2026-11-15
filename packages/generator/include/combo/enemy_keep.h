#ifndef COMBO_ENEMY_KEEP_H
#define COMBO_ENEMY_KEEP_H

#include <combo/types.h>

void* comboEnemyKeepAsset(u16 objectId, u32 offset);
void comboEnemyKeepReset(void);
void* comboEnemyArenaAlloc(u32 size);
#if defined(GAME_MM)
void comboEnemyArenaTrimOverlay(void*, u32 fileSize, u32 memorySize);
int comboEnemyArenaExpand(u32 size);
void* comboEnemyArenaAllocReserved(u32 size);
#endif

#endif
