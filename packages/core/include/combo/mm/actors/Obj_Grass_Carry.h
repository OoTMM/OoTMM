#ifndef COMBO_MM_OBJ_GRASS_CARRY_H
#define COMBO_MM_OBJ_GRASS_CARRY_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct Actor_ObjGrass_PackBush Actor_ObjGrass_PackBush;

typedef struct ALIGNED(4)
{
    Actor                       base;
    char                        unk[0x50];
    Actor_ObjGrass_PackBush*    bush;
    u16                         drop;
    char                        unk2[0x2];
    void*                       handler;
}
Actor_ObjGrassCarry;

_Static_assert(sizeof(Actor_ObjGrassCarry) == 0x1a0, "MM Actor_ObjGrass size is wrong");

#endif
