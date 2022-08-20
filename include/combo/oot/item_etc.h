#ifndef COMBO_OOT_ITEM_ETC_H
#define COMBO_OOT_ITEM_ETC_H

#include <combo/oot/actor.h>

typedef struct PACKED ALIGNED(0x4)
{
    Actor   base;
    void*   update;
    u16     shaderId;
    s16     gi;
    u8      objIndex;
    char    unk_145[3];
    void*   draw;
    void*   load;
}
Actor_ItemEtcetera;

_Static_assert(sizeof(Actor_ItemEtcetera) == 0x150, "OoT Actor_ItemEtcetera size is wrong");

#endif

