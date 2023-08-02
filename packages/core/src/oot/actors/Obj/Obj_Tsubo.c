#include <combo.h>

void ObjTsubo_InitWrapper(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc init;

    init = actorAddr(AC_OBJ_TSUBO, 0x80a653a8);
    init(&this->base, play);
}
