#include <combo.h>
#include <combo/item.h>

u8 EnSsh_skullMax2()
{
    return gComboData.skullCount;
};

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
    int importanceIndex;

    if (play->sceneId == SCE_MM_SPIDER_HOUSE_OCEAN)
        importanceIndex = 17;
    else
        importanceIndex = 16;

    EnSsh_ItemQuery(&q, play, 0);
    PlayerDisplayTextBox(play, 0x915, this);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "If you lift the curse... by collecting " TEXT_COLOR_RED);
    comboTextAppendNum(&b, EnSsh_skullMax2());
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " Skulltula tokens... I'll give you... ");
    comboTextAppendItemNameQueryEx(&b, &q, TF_PREPOS | TF_PROGRESSIVE, gComboData.staticHintsImportance[importanceIndex]);
    comboTextAppendStr(&b, "... Hurry... Please..." TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_FUNC(0x809756d0, EnSsh_DisplayHint);
