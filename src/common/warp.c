#include <combo.h>

void comboTriggerWarp(GameState_Play* play, int bossId)
{
    s32 entrance;

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

    /* Use shuffled index */
    bossId = (int)gComboData.boss[bossId];

    /* Set flags and entrance */
    switch (bossId)
    {
    case BOSSID_GOHMA:
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_EMERALD_TREE_DEAD);
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_GOHMA);
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_TREE_DEAD);
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_MIDO_TREE_DEAD);
        entrance = 0x0457;
        break;
    case BOSSID_KING_DODONGO:
        entrance = 0x0242;
        break;
    case BOSSID_BARINADE:
        entrance = 0x0221;
        break;
    case BOSSID_PHANTOM_GANON:
        entrance = 0x0608;
        break;
    case BOSSID_VOLVAGIA:
        entrance = 0x0564;
        break;
    case BOSSID_MORPHA:
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_LAKE_HYLIA_WATER);
        entrance = 0x060C;
        break;
    case BOSSID_BONGO_BONGO:
        entrance = 0x0580;
        break;
    case BOSSID_TWINROVA:
        entrance = 0x0610;
        break;
    case BOSSID_ODOLWA:
        gMmExtraBoss.boss |= (1 << 0);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE);
        entrance = 0x3010;
        break;
    case BOSSID_GOHT:
        gMmExtraBoss.boss |= (1 << 1);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH);
        entrance = 0xae70;
        break;
    case BOSSID_GYORG:
        gMmExtraBoss.boss |= (1 << 2);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB);
        entrance = 0x6a90;
        break;
    case BOSSID_TWINMOLD:
        gMmExtraBoss.boss |= (1 << 3);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_ST);
        entrance = 0x20f0;
        break;
    default:
        UNREACHABLE();
        break;
    }

#if defined(GAME_OOT)
    if (bossId < BOSSID_ODOLWA)
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
    if (bossId >= BOSSID_ODOLWA)
    {
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
