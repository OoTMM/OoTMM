#include <combo.h>
#include <combo/item.h>
#include <combo/draw.h>

static void ItemBHeart_ItemQuery(ComboItemQuery* q, Actor* this, PlayState* play)
{
    bzero(q, sizeof(ComboItemQuery));

    q->ovType = OV_COLLECTIBLE;
    q->gi = GI_OOT_HEART_CONTAINER;
    q->sceneId = play->sceneId;
    q->id = 0x1f;
}

static void ItemBHeart_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    ItemBHeart_ItemQuery(&q, this, play);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x80909518, ItemBHeart_GiveItem);

static void ItemBHeart_Draw(Actor* this, PlayState* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
    s16 gi;

    ItemBHeart_ItemQuery(&q, this, play);
    comboItemOverride(&o, &q);
    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        Matrix_RotateYS(-this->shape.rot.y * 2, MTXMODE_APPLY);
    }
    Draw_Gi(play, this, gi, 0);
}

PATCH_FUNC(0x80909620, ItemBHeart_Draw);
