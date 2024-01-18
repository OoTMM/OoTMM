#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define ADDR_FUNC_INIT 0x80a653a8
#endif

#if defined(GAME_MM)
# define ADDR_FUNC_INIT 0x809278c0
#endif

void ObjTsubo_InitWrapper(Actor_ObjTsubo* this, GameState_Play* play)
{
    ComboItemOverride o;
    ActorFunc init;

    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    /* Fix the aliases */
    ObjTsubo_Alias(this);

    /* Detect xflags */
    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));

    /* Forward init */
    init = actorAddr(AC_OBJ_TSUBO, ADDR_FUNC_INIT);
    init(&this->base, play);
}

int ObjTsubo_IsShuffled(Actor_ObjTsubo* this)
{
    if (!this->isExtended || comboXflagsGet(&this->xflag))
        return 0;
    return 1;
}
