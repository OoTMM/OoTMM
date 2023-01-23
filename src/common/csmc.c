#include <combo.h>
#include <combo/custom.h>

#define CSMC_NORMAL     0x00
#define CSMC_BOSS_KEY   0x01
#define CSMC_MAJOR      0x02
#define CSMC_KEY        0x03
#define CSMC_SPIDER     0x04
#define CSMC_FAIRY      0x05

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

static int csmcFromItemOot(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_STONE_EMERALD:
    case GI_OOT_STONE_RUBY:
    case GI_OOT_STONE_SAPPHIRE:
    case GI_OOT_MEDALLION_FOREST:
    case GI_OOT_MEDALLION_FIRE:
    case GI_OOT_MEDALLION_WATER:
    case GI_OOT_MEDALLION_SPIRIT:
    case GI_OOT_MEDALLION_SHADOW:
    case GI_OOT_MEDALLION_LIGHT:
    case GI_OOT_ARROW_FIRE:
    case GI_OOT_ARROW_ICE:
    case GI_OOT_ARROW_LIGHT:
    case GI_OOT_BOMB_BAG:
    case GI_OOT_BOMB_BAG2:
    case GI_OOT_BOMB_BAG3:
    case GI_OOT_BOOMERANG:
    case GI_OOT_BOOTS_HOVER:
    case GI_OOT_BOOTS_IRON:
    case GI_OOT_BOW:
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
    case GI_OOT_CHICKEN:
    case GI_OOT_EMPTY_BOTTLE:
    case GI_OOT_GERUDO_CARD:
    case GI_OOT_HAMMER:
    case GI_OOT_HOOKSHOT:
    case GI_OOT_LONGSHOT:
    case GI_OOT_LENS:
    case GI_OOT_MAGIC_UPGRADE:
    case GI_OOT_MAGIC_UPGRADE2:
    case GI_OOT_OCARINA_FAIRY:
    case GI_OOT_OCARINA_TIME:
    case GI_OOT_RUTO_LETTER:
    case GI_OOT_SCALE_SILVER:
    case GI_OOT_SCALE_GOLDEN:
    case GI_OOT_SHIELD_MIRROR:
    case GI_OOT_PROGRESSIVE_SHIELD_DEKU:
    case GI_OOT_PROGRESSIVE_SHIELD_HYLIAN:
    case GI_OOT_SLINGSHOT:
    case GI_OOT_BULLET_BAG2:
    case GI_OOT_BULLET_BAG3:
    case GI_OOT_SONG_EPONA:
    case GI_OOT_SONG_SARIA:
    case GI_OOT_SONG_STORMS:
    case GI_OOT_SONG_SUN:
    case GI_OOT_SONG_TIME:
    case GI_OOT_SONG_ZELDA:
    case GI_OOT_SONG_TP_FOREST:
    case GI_OOT_SONG_TP_FIRE:
    case GI_OOT_SONG_TP_WATER:
    case GI_OOT_SONG_TP_SHADOW:
    case GI_OOT_SONG_TP_SPIRIT:
    case GI_OOT_SONG_TP_LIGHT:
    case GI_OOT_SPELL_FIRE:
    case GI_OOT_SPELL_WIND:
    case GI_OOT_SPELL_LOVE:
    case GI_OOT_GORON_BRACELET:
    case GI_OOT_SILVER_GAUNTLETS:
    case GI_OOT_GOLDEN_GAUNTLETS:
    case GI_OOT_SWORD_KOKIRI:
    case GI_OOT_SWORD_MASTER:
    case GI_OOT_SWORD_KNIFE:
    case GI_OOT_SWORD_BIGGORON:
    case GI_OOT_TUNIC_GORON:
    case GI_OOT_TUNIC_ZORA:
    case GI_OOT_ZELDA_LETTER:
    case GI_OOT_MAGIC_BEAN:
    case GI_OOT_MILK_BOTTLE:
    case GI_OOT_STONE_OF_AGONY:
    case GI_OOT_WALLET2:
    case GI_OOT_WALLET3:
    case GI_OOT_POCKET_CUCCO:
    case GI_OOT_COJIRO:
    case GI_OOT_ODD_MUSHROOM:
    case GI_OOT_ODD_POTION:
    case GI_OOT_POACHER_SAW:
    case GI_OOT_BROKEN_GORON_SWORD:
    case GI_OOT_PRESCRIPTION:
    case GI_OOT_EYEBALL_FROG:
    case GI_OOT_EYE_DROPS:
    case GI_OOT_CLAIM_CHECK:
    case GI_OOT_MASK_TRUTH:
    case GI_OOT_MASK_SKULL:
    case GI_OOT_MASK_SPOOKY:
    case GI_OOT_MASK_KEATON:
    case GI_OOT_MASK_BUNNY:
    case GI_OOT_MASK_GORON:
    case GI_OOT_MASK_ZORA:
    case GI_OOT_MASK_GERUDO:
    case GI_OOT_STICK_UPGRADE:
    case GI_OOT_STICK_UPGRADE2:
    case GI_OOT_NUT_UPGRADE:
    case GI_OOT_NUT_UPGRADE2:
    case GI_OOT_DEFENSE_UPGRADE:
        return CSMC_MAJOR;
    case GI_OOT_SMALL_KEY:
    case GI_OOT_SMALL_KEY_FOREST:
    case GI_OOT_SMALL_KEY_FIRE:
    case GI_OOT_SMALL_KEY_WATER:
    case GI_OOT_SMALL_KEY_SPIRIT:
    case GI_OOT_SMALL_KEY_SHADOW:
    case GI_OOT_SMALL_KEY_GANON:
    case GI_OOT_SMALL_KEY_BOTW:
    case GI_OOT_SMALL_KEY_GF:
    case GI_OOT_SMALL_KEY_GTG:
        return CSMC_KEY;
    case GI_OOT_BOSS_KEY:
    case GI_OOT_BOSS_KEY_FOREST:
    case GI_OOT_BOSS_KEY_FIRE:
    case GI_OOT_BOSS_KEY_WATER:
    case GI_OOT_BOSS_KEY_SPIRIT:
    case GI_OOT_BOSS_KEY_SHADOW:
    case GI_OOT_BOSS_KEY_GANON:
        return CSMC_BOSS_KEY;
    case GI_OOT_GS_TOKEN:
        return CSMC_SPIDER;
    }

    return CSMC_NORMAL;
}

