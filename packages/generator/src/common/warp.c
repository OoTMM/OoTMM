#include <combo.h>
#include <combo/entrance.h>
#include <combo/dungeon.h>
#include <combo/config.h>
#include <combo/multi.h>

void comboTriggerWarp(PlayState* play, int bossId)
{
    int dungeonId;
    int dungeonClearFlags;
    u32 dungeonExit;

    /* Flag the boss as dead */
    DungeonBoss_SetFlag(bossId);
    Multi_SendEvent(MULTI_EVENT_BOSS(bossId));

    /* Compute shuffled index and set flags */
    dungeonId = (int)gComboConfig.boss[bossId];
    dungeonClearFlags = DUNGEONCLEARFLAG_BOSS;
    if (Config_Flag(CFG_REGION_STATE_DUNGEONS))
        dungeonClearFlags |= DUNGEONCLEARFLAG_EFFECT;
    Dungeon_SetFlags(dungeonId, dungeonClearFlags);

    /* Set entrance - need special case for warp dungeons */
    dungeonExit = gComboConfig.dungeonWarps[dungeonId];
    switch (dungeonExit)
    {
    case ENTR_OOT_KOKIRI_FOREST_FROM_DEKU_TREE:
        dungeonExit = 0x0457; /* Works OK as adult */
        break;
    case ENTR_OOT_MOUNTAIN_TRAIL_FROM_DODONGO_CAVERN:
        dungeonExit = 0x0242; /* Works OK as adult */
        break;
    case ENTR_OOT_ZORA_FOUNTAIN_FROM_JABU_JABU:
        dungeonExit = 0x0221; /* Works OK as adult */
        break;
    case ENTR_OOT_SACRED_MEADOW_FROM_TEMPLE_FOREST:
        dungeonExit = ENTR_OOT_WARP_SONG_MEADOW; /* Normal exit WW as child */
        break;
    case ENTR_OOT_DEATH_CRATER_FROM_TEMPLE_FIRE:
        dungeonExit = ENTR_OOT_WARP_SONG_CRATER; /* Normal exit WW as child */
        break;
    case ENTR_OOT_LAKE_HYLIA_FROM_TEMPLE_WATER:
        dungeonExit = ENTR_OOT_WARP_SONG_LAKE; /* Normal exit WW as child */
        break;
    case ENTR_OOT_GRAVEYARD_FROM_TEMPLE_SHADOW:
        dungeonExit = ENTR_OOT_WARP_SONG_GRAVE; /* Normal exit crashes as child */
        break;
    case ENTR_OOT_DESERT_COLOSSUS_FROM_TEMPLE_SPIRIT:
        dungeonExit = ENTR_OOT_DESERT_COLOSSUS_FROM_TEMPLE_SPIRIT; /* Normal exit crashes as child */
        break;
    case (ENTR_MM_WOODFALL_FROM_TEMPLE | MASK_FOREIGN_ENTRANCE):
        if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF) && MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE))
            dungeonExit = 0x3010 | MASK_FOREIGN_ENTRANCE;
        else
            dungeonExit = 0x8600 | MASK_FOREIGN_ENTRANCE;
        break;
    case (ENTR_MM_SNOWHEAD_FROM_TEMPLE | MASK_FOREIGN_ENTRANCE):
        dungeonExit = 0x9a70 | MASK_FOREIGN_ENTRANCE;
        break;
    case (ENTR_MM_GREAT_BAY_FROM_TEMPLE | MASK_FOREIGN_ENTRANCE):
        dungeonExit = 0x6a70 | MASK_FOREIGN_ENTRANCE;
        break;
    case (ENTR_MM_STONE_TOWER_FROM_TEMPLE | MASK_FOREIGN_ENTRANCE):
    case (ENTR_MM_STONE_TOWER_INVERTED_FROM_TEMPLE | MASK_FOREIGN_ENTRANCE):
        dungeonExit = 0x2070 | MASK_FOREIGN_ENTRANCE;
        break;
    }

#if defined(GAME_MM)
    dungeonExit ^= MASK_FOREIGN_ENTRANCE;
    gSave.playerForm = MM_PLAYER_FORM_HUMAN;
    gSave.equippedMask = 0;
#endif

    comboTransition(play, dungeonExit);
}
