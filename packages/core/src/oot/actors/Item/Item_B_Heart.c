#include <combo.h>
#include <combo/item.h>

static void ItemBHeart_ItemQuery(ComboItemQuery* q, Actor* this, GameState_Play* play, int flags)
{
    bzero(q, sizeof(ComboItemQuery));

    q->ovType = OV_COLLECTIBLE;
    q->ovFlags = flags;
    q->gi = GI_OOT_HEART_CONTAINER;
    q->sceneId = play->sceneId;
    q->id = 0x1f;
}

static void ItemBHeart_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    ItemBHeart_ItemQuery(&q, this, play, OVF_PROGRESSIVE | OVF_DOWNGRADE);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x80909518, ItemBHeart_GiveItem);

static void ItemBHeart_Draw(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;

    ItemBHeart_ItemQuery(&q, this, play, OVF_PROGRESSIVE);
    comboItemOverride(&o, &q);
    comboDrawGI(play, this, o.gi, 0);
}

PATCH_FUNC(0x80909620, ItemBHeart_Draw);
