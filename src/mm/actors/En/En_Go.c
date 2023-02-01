#include <combo.h>

void EnGo_AfterGivingItem(Actor* actor)
{
    gMmExtraFlags.powderKeg = 1;
}

int EnGo_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (!gMmExtraFlags.powderKeg)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_GORON_KEG, GI_MM_POWDER_KEG);
    }
    else if (gMmSave.inventory.items[ITS_MM_KEG] == ITEM_MM_POWDER_KEG)
    {
        gi = GI_MM_POWDER_KEG;
    }
    else
    {
        gi = GI_MM_RECOVERY_HEART;
    }
    return GiveItem(this, play, gi, a, b);
}

static void powderKegHint(GameState_Play* play)
{
    s16 gi;
    char* b;
    char* start;

    gi = comboOverrideEx(OV_NPC, 0, NPC_MM_GORON_KEG, GI_MM_POWDER_KEG, 0);

    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "If you pass my test, I will give you ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "!" TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

void EnGo_AfterTextBox(Actor* this, GameState_Play* play, s16 messageId)
{
    if (messageId == 0x0c81)
    {
        powderKegHint(play);
    }
}
