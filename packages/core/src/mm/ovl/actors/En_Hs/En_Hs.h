#ifndef COMBO_OVL_EN_HS_H
#define COMBO_OVL_EN_HS_H

#include <combo/actor.h>

struct Actor_EnHs;

typedef void (*Actor_EnHs_ActionFunc)(struct Actor_EnHs*, GameState_Play*);

#define HS_GET_EXIT_INDEX(this) ((this)->variable & 0xF)

#define HS_TYPE_UNK1 1

#define OBJECT_HS_LIMB_MAX 0x10

// params mystery: Vanilla Grog is 0xFE01
//   0xFE00 space is never checked in Grog code
//   at the same time, type UNK1 is only checked directly with params == 1, no &F
//   so HS_TYPE_UNK1 is never valid and is unused, as 0xFE00 is still present even if its not doing anything else

// The count of [chicks following the player]   is stored in [this->actor.home.rot.z] (incremented by 2)
// The count of [chicks transformed into adult] is stored in [this->actor.home.rot.x] (incremented by 2)

typedef struct Actor_EnHs 
{
    Actor actor;
    ColliderCylinder collider;
    SkelAnime skelAnime;
    Vec3s jointTable[OBJECT_HS_LIMB_MAX];
    Vec3s morphTable[OBJECT_HS_LIMB_MAX];
    Vec3s headRot;
    Vec3s torsoRot; // Set but unused
    u16 stateFlags;
    s16 stateTimer;
    Vec3f nwcPos[20]; // actual chick position are even values, odd values seem to be extra values for smoother chain
    Actor_EnHs_ActionFunc actionFunc;
} 
Actor_EnHs; // size = 0x398

_Static_assert(sizeof(Actor_EnHs) == 0x398, "Size of MM Actor_EnHs is wrong");

#endif