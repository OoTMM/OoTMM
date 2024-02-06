#include <combo.h>
#include <combo/csmc.h>
#include <combo/item.h>

void EnKusa_Aliases(Xflag* xflag)
{
    switch (xflag->sceneId)
    {
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        xflag->sceneId = SCE_MM_SOUTHERN_SWAMP;
        if (xflag->roomId == 0x00)
            xflag->id += 6;
        else
            xflag->id += 3;
        break;
    case SCE_MM_IKANA_CANYON:
        if (xflag->setupId == 3)
        {
            xflag->setupId = 0;
            xflag->id += 30;
        }
        break;
    case SCE_MM_GREAT_BAY_COAST:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            xflag->id -= 4;
        }
        break;
    case SCE_MM_WOODFALL:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            xflag->id += 8;
        }
        break;
    case SCE_MM_GROTTOS:
        if (xflag->roomId == 0x04)
            xflag->roomId = 0x20 | (GROTTO_CHEST_FLAG(gSaveContext) & 0x1f);
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
    init = actorAddr(AC_EN_KUSA, 0x80935674);
    init(&this->base, play);
}

void EnKusa_SpawnShuffledDrop(Actor_EnKusa* this, GameState_Play* play)
{
    ActorFunc EnKusa_SpawnDrop;

    if (comboXflagsGet(&this->xflag))
    {
        /* Already spawned */
        EnKusa_SpawnDrop = actorAddr(AC_EN_KUSA, 0x80934ffc);
        EnKusa_SpawnDrop(&this->base, play);
        return;
    }

    /* Spawn a custom item */
    EnItem00_DropCustom(play, &this->base.world.pos, &this->xflag);
}

void EnKusa_DrawWrapper(Actor_EnKusa* this, GameState_Play* play)
{
    ActorFunc EnKusa_Draw;
    ComboItemOverride o;
    int alt;

    if (comboConfig(CFG_MM_SHUFFLE_GRASS) && !comboXflagsGet(&this->xflag))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    /* Prepare */
    if ((this->base.variable & 3) == 0)
        alt = 0;
    else
        alt = 1;
    csmcGrassPreDraw(play, o.gi, CSMC_GRASS_NORMAL, alt, 0);

    /* Draw the actor */
    EnKusa_Draw = actorAddr(AC_EN_KUSA, alt ? 0x809365cc : 0x80936414);
    EnKusa_Draw(&this->base, play);
}
