#ifndef COMBO_ENEMY_MINISLIME_H
#define COMBO_ENEMY_MINISLIME_H

#include <combo/enemy_flight.h>

#if defined(GAME_MM)
#define ACTOR_ENEMY_MINISLIME 0x057
#else
#define ACTOR_ENEMY_MINISLIME 0x0f3
#endif

enum { MINISLIME_COUNT = 15 };
enum { MINISLIME_ACTIVE, MINISLIME_TO_GEKKO, MINISLIME_REGROUP, MINISLIME_DEFEAT_IDLE };

typedef struct {
    Vec3f pos, velocity;
    Vec3s rotation;
    f32 scale;
} MinislimeIceShard;

typedef struct EnemyMinislime {
    Actor actor;
    ColliderCylinder body;
    Actor* owner;
    struct EnemyMinislime** ownerSlot;
    MinislimeIceShard shards[10];
    Vec3f freezeAnchor;
    Vec3f hookSafePos;
    Vec3f motionScale;
    f32 shardFloor;
#if defined(GAME_OOT)
    struct {
        Vec3f pos, velocity;
        u8 life;
    } smoke[2];
#endif
    s16 spin;
    u8 mode;
    u8 launch;
    u8 wasHooked;
    u8 shattering;
    u8 attackDelay;
    u8 knockback;
    u8 throwTimer;
    u8 freeze;
    u8 frozenAlpha;
    u8 bounce;
    u8 bounceWindup;
    u8 idleTimer;
    u8 melt;
} EnemyMinislime;

#endif
