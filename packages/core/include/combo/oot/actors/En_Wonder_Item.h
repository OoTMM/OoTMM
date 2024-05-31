#ifndef COMBO_OOT_ACTOR_EN_WONDER_ITEM_H
#define COMBO_OOT_ACTOR_EN_WONDER_ITEM_H

#include <combo/actor.h>
#include <combo/xflags.h>

typedef struct Actor_EnWonderItem Actor_EnWonderItem;

typedef void (*Actor_EnWonderItem_Func)(Actor_EnWonderItem*, GameState_Play*);

struct ALIGNED(4) Actor_EnWonderItem
{
    Actor                   base;
    Actor_EnWonderItem_Func handler;
    char                    unk_140[0x04];
    u16                     type;
    u16                     itemId;
    char                    unk_148[0x0a];
    s16                     flag;
    char                    unk_154[0x6c];
    Xflag                   xflag;
    u8                      isExtended:1;
};

ASSERT_SIZE(Actor_EnWonderItem, 0x1c8);

#endif
