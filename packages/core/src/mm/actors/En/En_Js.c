#include <combo.h>
#include <combo/item.h>

static void EnJs_ItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(*q));

    if (!gMmExtraFlags2.maskFierceDeity)
    {
        q->ovType = OV_NPC;
        q->gi = GI_MM_MASK_FIERCE_DEITY;
        q->id = NPC_MM_MASK_FIERCE_DEITY;
    }
    else
    {
        q->ovType = OV_NONE;
        q->gi = GI_MM_RECOVERY_HEART;
    }
}

static void EnJs_AskForFight(GameState_Play* play, u16 unk, Actor* this)
{
    char* b;
    char* start;

    PlayerDisplayTextBox(play, 0x21fe, this);
    b = play->msgCtx.font.textBuffer.schar;
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
    ComboItemQuery q;

    EnJs_ItemQuery(&q);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x8096a370, EnJs_GiveItem);

static void EnJs_DisplayHint(GameState_Play* play, s16 messageId)
{
    ComboItemQuery q;
    char* b;
    char* start;

    /* Hint */
    EnJs_ItemQuery(&q);
    DisplayTextBox2(play, messageId);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "You have only weak masks..." TEXT_NL "Having better masks would give you ");
    comboTextAppendItemNameQuery(&b, &q, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "..." TEXT_BB "So...you'll play?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_CALL(0x8096a4c8, EnJs_DisplayHint);

static void EnJs_DisplayWeak(Actor* this, GameState_Play* play)
{
    char* b;

    DisplayTextBox2(play, 0x21ff);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "You're too weak..." TEXT_SIGNAL TEXT_END);
}

void EnJs_TryStartFight(Actor* this)
{
    int canFight;
    void (*EnJs_SetFreeCamera)(Actor*, int);

    canFight = 0;
    if (comboConfig(CFG_GOAL_TRIFORCE) || comboConfig(CFG_GOAL_TRIFORCE3))
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
