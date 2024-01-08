#include <combo.h>

static void ObjMure3_InitHijack(Actor_ObjMure3* this, void* data)
{
    Actor_ProcessInitChain(&this->base, data);
    this->actorIndex = g.actorIndex;
}

PATCH_CALL(0x80b55684, ObjMure3_InitHijack);

static void ObjMure3_SetPostSpawnHandler(Actor_ObjMure3* this)
{
    int count;

    /* Default */
    this->handler = actorAddr(AC_OBJ_MURE3, 0x80b5579c);

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

PATCH_CALL(0x80b55770, ObjMure3_SetPostSpawnHandler);
