#ifndef COMBO_OOT_ACTOR_ITEM_CUSTOM_H
#define COMBO_OOT_ACTOR_ITEM_CUSTOM_H

#if defined(GAME_OOT)
# include <combo/oot/actor.h>
#endif

#if defined(GAME_MM)
# include <combo/mm/actor.h>
#endif

typedef struct
{
    Actor base;
    s16   gi;
    u8    type;
    u8    flag;
}
Actor_ItemCustom;

#endif
