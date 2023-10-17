#include <combo.h>

static void ElfMsg6_Update(Actor* this, GameState_Play* play)
{
    ActorFunc handler;

    /* Make tatl shut up (except near ocean) */
    if (!(play->sceneId == SCE_MM_TERMINA_FIELD && this->variable == 0xe05))
        return;

    handler = *(void**)((char*)this + 0x144);
    handler(this, play);
}

PATCH_FUNC(0x80ba22b0, ElfMsg6_Update);
