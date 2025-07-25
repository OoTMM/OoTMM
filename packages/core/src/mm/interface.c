#include <combo.h>
#include <combo/interface.h>
#include <combo/config.h>

void Interface_UpdateButtonsPart2Wrapper(PlayState* play)
{
    u8* ptr;
    u8 itemId;
    u8 buttons[3];

    for (int i = 0; i < 3; ++i)
    {
        ptr = &gSave.info.itemEquips.buttonItems[0][i + 1];
        itemId = *ptr;
        buttons[i] = itemId;

        if (itemId == ITEM_MM_OCARINA_FAIRY)
            *ptr = ITEM_MM_OCARINA_OF_TIME;
    }

    Interface_UpdateButtonsPart2(play);

    for (int i = 0; i < 3; ++i)
    {
        ptr = &gSave.info.itemEquips.buttonItems[0][i + 1];
        *ptr = buttons[i];
    }
}

void Interface_SkipMoonCrash(PlayState* play)
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
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_FADE_WHITE;
}

PATCH_CALL(0x8011d3f4, Interface_SkipMoonCrash)

ALIGNED(4) u8 kColorCButtons[4] = { 255, 240, 0, 0 };
EXPORT_SYMBOL(COLOR_C_BUTTONS, kColorCButtons);

Gfx* Interface_DrawCButton(Gfx* gfx, s16 rectLeft, s16 rectTop, s16 rectWidth, s16 rectHeight, u16 dsdx, u16 dtdy, s16 r, s16 g, s16 b, s16 a)
{
    return Gfx_DrawRect_DropShadow(gfx, rectLeft, rectTop, rectWidth, rectHeight, dsdx, dtdy, kColorCButtons[0], kColorCButtons[1], kColorCButtons[2], a);
}

PATCH_CALL(0x8011725c, Interface_DrawCButton);
PATCH_CALL(0x801172bc, Interface_DrawCButton);
PATCH_CALL(0x8011731c, Interface_DrawCButton);
PATCH_CALL(0x80117674, Interface_DrawCButton);
