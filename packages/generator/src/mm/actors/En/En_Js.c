#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>
#include <combo/actor.h>

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

static void EnJs_AskForFight(PlayState* play, u16 unk, Actor* this)
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
    if (Actor_HasParentZ(this))
    {
        gMmExtraFlags2.maskFierceDeity = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x8096a2fc, EnJs_HasGivenItem);

void EnJs_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnJs_ItemQuery(&q);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x8096a370, EnJs_GiveItem);

static void EnJs_DisplayHint(PlayState* play, s16 messageId)
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
    comboTextAppendItemNameQueryEx(&b, &q, TF_PREPOS | TF_PROGRESSIVE, gComboConfig.staticHintsImportance[9]);
    comboTextAppendStr(&b, "..." TEXT_BB "So...you'll play?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_CALL(0x8096a4c8, EnJs_DisplayHint);

static void EnJs_DisplayWeak(Actor* this, PlayState* play)
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
    if (Config_Flag(CFG_GOAL_TRIFORCE) || Config_Flag(CFG_GOAL_TRIFORCE3))
    {
        if (gOotExtraFlags.triforceWin)
            canFight = 1;
    }
    else if (!Config_Flag(CFG_MM_MAJORA_CHILD_CUSTOM) || SpecialConds_Eval(SPECIAL_MAJORA))
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

static s32 EnJs_GetRemainingMasksFixed(void)
{
    s32 count = 0;

    if (gSave.info.inventory.items[ITS_MM_MASK_TRUTH] == ITEM_MM_MASK_TRUTH)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_KAFEI] == ITEM_MM_MASK_KAFEI)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_ALL_NIGHT] == ITEM_MM_MASK_ALL_NIGHT)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_BUNNY] == ITEM_MM_MASK_BUNNY)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_KEATON] == ITEM_MM_MASK_KEATON)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_GARO] == ITEM_MM_MASK_GARO)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_ROMANI] == ITEM_MM_MASK_ROMANI)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_TROUPE_LEADER] == ITEM_MM_MASK_TROUPE_LEADER)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_POSTMAN] == ITEM_MM_MASK_POSTMAN)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_COUPLE] == ITEM_MM_MASK_COUPLE)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_GREAT_FAIRY] == ITEM_MM_MASK_GREAT_FAIRY)
        count++;

    if ((gSave.info.inventory.items[ITS_MM_MASK_GIBDO] == ITEM_MM_MASK_GIBDO) ||
        (gMmExtraItems.gibdoSpooky & (1 << 0)))
    {
        count++;
    }

    if (gSave.info.inventory.items[ITS_MM_MASK_DON_GERO] == ITEM_MM_MASK_DON_GERO)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_KAMARO] == ITEM_MM_MASK_KAMARO)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_CAPTAIN] == ITEM_MM_MASK_CAPTAIN)
        count++;

    if ((gSave.info.inventory.items[ITS_MM_MASK_STONE] == ITEM_MM_MASK_STONE) ||
        (gMmExtraItems.stoneGerudoSkull & (1 << 0)))
    {
        count++;
    }

    if (gSave.info.inventory.items[ITS_MM_MASK_BREMEN] == ITEM_MM_MASK_BREMEN)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_BLAST] == ITEM_MM_MASK_BLAST)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_SCENTS] == ITEM_MM_MASK_SCENTS)
        count++;

    if (gSave.info.inventory.items[ITS_MM_MASK_GIANT] == ITEM_MM_MASK_GIANT)
        count++;
    count -= ((s32 (*)(s32))actorAddr(0xbf, 0x80968e38))(0);

    return count;
}

PATCH_FUNC(0x80968f48, EnJs_GetRemainingMasksFixed);