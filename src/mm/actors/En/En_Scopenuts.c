#include <combo.h>

static void EnScopenuts_AlterMessage(GameState_Play* play)
{
    s16 gi;
    char* b;
    char* start;

    gi = comboOverrideEx(OV_NPC, 0, NPC_MM_SCRUB_TELESCOPE, GI_MM_HEART_PIECE, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "Please! I'll sell you ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, " if you just keep this place a secret..." TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void EnScopenuts_DisplayTextBox(GameState_Play* play, s16 messageId, Actor* this)
{
    PlayerDisplayTextBox(play, messageId, this);

    if (messageId == 0x1631)
        EnScopenuts_AlterMessage(play);
}

PATCH_CALL(0x80bcb588, EnScopenuts_DisplayTextBox);
PATCH_CALL(0x80bcb79c, EnScopenuts_DisplayTextBox);

int EnScopenuts_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_TELESCOPE, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80bcb968, EnScopenuts_GiveItem);
