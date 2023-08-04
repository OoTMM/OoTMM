#include <combo.h>
#include <combo/custom.h>

static ActorFunc sDraw;

static void ObjTsubo_Aliases(Xflag* xflag)
{
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

void ObjTsubo_InitWrapper(Actor_ObjTsubo* this, GameState_Play* play)
{
    ActorFunc init;

    /* Compute funcs */
    sDraw = actorAddr(AC_OBJ_TSUBO, 0x80a65fbc);

    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.id = g.actorIndex;

    /* Fix the aliases */
    ObjTsubo_Aliases(&this->xflag);

    /* Forward init */
    init = actorAddr(AC_OBJ_TSUBO, 0x80a653a8);
    init(&this->base, play);
}

void ObjTsubo_SpawnShuffledDrop(Actor_ObjTsubo* this, GameState_Play* play)
{
    u16 var;
    Actor_EnItem00* item;

    if (comboXflagsGet(&this->xflag))
    {
        /* Already spawned */
        var = this->base.variable;
        if ((var & 0xff) < 0x1a)
        {
            Item_DropCollectible(play, &this->base.position, (var & 0xff) | (((var >> 9) & 0x3f) << 8));
        }
        return;
    }

    /* Spawn an extended item */
    g.spawnExtended = 1;
    item = Item_DropCollectible(play, &this->base.position, 0x0000);
    g.spawnExtended = 0;

    item->xflag.sceneId = this->xflag.sceneId;
    item->xflag.setupId = this->xflag.setupId;
    item->xflag.roomId = this->xflag.roomId;
    item->xflag.id = this->xflag.id;
    item->extendedGi = GI_OOT_RUPEE_GREEN;
    item->extendedGiDraw = 0;
}

static const Gfx kDrawListNormalTop[] = {
    gsDPLoadTextureBlock(0x06001000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 4, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kDrawListNormalSide[] = {
    gsDPLoadTextureBlock(0x06000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kDrawListNormalDangeonTop[] = {
    gsDPLoadTextureBlock(0x050118A0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 4, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kDrawListNormalDangeonSide[] = {
    gsDPLoadTextureBlock(0x050108A0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kDrawListMajorTop[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_POT_MAJOR_TOP, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 4, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kDrawListMajorSide[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_POT_MAJOR_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

void ObjTsubo_DrawWrapper(Actor_ObjTsubo* this, GameState_Play* play)
{
    const void* dlistSide;
    const void* dlistTop;

    /* Checks flag and dangeon_keep */
    if (comboConfig(CFG_OOT_SHUFFLE_POTS) && !comboXflagsGet(&this->xflag))
    {
        dlistSide = kDrawListMajorSide;
        dlistTop = kDrawListMajorTop;
    }
    else if (this->base.variable & 0x0100)
    {
        dlistSide = kDrawListNormalSide;
        dlistTop = kDrawListNormalTop;
    }
    else
    {
        dlistSide = kDrawListNormalDangeonSide;
        dlistTop = kDrawListNormalDangeonTop;
    }

    /* Setup the textures */
    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x09, gCustomKeep);
    gSPSegment(POLY_OPA_DISP++, 0x0a, dlistSide);
    gSPSegment(POLY_OPA_DISP++, 0x0b, dlistTop);
    CLOSE_DISPS();

    /* Draw */
    sDraw(&this->base, play);
}
