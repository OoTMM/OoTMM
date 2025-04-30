#if 0
#include <combo.h>
#include <combo/global.h>
#include <combo/actor.h>

#if defined(GAME_OOT)
# define ADDR_HANDLER_POST_SPAWN 0x80b5579c
#else
# define ADDR_HANDLER_POST_SPAWN 0x8098f680
#endif

void ObjMure3_Alias(Xflag* xflag)
{
#if defined(GAME_OOT)
    switch (xflag->sceneId)
    {
    case SCE_OOT_LOST_WOODS:
        xflag->setupId = 2;
        xflag->id = 11;
        break;
    }
#endif
}

void ObjMure3_SetPostSpawnHandler(Actor_ObjMure3* this)
{
    int count;

    /* Default */
    this->handler = actorAddr(ACTOR_OBJ_MURE3, ADDR_HANDLER_POST_SPAWN);

    /* Upgrade spawns */
    count = ((this->base.params & 0xe000) == 0x4000) ? 7 : 5;
    for (int i = 0; i < count; ++i)
    {
        if (this->children[i])
        {
            EnItem00_XflagInitFreestanding(this->children[i], gPlay, this->actorIndex, i);
        }
    }
}
#endif
