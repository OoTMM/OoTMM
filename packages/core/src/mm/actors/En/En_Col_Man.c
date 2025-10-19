#include <combo.h>
#include <combo/item.h>
#include <combo/draw.h>

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

void EnColMan_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnColMan_ItemQuery(&q);
    comboGiveItem(actor, play, &q, a, b);
}

PATCH_CALL(0x80afdecc, EnColMan_GiveItem);

void EnColMan_DrawHeartPiece(Actor* this, PlayState* play)
{
    static const float scale = 35.f;
    ComboItemOverride o;
    s16 gi;

    EnColMan_ItemOverride(&o);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        Matrix_RotateYS(-this->shape.rot.y * 2, MTXMODE_APPLY);
    }
    Draw_Gi(play, this, gi, 0);
}

PATCH_FUNC(0x80afe414, EnColMan_DrawHeartPiece);
