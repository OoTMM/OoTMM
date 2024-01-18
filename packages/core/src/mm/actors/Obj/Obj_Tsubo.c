#include <combo.h>
#include <combo/custom.h>
#include <combo/csmc.h>
#include <combo/item.h>

void ObjTsubo_Alias(Actor_ObjTsubo* this)
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
            switch (xflag->id)
            {
            case 13: xflag->id = 123; break;
            case 14: xflag->id = 124; break;
            case 15: xflag->id = 125; break;
            case 16: xflag->id = 133; break;
            case 17: xflag->id = 119; break;
            case 18: xflag->id = 129; break;
            case 19: xflag->id = 118; break;
            case 20: xflag->id = 128; break;
            case 21: xflag->id = 130; break;
            case 22: xflag->id = 120; break;
            case 23: xflag->id = 121; break;
            case 24: xflag->id = 62;  break;
            case 25: xflag->id = 61;  break;
            case 26: xflag->id = 63;  break;
            case 27: xflag->id = 131;  break;
            default: UNREACHABLE(); break;
            }
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

void ObjTsubo_SpawnShuffledDrop(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc ObjTsubo_SpawnDrop;

    if (ObjTsubo_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->base.position, &this->xflag);
        return;
    }

    /* Already spawned */
    ObjTsubo_SpawnDrop = (ActorFunc)actorAddr(AC_OBJ_TSUBO, 0x80927690);
    ObjTsubo_SpawnDrop(&this->base, play);
}

void ObjTsubo_SpawnShuffledDropFlexible(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc ObjTsubo_SpawnDropFlexible;

    if (ObjTsubo_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->base.position, &this->xflag);
        return;
    }

    ObjTsubo_SpawnDropFlexible = (ActorFunc)actorAddr(AC_OBJ_TSUBO, 0x8092762c);
    ObjTsubo_SpawnDropFlexible(&this->base, play);
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

    if (ObjTsubo_IsShuffled(this))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    csmcPotPreDraw(&this->base, play, o.gi, type);

    /* Draw */
    DrawSimpleOpa(play, dlist);
}

PATCH_FUNC(0x809294b0, ObjTsubo_Draw);
