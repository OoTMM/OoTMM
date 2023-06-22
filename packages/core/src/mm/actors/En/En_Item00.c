#include <combo.h>
#include <combo/item.h>

static void EnItem00_ItemQuery(ComboItemQuery* q, Actor_EnItem00* this, GameState_Play* play, s16 gi, int flags)
{
    memset(q, 0, sizeof(*q));

    if (this->base.variable == 0x06)
    {
        q->ovType = OV_COLLECTIBLE;
        q->sceneId = play->sceneId;
        q->id = this->collectibleFlag;
        q->gi = gi;
        q->ovFlags = flags;
    }
    else
    {
        q->ovType = OV_NONE;
        q->gi = gi;
    }
}

void EnItem00_GiveItem(Actor_EnItem00* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Actor_Player* link;
    s16 itemId;

    EnItem00_ItemQuery(&q, this, play, gi, OVF_PROGRESSIVE | OVF_DOWNGRADE);
    link = GET_LINK(play);
    itemId = -1;

    if (q.ovType == OV_NONE)
    {
        itemId = kExtendedGetItems[gi - 1].itemId;
        if (GetItemCollectBehavior(itemId) == 0xff)
            itemId = -1;
    }

    if (itemId >= 0)
    {
        this->base.attachedA = &link->base;
        AddItemWithIcon(play, link, &kExtendedGetItems[gi - 1]);
        return;
    }

    comboGiveItem(&this->base, play, &q, a, b);
}

PATCH_CALL(0x800a700c, EnItem00_GiveItem);
PATCH_CALL(0x800a6a8c, EnItem00_GiveItem);

void EnItem00_DrawHeartPiece(Actor_EnItem00* this, GameState_Play* play)
{
    static const float scale = 12.5f;

    ComboItemQuery q;
    ComboItemOverride o;

    EnItem00_ItemQuery(&q, this, play, GI_OOT_HEART_PIECE, OVF_PROGRESSIVE);
    comboItemOverride(&o, &q);
    ModelViewScale(scale, scale, scale, MAT_MUL);
    comboDrawGI(play, &this->base, o.gi, 0);
}

PATCH_FUNC(0x800a75b8, EnItem00_DrawHeartPiece);
