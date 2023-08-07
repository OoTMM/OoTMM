#include <combo.h>
#include <combo/csmc.h>
#include <combo/custom.h>
#include <combo/dungeon.h>

#if defined(GAME_OOT)
# define CHEST_TEX_NORMAL_FRONT     0x06001798
# define CHEST_TEX_NORMAL_SIDE      0x06002798
# define CHEST_TEX_BOSS_KEY_FRONT   0x06003798
# define CHEST_TEX_BOSS_KEY_SIDE    0x06002f98
#else
# define CHEST_TEX_NORMAL_FRONT     0x06001e60
# define CHEST_TEX_NORMAL_SIDE      0x06002e60
# define CHEST_TEX_BOSS_KEY_FRONT   0x06004e60
# define CHEST_TEX_BOSS_KEY_SIDE    0x06005e60
#endif

static const Gfx kListNormalFront[] = {
    gsDPLoadTextureBlock(CHEST_TEX_NORMAL_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListNormalSide[] = {
    gsDPLoadTextureBlock(CHEST_TEX_NORMAL_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListBossKeyFront[] = {
    gsDPLoadTextureBlock(CHEST_TEX_BOSS_KEY_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListBossKeySide[] = {
    gsDPLoadTextureBlock(CHEST_TEX_BOSS_KEY_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListMajorFront[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_MAJOR_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListMajorSide[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_MAJOR_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListKeyFront[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_KEY_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListKeySide[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_KEY_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListSpiderFront[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_SPIDER_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListSpiderSide[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_SPIDER_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListFairyFront[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_FAIRY_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListFairySide[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_FAIRY_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListHeartFront[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_HEART_FRONT, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx kListHeartSide[] = {
    gsDPLoadTextureBlock(0x09000000 | CUSTOM_KEEP_CHEST_HEART_SIDE, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static int csmcEnabledActor(Actor* this, GameState_Play* play)
{
    if (!csmcEnabled())
        return 0;

#if defined(GAME_OOT)
    if (play->sceneId == SCE_OOT_TREASURE_SHOP && (this->variable & 0x1f) != 0x0a)
        return 0;
#endif

#if defined(GAME_MM)
    if (play->sceneId == SCE_MM_MOON_GORON)
        return 0;
#endif

    return 1;
}

void csmcChestInit(Actor* this, GameState_Play* play, s16 gi)
{
    int type;

    if (!csmcEnabledActor(this, play))
        return;

    type = csmcFromItem(gi);
    if (type == CSMC_MAJOR || type == CSMC_BOSS_KEY)
    {
        ActorSetScale(this, 0.01f);
        ActorSetUnk(this, 40.f);


#if defined(GAME_OOT)
        /* Fix for IGT chest */
        if (play->sceneId == SCE_OOT_INSIDE_GANON_CASTLE)
        {
            if ((this->variable & 0x1f) == (gComboData.mq & (1 << MQ_GANON_CASTLE) ? 0x04 : 0x11))
                this->position.z -= 10.f;
        }
#endif
    }
    else
    {
#if defined(GAME_OOT)
        ActorSetScale(this, 0.005f);
        /* Fix for spirit temple chest */
        if (play->sceneId == SCE_OOT_TEMPLE_SPIRIT && (this->variable & 0x1f) == 0x04)
        {
            this->position.x += 40.f;
            this->position.z += 40.f;
        }
#else
        ActorSetScale(this, 0.0075f);
#endif
        ActorSetUnk(this, 20.f);
    }

}

void csmcChestPreDraw(Actor* this, GameState_Play* play, s16 gi)
{
    int type;
    const void* listFront;
    const void* listSide;

    if (csmcEnabledActor(this, play))
        type = csmcFromItem(gi);
    else
        type = CSMC_NORMAL;
    switch (type)
    {
    case CSMC_NORMAL:
        listFront = kListNormalFront;
        listSide = kListNormalSide;
        break;
    case CSMC_BOSS_KEY:
    case CSMC_SOUL:
        listFront = kListBossKeyFront;
        listSide = kListBossKeySide;
        break;
    case CSMC_MAJOR:
        listFront = kListMajorFront;
        listSide = kListMajorSide;
        break;
    case CSMC_KEY:
        listFront = kListKeyFront;
        listSide = kListKeySide;
        break;
    case CSMC_SPIDER:
        listFront = kListSpiderFront;
        listSide = kListSpiderSide;
        break;
    case CSMC_FAIRY:
        listFront = kListFairyFront;
        listSide = kListFairySide;
        break;
    case CSMC_HEART:
        listFront = kListHeartFront;
        listSide = kListHeartSide;
        break;
    }

    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x09, gCustomKeep);
    gSPSegment(POLY_OPA_DISP++, 0x0a, listFront);
    gSPSegment(POLY_OPA_DISP++, 0x0b, listSide);
    gSPSegment(POLY_XLU_DISP++, 0x09, gCustomKeep);
    gSPSegment(POLY_XLU_DISP++, 0x0a, listFront);
    gSPSegment(POLY_XLU_DISP++, 0x0b, listSide);
    CLOSE_DISPS();
}

int csmcChestLarge(s16 gi)
{
    if (!csmcEnabled())
        return -1;
    switch (csmcFromItem(gi))
    {
    case CSMC_MAJOR:
    case CSMC_BOSS_KEY:
        return 1;
    default:
        return 0;
    }
}
