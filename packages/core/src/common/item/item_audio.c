#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>

#define FANFARE_FOREIGN(x) ((x) | MASK_FOREIGN_BGM)

#if defined(GAME_OOT)
# define FANFARE_OOT(x) (x)
# define FANFARE_MM(x) FANFARE_FOREIGN(x)
#endif

#if defined(GAME_MM)
# define FANFARE_OOT(x) FANFARE_FOREIGN(x)
# define FANFARE_MM(x) (x)
#endif

#define SFX_RUPEE                   NA_SE_SY_GET_RUPY
#define SFX_MINOR_GI                NA_SE_SY_GET_ITEM
#define SFX_MINOR_QUICK             NA_SE_SY_GET_BOXITEM

#define FANFARE_MAJOR               NA_BGM_ITEM_GET
#define FANFARE_HEART_CONTAINER     NA_BGM_HEART_GET
#define FANFARE_HEART_PIECE         NA_BGM_SMALL_ITEM_GET
#define FANFARE_SONG                NA_BGM_OOT_OCA_FAIRY_GET
#define FANFARE_SONG_TIME           NA_BGM_OCA_TIME
#define FANFARE_SONG_EPONA          NA_BGM_OCA_EPONA
#define FANFARE_SONG_SUN            NA_BGM_OCA_SUNS
#define FANFARE_SONG_STORMS         NA_BGM_OCA_STORM
#define FANFARE_MASK                FANFARE_MM(NA_BGM_MM_GET_NEW_MASK)
#define FANFARE_MEDALLION           FANFARE_OOT(NA_BGM_OOT_MEDALLION_GET)
#define FANFARE_STONE               FANFARE_OOT(NA_BGM_OOT_SPIRITUAL_STONE)
#define FANFARE_SONG_ZELDA          FANFARE_OOT(NA_BGM_OOT_OCA_ZELDA)
#define FANFARE_SONG_SARIA          FANFARE_OOT(NA_BGM_OOT_OCA_SARIA)
#define FANFARE_SONG_TP_LIGHT       FANFARE_OOT(NA_BGM_OOT_OCA_LIGHT)
#define FANFARE_SONG_TP_FOREST      FANFARE_OOT(NA_BGM_OOT_OCA_MINUET)
#define FANFARE_SONG_TP_FIRE        FANFARE_OOT(NA_BGM_OOT_OCA_BOLERO)
#define FANFARE_SONG_TP_WATER       FANFARE_OOT(NA_BGM_OOT_OCA_SERENADE)
#define FANFARE_SONG_TP_SHADOW      FANFARE_OOT(NA_BGM_OOT_OCA_NOCTURNE)
#define FANFARE_SONG_TP_SPIRIT      FANFARE_OOT(NA_BGM_OOT_OCA_REQUIEM)
#define FANFARE_SONG_SOARING        FANFARE_MM(NA_BGM_MM_OCARINA_SOARING)
#define FANFARE_SONG_HEALING        FANFARE_MM(NA_BGM_MM_OCARINA_HEALING)
#define FANFARE_SONG_AWAKENING      FANFARE_MM(NA_BGM_MM_OCARINA_SONATA)
#define FANFARE_SONG_GORON_HALF     FANFARE_MM(NA_BGM_MM_OCARINA_LULLABY_INTRO)
#define FANFARE_SONG_GORON          FANFARE_MM(NA_BGM_MM_OCARINA_LULLABY)
#define FANFARE_SONG_ZORA           FANFARE_MM(NA_BGM_MM_OCARINA_NEW_WAVE)
#define FANFARE_SONG_EMPTINESS      FANFARE_MM(NA_BGM_MM_OCARINA_ELEGY)
#define FANFARE_SONG_ORDER          FANFARE_MM(NA_BGM_MM_OCARINA_OATH)

