#include <combo.h>
#include <combo/dungeon.h>
#include <combo/config.h>

void Dungeon_SetFlags(int dungeonId, int flags)
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
        gSharedCustomSave.oot.waterBeaten = 1;
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            BITMAP16_SET(gOotSave.info.eventsChk, EV_OOT_CHK_LAKE_HYLIA_WATER);
        }
        break;
    case DUNGEONID_TEMPLE_WOODFALL:
        if (flags & DUNGEONCLEARFLAG_BOSS)
            gMmExtraBoss.dungeon |= (1 << 0);
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF);
            if (Config_Flag(CFG_MM_CLEAR_OPEN_WF))
            {
                MM_SET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE);
            }
        }
        break;
    case DUNGEONID_TEMPLE_SNOWHEAD:
        if (flags & DUNGEONCLEARFLAG_BOSS)
            gMmExtraBoss.dungeon |= (1 << 1);
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH);
        }
        break;
    case DUNGEONID_TEMPLE_GREAT_BAY:
        if (flags & DUNGEONCLEARFLAG_BOSS)
            gMmExtraBoss.dungeon |= (1 << 2);
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB);
            if (Config_Flag(CFG_MM_CLEAR_OPEN_GB))
            {
                MM_SET_EVENT_WEEK(EV_MM_WEEK_GREAT_BAY_TURTLE);
            }
        }
        break;
    case DUNGEONID_TEMPLE_STONE_TOWER:
    case DUNGEONID_TEMPLE_STONE_TOWER_INVERTED:
        if (flags & DUNGEONCLEARFLAG_BOSS)
            gMmExtraBoss.dungeon |= (1 << 3);
        if (flags & DUNGEONCLEARFLAG_EFFECT)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_ST);
        }
        break;
    }
}

int Dungeon_Boss(int dungeonId)
{
    for (int i = 0; i <= BOSSID_TWINMOLD; ++i)
    {
        if (gComboConfig.boss[i] == dungeonId)
            return i;
    }
    return -1;
}

static const u8 kOotBossScenes[] = {
    SCE_OOT_LAIR_GOHMA,
    SCE_OOT_LAIR_KING_DODONGO,
    SCE_OOT_LAIR_BARINADE,
    SCE_OOT_LAIR_PHANTOM_GANON,
    SCE_OOT_LAIR_VOLVAGIA,
    SCE_OOT_LAIR_MORPHA,
    SCE_OOT_LAIR_BONGO_BONGO,
    SCE_OOT_LAIR_TWINROVA,
};

static const u8 kOotBossRooms[] = {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    3,
};

int DungeonBoss_GetFlag(int bossId)
{
    u8 sceneId;
    u8 roomId;

    if (bossId < BOSSID_ODOLWA)
    {
        sceneId = kOotBossScenes[bossId];
        roomId = kOotBossRooms[bossId];

#if defined(GAME_OOT)
        if (gPlay->sceneId == sceneId)
            return Flags_GetClear(gPlay, roomId);
#endif
        return gOotSave.info.perm[sceneId].roomClear & (1 << roomId);
    }
    else
    {
        return gMmExtraBoss.boss & (1 << (bossId - BOSSID_ODOLWA));
    }
}

void DungeonBoss_SetFlag(int bossId)
{
    u8 sceneId;
    u8 roomId;
    u8 flag;

    if (bossId < BOSSID_ODOLWA)
    {
        sceneId = kOotBossScenes[bossId];
        roomId = kOotBossRooms[bossId];

#if defined(GAME_OOT)
        if (gPlay->sceneId == sceneId)
            Flags_SetClear(gPlay, roomId);
#endif
        gOotSave.info.perm[sceneId].roomClear |= (1 << roomId);
    }
    else
    {
        flag = (1 << (bossId - BOSSID_ODOLWA));
        gMmExtraBoss.boss |= flag;
        gMmExtraBoss.bossCycle |= flag;
    }
}
