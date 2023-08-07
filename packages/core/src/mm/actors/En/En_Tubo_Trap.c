#include <combo.h>
#include <combo/custom.h>
#include <combo/csmc.h>
#include <combo/item.h>

void EnTuboTrap_InitWrapper(Actor_EnTuboTrap* this, GameState_Play* play)
{
    ActorFunc EnTuboTrap_Init;

    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.id = g.actorIndex;

    /* Forward init */
    EnTuboTrap_Init = actorAddr(AC_EN_TUBO_TRAP, 0x809307e0);
    EnTuboTrap_Init(&this->base, play);
}

static void EnTuboTrap_Draw(Actor_EnTuboTrap* this, GameState_Play* play)
{
    ComboItemOverride o;

    if (comboConfig(CFG_MM_SHUFFLE_POTS) && !comboXflagsGet(&this->xflag))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    csmcPotPreDraw(&this->base, play, o.gi, CSMC_POT_NORMAL_DANGEON);

    /* Draw the pot */
    DrawSimpleOpa(play, 0x05017ea0);
}

PATCH_FUNC(0x809313d8, EnTuboTrap_Draw);

void EnTuboTrap_SpawnShuffledDrop(Actor_EnTuboTrap* this, GameState_Play* play)
{
    if (comboXflagsGet(&this->xflag))
    {
        /* Already spawned */
        return;
    }

    /* Spawn an extended item */
    DropCustomItem(play, &this->base.position, &this->xflag);
}
