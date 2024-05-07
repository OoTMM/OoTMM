#include <combo.h>
#include <combo/dungeon.h>
#include <combo/config.h>

void comboDungeonSetFlags(int dungeonId, int mmCycle)
{
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
        gMiscFlags.erWaterBeaten = 1;
        break;
    case DUNGEONID_TEMPLE_WOODFALL:
        gMmExtraBoss.boss |= (1 << 0);
        if (mmCycle)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF);
            if (Config_Flag(CFG_MM_CLEAR_OPEN_WF))
            {
                MM_SET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE);
            }
        }
        gMiscFlags.erSwampClear = 1;
        break;
    case DUNGEONID_TEMPLE_SNOWHEAD:
        gMmExtraBoss.boss |= (1 << 1);
        if (mmCycle)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH);
        }
        gMiscFlags.erSpring = 1;
        break;
    case DUNGEONID_TEMPLE_GREAT_BAY:
        gMmExtraBoss.boss |= (1 << 2);
        if (mmCycle)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB);
            if (Config_Flag(CFG_MM_CLEAR_OPEN_GB))
            {
                MM_SET_EVENT_WEEK(EV_MM_WEEK_GREAT_BAY_TURTLE);
            }
        }
        gMiscFlags.erCoastClear = 1;
        break;
    case DUNGEONID_TEMPLE_STONE_TOWER_INVERTED:
        gMmExtraBoss.boss |= (1 << 3);
        if (mmCycle)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_ST);
        }
        gMiscFlags.erValleyClear = 1;
        break;
    }
}

int comboBossDungeon(int dungeonId)
{
    for (int i = 0; i <= BOSSID_TWINMOLD; ++i)
    {
        if (gComboConfig.boss[i] == dungeonId)
            return i;
    }
    return -1;
}