void comboPlayItemFanfare(s16 gi, int isShort)
{
    int fanfare;
    int sfx;
    int tmp;

    if (gi < 0)
        gi = -gi;

    fanfare = -1;
    sfx = -1;

    /* We need to resolve stray fairies */
    if (gi == GI_MM_STRAY_FAIRY)
    {
        tmp = comboStrayFairyIndex();
        switch (tmp)
        {
        case 0: gi = GI_MM_STRAY_FAIRY_WF; break;
        case 1: gi = GI_MM_STRAY_FAIRY_SH; break;
        case 2: gi = GI_MM_STRAY_FAIRY_GB; break;
        case 3: gi = GI_MM_STRAY_FAIRY_ST; break;
        case 4: gi = GI_MM_STRAY_FAIRY_TOWN; break;
        }
    }

    /* Special cases */
    switch (gi)
    {
    case GI_OOT_OCARINA_FAIRY:
    case GI_MM_OCARINA_FAIRY:
    case GI_OOT_OCARINA_TIME:
    case GI_MM_OCARINA_OF_TIME:
        fanfare = FANFARE_SONG;
        break;
    case GI_OOT_STONE_EMERALD:
    case GI_OOT_STONE_RUBY:
    case GI_OOT_STONE_SAPPHIRE:
        fanfare = FANFARE_STONE;
        break;
    case GI_OOT_MEDALLION_FOREST:
    case GI_OOT_MEDALLION_FIRE:
    case GI_OOT_MEDALLION_WATER:
    case GI_OOT_MEDALLION_SPIRIT:
    case GI_OOT_MEDALLION_SHADOW:
    case GI_OOT_MEDALLION_LIGHT:
        fanfare = FANFARE_MEDALLION;
        break;
    case GI_OOT_SONG_TP_LIGHT:
        fanfare = FANFARE_SONG_TP_LIGHT;
        break;
    case GI_OOT_SONG_TP_FOREST:
        fanfare = FANFARE_SONG_TP_FOREST;
        break;
    case GI_OOT_SONG_TP_FIRE:
        fanfare = FANFARE_SONG_TP_FIRE;
        break;
    case GI_OOT_SONG_TP_WATER:
        fanfare = FANFARE_SONG_TP_WATER;
        break;
    case GI_OOT_SONG_TP_SHADOW:
        fanfare = FANFARE_SONG_TP_SHADOW;
        break;
    case GI_OOT_SONG_TP_SPIRIT:
        fanfare = FANFARE_SONG_TP_SPIRIT;
        break;
    case GI_OOT_SONG_ZELDA:
        fanfare = FANFARE_SONG_ZELDA;
        break;
    case GI_OOT_SONG_SARIA:
        fanfare = FANFARE_SONG_SARIA;
        break;
    case GI_OOT_SONG_TIME:
    case GI_MM_SONG_TIME:
        fanfare = FANFARE_SONG_TIME;
        break;
    case GI_OOT_SONG_EPONA:
    case GI_MM_SONG_EPONA:
        fanfare = FANFARE_SONG_EPONA;
        break;
    case GI_OOT_SONG_SUN:
    case GI_MM_SONG_SUN:
        fanfare = FANFARE_SONG_SUN;
        break;
    case GI_OOT_SONG_STORMS:
    case GI_MM_SONG_STORMS:
        fanfare = FANFARE_SONG_STORMS;
        break;
    case GI_MM_SONG_SOARING:
        fanfare = FANFARE_SONG_SOARING;
        break;
    case GI_MM_SONG_HEALING:
        fanfare = FANFARE_SONG_HEALING;
        break;
    case GI_MM_SONG_AWAKENING:
        fanfare = FANFARE_SONG_AWAKENING;
        break;
    case GI_MM_SONG_GORON_HALF:
        fanfare = FANFARE_SONG_GORON_HALF;
        break;
    case GI_MM_SONG_GORON:
        fanfare = FANFARE_SONG_GORON;
        break;
    case GI_MM_SONG_ZORA:
        fanfare = FANFARE_SONG_ZORA;
        break;
    case GI_OOT_SONG_EMPTINESS:
    case GI_MM_SONG_EMPTINESS:
        fanfare = FANFARE_SONG_EMPTINESS;
        break;
    case GI_MM_SONG_ORDER:
        fanfare = FANFARE_SONG_ORDER;
        break;
    case GI_OOT_HEART_PIECE:
    case GI_OOT_TC_HEART_PIECE:
        if (gOotSave.info.inventory.quest.heartPieces)
            fanfare = FANFARE_HEART_PIECE;
        break;
    case GI_MM_HEART_PIECE:
        if (gMmSave.info.inventory.quest.heartPieces)
            fanfare = FANFARE_HEART_PIECE;
        break;
    case GI_MM_GS_TOKEN_SWAMP:
        if (gMmSave.info.skullCountSwamp >= 30)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_GS_TOKEN_OCEAN:
        if (gMmSave.info.skullCountOcean >= 30)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_OOT_GS_TOKEN:
        tmp = gOotSave.info.inventory.goldTokens;
        if (tmp == 10 || tmp == 20 || tmp == 30 || tmp == 40 || tmp == 50 || tmp >= 100)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_STRAY_FAIRY_WF:
        if (gMmSave.info.inventory.strayFairies[0] == gComboConfig.strayFairyRewardCount)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_STRAY_FAIRY_SH:
        if (gMmSave.info.inventory.strayFairies[1] == gComboConfig.strayFairyRewardCount)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_STRAY_FAIRY_GB:
        if (gMmSave.info.inventory.strayFairies[2] == gComboConfig.strayFairyRewardCount)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_STRAY_FAIRY_ST:
        if (gMmSave.info.inventory.strayFairies[3] == gComboConfig.strayFairyRewardCount)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_STRAY_FAIRY_TOWN:
        fanfare = FANFARE_MAJOR;
        break;
    }

    if (fanfare == -1 && sfx == -1)
    {
        switch (comboItemType(gi))
        {
        case ITT_MASK:
            fanfare = FANFARE_MASK;
            break;
        case ITT_MINOR:
            sfx = isShort ? SFX_MINOR_QUICK : SFX_MINOR_GI;
            break;
        case ITT_RUPEE:
            sfx = isShort ? SFX_RUPEE : SFX_MINOR_GI;
            break;
        case ITT_HEART:
            fanfare = FANFARE_HEART_CONTAINER;
            break;
        case ITT_SKULL:
        case ITT_FAIRY:
            fanfare = FANFARE_HEART_PIECE;
            break;
        default:
            fanfare = FANFARE_MAJOR;
            break;
        }
    }

    if (fanfare != -1)
        Audio_PlayFanfare(fanfare);
    else if (sfx != -1)
        PlaySound(sfx);
}
