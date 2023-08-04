#include <combo.h>
#include <combo/custom.h>

static ActorFunc sDraw;

static void ObjTsubo_Aliases(Actor_ObjTsubo* this, GameState_Play* play)
{
    switch (play->sceneId)
    {
    case SCE_OOT_KAKARIKO_VILLAGE:
    case SCE_OOT_LON_LON_RANCH:
        if (this->extendedSetupId == 1)
        {
            this->extendedSetupId = 0;
            this->extendedId += 1;
        }
        break;
    case SCE_OOT_ZORA_DOMAIN:
        if (this->extendedSetupId == 2)
        {
            this->extendedSetupId = 0;
            switch (this->extendedId)
            {
            case 1: this->extendedId = 25; break;
            case 2: this->extendedId = 22; break;
            case 3: this->extendedId = 24; break;
            case 4: this->extendedId = 21; break;
            case 5: this->extendedId = 23; break;
            }
        }
        break;
    case SCE_OOT_ZORA_FOUNTAIN:
        if (this->extendedSetupId == 1)
            this->extendedSetupId = 0;
        break;
    case SCE_OOT_DEATH_MOUNTAIN_CRATER:
        if (this->extendedSetupId == 0)
        {
            this->extendedSetupId = 2;
            this->extendedId -= 2;
        }
        break;
    case SCE_OOT_GORON_CITY:
        if (this->extendedSetupId == 2)
        {
            this->extendedSetupId = 0;
            switch (this->extendedRoomId)
            {
            case 1: this->extendedId += 4; break;
            case 3: this->extendedId += 33; break;
            }
        }
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
    this->extendedSetupId = g.sceneSetupId;
    this->extendedRoomId = this->base.room;
    this->extendedId = g.actorIndex;

    /* Fix the aliases */
    ObjTsubo_Aliases(this, play);

    /* Forward init */
    init = actorAddr(AC_OBJ_TSUBO, 0x80a653a8);
    init(&this->base, play);
}

void ObjTsubo_SpawnShuffledDrop(Actor_ObjTsubo* this, GameState_Play* play)
{
    u16 var;
    Actor_EnItem00* item;

    if (comboXflagsGet(play->sceneId, g.sceneSetupId, this->extendedRoomId, this->extendedId))
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

    item->extendedSetupId = this->extendedSetupId;
    item->extendedRoomId = this->extendedRoomId;
    item->extendedId = this->extendedId;
    item->extendedGi = GI_OOT_RUPEE_GREEN;
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
    if (!comboXflagsGet(play->sceneId, g.sceneSetupId, this->extendedRoomId, this->extendedId))
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
