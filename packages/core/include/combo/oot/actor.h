#ifndef INCLUDED_COMBO_OOT_ACTOR_H
#define INCLUDED_COMBO_OOT_ACTOR_H

#include <combo/util.h>
#include <combo/common/actor.h>
#include <combo/common/color.h>

typedef struct
{
    u32     count;
    Actor*  first;
}
ActorList;

_Static_assert(sizeof(ActorList) == 0x8, "OoT ActorList size is wrong");

typedef struct TargetContextEntry
{
    /* 0x00 */ Vec3f    pos;
    /* 0x0C */ f32      unk_0C; // radius?
    /* 0x10 */ Color_RGB8 color;
}
TargetContextEntry; // size = 0x14

_Static_assert(sizeof(TargetContextEntry) == 0x14, "TargetContextEntry size is wrong");

typedef struct TargetContext
{
    /* 0x00 */ Vec3f    naviRefPos; // possibly wrong
    /* 0x0C */ Vec3f    targetCenterPos;
    /* 0x18 */ Color_RGBAf naviInner;
    /* 0x28 */ Color_RGBAf naviOuter;
    /* 0x38 */ Actor*   arrowPointedActor;
    /* 0x3C */ Actor*   targetedActor;
    /* 0x40 */ f32      unk_40;
    /* 0x44 */ f32      unk_44;
    /* 0x48 */ s16      unk_48;
    /* 0x4A */ u8       activeCategory;
    /* 0x4B */ u8       unk_4B;
    /* 0x4C */ s8       unk_4C;
    /* 0x4D */ char     unk_4D[0x03];
    /* 0x50 */ TargetContextEntry arr_50[3];
    /* 0x8C */ Actor*   unk_8C;
    /* 0x90 */ Actor*   bgmEnemy; // The nearest enemy to player with the right flags that will trigger NA_BGM_ENEMY
    /* 0x94 */ Actor*   unk_94;
}
TargetContext; // size = 0x98

_Static_assert(sizeof(TargetContext) == 0x98, "TargetContext size is wrong");

typedef struct ActorContext
{
    char            unk_000[0xc];
    ActorList       actors[12];
    TargetContext   targetCtx;
    char            unk_104[0x3c];
}
ActorContext;

ASSERT_OFFSET(ActorContext, actors, 0x00c);
ASSERT_OFFSET(ActorContext, targetCtx, 0x06c);
ASSERT_OFFSET(ActorContext, unk_104, 0x104);
_Static_assert(sizeof(ActorContext) == 0x140, "OOT ActorContext size is wrong");

typedef struct
{
    /* 0x00 */ u8 numActors;
    /* 0x04 */ TransitionActorEntry* list;
}
TransitionActorContext; // size = 0x8

#endif
