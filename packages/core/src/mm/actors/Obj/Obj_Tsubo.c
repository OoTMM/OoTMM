#include <combo.h>

void ObjTsubo_InitWrapper(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc ObjTsubo_Init;

    /* Setup the xflag */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = 0; /* TODO - Chnage this */
    this->xflag.roomId = this->base.room;
    this->xflag.id = g.actorIndex;

    /* Invoke init */
    ObjTsubo_Init = (ActorFunc)actorAddr(AC_OBJ_TSUBO, 0x809278c0);
    ObjTsubo_Init(&this->base, play);
}

void ObjTsubo_SpawnShuffledDrop(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc ObjTsubo_SpawnDrop;

    if (comboXflagsGet(&this->xflag))
    {
        /* Already spawned */
        ObjTsubo_SpawnDrop = (ActorFunc)actorAddr(AC_OBJ_TSUBO, 0x80927690);
        ObjTsubo_SpawnDrop(&this->base, play);
        return;
    }

    /* Spawn a custom item */
    DropCustomItem(play, &this->base.position, &this->xflag);
}
