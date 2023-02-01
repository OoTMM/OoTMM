#include <combo.h>

void comboTriggerWarp(GameState_Play* play, int index)
{
    s32 entrance;

    /* Use shuffled index */
    index = (int)gComboData.blueWarps[index];

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
        entrance = 0x060C;
        break;
    case 8: /* Woodfall */
        break;
    case 9: /* Snowhead */
        break;
    case 10: /* Great Bay */
        break;
    case 11: /* Stone Tower */
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
        entrance |= 0x80000000;
        gComboCtx.entrance = entrance;
        comboGameSwitch();
    }
#endif
}
