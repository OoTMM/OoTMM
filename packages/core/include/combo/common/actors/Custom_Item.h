#ifndef COMBO_ACTORS_CUSTOM_ITEM_H
#define COMBO_ACTORS_CUSTOM_ITEM_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct GameState_Play GameState_Play;
typedef struct Actor_CustomItem Actor_CustomItem;

typedef void (*ActorFunc_CustomItem)(Actor_CustomItem*, GameState_Play*);

typedef struct ALIGNED(4) Actor_CustomItem
{
    Actor                   base;
    ActorFunc_CustomItem    handler;
    Xflag                   xflag;
    s16                     extendedGi;
    s16                     extendedGiDraw;
    u8                      extendedMajor;
    u8                      timer;
}
Actor_CustomItem;

Actor_CustomItem* DropCustomItem(GameState_Play* play, const Vec3f* pos, const Xflag* xflag);

#endif
