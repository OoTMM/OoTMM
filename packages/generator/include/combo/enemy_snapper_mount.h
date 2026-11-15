#ifndef COMBO_ENEMY_SNAPPER_MOUNT_H
#define COMBO_ENEMY_SNAPPER_MOUNT_H

#include <combo.h>

#if defined(GAME_MM)
#define ACTOR_ENEMY_SNAPPER_MOUNT 0x052
#else
#define ACTOR_ENEMY_SNAPPER_MOUNT 0x1aa
#endif

enum {
    SNAPPER_MOUNT_EMERGE,
    SNAPPER_MOUNT_READY,
    SNAPPER_MOUNT_REAR,
    SNAPPER_MOUNT_RETREAT,
    SNAPPER_MOUNT_EXTEND,
    SNAPPER_MOUNT_SPINUP,
    SNAPPER_MOUNT_CHARGE,
    SNAPPER_MOUNT_RETRACT,
    SNAPPER_MOUNT_RECOVER,
    SNAPPER_MOUNT_FLIPPING,
    SNAPPER_MOUNT_OVERTURNED,
    SNAPPER_MOUNT_DEFEAT,
};

typedef struct {
    Vec3f pos, velocity;
    Vec3s rotation;
    f32 scale;
} SnapperMountDebris;

typedef struct {
    Actor actor;
    void* model;
    SkelAnime normalSkel, shellSkel;
    Vec3s normalJoints[13], normalMorph[13], shellJoints[4], shellMorph[4];
    ColliderCylinder body;
    u8 mode;
    u8 previousMode;
    u8 carrying;
    u8 walkToRider;
    u8 chargeFrames;
    u8 wallImpact;
    u8 spinTimer;
    f32 spikes;
    f32 shellScale;
    f32 limbStretch;
    f32 bodyHeight;
    f32 riderLift;
    s16 spin;
    u8 debrisTimer;
    SnapperMountDebris debris[20];
} EnemySnapperMount;

#endif
