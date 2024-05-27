#ifndef COMBO_COMMON_ACTOR_ITEM_DECOY_H
#define COMBO_COMMON_ACTOR_ITEM_DECOY_H

#include <combo/actor.h>

typedef struct Actor_ItemDecoy Actor_ItemDecoy;

typedef void (*Actor_ItemDecoyFunc)(Actor_ItemDecoy* this, GameState_Play* play);

typedef struct Actor_ItemDecoy
{
    Actor               base;
    Actor_ItemDecoyFunc handler;
    s16                 gi;
    u8                  player;
    u8                  playerFrom;
    s16                 count;
    u16                 timer;
}
Actor_ItemDecoy;

#endif
