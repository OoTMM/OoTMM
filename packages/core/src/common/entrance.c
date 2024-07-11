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
    s32 key;
    s32 value;
}
Entrance;

static Entrance* gEntrances;

void comboInitEntrances(void)
{
    size_t size;

    size = comboDmaLoadFile(NULL, COMBO_VROM_ENTRANCES);
    gEntrances = malloc(size);
    comboDmaLoadFile(gEntrances, COMBO_VROM_ENTRANCES);
}

s32 comboEntranceOverride(u32 entranceId)
{
    int i;

    i = 0;
    for (;;)
    {
        if (gEntrances[i].key == -1)
            return -1;
        if (gEntrances[i].key == (s32)entranceId)
            return gEntrances[i].value;
        i++;
    }
}

void comboTransition(GameState_Play* play, u32 entrance)
{
#if defined(GAME_OOT)
    g.nextEntrance = entrance;
    play->nextEntranceIndex = ENTR_EXTENDED;
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_GFX_BLACK;
#endif

#if defined(GAME_MM)
    gNoTimeFlow = 0;
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
    play->nextEntrance = ENTR_EXTENDED;
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
#endif
}
