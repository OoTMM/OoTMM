#include <combo.h>
#include <combo/custom.h>
#include <combo/csmc.h>
#include <combo/item.h>

static int ObjTsubo_IsShuffled(Actor_ObjTsubo* this)
{
    u8 potType;

    /* Check for stray fairy */
    potType = (this->base.variable >> 7) & 3;
    if ((potType == 0 || potType == 2) && ((this->base.variable & 0x1f) == 0x11))
        return 0;

    /* Check for enemy */
    if ((this->base.initRot.z & 3) > 1)
        return 0;

    /* Check for dynamic pots */
    if (this->xflag.id == 0xff)
        return 0;

    return 1;
}

static void ObjTsubo_Aliases(Actor_ObjTsubo* this)
{
    Xflag* xflag;

    xflag = &this->xflag;

    /* Check for zora game pots */
    if (xflag->sceneId == SCE_MM_ZORA_CAPE && this->base.variable == 0x13f)
    {
        xflag->sceneId = SCE_MM_EXTRA;
        xflag->setupId = 0;
        xflag->roomId = 0;
        switch ((s16)(this->base.initPos.x))
        {
        case 0x04D0: xflag->id = 0; break;
        case 0x0560: xflag->id = 1; break;
        case 0x05BD: xflag->id = 2; break;
        case 0x056F: xflag->id = 3; break;
        case 0x0543: xflag->id = 4; break;
        default: UNREACHABLE(); break;
        }
        return;
    }

    switch (xflag->sceneId)
    {
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        xflag->sceneId = SCE_MM_SOUTHERN_SWAMP;
        xflag->id += 3;
        break;
    case SCE_MM_GORON_SHRINE:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            switch (xflag->roomId)
            {
            case 0: xflag->id -= 2; break;
            case 1: xflag->id += 3; break;
            }
        }
        break;
    case SCE_MM_GREAT_BAY_COAST:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            if (xflag->id <= 15)
                xflag->id += 48;
            else
                xflag->id += 102;
        }
        break;
    case SCE_MM_ZORA_CAPE:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            xflag->id += 9;
        }
        break;
    case SCE_MM_WOODFALL:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            xflag->id -= 36;
        }
        break;
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
        xflag->sceneId = SCE_MM_MOUNTAIN_VILLAGE_WINTER;
        xflag->id = 38;
        break;
    case SCE_MM_GORON_RACETRACK:
        xflag->setupId = 0;
        break;
    }
}

void ObjTsubo_InitWrapper(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc ObjTsubo_Init;

    /* Setup the xflag */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    /* Resolve aliases */
    ObjTsubo_Aliases(this);

    /* Invoke init */
    ObjTsubo_Init = (ActorFunc)actorAddr(AC_OBJ_TSUBO, 0x809278c0);
    ObjTsubo_Init(&this->base, play);
}

void ObjTsubo_SpawnShuffledDrop(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc ObjTsubo_SpawnDrop;

    if (!ObjTsubo_IsShuffled(this) || comboXflagsGet(&this->xflag))
    {
        /* Already spawned */
        ObjTsubo_SpawnDrop = (ActorFunc)actorAddr(AC_OBJ_TSUBO, 0x80927690);
        ObjTsubo_SpawnDrop(&this->base, play);
        return;
    }

    /* Spawn a custom item */
    DropCustomItem(play, &this->base.position, &this->xflag);
}

void ObjTsubo_SpawnShuffledDropFlexible(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc ObjTsubo_SpawnDropFlexible;

    if (!ObjTsubo_IsShuffled(this) || comboXflagsGet(&this->xflag))
    {
        /* Already spawned */
        ObjTsubo_SpawnDropFlexible = (ActorFunc)actorAddr(AC_OBJ_TSUBO, 0x8092762c);
        ObjTsubo_SpawnDropFlexible(&this->base, play);
        return;
    }

    /* Spawn a custom item */
    DropCustomItem(play, &this->base.position, &this->xflag);
}

void ObjTsubo_Draw(Actor_ObjTsubo* this, GameState_Play* play)
{
    ComboItemOverride o;
    int type;
    u32 dlist;

    /* Get the default pot drawing */
    switch ((this->base.variable >> 7) & 3)
    {
    case 0:
    case 3:
        dlist = 0x05017ea0;
        type = CSMC_POT_NORMAL_DANGEON;
        break;
    case 1:
        dlist = 0x06000278;
        type = CSMC_POT_MAGIC;
        break;
    case 2:
        dlist = 0x060017c0;
        type = CSMC_POT_NORMAL;
        break;
    }

    if (comboConfig(CFG_OOT_SHUFFLE_POTS) && ObjTsubo_IsShuffled(this) && !comboXflagsGet(&this->xflag))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    csmcPotPreDraw(&this->base, play, o.gi, type);

    /* Draw */
    DrawSimpleOpa(play, dlist);
}

PATCH_FUNC(0x809294b0, ObjTsubo_Draw);
