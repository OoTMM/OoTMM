#ifndef COMBO_ENEMY_ACTOR_LAYOUT_H
#define COMBO_ENEMY_ACTOR_LAYOUT_H

#include <combo/actor.h>
#include <combo/actor_id.h>

typedef struct
{
    Actor actor;
    SkelAnime skeleton;
    ActorFunc action;
    s16 timer, switchFlag;
} EnemyNativeWallmaster;
#if defined(GAME_OOT)
ASSERT_OFFSET(EnemyNativeWallmaster, switchFlag, 0x186);
#else
ASSERT_OFFSET(EnemyNativeWallmaster, switchFlag, 0x18e);
#endif

static inline int comboEnemyFrozenWallmaster(const Actor* actor)
{
    return actor && actor->id == ACTOR_EN_WALLMAS &&
           (((const EnemyNativeWallmaster*)actor)->switchFlag == 0x17f ||
            ((const EnemyNativeWallmaster*)actor)->switchFlag == 0x17e);
}

typedef struct
{
    Actor actor;
    ColliderCylinder body;
    ColliderJntSph explosion;
    ColliderJntSphElement explosionElement;
    s16 timer;
    s16 flashSpeedScale;
    f32 flashIntensity;
    u8 colliderSetOC;
#if defined(GAME_MM)
    u8 isPowderKeg;
    s16 unk1FA;
    u8 unk1FC;
#endif
    ActorFunc action;
} EnemyNativeBomb;

typedef struct
{
    Actor actor;
    ColliderQuad collider;
    WeaponInfo weapon;
    Vec3f tip;
    Vec3f previousTip;
    Actor* attached;
    Vec3f offset;
#if defined(GAME_MM)
    s8 unk208;
#endif
    s16 timer;
    ActorFunc action;
} EnemyNativeHookshot;

typedef struct
{
    Actor actor;
    ColliderCylinder stand;
    ColliderCylinder flame;
    s16 litTimer;
} EnemyNativeTorch;

#if defined(GAME_OOT)
ASSERT_OFFSET(EnemyNativeBomb, timer, 0x1e8);
ASSERT_SIZE(EnemyNativeBomb, 0x1f8);
ASSERT_OFFSET(EnemyNativeHookshot, attached, 0x1f0);
ASSERT_OFFSET(EnemyNativeHookshot, offset, 0x1f4);
ASSERT_SIZE(EnemyNativeHookshot, 0x208);
ASSERT_OFFSET(EnemyNativeTorch, litTimer, 0x1d4);
#else
ASSERT_OFFSET(EnemyNativeBomb, timer, 0x1f0);
ASSERT_OFFSET(EnemyNativeBomb, isPowderKeg, 0x1f9);
ASSERT_SIZE(EnemyNativeBomb, 0x204);
ASSERT_OFFSET(EnemyNativeHookshot, attached, 0x1f8);
ASSERT_OFFSET(EnemyNativeHookshot, offset, 0x1fc);
ASSERT_SIZE(EnemyNativeHookshot, 0x210);
ASSERT_OFFSET(EnemyNativeTorch, litTimer, 0x1dc);
#endif

typedef struct
{
    Actor actor;
#if defined(GAME_OOT)
    Vec3f bodyParts[4];
#endif
    SkelAnime skeleton;
    ActorFunc action;
#if defined(GAME_MM)
    u8 damageEffect;
#endif
    s16 timer, pitch, yaw;
    Vec3s joints[9], morph[9];
    ColliderJntSph collider;
    ColliderJntSphElement elements[1];
#if defined(GAME_MM)
    Vec3f bodyParts[4];
    f32 damageAlpha, frozenSteamScale, damageScale;
#endif
} EnemyNativeGuay;

#if defined(GAME_OOT)
ASSERT_OFFSET(EnemyNativeGuay, action, 0x1b0);
ASSERT_OFFSET(EnemyNativeGuay, collider, 0x228);
ASSERT_SIZE(EnemyNativeGuay, 0x288);
#else
ASSERT_OFFSET(EnemyNativeGuay, action, 0x188);
ASSERT_OFFSET(EnemyNativeGuay, collider, 0x200);
ASSERT_SIZE(EnemyNativeGuay, 0x29c);
#endif

static inline void comboEnemyNativeGuaySize(Actor* actor)
{
    EnemyNativeGuay* guay = (EnemyNativeGuay*)actor;

    if (actor->params == 1 && actor->update)
    {
        actor->scale.x = actor->scale.y = actor->scale.z = 0.03f;
        guay->collider.elements[0].dim.worldSphere.radius =
            guay->collider.elements[0].dim.modelSphere.radius * 3;
    }
}

#if defined(GAME_OOT)
typedef struct
{
    Actor actor;
    u8 animation[0x240 - sizeof(Actor)];
    ActorFunc action;
    s16 timeAlive, isNearWall, fallTargetYaw, deathTimer, knockbackTimer;
    s16 isMirroringPlayer, isPlayerOutOfRange, isKnockedback, hasSearchedForFlameCircles;
    f32 hoverVelocityTimer, animLastFrame, targetHeight, playerHeightOffset, headPitch, focusHeightOffset;
    Vec3f headPos, fireballRot, home;
} EnemyNativeAnubis;

ASSERT_OFFSET(EnemyNativeAnubis, action, 0x240);
ASSERT_OFFSET(EnemyNativeAnubis, isPlayerOutOfRange, 0x250);
ASSERT_OFFSET(EnemyNativeAnubis, home, 0x288);

typedef struct
{
    Actor actor;
    SkelAnime skeleton;
    Vec3s joints[24], morph[24];
    s32 state;
} EnemyNativePeahat;

ASSERT_OFFSET(EnemyNativePeahat, state, 0x2a0);

typedef struct
{
    Actor actor;
    ActorFunc action;
    s16 maxCurSpawns, curNumSpawn;
    s16 spawnType, maxTotalSpawns, totalNumSpawn, outOfRangeTimer, fieldSpawnTimer;
    s16 killCount, numLeeverSpawns, leeverIndex, timer;
    u8 reduceLeevers;
    f32 spawnRange;
    Actor* bigLeever;
} EnemyNativeEncounterSpawner;

typedef struct
{
    Actor actor;
    u8 unused[0x270 - sizeof(Actor)];
    s16 aimType;
} EnemyNativeLeeverAim;

ASSERT_OFFSET(EnemyNativeLeeverAim, aimType, 0x270);
ASSERT_OFFSET(EnemyNativeEncounterSpawner, leeverIndex, 0x152);
ASSERT_OFFSET(EnemyNativeEncounterSpawner, timer, 0x154);
ASSERT_SIZE(EnemyNativeEncounterSpawner, 0x160);

ASSERT_OFFSET(EnemyNativeEncounterSpawner, curNumSpawn, 0x142);
#endif

#endif
