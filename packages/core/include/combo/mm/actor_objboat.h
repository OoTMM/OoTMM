#ifndef COMBO_MM_ACTOR_OBJBOAT_H
#define COMBO_MM_ACTOR_OBJBOAT_H

#include <combo/common/actor.h>

typedef struct {
    /* 0x000 */ Actor base;
    /* 0x144 */ char pad144[0x18];
    /* 0x15C */ u8 pathProgress;
    /* 0x15D */ s8 speedMultiplier;
    /* 0x15E */ u8 unk15E;
    /* 0x15F */ u8 unk15F;
    /* 0x160 */ char pad160[0x8];
} Actor_ObjBoat; // size = 0x168

ASSERT_OFFSET(Actor_ObjBoat, pathProgress, 0x15c);

_Static_assert(sizeof(Actor_ObjBoat) == 0x168, "MM Obj_Boat actor size is wrong");

#endif