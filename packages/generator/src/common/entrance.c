#include <combo.h>
#include <combo/entrance.h>
#include <combo/dma.h>
#include <combo/mask.h>
#include <combo/config.h>
#include <combo/global.h>

#if defined(GAME_OOT)
# define ENTRANCES_ADDR 0x03ff4000
#else
# define ENTRANCES_ADDR 0x03ff5000
#endif

s8  gIsEntranceOverride;
s32 gLastEntrance;
s32 gLastScene;

typedef struct PACKED ALIGNED(4)
{
    u32 key;
    u32 value;
}
Entrance;

static Entrance sEntranceCache[16];
static u16 sEntranceCount;
static u8 sEntranceCacheCursor;

void comboInitEntrances(void)
{
    memset(sEntranceCache, 0xff, sizeof(sEntranceCache));
    sEntranceCacheCursor = 0;
    sEntranceCount = (u16)(comboDmaLoadFile(NULL, COMBO_VROM_ENTRANCES) / sizeof(Entrance));
}

static u32 comboEntranceOverrideRaw(u32 entranceId)
{
    ALIGNED(8) Entrance tmp;
    Entrance* e;
    u16 min;
    u16 max;
    u16 target;

    /* Check the cache */
    for (int i = 0; i < ARRAY_COUNT(sEntranceCache); ++i)
    {
        e = &sEntranceCache[i];
        if (e->key == entranceId)
            return e->value;
    }

    /* Check the main entrance table */
    min = 0;
    max = sEntranceCount;

    while (min < max)
    {
        target = (min + max) / 2;
        comboDmaLoadFilePartial(&tmp, COMBO_VROM_ENTRANCES, target * sizeof(Entrance), sizeof(Entrance));

        if (tmp.key == entranceId)
        {
            /* Write to the cache */
            sEntranceCache[sEntranceCacheCursor].key = tmp.key;
            sEntranceCache[sEntranceCacheCursor].value = tmp.value;
            sEntranceCacheCursor = (sEntranceCacheCursor + 1) % ARRAY_COUNT(sEntranceCache);

            return tmp.value;
        }
        else if (tmp.key < entranceId)
            min = target + 1;
        else
            max = target;
    }

    return ENTR_INVALID;
}

u32 comboEntranceOverride(u32 entranceId)
{
    u32 override;

    entranceId = Entrance_ToAbsolute(entranceId);
    override = comboEntranceOverrideRaw(entranceId);
    if (override == ENTR_INVALID)
        return ENTR_INVALID;
    return Entrance_ToRelative(override);
}


void comboTransitionOverride(PlayState* play, u32 entrance)
{
#if defined(GAME_OOT)
    g.nextEntrance = entrance;
    play->nextEntrance = ENTR_EXTENDED;
#endif

#if defined(GAME_MM)
    if (gSave.playerForm == MM_PLAYER_FORM_FIERCE_DEITY && !Config_Flag(CFG_MM_FD_ANYWHERE))
    {
        switch (play->sceneId)
        {
        case SCE_MM_LAIR_ODOLWA:
        case SCE_MM_LAIR_GOHT:
        case SCE_MM_LAIR_GYORG:
        case SCE_MM_LAIR_TWINMOLD:
        case SCE_MM_LAIR_MAJORA:
            gSave.playerForm = MM_PLAYER_FORM_HUMAN;
            gSave.equippedMask = MASK_NONE;
            break;
        }
    }

    g.nextEntrance = entrance;
    g.isNextEntranceInitialSong = (entrance == ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER) && (entrance != gSave.entrance);
    play->nextEntrance = ENTR_EXTENDED;
#endif
}


void comboTransition(PlayState* play, u32 entrance)
{
    play->transitionTrigger = TRANS_TRIGGER_START;

#if defined(GAME_OOT)
    play->transitionType = TRANS_GFX_BLACK;
#endif

#if defined(GAME_MM)
    play->transitionType = TRANS_TYPE_FADE_BLACK;
#endif

    comboTransitionOverride(play, entrance);
}