static int csmcFromItemMm(s16 gi)
{
    switch (gi)
    {
    case GI_MM_REMAINS_ODOLWA:
    case GI_MM_REMAINS_GOHT:
    case GI_MM_REMAINS_GYORG:
    case GI_MM_REMAINS_TWINMOLD:
    case GI_MM_MASK_DEKU:
    case GI_MM_MASK_GORON:
    case GI_MM_MASK_ZORA:
    case GI_MM_MASK_CAPTAIN:
    case GI_MM_MASK_GIANT:
    case GI_MM_MASK_ALL_NIGHT:
    case GI_MM_MASK_BUNNY:
    case GI_MM_MASK_KEATON:
    case GI_MM_MASK_GARO:
    case GI_MM_MASK_ROMANI:
    case GI_MM_MASK_TROUPE_LEADER:
    case GI_MM_MASK_POSTMAN:
    case GI_MM_MASK_COUPLE:
    case GI_MM_MASK_GREAT_FAIRY:
    case GI_MM_MASK_GIBDO:
    case GI_MM_MASK_DON_GERO:
    case GI_MM_MASK_KAMARO:
    case GI_MM_MASK_TRUTH:
    case GI_MM_MASK_STONE:
    case GI_MM_MASK_BREMEN:
    case GI_MM_MASK_BLAST:
    case GI_MM_MASK_SCENTS:
    case GI_MM_MASK_KAFEI:
    case GI_MM_MASK_FIERCE_DEITY:
    case GI_MM_BOTTLED_POTION_RED:
    case GI_MM_BOTTLED_GOLD_DUST:
    case GI_MM_EMPTY_BOTTLE:
    case GI_MM_BOTTLED_MILK:
    case GI_MM_MAGIC_UPGRADE:
    case GI_MM_MAGIC_UPGRADE2:
    case GI_MM_SONG_TIME:
    case GI_MM_SONG_AWAKENING:
    case GI_MM_SONG_HEALING:
    case GI_MM_SONG_EPONA:
    case GI_MM_SONG_SOARING:
    case GI_MM_SONG_GORON_HALF:
    case GI_MM_SONG_GORON:
    case GI_MM_SONG_ZORA:
    case GI_MM_SONG_STORMS:
    case GI_MM_SONG_EMPTINESS:
    case GI_MM_SONG_ORDER:
    case GI_MM_BOW:
    case GI_MM_QUIVER2:
    case GI_MM_QUIVER3:
    case GI_MM_OCARINA_OF_TIME:
    case GI_MM_SWORD_KOKIRI:
    case GI_MM_SWORD_RAZOR:
    case GI_MM_SWORD_GILDED:
    case GI_MM_SHIELD_MIRROR:
    case GI_MM_PROGRESSIVE_SHIELD_HERO:
    case GI_MM_BOMB_BAG:
    case GI_MM_BOMB_BAG2:
    case GI_MM_BOMB_BAG3:
    case GI_MM_LENS:
    case GI_MM_ARROW_FIRE:
    case GI_MM_ARROW_ICE:
    case GI_MM_ARROW_LIGHT:
    case GI_MM_POWDER_KEG:
    case GI_MM_HOOKSHOT:
    case GI_MM_PICTOGRAPH_BOX:
    case GI_MM_MAGIC_BEAN:
    case GI_MM_MOON_TEAR:
    case GI_MM_DEED_LAND:
    case GI_MM_DEED_SWAMP:
    case GI_MM_DEED_MOUNTAIN:
    case GI_MM_DEED_OCEAN:
    case GI_MM_ROOM_KEY:
    case GI_MM_LETTER_TO_KAFEI:
    case GI_MM_PENDANT_OF_MEMORIES:
    case GI_MM_LETTER_TO_MAMA:
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
    case GI_MM_GREAT_FAIRY_SWORD:
    case GI_MM_SPIN_UPGRADE:
    case GI_MM_BOMBER_NOTEBOOK:
    case GI_MM_DEFENSE_UPGRADE:
    case GI_MM_BOTTLED_CHATEAU_ROMANI:
        return CSMC_MAJOR;
    case GI_MM_SMALL_KEY:
    case GI_MM_SMALL_KEY_WF:
    case GI_MM_SMALL_KEY_SH:
    case GI_MM_SMALL_KEY_GB:
    case GI_MM_SMALL_KEY_ST:
        return CSMC_KEY;
    case GI_MM_BOSS_KEY:
    case GI_MM_BOSS_KEY_WF:
    case GI_MM_BOSS_KEY_SH:
    case GI_MM_BOSS_KEY_GB:
    case GI_MM_BOSS_KEY_ST:
        return CSMC_BOSS_KEY;
    case GI_MM_GS_TOKEN:
    case GI_MM_GS_TOKEN_OCEAN:
    case GI_MM_GS_TOKEN_SWAMP:
        return CSMC_SPIDER;
    case GI_MM_STRAY_FAIRY:
    case GI_MM_STRAY_FAIRY_WF:
    case GI_MM_STRAY_FAIRY_SH:
    case GI_MM_STRAY_FAIRY_GB:
    case GI_MM_STRAY_FAIRY_ST:
    case GI_MM_STRAY_FAIRY_TOWN:
        return CSMC_FAIRY;
    }
    return CSMC_NORMAL;
}

