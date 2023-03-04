#include <combo.h>

void comboTriggerWarp(GameState_Play* play, int bossId)
{
    s32 entrance;
    int dungeonId;
    int dungeonEntranceId;
    int isMmEntrance;

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

    /* Set entrance */
    isMmEntrance = 0;
    switch (dungeonEntranceId)
    {
    case DUNGEONID_DEKU_TREE:
        entrance = 0x0457;
        break;
    case DUNGEONID_DODONGOS_CAVERN:
        entrance = 0x0242;
        break;
    case DUNGEONID_JABU_JABU:
        entrance = 0x0221;
        break;
    case DUNGEONID_TEMPLE_FOREST:
        entrance = 0x0608;
        break;
    case DUNGEONID_TEMPLE_FIRE:
        entrance = 0x0564;
        break;
    case DUNGEONID_TEMPLE_WATER:
        entrance = 0x060C;
        break;
    case DUNGEONID_TEMPLE_SHADOW:
        entrance = 0x0580;
        break;
    case DUNGEONID_TEMPLE_SPIRIT:
        entrance = 0x0610;
        break;
    case DUNGEONID_TEMPLE_WOODFALL:
        isMmEntrance = 1;
        if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF) && MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE))
        {
            entrance = 0x3010;
        }
        else
        {
            entrance = 0x8600;
        }
        break;
    case DUNGEONID_TEMPLE_SNOWHEAD:
        isMmEntrance = 1;
        if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH))
        {
            entrance = 0xae70;
        }
        else
        {
            entrance = 0x9a80;
        }
        break;
    case DUNGEONID_TEMPLE_GREAT_BAY:
        isMmEntrance = 1;
        entrance = 0x6a70;
        break;
    case DUNGEONID_TEMPLE_STONE_TOWER:
    case DUNGEONID_TEMPLE_STONE_TOWER_INVERTED:
        isMmEntrance = 1;
        entrance = 0x2070;
        break;
    case DUNGEONID_SPIDER_HOUSE_SWAMP:
        isMmEntrance = 1;
        entrance = 0x8480;
        break;
    case DUNGEONID_SPIDER_HOUSE_OCEAN:
        isMmEntrance = 1;
        entrance = 0x6880;
        break;
    case DUNGEONID_BOTTOM_OF_THE_WELL:
        entrance = 0x2a6;
        break;
    case DUNGEONID_ICE_CAVERN:
        entrance = 0x3d4;
        break;
    case DUNGEONID_GERUDO_TRAINING_GROUNDS:
        entrance = 0x3a8;
        break;
    default:
        UNREACHABLE();
        break;
    }

#if defined(GAME_OOT)
    if (!isMmEntrance)
    {
        TransitionContext* t;

        t = &play->transition;
        t->type = TRANS_TYPE_NORMAL;
        t->gfx = TRANS_GFX_BLACK;
        t->entrance = entrance;
    }
    else
    {
        comboGameSwitch(play, entrance);
    }
#endif

#if defined(GAME_MM)
    if (isMmEntrance)
    {
        gNoTimeFlow = 0;

        gSave.playerForm = MM_PLAYER_FORM_HUMAN;
        gSave.equippedMask = 0;

        play->nextEntrance = entrance;
        play->transitionType = TRANS_TYPE_NORMAL;
        play->transitionGfx = TRANS_GFX_BLACK;
    }
    else
    {
        comboGameSwitch(play, entrance);
    }
#endif
}
