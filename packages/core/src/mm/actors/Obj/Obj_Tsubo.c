#include <combo.h>
#include <combo/custom.h>
#include <combo/csmc.h>
#include <combo/item.h>
#include <combo/actor.h>

#if 0
void ObjTsubo_SpawnShuffledDrop(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc ObjTsubo_SpawnDrop;

    if (ObjTsubo_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->base.world.pos, &this->xflag);
        return;
    }

    /* Already spawned */
    ObjTsubo_SpawnDrop = (ActorFunc)actorAddr(AC_OBJ_TSUBO, 0x80927690);
    ObjTsubo_SpawnDrop(&this->base, play);
}

void ObjTsubo_SpawnShuffledDropFlexible(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc ObjTsubo_SpawnDropFlexible;

    if (ObjTsubo_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->base.world.pos, &this->xflag);
        return;
    }

    ObjTsubo_SpawnDropFlexible = (ActorFunc)actorAddr(AC_OBJ_TSUBO, 0x8092762c);
    ObjTsubo_SpawnDropFlexible(&this->base, play);
}
PATCH_FUNC(0x809294b0, ObjTsubo_Draw);
#endif
