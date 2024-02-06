#include <combo.h>

#if defined(GAME_OOT)
# define ADDR_HANDLER_POST_SPAWN 0x80b5579c
#else
# define ADDR_HANDLER_POST_SPAWN 0x8098f680
#endif

void ObjMure3_InitHijack(Actor_ObjMure3* this, void* data)
{
    Actor_ProcessInitChain(&this->base, data);
    this->actorIndex = g.actorIndex;
}

void ObjMure3_SetPostSpawnHandler(Actor_ObjMure3* this)
{
    int count;

    /* Default */
    this->handler = actorAddr(AC_OBJ_MURE3, ADDR_HANDLER_POST_SPAWN);

    /* Upgrade spawns */
    count = ((this->base.variable & 0xe000) == 0x4000) ? 7 : 5;
    for (int i = 0; i < count; ++i)
    {
        if (this->children[i])
        {
            EnItem00_XflagInitFreestanding(this->children[i], gPlay, this->actorIndex, i);
        }
    }
}
