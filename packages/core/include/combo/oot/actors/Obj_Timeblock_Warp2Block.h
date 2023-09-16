#ifndef COMBO_OOT_OBJ_TIMEBLOCK_WARP2BLOCK_H
#define COMBO_OOT_OBJ_TIMEBLOCK_WARP2BLOCK_H

#include <combo/common/actor.h>

typedef struct ALIGNED(4)
{
    Actor           base;
    char            unk_13c[0x20];
    /* 0x015C */ s16 demoEffectTimer;
    /* 0x015E */ s16 songEndTimer;
    /* 0x0160 */ s16 demoEffectFirstPartTimer;
    /* 0x0162 */ u16 unk_162;
    /* 0x0164 */ u8 unk_164; // s16 in Warp2Block
    /* 0x0165 */ u8 unk_165; // not present in Warp2Block
    /* 0x0166 */ u8 unk_166; // not present in Warp2Block
    /* 0x0167 */ u8 unk_167; // not present in Warp2Block
    /* 0x0168 */ u8 isVisible; // not present in Warp2Block
}
Actor_ObjTimeblockWarp2Block; // size = 0x016C

_Static_assert(sizeof(Actor_ObjTimeblockWarp2Block) == 0x16c, "OoT Actor_ObjTimeblockWarp2Block size is wrong");

#endif
