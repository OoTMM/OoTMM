#ifndef COMBO_ENEMY_RESOURCE_H
#define COMBO_ENEMY_RESOURCE_H

#include <combo.h>

typedef struct {
    ActorEntry original;
    s8 room;
    u8 state;
    u8 actorIndex;
} EnemyNativeResource;

_Static_assert(sizeof(EnemyNativeResource) == 20, "compact native resource state retains exact ActorEntry");

#define ENEMY_RESOURCE_WAITING 1
#define ENEMY_RESOURCE_PENDING 2
#define ENEMY_RESOURCE_RESTORED 3

extern u8 gCurrentSpawnActorNum;
int comboEnemyNativeResourceInit(EnemyNativeResource*, const ActorEntry*, s32 room);
int comboEnemyNativeResourceAlreadyComplete(PlayState*, const EnemyNativeResource*);
void comboEnemyNativeResourceDefeated(EnemyNativeResource*);
void comboEnemyNativeResourceUpdate(PlayState*, EnemyNativeResource*);

#endif
