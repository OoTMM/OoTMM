#include <combo.h>
#include <combo/item.h>

static void EnScopenuts_ItemQuery(ComboItemQuery* q, int flags)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NPC;
    q->gi = GI_MM_HEART_PIECE;
    q->id = NPC_MM_SCRUB_TELESCOPE;
    q->ovFlags = flags;
}

static void EnScopenuts_AlterMessage(GameState_Play* play)
{
    ComboItemQuery q;
    char* b;
    char* start;

    EnScopenuts_ItemQuery(&q, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "Please! I'll sell you ");
    comboTextAppendItemNameQuery(&b, &q, TF_PREPOS | TF_PROGRESSIVE);
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

void EnScopenuts_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnScopenuts_ItemQuery(&q, OVF_PROGRESSIVE | OVF_DOWNGRADE);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x80bcb968, EnScopenuts_GiveItem);
