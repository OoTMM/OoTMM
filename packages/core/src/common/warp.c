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

    /* Set flags */
    switch (dungeonId)
    {
    case DUNGEONID_DEKU_TREE:
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_EMERALD_TREE_DEAD);
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_GOHMA);
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_TREE_DEAD);
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_MIDO_TREE_DEAD);
        break;
    case DUNGEONID_TEMPLE_WATER:
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_LAKE_HYLIA_WATER);
        break;
    case DUNGEONID_TEMPLE_WOODFALL:
        gMmExtraBoss.boss |= (1 << 0);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF);
        gMiscFlags.erSwampClear = 1;
        break;
    case DUNGEONID_TEMPLE_SNOWHEAD:
        gMmExtraBoss.boss |= (1 << 1);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH);
        gMiscFlags.erSpring = 1;
        break;
    case DUNGEONID_TEMPLE_GREAT_BAY:
        gMmExtraBoss.boss |= (1 << 2);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB);
        gMiscFlags.erCoastClear = 1;
        break;
    case DUNGEONID_TEMPLE_STONE_TOWER_INVERTED:
        gMmExtraBoss.boss |= (1 << 3);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_ST);
        break;
    }

    /* Set entrance - need special case for warp dungeons */
    comboGetDungeonExit(&descr, dungeonEntranceId);
    switch (dungeonEntranceId)
    {
    case DUNGEONID_DEKU_TREE:
        descr.id = 0x0457; // Works OK as adult
        break;
    case DUNGEONID_DODONGOS_CAVERN:
        descr.id = 0x0242; // Works OK as adult
        break;
    case DUNGEONID_JABU_JABU:
        descr.id = 0x0221; // Works OK as adult
        break;
    case DUNGEONID_TEMPLE_FOREST:
        descr.id = 0x0600; // Normal exit WW as child
        break;
    case DUNGEONID_TEMPLE_FIRE:
        descr.id = 0x04f6; // Normal exit WW as child
        break;
    case DUNGEONID_TEMPLE_WATER:
        descr.id = 0x0604; // Normal exit WW as child
        break;
    case DUNGEONID_TEMPLE_SHADOW:
        descr.id = 0x0568; // Normal exit crashes as child
        break;
    case DUNGEONID_TEMPLE_SPIRIT:
        descr.id = 0x01e1; // Normal exit crashes as child
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
    gNoTimeFlow = 0;
    gSave.playerForm = MM_PLAYER_FORM_HUMAN;
    gSave.equippedMask = 0;
#endif

    comboTransitionDescr(play, &descr);
}
