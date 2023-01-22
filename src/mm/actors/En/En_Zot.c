#include <combo.h>

int EnZot_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParent(this);
    if (ret)
        gMmExtraFlags.zoraHallLights = 1;
    return ret;
}

PATCH_CALL(0x80b98f40, EnZot_HasGivenItem);

int EnZot_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (!gMmExtraFlags.zoraHallLights)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_ZORA_HALL_LIGHTS, gi);
    }
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b98f7c, EnZot_GiveItem);

static void appendTextNpcItem(char** buf, GameState_Play* play, s16 npcId, s16 gi)
{
    s16 itemId;

    gi = comboOverrideEx(OV_NPC, 0, npcId, gi, 0);
    itemId = comboItemFromGI(gi);
    comboTextAppendItemName(buf, itemId, TF_PREPOS | TF_PROGRESSIVE);
}

void EnZot_DisplayBeaverHint(Actor* this, GameState_Play* play)
{
    char* b;
    char* start;

    PlayerDisplayTextBox(play, 0x1260, this);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "I've heared that the beavers are betting ");
    appendTextNpcItem(&b, play, NPC_MM_BEAVER_1, GI_MM_EMPTY_BOTTLE);
    comboTextAppendStr(&b, " and ");
    appendTextNpcItem(&b, play, NPC_MM_BEAVER_2, GI_MM_HEART_PIECE);
    comboTextAppendStr(&b, "." TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_FUNC(0x80b97110, EnZot_DisplayBeaverHint);
