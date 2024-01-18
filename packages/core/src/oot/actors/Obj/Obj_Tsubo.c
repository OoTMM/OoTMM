#include <combo.h>
#include <combo/custom.h>
#include <combo/item.h>
#include <combo/csmc.h>

void ObjTsubo_Alias(Actor_ObjTsubo* this)
{
    Xflag* xflag;

    xflag = &this->xflag;

    switch (xflag->sceneId)
    {
    case SCE_OOT_KAKARIKO_VILLAGE:
    case SCE_OOT_LON_LON_RANCH:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            xflag->id += 1;
        }
        break;
    case SCE_OOT_ZORA_DOMAIN:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            switch (xflag->id)
            {
            case 1: xflag->id = 25; break;
            case 2: xflag->id = 22; break;
            case 3: xflag->id = 24; break;
            case 4: xflag->id = 21; break;
            case 5: xflag->id = 23; break;
            }
        }
        break;
    case SCE_OOT_ZORA_FOUNTAIN:
        if (xflag->setupId == 1)
            xflag->setupId = 0;
        break;
    case SCE_OOT_DEATH_MOUNTAIN_CRATER:
        if (xflag->setupId == 0)
        {
            xflag->setupId = 2;
            xflag->id -= 2;
        }
        break;
    case SCE_OOT_GORON_CITY:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            switch (xflag->roomId)
            {
            case 1: xflag->id += 4; break;
            case 3: xflag->id += 33; break;
            }
        }
        break;
    case SCE_OOT_LAIR_GANONDORF:
        xflag->sceneId = SCE_OOT_GANON_TOWER;
        xflag->roomId = 8;
        xflag->id -= 46;
        break;
    case SCE_OOT_GANON_TOWER_COLLAPSING:
        xflag->sceneId = SCE_OOT_GANON_TOWER;
        xflag->roomId = 8;
        xflag->id -= 15;
        break;
    default:
        break;
    }
}

void ObjTsubo_SpawnShuffledDrop(Actor_ObjTsubo* this, GameState_Play* play)
{
    u16 var;

    if (ObjTsubo_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->base.position, &this->xflag);
        return;
    }

    /* Already spawned */
    var = this->base.variable;
    if ((var & 0xff) < 0x1a)
    {
        Item_DropCollectible(play, &this->base.position, (var & 0xff) | (((var >> 9) & 0x3f) << 8));
    }
}

void ObjTsubo_DrawWrapper(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc draw;
    ComboItemOverride o;
    int type;

    if (ObjTsubo_IsShuffled(this))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    if (this->base.variable & (1 << 8))
        type = CSMC_POT_NORMAL;
    else
        type = CSMC_POT_NORMAL_DANGEON;
    csmcPotPreDraw(&this->base, play, o.gi, type);

    /* Draw */
    draw = actorAddr(AC_OBJ_TSUBO, 0x80a65fbc);
    draw(&this->base, play);
}
