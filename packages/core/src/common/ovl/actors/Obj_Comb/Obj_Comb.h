#ifndef OVL_OBJ_COMB_H
#define OVL_OBJ_COMB_H

#include <combo/actor.h>
#include <combo/xflags.h>

typedef struct Actor_ObjComb Actor_ObjComb;

typedef void (*Actor_ObjCombFunc)(Actor_ObjComb*, GameState_Play*);

struct Actor_ObjComb
{
    Actor                   actor;
    Actor_ObjCombFunc       actionFunc;
    ColliderJntSph          collider;
    ColliderJntSphElement   colliderItems[1];

#if defined(GAME_OOT)
    s16                     unk_1B0;
    s16                     unk_1B2;
#endif

#if defined(GAME_MM)
    s16                     unk_1A8;
    s16                     unk_1AA;
    s16                     unk_1AC;
    s16                     unk_1AE;
    s16                     unk_1B0;
    s8                      unk_1B2;
    s8                      unk_1B3;
    s8                      unk_1B4;
    s8                      unk_1B5;
    s8                      unk_1B6;
    s8                      unk_1B7;
    s8                      unk_1B8;
#endif

    Xflag   xflag;
    u8      isExtended;
};

#endif
