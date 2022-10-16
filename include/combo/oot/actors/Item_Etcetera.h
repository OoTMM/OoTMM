#ifndef COMBO_OOT_ITEM_ETC_H
#define COMBO_OOT_ITEM_ETC_H

#include <combo/common/actor.h>

typedef struct Actor_ItemEtcetera Actor_ItemEtcetera;

typedef void (*Actor_ItemEtcetera_Func)(Actor_ItemEtcetera*, GameState_Play*);

struct PACKED ALIGNED(0x4) Actor_ItemEtcetera
{
    Actor                       base;
    Actor_ItemEtcetera_Func     update;
    u16                         shaderId;
    s16                         gi;
    s8                          objIndex;
    char                        unk_145[3];
    Actor_ItemEtcetera_Func     draw;
    Actor_ItemEtcetera_Func     load;
};

_Static_assert(sizeof(Actor_ItemEtcetera) == 0x150, "OoT Actor_ItemEtcetera size is wrong");

#endif
