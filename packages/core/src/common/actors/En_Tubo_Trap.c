#include <combo.h>
#include <combo/item.h>
#include <combo/csmc.h>
#include <combo/global.h>
#include <combo/actor.h>

#if defined(GAME_OOT)
# define ADDR_FUNC_INIT 0x80a77b30
# define ADDR_FUNC_DRAW 0x80a78684
# define ADDR_DLIST     0x5017870
#endif

#if defined(GAME_MM)
# define ADDR_FUNC_INIT 0x809307e0
# define ADDR_FUNC_DRAW 0x809313d8
# define ADDR_DLIST     0x5017ea0
#endif

void EnTuboTrap_InitWrapper(Actor_EnTuboTrap* this, PlayState* play)
{
    ActorFunc EnTuboTrap_Init;

    /* Xflag init */
    comboXflagInit(&this->xflag, &this->base, play);
    this->isExtended = Xflag_IsShuffled(&this->xflag);

    /* Forward init */
    EnTuboTrap_Init = actorAddr(ACTOR_EN_TUBO_TRAP, ADDR_FUNC_INIT);
    EnTuboTrap_Init(&this->base, play);
}

int EnTuboTrap_IsShuffled(Actor_EnTuboTrap* this)
{
    return this->isExtended && !comboXflagsGet(&this->xflag);
}

static void EnTuboTrap_Draw(Actor_EnTuboTrap* this, PlayState* play)
{
    ComboItemOverride o;

    if (EnTuboTrap_IsShuffled(this))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    csmcPotPreDraw(&this->base, play, o.gi, o.cloakGi, CSMC_POT_NORMAL_DANGEON);

    /* Draw the pot */
    Gfx_DrawDListOpa(play, (void*)ADDR_DLIST);
}

PATCH_FUNC(ADDR_FUNC_DRAW, EnTuboTrap_Draw);
