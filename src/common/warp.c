#include <combo.h>

void comboTriggerWarp(GameState_Play* play, int bossId)
{
    int index;
    s32 entrance;

    /* Flag the actual boss as dead (MM) */
    switch (bossId)
    {
    case 8:
        gMmExtraBoss.bossCycle |= (1 << 0);
        break;
    case 9:
        gMmExtraBoss.bossCycle |= (1 << 1);
        break;
    case 10:
        gMmExtraBoss.bossCycle |= (1 << 2);
        break;
    case 11:
        gMmExtraBoss.bossCycle |= (1 << 3);
        break;
    }

    /* Use shuffled index */
    index = (int)gComboData.blueWarps[bossId];

    /* Set flags and entrance */
    switch (index)
    {
    case 0: /* Deku Tree */
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_EMERALD_TREE_DEAD);
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_GOHMA);
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_TREE_DEAD);
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_MIDO_TREE_DEAD);
        entrance = 0x0457;
        break;
    case 1: /* Dodongo Cavern */
        entrance = 0x0242;
        break;
    case 2: /* Jabu-Jabu */
        entrance = 0x0221;
        break;
    case 3: /* Forest */
        entrance = 0x0608;
        break;
    case 4: /* Fire */
        entrance = 0x0564;
        break;
    case 5: /* Water */
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_LAKE_HYLIA_WATER);
        entrance = 0x060C;
        break;
    case 6: /* Shadow */
        entrance = 0x0580;
        break;
    case 7: /* Spirit */
        entrance = 0x0610;
        break;
    case 8: /* Woodfall */
        gMmExtraBoss.boss |= (1 << 0);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE);
        entrance = 0x3010;
        break;
    case 9: /* Snowhead */
        gMmExtraBoss.boss |= (1 << 1);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH);
        entrance = 0xae70;
        break;
    case 10: /* Great Bay */
        gMmExtraBoss.boss |= (1 << 2);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB);
        entrance = 0x6a90;
        break;
    case 11: /* Stone Tower */
        gMmExtraBoss.boss |= (1 << 3);
        MM_SET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_ST);
        entrance = 0x20f0;
        break;
    default:
        UNREACHABLE();
        break;
    }

#if defined(GAME_OOT)
    if (index < 8)
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
    if (index >= 8)
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
