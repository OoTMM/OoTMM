#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>

void EnGo_AfterGivingItem(Actor* actor)
{
    gMmExtraFlags.powderKeg = 1;
}

static void EnGo_ItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(*q));
    if (!gMmExtraFlags.powderKeg)
    {
        q->ovType = OV_NPC;
        q->gi = GI_MM_POWDER_KEG;
        q->id = NPC_MM_GORON_KEG;
    }
    else
    {
        q->ovType = OV_NONE;
        q->gi = (gMmSave.info.inventory.items[ITS_MM_KEG] == ITEM_MM_POWDER_KEG) ? GI_MM_POWDER_KEG : GI_MM_RECOVERY_HEART;
    }
}

void EnGo_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnGo_ItemQuery(&q);
    comboGiveItem(this, play, &q, a, b);
}

static void powderKegHint(PlayState* play)
{
    ComboItemQuery q;
    char* b;
    char* start;

    EnGo_ItemQuery(&q);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "If you pass my test, I will give you ");
    comboTextAppendItemNameQueryEx(&b, &q, TF_PREPOS | TF_PROGRESSIVE, gComboConfig.staticHintsImportance[13]);
    comboTextAppendStr(&b, "!" TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void strengthText(PlayState* play)
{
    if (!Config_Flag(CFG_MM_KEG_STRENGTH_3)) return;

    char* b;

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "I'm the Goron who sells " TEXT_COLOR_RED "Powder" TEXT_NL "Kegs");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, ", the most famous product of" TEXT_NL);
    comboTextAppendStr(&b, "the Gorons." TEXT_NL TEXT_BOX_BREAK_2);
    comboTextAppendStr(&b, "But the rules say I can't sell" TEXT_NL);
    comboTextAppendStr(&b, TEXT_COLOR_RED "Powder Kegs");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " to anyone who isn't" TEXT_NL);
    comboTextAppendStr(&b, TEXT_COLOR_RED "strong enough");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " to carry them. " TEXT_SFX("\x39\x1c") "Sorry." TEXT_END);
}

static void strengthTextRepeat(PlayState* play)
{
    if (!Config_Flag(CFG_MM_KEG_STRENGTH_3)) return;

    char* b;

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "The rules say I can't sell " TEXT_COLOR_RED "Powder" TEXT_NL);
    comboTextAppendStr(&b, "Kegs");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " to anyone who isn't " TEXT_COLOR_RED "strong" TEXT_NL);
    comboTextAppendStr(&b, "enough");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " to carry them. " TEXT_SFX("\x39\x1c") "Sorry." TEXT_END);
}

void EnGo_AfterTextBox(Actor* this, PlayState* play, s16 messageId)
{
    switch (messageId)
    {
    case 0x0c81:
        powderKegHint(play);
        break;
    case 0xc8e:
        strengthText(play);
        break;
    case 0xc8f:
        strengthTextRepeat(play);
        break;
    }
}
