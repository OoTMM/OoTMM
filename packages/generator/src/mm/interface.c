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

void LoadOotItemIcon(void* dst, int id)
{
    DmaEntry dma;
    comboDmaLookupForeignId(&dma, 8);
    u32 textureFileAddress = dma.pstart;
    u32 textureOffset = 0x1000 * id;
    DMARomToRam((textureFileAddress + textureOffset) | PI_DOM1_ADDR2, dst, 0x1000);
}

void comboCustomItemIcon(void* dst, int itemId)
{
    switch(itemId)
    {
    case ITEM_MM_SPELL_FIRE:
        itemId = ITEM_OOT_SPELL_FIRE;
        break;
    case ITEM_MM_SPELL_WIND:
        itemId = ITEM_OOT_SPELL_WIND;
        break;
    case ITEM_MM_SPELL_LOVE:
        itemId = ITEM_OOT_SPELL_LOVE;
        break;
    case ITEM_MM_BOOTS_IRON:
        itemId = ITEM_OOT_BOOTS_IRON;
        break;
    case ITEM_MM_BOOTS_HOVER:
        itemId = ITEM_OOT_BOOTS_HOVER;
        break;
    case ITEM_MM_TUNIC_GORON:
        itemId = ITEM_OOT_TUNIC_GORON;
        break;
    case ITEM_MM_TUNIC_ZORA:
        itemId = ITEM_OOT_TUNIC_ZORA;
        break;
    case ITEM_MM_HAMMER:
        itemId = ITEM_OOT_HAMMER;
        break;
    case ITEM_MM_BOOMERANG:
        itemId = ITEM_OOT_BOOMERANG;
        break;
    case ITEM_MM_RUTO_LETTER:
        itemId = ITEM_OOT_RUTO_LETTER;
        break;
    default:
        return;
    }
    LoadOotItemIcon(dst, itemId);
}

void comboItemIcon(void* dst, int itemId)
{
    if (itemId < ITEM_MM_CUSTOM_MIN)
    {
        LoadIcon(0xa36c10, itemId, dst, 0x1000);
    }
    else
    {
        comboCustomItemIcon(dst, itemId);
    }
}
