#include <combo.h>
#include <combo/item.h>

static void ItemBHeart_ItemQuery(ComboItemQuery* q, GameState_Play* play, int flags)
{
    bzero(q, sizeof(ComboItemQuery));

    q->ovType = OV_COLLECTIBLE;
    q->ovFlags = flags;
    q->gi = GI_MM_HEART_CONTAINER;
    q->sceneId = play->sceneId;
    q->id = 0x1f;
}

void ItemBHeart_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    ItemBHeart_ItemQuery(&q, play, OVF_PROGRESSIVE | OVF_DOWNGRADE);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x808bcf38, ItemBHeart_GiveItem);

void ItemBHeart_Draw(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;

    ItemBHeart_ItemQuery(&q, play, OVF_PROGRESSIVE);
    comboItemOverride(&o, &q);
    comboDrawGI(play, this, o.gi, DRAW_RAW);
}

PATCH_FUNC(0x808bcfc4, ItemBHeart_Draw);
