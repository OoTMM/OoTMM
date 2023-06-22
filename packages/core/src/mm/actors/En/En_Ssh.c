#include <combo.h>
#include <combo/item.h>

static void EnSsh_ItemQuery(ComboItemQuery* q, GameState_Play* play, int flags)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NPC;
    q->ovFlags = flags;

    if (play->sceneId == SCE_MM_SPIDER_HOUSE_OCEAN)
    {
        q->gi = GI_MM_WALLET2;
        q->id = NPC_MM_SPIDER_HOUSE_OCEAN;
    }
    else
    {
        q->gi = GI_MM_MASK_TRUTH;
        q->id = NPC_MM_SPIDER_HOUSE_SWAMP;
    }
}

static void EnSsh_DisplayHint(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;
    char* b;
    char* start;

    EnSsh_ItemQuery(&q, play, 0);
    PlayerDisplayTextBox(play, 0x915, this);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "If you lift the curse... I'll give you... ");
    comboTextAppendItemNameQuery(&b, &q, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "... Hurry... Please..." TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_FUNC(0x809756d0, EnSsh_DisplayHint);
