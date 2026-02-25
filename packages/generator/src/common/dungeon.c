#include <combo.h>
#include <combo/dungeon.h>
#include <combo/config.h>

void comboDungeonSetFlags(int dungeonId, int flags)
{
    switch (dungeonId)
    {
    case DUNGEONID_DEKU_TREE:
        BITMAP16_SET(gOotSave.info.eventsChk, EV_OOT_CHK_EMERALD_TREE_DEAD);
        BITMAP16_SET(gOotSave.info.eventsChk, EV_OOT_CHK_GOHMA);
        BITMAP16_SET(gOotSave.info.eventsChk, EV_OOT_CHK_TREE_DEAD);
        BITMAP16_SET(gOotSave.info.eventsChk, EV_OOT_CHK_MIDO_TREE_DEAD);
        break;
    case DUNGEONID_TEMPLE_WATER:
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            BITMAP16_SET(gOotSave.info.eventsChk, EV_OOT_CHK_LAKE_HYLIA_WATER);
            gMiscFlags.wispOotLake = 1;
        }
        if (flags & DUNGEONCLEARFLAG_WISP)
            gMiscFlags.wispOotLake = 1;
        break;
    case DUNGEONID_TEMPLE_WOODFALL:
        if (flags & DUNGEONCLEARFLAG_BOSS)
            gMmExtraBoss.boss |= (1 << 0);
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF);
            if (Config_Flag(CFG_MM_CLEAR_OPEN_WF))
            {
                MM_SET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE);
            }
        }
        if (flags & DUNGEONCLEARFLAG_WISP)
            gMiscFlags.wispMmSwamp = 1;
        break;
    case DUNGEONID_TEMPLE_SNOWHEAD:
        if (flags & DUNGEONCLEARFLAG_BOSS)
            gMmExtraBoss.boss |= (1 << 1);
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH);
        }
        if (flags & DUNGEONCLEARFLAG_WISP)
            gMiscFlags.wispMmMountain = 1;
        break;
    case DUNGEONID_TEMPLE_GREAT_BAY:
        if (flags & DUNGEONCLEARFLAG_BOSS)
            gMmExtraBoss.boss |= (1 << 2);
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB);
            if (Config_Flag(CFG_MM_CLEAR_OPEN_GB))
            {
                MM_SET_EVENT_WEEK(EV_MM_WEEK_GREAT_BAY_TURTLE);
            }
        }
        if (flags & DUNGEONCLEARFLAG_WISP)
            gMiscFlags.wispMmOcean = 1;
        break;
    case DUNGEONID_TEMPLE_STONE_TOWER:
    case DUNGEONID_TEMPLE_STONE_TOWER_INVERTED:
        if (flags & DUNGEONCLEARFLAG_BOSS)
            gMmExtraBoss.boss |= (1 << 3);
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_ST);
        }
        if (flags & DUNGEONCLEARFLAG_WISP)
            gMiscFlags.wispMmValley = 1;
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
