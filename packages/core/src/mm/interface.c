#include <combo.h>
#include <combo/interface.h>
#include <combo/config.h>

void Interface_UpdateButtonsPart2Wrapper(GameState_Play* play)
{
    u8* ptr;
    u8 itemId;
    u8 buttons[3];

    for (int i = 0; i < 3; ++i)
    {
        ptr = &gSave.itemEquips.buttonItems[0][i + 1];
        itemId = *ptr;
        buttons[i] = itemId;

        if (itemId == ITEM_MM_OCARINA_FAIRY)
            *ptr = ITEM_MM_OCARINA_OF_TIME;
    }

    Interface_UpdateButtonsPart2(play);

    for (int i = 0; i < 3; ++i)
    {
        ptr = &gSave.itemEquips.buttonItems[0][i + 1];
        *ptr = buttons[i];
    }
}

void Interface_SkipMoonCrash(GameState_Play* play)
{
    if(play->actorCtx.flags & (1 << 1))
        AudioSeq_QueueSeqCmd((14 << 28) | (1 << 8));
    if(Config_Flag(CFG_MM_MOON_CRASH_CYCLE))
    {
        gSave.day = 0;
        gSave.daysElapsed = 0;
        play->nextEntrance = ENTR_MM_CLOCK_TOWER_MOON_CRASH;
        Sram_SaveEndOfCycle(play);
    }
    else
    {
        gSave.day = 4;
        gSave.daysElapsed = 4;
        play->nextEntrance = 0x54c0;
    }
    gSave.cutscene = 0;
    gSave.time = CLOCK_TIME(6, 0) + 10;
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_FADE_WHITE;
}

PATCH_CALL(0x8011d3f4, Interface_SkipMoonCrash)
