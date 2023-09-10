#include <combo.h>
#include <combo/item.h>

int EnZot_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParent(this);
    if (ret)
        gMmExtraFlags.zoraHallLights = 1;
    return ret;
}

PATCH_CALL(0x80b98f40, EnZot_HasGivenItem);

void EnZot_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (!gMmExtraFlags.zoraHallLights)
    {
        npc = NPC_MM_ZORA_HALL_LIGHTS;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80b98f7c, EnZot_GiveItem);

static void EnZot_BeaverItemQuery(ComboItemQuery* q, int index, int flags)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NPC;
    q->gi = index == 0 ? GI_MM_BOTTLE_EMPTY : GI_MM_HEART_PIECE;
    q->id = index == 0 ? NPC_MM_BEAVER_1 : NPC_MM_BEAVER_2;
    q->ovFlags = flags;
}

void EnZot_DisplayBeaverHint(Actor* this, GameState_Play* play)
{
    ComboItemQuery q1;
    ComboItemQuery q2;
    char* b;
    char* start;

    EnZot_BeaverItemQuery(&q1, 0, 0);
    EnZot_BeaverItemQuery(&q2, 1, 0);
    PlayerDisplayTextBox(play, 0x1260, this);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "I've heared that the beavers are betting ");
    comboTextAppendItemNameQuery(&b, &q1, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, " and ");
    comboTextAppendItemNameQuery(&b, &q2, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "." TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

PATCH_FUNC(0x80b97110, EnZot_DisplayBeaverHint);
