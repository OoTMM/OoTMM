#include <combo.h>
#include <combo/csmc.h>
#include <combo/item.h>

void EnKusa_Aliases(Xflag* xflag)
{
    /* Check for generic grotto */
    if (xflag->sceneId == SCE_OOT_GROTTOS && xflag->roomId == 0x00)
    {
        xflag->roomId = 0x20 | (gGrottoData & 0x1f);
        return;
    }

    switch (xflag->sceneId)
    {
    case SCE_OOT_MARKET_CHILD_NIGHT:
        xflag->sceneId = SCE_OOT_MARKET_CHILD_DAY;
        xflag->id += 16;
        break;
    case SCE_OOT_HYRULE_FIELD:
        switch (xflag->setupId)
        {
        case 1: xflag->id += 2; break;
        case 2: xflag->id += 8; break;
        }
        xflag->setupId = 0;
        break;
    case SCE_OOT_KAKARIKO_VILLAGE:
        switch (xflag->setupId)
        {
        case 1: xflag->id += 5; break;
        case 2: xflag->id += 12; break;
        case 3: xflag->id += 14; break;
        }
        xflag->setupId = 0;
        break;
    case SCE_OOT_GRAVEYARD:
        switch (xflag->setupId)
        {
        case 1: xflag->id -= 4; break;
        case 2: xflag->id -= 13; break;
        case 3: xflag->id -= 13; break;
        }
        xflag->setupId = 0;
        break;
    case SCE_OOT_ZORA_RIVER:
        if (xflag->setupId == 2)
        {
            xflag->id += 28;
            xflag->setupId = 0;
        }
        break;
    case SCE_OOT_KOKIRI_FOREST:
        if (xflag->setupId == 3)
        {
            xflag->setupId = 2; /* Next case */
            xflag->id -= 7;
        }
        if (xflag->setupId == 2 && xflag->id < 23)
        {
            xflag->setupId = 0;
            xflag->id += 45;
        }
        if (xflag->setupId == 0 && xflag->roomId == 2 && xflag->id == 9)
        {
            xflag->roomId = 0;
            xflag->id = 62;
        }
        break;
    case SCE_OOT_LAKE_HYLIA:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            if (xflag->id < 34)
                xflag->id -= 8;
            else
                xflag->id -= 4;
        }
        break;
    case SCE_OOT_LOST_WOODS:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            switch (xflag->roomId)
            {
            case 2: xflag->id -= 2; break;
            case 7: xflag->id -= 1; break;
            case 8: xflag->id -= 1; break;
            }
        }
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
    EnItem00_DropCustom(play, &this->base.world.pos, &this->xflag);
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
    csmcGrassPreDraw(play, o.gi, CSMC_GRASS_NORMAL, alt, 0);

    /* Draw the actor */
    EnKusa_Draw = actorAddr(AC_EN_KUSA, 0x80a80a50);
    EnKusa_Draw(&this->base, play);
}
