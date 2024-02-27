#include <combo.h>
#include <combo/entrance.h>
#include <combo/dungeon.h>

void comboTriggerWarp(GameState_Play* play, int bossId)
{
    EntranceDescr descr;
    int dungeonId;
    int dungeonEntranceId;

#if defined(GAME_MM)
    /* Flag the actual boss as dead (MM) */
    switch (bossId)
    {
    case BOSSID_ODOLWA:
        gMmExtraBoss.bossCycle |= (1 << 0);
        break;
    case BOSSID_GOHT:
        gMmExtraBoss.bossCycle |= (1 << 1);
        break;
    case BOSSID_GYORG:
        gMmExtraBoss.bossCycle |= (1 << 2);
        break;
    case BOSSID_TWINMOLD:
        gMmExtraBoss.bossCycle |= (1 << 3);
        break;
    }
#endif

    /* Compute shuffled indices */
    dungeonId = (int)gComboData.boss[bossId];
    dungeonEntranceId = (int)gComboData.dungeons[dungeonId];

    comboDungeonSetFlags(dungeonId, 1);

    /* Set entrance - need special case for warp dungeons */
    comboGetDungeonExit(&descr, dungeonEntranceId);
    switch (dungeonEntranceId)
    {
    case DUNGEONID_DEKU_TREE:
        descr.id = 0x0457; /* Works OK as adult */
        break;
    case DUNGEONID_DODONGOS_CAVERN:
        descr.id = 0x0242; /* Works OK as adult */
        break;
    case DUNGEONID_JABU_JABU:
        descr.id = 0x0221; /* Works OK as adult */
        break;
    case DUNGEONID_TEMPLE_FOREST:
        descr.id = 0x0600; /* Normal exit WW as child */
        break;
    case DUNGEONID_TEMPLE_FIRE:
        descr.id = 0x04f6; /* Normal exit WW as child */
        break;
    case DUNGEONID_TEMPLE_WATER:
        descr.id = 0x0604; /* Normal exit WW as child */
        break;
    case DUNGEONID_TEMPLE_SHADOW:
        descr.id = 0x0568; /* Normal exit crashes as child */
        break;
    case DUNGEONID_TEMPLE_SPIRIT:
        descr.id = 0x01e1; /* Normal exit crashes as child */
        break;
    case DUNGEONID_TEMPLE_WOODFALL:
        if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF) && MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE))
        {
            descr.id = 0x3010;
        }
        else
        {
            descr.id = 0x8600;
        }
        break;
    case DUNGEONID_TEMPLE_SNOWHEAD:
        if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH))
        {
            descr.id = 0xae70;
        }
        else
        {
            descr.id = 0x9a70;
        }
        break;
    case DUNGEONID_TEMPLE_GREAT_BAY:
        descr.id = 0x6a70;
        break;
    case DUNGEONID_TEMPLE_STONE_TOWER:
    case DUNGEONID_TEMPLE_STONE_TOWER_INVERTED:
        descr.id = 0x2070;
        break;
    }

#if defined(GAME_MM)
    gSave.playerForm = MM_PLAYER_FORM_HUMAN;
    gSave.equippedMask = 0;
#endif

    comboTransitionDescr(play, &descr);
}
