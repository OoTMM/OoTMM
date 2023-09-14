#ifndef COMBO_MM_OBJ_GRASS_H
#define COMBO_MM_OBJ_GRASS_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct ALIGNED(4) Actor_ObjGrass_PackBush
{
    Vec3f   position;
    char    unk[0x8];
}
Actor_ObjGrass_PackBush;

typedef struct ALIGNED(4)
{
    char                    unk_00[0xc];
    Actor_ObjGrass_PackBush bushes[12];
    u16                     count;
    char                    unk_fe[2];
}
Actor_ObjGrass_Pack;

typedef struct ALIGNED(4)
{
    Actor                   base;
    Actor_ObjGrass_Pack     packs[40];
    u16                     count;
    char                    unk[0x95a];

    /* Custom */
    Xflag                   xflags[40];
}
Actor_ObjGrass;

_Static_assert(sizeof(Actor_ObjGrass_PackBush) == 0x14, "MM Actor_ObjGrass_PackBush size is wrong");
_Static_assert(sizeof(Actor_ObjGrass_Pack) == 0x100, "MM Actor_ObjGrass_Pack size is wrong");
_Static_assert(sizeof(Actor_ObjGrass) == 0x3368, "MM Actor_ObjGrass size is wrong");

#endif
