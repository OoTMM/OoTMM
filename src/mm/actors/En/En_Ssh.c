#include <combo.h>

static void hintSkullReward(GameState_Play* play, s16 gi)
{
    char* b;
    char* start;

    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "If you lift the curse... I'll give you... ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "... Hurry... Please..." TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void EnSsh_DisplayHint(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (play->sceneId == SCE_MM_SPIDER_HOUSE_OCEAN)
    {
        gi = comboOverrideEx(OV_NPC, 0, NPC_MM_SPIDER_HOUSE_OCEAN, GI_MM_WALLET2, 0);
    }
    else
    {
        gi = comboOverrideEx(OV_NPC, 0, NPC_MM_SPIDER_HOUSE_SWAMP, GI_MM_MASK_TRUTH, 0);
    }
    PlayerDisplayTextBox(play, 0x915, this);
    hintSkullReward(play, gi);
}

PATCH_FUNC(0x809756d0, EnSsh_DisplayHint);
