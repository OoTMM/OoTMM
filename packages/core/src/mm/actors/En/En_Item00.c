#include <combo.h>
#include <combo/item.h>

static void EnItem00_ItemQuery(ComboItemQuery* q, Actor_EnItem00* this, GameState_Play* play, s16 gi)
{
    memset(q, 0, sizeof(*q));

    if (this->base.variable == 0x06)
    {
        q->ovType = OV_COLLECTIBLE;
        q->sceneId = comboSceneKey(play->sceneId);
        q->id = this->collectibleFlag;
        q->gi = gi;
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

    EnItem00_ItemQuery(&q, this, play, gi);
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

    EnItem00_ItemQuery(&q, this, play, GI_OOT_HEART_PIECE);
    comboItemOverride(&o, &q);
    ModelViewScale(scale, scale, scale, MAT_MUL);
    comboDrawGI(play, &this->base, o.gi, 0);
}

PATCH_FUNC(0x800a75b8, EnItem00_DrawHeartPiece);

static s16 bombDrop(s16 dropId)
{
    int hasChuBag;
    int hasBombBag;
    u8  bombCount;
    u8  bombchuCount;

    if (!comboConfig(CFG_MM_BOMBCHU_BAG))
        return dropId;

    hasChuBag = (gMmSave.inventory.items[ITS_MM_BOMBCHU] == ITEM_MM_BOMBCHU);
    hasBombBag = (gMmSave.inventory.upgrades.bombBag > 0);

    if (!hasChuBag)
    {
        if (!hasBombBag)
            return -1;
        return dropId;
    }

    if (!hasBombBag)
        return ITEM00_BOMBCHU;

    /* We have both, check for ammo */
    bombCount = gMmSave.inventory.ammo[ITS_MM_BOMBS];
    bombchuCount = gMmSave.inventory.ammo[ITS_MM_BOMBCHU];

    /* Low on ammo */
    if (bombCount < 15 || bombchuCount < 15)
    {
        if (bombchuCount < bombCount)
            return ITEM00_BOMBCHU;
        return dropId;
    }

    /* Not low, return at random */
    if (RandFloat() < 0.5f)
        return ITEM00_BOMBCHU;
    return dropId;
}

/* TODO: Flexible drops would ideally need to be patched on top of this */
static s16 EnItem00_FixDropWrapper(s16 dropId)
{
    switch (dropId)
    {
    case ITEM00_BOMB:
    case ITEM00_BOMBS_5:
        dropId = bombDrop(dropId);
        break;
    default:
        break;
    }

    if (dropId == ITEM00_BOMBCHU)
        return dropId;

    return EnItem00_FixDrop(dropId);
}

PATCH_CALL(0x800a7994, EnItem00_FixDropWrapper);
PATCH_CALL(0x800a7c44, EnItem00_FixDropWrapper);
PATCH_CALL(0x800a8024, EnItem00_FixDropWrapper);

void EnItem00_AliasFreestandingRupee(Xflag* xflag)
{

}
