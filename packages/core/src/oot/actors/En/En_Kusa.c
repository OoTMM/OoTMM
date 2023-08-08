#include <combo.h>
#include <combo/csmc.h>
#include <combo/item.h>

void EnKusa_Aliases(Xflag* xflag)
{
    /* Check for generic grotto */
    if (xflag->sceneId == SCE_OOT_GROTTOS && xflag->roomId == 0x00)
    {
        xflag->roomId = 0x20 | gSaveContext.grottoChestFlag;
        return;
    }

    switch (xflag->sceneId)
    {
    case SCE_OOT_MARKET_CHILD_NIGHT:
        xflag->sceneId = SCE_OOT_MARKET_CHILD_DAY;
        xflag->id += 16;
        break;
    default:
        break;
    }
}

void EnKusa_InitWrapper(Actor_EnKusa* this, GameState_Play* play)
{
    ActorFunc init;

    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    /* Aliases */
    EnKusa_Aliases(&this->xflag);

    /* Forward init */
    init = actorAddr(AC_EN_KUSA, 0x80a7ff78);
    init(&this->base, play);
}

void EnKusa_SpawnShuffledDrop(Actor_EnKusa* this, GameState_Play* play)
{
    ActorFunc EnKusa_SpawnDrop;

    if (comboXflagsGet(&this->xflag))
    {
        /* Already spawned */
        EnKusa_SpawnDrop = actorAddr(AC_EN_KUSA, 0x80a7f964);
        EnKusa_SpawnDrop(&this->base, play);
        return;
    }

    /* Spawn a custom item */
    DropCustomItem(play, &this->base.position, &this->xflag);
}

void EnKusa_DrawWrapper(Actor_EnKusa* this, GameState_Play* play)
{
    ActorFunc EnKusa_Draw;
    ComboItemOverride o;
    int alt;

    if (comboConfig(CFG_OOT_SHUFFLE_GRASS) && !comboXflagsGet(&this->xflag))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    /* Prepare */
    if ((this->base.variable & 3) == 0)
        alt = 0;
    else
        alt = 1;
    csmcGrassPreDraw(play, o.gi, CSMC_GRASS_NORMAL, alt);

    /* Draw the actor */
    EnKusa_Draw = actorAddr(AC_EN_KUSA, 0x80a80a50);
    EnKusa_Draw(&this->base, play);
}
