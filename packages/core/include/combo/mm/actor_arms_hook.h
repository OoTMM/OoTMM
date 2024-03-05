#ifndef COMBO_MM_ACTOR_ARMSHOOK_H
#define COMBO_MM_ACTOR_ARMSHOOK_H

#include <combo/common/actor.h>

struct Actor_ArmsHook;

typedef void (*ArmsHookActionFunc)(struct Actor_ArmsHook*, GameState_Play*);

typedef struct Actor_ArmsHook {
    /* 0x000 */ Actor actor;
    /* 0x144 */ u8 collider[0x80]; /* ColliderQuad collider; */
    /* 0x1C4 */ u8 weaponInfo[0x1C]; /* WeaponInfo weaponInfo; */
    /* 0x1E0 */ u8 unk1E0[0xC]; /* Vec3f unk1E0; */
    /* 0x1EC */ u8 unk1EC[0xC]; /* Vec3f unk1EC; */
    /* 0x1F8 */ Actor* grabbed;
    /* 0x1FC */ u8 unk1FC[0xC]; /* Vec3f unk1FC; */
    /* 0x208 */ s8 unk_208;
    /* 0x20A */ s16 timer;
    /* 0x20C */ ArmsHookActionFunc actionFunc;
} Actor_ArmsHook; /* size = 0x210 */

_Static_assert(sizeof(Actor_ArmsHook) == 0x210, "Actor_ArmsHook size is wrong");

#endif
