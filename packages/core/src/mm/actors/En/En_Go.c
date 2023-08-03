#include <combo.h>
#include <combo/item.h>

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
        q->gi = (gMmSave.inventory.items[ITS_MM_KEG] == ITEM_MM_POWDER_KEG) ? GI_MM_POWDER_KEG : GI_MM_RECOVERY_HEART;
    }
}

void EnGo_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnGo_ItemQuery(&q);
    comboGiveItem(this, play, &q, a, b);
}

static void powderKegHint(GameState_Play* play)
{
    ComboItemQuery q;
    char* b;
    char* start;

    EnGo_ItemQuery(&q);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "If you pass my test, I will give you ");
    comboTextAppendItemNameQuery(&b, &q, TF_PREPOS | TF_PROGRESSIVE);
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
