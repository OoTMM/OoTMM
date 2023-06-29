#include <combo.h>
#include <combo/item.h>

static void EnGm_ItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(ComboItemQuery));

    q->ovType = OV_NPC;
    q->gi = GI_OOT_SWORD_KNIFE;
    q->id = NPC_OOT_MEDIGORON;
}

int EnGm_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        BITMAP16_SET(gSave.eventsMisc, EV_OOT_INF_MEDIGORON);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80aa0030, EnGm_HasGivenItem);

void EnGm_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnGm_ItemQuery(&q);
    comboGiveItem(actor, play, &q, a, b);
}

PATCH_CALL(0x80a9ffd8, EnGm_GiveItem);
PATCH_CALL(0x80aa006c, EnGm_GiveItem);

int EnGm_GetState(void)
{
    if (gSave.age == AGE_CHILD)
        return 0;

    if (BITMAP16_GET(gSave.eventsMisc, EV_OOT_INF_MEDIGORON))
        return 3;

    return 1;
}

PATCH_FUNC(0x80a9f8dc, EnGm_GetState);

static void hintMedigoron(GameState_Play* play)
{
    ComboItemQuery q;
    char* b;
    char* start;

    b = play->msgCtx.textBuffer;
    start = b;

    EnGm_ItemQuery(&q);
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "How about buying ");
    comboTextAppendItemNameQuery(&b, &q, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, " for 200 Rupees?" TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Buy" TEXT_NL "Don't buy" TEXT_END);
    comboTextAutoLineBreaks(start);
}

void EnGm_TalkedTo(Actor* this, GameState_Play* play)
{
    if (this->messageId == 0x304f)
        hintMedigoron(play);
}
