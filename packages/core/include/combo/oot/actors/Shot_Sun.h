#ifndef COMBO_OOT_SHOT_SUN_H
#define COMBO_OOT_SHOT_SUN_H

#include <combo/common/actor.h>

typedef struct Actor_ShotSun Actor_ShotSun;

typedef void (*ShotSunActionFunc)(Actor_ShotSun*, GameState_Play*);

struct Actor_ShotSun
{
    /* 0x0000 */ Actor base;
    /* 0x013C */ ColliderCylinder collider;
    /* 0x0188 */ ShotSunActionFunc actionFunc;
    /* 0x018C */ Vec3s hitboxPos;
    /* 0x0192 */ s16 timer; // Frames until fairy spawns
    /* 0x0194 */ u8 fairySpawnerState;

    /* Extended flags */
    /* 0x0195 */ Xflag xflag;
};

_Static_assert(sizeof(Actor_ShotSun) == 0x019C, "OoT Actor_ShotSun size is wrong");

#endif
