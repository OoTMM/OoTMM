#include <combo.h>

static void EnJs_AskForFight(GameState_Play* play, u16 unk, Actor* this)
{
    char* b;
    char* start;

    PlayerDisplayTextBox(play, 0x21fe, this);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "So...you'll play?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_CALL(0x8096a25c, EnJs_AskForFight);

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

void EnJs_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags2.maskFierceDeity)
        gi = GI_MM_RECOVERY_HEART;
    else
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_FIERCE_DEITY, gi);
    GiveItem(this, play, gi, a, b);
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

static void EnJs_DisplayWeak(Actor* this, GameState_Play* play)
{
    char* b;

    DisplayTextBox2(play, 0x21ff);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "You're too weak..." TEXT_SIGNAL TEXT_END);
}

void EnJs_TryStartFight(Actor* this)
{
    int canFight;
    void (*EnJs_SetFreeCamera)(Actor*, int);

    canFight = 0;
    if (comboConfig(CFG_GOAL_TRIFORCE))
    {
        if (gOotExtraFlags.triforceWin)
            canFight = 1;
    }
    else if (!comboConfig(CFG_MM_MAJORA_CHILD_CUSTOM) || comboSpecialCond(SPECIAL_MAJORA))
    {
        canFight = 1;
    }

    if (canFight)
    {
        /* Start the fight*/
        DisplayTextBox2(gPlay, 0x2200);
        EnJs_SetFreeCamera = actorAddr(0xbf, 0x809696ec);
        EnJs_SetFreeCamera(this, 0);
    }
    else
    {
        /* Too weak */
        EnJs_DisplayWeak(this, gPlay);
    }
}

PATCH_CALL(0x8096a534, EnJs_TryStartFight);
