#ifndef COMBO_ENEMY_SCRIPTED_H
#define COMBO_ENEMY_SCRIPTED_H

#include <combo.h>
#include <combo/enemy_native.h>

#define ENEMY_SCRIPTED_MM 128u
#define ENEMY_SCRIPTED_MAGIC 0x45534131u
typedef struct {
    Actor actor;
    u32 binding;
    ActorEntry original;
    s16 targetId;
    u16 targetParams;
    u8 actorIndex;
    u8 pending;
    u32 placementVrom, placementVisit;
    u8 placementRoom, placementSlot;
} EnemyScriptedBinding;

int comboEnemyScriptedCapturePlacement(Actor*, PlayState*, const ActorEntry*);
Actor* comboEnemyScriptedSpawnTarget(Actor*, PlayState*, f32, f32, f32, s16, EnemyFamily*);
void comboEnemyScriptedCancelPlacement(Actor*, PlayState*);

#endif
