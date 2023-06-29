#include <combo.h>
#include <combo/item.h>

static void EnColMan_ItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NPC;
    q->gi = GI_MM_HEART_PIECE;
    q->id = NPC_MM_LAB_FISH_HEART_PIECE;
}

static void EnColMan_ItemOverride(ComboItemOverride* o)
{
    ComboItemQuery q;

    EnColMan_ItemQuery(&q);
    comboItemOverride(o, &q);
}

void EnColMan_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnColMan_ItemQuery(&q);
    comboGiveItem(actor, play, &q, a, b);
}

PATCH_CALL(0x80afdecc, EnColMan_GiveItem);

void EnColMan_DrawHeartPiece(Actor* this, GameState_Play* play)
{
    static const float scale = 12.5f;
    ComboItemOverride o;

    EnColMan_ItemOverride(&o);
    ModelViewScale(scale, scale, scale, MAT_MUL);
    comboDrawGI(play, this, o.gi, 0);
}

PATCH_FUNC(0x80afe414, EnColMan_DrawHeartPiece);