static int csmcFromItem(s16 gi)
{
    if (gi < 0)
        gi = -gi;
#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif
    if (gi & MASK_FOREIGN_GI)
        return csmcFromItemMm(gi & ~MASK_FOREIGN_GI);
    else
        return csmcFromItemOot(gi);
}

static int csmcEnabled(Actor* this, GameState_Play* play)
{
    if (!comboConfig(CFG_CSMC))
        return 0;

#if defined(GAME_OOT)
    if (play->sceneId == SCE_OOT_TREASURE_SHOP && (this->variable & 0x1f) != 0x0a)
        return 0;
#endif

    return 1;
}

void comboCsmcInit(Actor* this, GameState_Play* play, s16 gi)
{
    int type;

    if (!csmcEnabled(this, play))
        return;

    type = csmcFromItem(gi);
    if (type == CSMC_MAJOR || type == CSMC_BOSS_KEY)
    {
        ActorSetScale(this, 0.01f);
        ActorSetUnk(this, 40.f);

#if defined(GAME_OOT)
        /* Fix for IGT chest */
        if (play->sceneId == SCE_OOT_INSIDE_GANON_CASTLE && (this->variable & 0x1f) == 0x11)
        {
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

void comboCsmcPreDraw(Actor* this, GameState_Play* play, s16 gi)
{
    int type;
    const void* listFront;
    const void* listSide;

    if (csmcEnabled(this, play))
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

int comboCsmcChestSize(s16 gi)
{
    if (!comboConfig(CFG_CSMC))
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
