#include <combo.h>
#include <combo/item.h>

#define FANFARE_MAJOR               0x922
#define FANFARE_HEART_CONTAINER     0x924
#define FANFARE_HEART_PIECE         0x39
#define SFX_RUPEE                   0x4803
#define SFX_MINOR_GI                0x4824
#define SFX_MINOR_QUICK             0x4831

#if defined(GAME_OOT)
# define FANFARE_MASK               FANFARE_MAJOR
# define FANFARE_STONE              0x932
# define FANFARE_MEDALLION          0x943
# define FANFARE_SONG               0x93d
# define FANFARE_SONG_TP_LIGHT      0x925
# define FANFARE_SONG_TP_FOREST     0x934
# define FANFARE_SONG_TP_FIRE       0x933
# define FANFARE_SONG_TP_WATER      0x935
# define FANFARE_SONG_TP_SHADOW     0x937
# define FANFARE_SONG_TP_SPIRIT     0x936
# define FANFARE_SONG_ZELDA         0x946
# define FANFARE_SONG_SARIA         0x944
# define FANFARE_SONG_TIME          0x948
# define FANFARE_SONG_EPONA         0x945
# define FANFARE_SONG_SUN           0x947
# define FANFARE_SONG_STORMS        0x949
# define FANFARE_SONG_SOARING       FANFARE_SONG
# define FANFARE_SONG_HEALING       FANFARE_SONG
# define FANFARE_SONG_AWAKENING     FANFARE_SONG
# define FANFARE_SONG_GORON_HALF    FANFARE_SONG
# define FANFARE_SONG_GORON         FANFARE_SONG
# define FANFARE_SONG_ZORA          FANFARE_SONG
# define FANFARE_SONG_EMPTINESS     FANFARE_SONG
# define FANFARE_SONG_ORDER         FANFARE_SONG
#endif

#if defined(GAME_MM)
# define FANFARE_MASK               0x37
# define FANFARE_STONE              FANFARE_MAJOR
# define FANFARE_MEDALLION          FANFARE_MAJOR
# define FANFARE_SONG               0x952
# define FANFARE_SONG_TP_LIGHT      FANFARE_SONG
# define FANFARE_SONG_TP_FOREST     FANFARE_SONG
# define FANFARE_SONG_TP_FIRE       FANFARE_SONG
# define FANFARE_SONG_TP_WATER      FANFARE_SONG
# define FANFARE_SONG_TP_SHADOW     FANFARE_SONG
# define FANFARE_SONG_TP_SPIRIT     FANFARE_SONG
# define FANFARE_SONG_ZELDA         FANFARE_SONG
# define FANFARE_SONG_SARIA         FANFARE_SONG
# define FANFARE_SONG_TIME          0x934
# define FANFARE_SONG_EPONA         0x932
# define FANFARE_SONG_SUN           0x933
# define FANFARE_SONG_STORMS        0x935
# define FANFARE_SONG_SOARING       0x947
# define FANFARE_SONG_HEALING       0x948
# define FANFARE_SONG_AWAKENING     0x95b
# define FANFARE_SONG_GORON_HALF    0x951
# define FANFARE_SONG_GORON         0x95c
# define FANFARE_SONG_ZORA          0x95d
# define FANFARE_SONG_EMPTINESS     0x95e
# define FANFARE_SONG_ORDER         0x95f
#endif

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
        if (gOotSave.inventory.quest.heartPieces)
            fanfare = FANFARE_HEART_PIECE;
        break;
    case GI_MM_HEART_PIECE:
        if (gMmSave.inventory.quest.heartPieces)
            fanfare = FANFARE_HEART_PIECE;
        break;
    case GI_MM_GS_TOKEN_SWAMP:
        if (gMmSave.skullCountSwamp >= 30)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_GS_TOKEN_OCEAN:
        if (gMmSave.skullCountOcean >= 30)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_OOT_GS_TOKEN:
        tmp = gOotSave.inventory.goldTokens;
        if (tmp == 10 || tmp == 20 || tmp == 30 || tmp == 40 || tmp == 50 || tmp >= 100)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_STRAY_FAIRY_WF:
        if (gMmSave.inventory.strayFairies[0] >= 15)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_STRAY_FAIRY_SH:
        if (gMmSave.inventory.strayFairies[1] >= 15)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_STRAY_FAIRY_GB:
        if (gMmSave.inventory.strayFairies[2] >= 15)
            fanfare = FANFARE_MAJOR;
        break;
    case GI_MM_STRAY_FAIRY_ST:
        if (gMmSave.inventory.strayFairies[3] >= 15)
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
