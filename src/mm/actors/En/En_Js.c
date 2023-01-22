#include <combo.h>

int EnJs_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.maskFierceDeity = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x8096a2fc, EnJs_HasGivenItem);

int EnJs_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags2.maskFierceDeity)
        gi = GI_MM_RECOVERY_HEART;
    else
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_FIERCE_DEITY, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x8096a370, EnJs_GiveItem);

static void EnJs_DisplayHint(GameState_Play* play, s16 messageId)
{
    s16 gi;
    char* b;
    char* start;

    gi = comboOverrideEx(OV_NPC, 0, NPC_MM_MASK_FIERCE_DEITY, GI_MM_MASK_FIERCE_DEITY, 0);

    /* Hint */
    DisplayTextBox2(play, messageId);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "You have only weak masks..." TEXT_NL "Having better masks would give you ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "..." TEXT_BB "So...you'll play?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_CALL(0x8096a4c8, EnJs_DisplayHint);
