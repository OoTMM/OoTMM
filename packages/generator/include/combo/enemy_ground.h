#ifndef COMBO_ENEMY_GROUND_H
#define COMBO_ENEMY_GROUND_H

#include <combo.h>

typedef struct {
    Actor* actor;
    Vec3f position;
    CollisionPoly* floor;
    f32 floorHeight;
    s8 room;
    u8 category;
    u8 floorBgId;
    u8 active;
} EnemyGroundStep;

typedef struct {
    EnemyGroundStep steps[2];
} EnemyGroundGuard;

int comboEnemyAirBound(Actor* actor);
int comboEnemyAirPlacement(PlayState* play, Actor* actor, f32 clearance);
void comboEnemyGroundGuardBegin(PlayState* play, Actor* actor, EnemyGroundGuard* guard);
void comboEnemyGroundGuardBeginNative(Actor* actor, EnemyGroundGuard* guard);
void comboEnemyGroundGuardEnd(PlayState* play, EnemyGroundGuard* guard);

#endif
