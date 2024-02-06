#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

static void EnItem00_ItemQuery(ComboItemQuery* q, Actor_EnItem00* this, GameState_Play* play, s16 gi)
{
    memset(q, 0, sizeof(*q));

    switch (this->base.variable)
    {
    case 0x06:
        q->ovType = OV_COLLECTIBLE;
        q->sceneId = play->sceneId;
        q->id = this->collectibleFlag;
        q->gi = GI_OOT_HEART_PIECE;
        break;
    case 0x11:
        q->ovType = OV_COLLECTIBLE;
        q->sceneId = play->sceneId;
        q->id = this->collectibleFlag;
        q->gi = GI_OOT_SMALL_KEY;
        break;
    default:
        q->ovType = OV_NONE;
        q->gi = gi;
        break;
    }
}

void EnItem00_GiveItemDefaultRange(Actor_EnItem00* this, GameState_Play* play, s16 gi)
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

     comboGiveItem(&this->base, play, &q, 50.f, 10.f);
}

PATCH_CALL(0x80012e4c, EnItem00_GiveItemDefaultRange);

void EnItem00_DrawHeartPieceSmallKey(Actor_EnItem00* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
    float scale;

    switch (this->base.variable)
    {
    case 0x06:
        scale = 12.5f;
        break;
    case 0x11:
        scale = 8.3333f;
        break;
    default:
        UNREACHABLE();
    }

    EnItem00_ItemQuery(&q, this, play, -1);
    comboItemOverride(&o, &q);
    ModelViewScale(scale, scale, scale, MAT_MUL);
    comboDrawGI(play, &this->base, o.gi, 0);
}

PATCH_FUNC(0x80013498, EnItem00_DrawHeartPieceSmallKey);

static s16 bombDrop(s16 dropId)
{
    int hasChuBag;
    int hasBombBag;
    u8  bombCount;
    u8  bombchuCount;

    if (!comboConfig(CFG_OOT_BOMBCHU_BAG))
        return dropId;

    hasChuBag = (gOotSave.inventory.items[ITS_OOT_BOMBCHU] == ITEM_OOT_BOMBCHU_10);
    hasBombBag = (gOotSave.inventory.upgrades.bombBag > 0);

    if (!hasChuBag)
    {
        if (!hasBombBag)
            return -1;
        return dropId;
    }

    if (!hasBombBag)
        return ITEM00_BOMBCHU;

    /* We have both, check for ammo */
    bombCount = gOotSave.inventory.ammo[ITS_OOT_BOMBS];
    bombchuCount = gOotSave.inventory.ammo[ITS_OOT_BOMBCHU];

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
    case ITEM00_BOMBS_5_ALT:
        dropId = bombDrop(dropId);
        break;
    default:
        break;
    }

    if (dropId == ITEM00_BOMBCHU)
        return dropId;

    return EnItem00_FixDrop(dropId);
}

PATCH_CALL(0x8001376c, EnItem00_FixDropWrapper);
PATCH_CALL(0x80013998, EnItem00_FixDropWrapper);
PATCH_CALL(0x80013dec, EnItem00_FixDropWrapper);

void EnItem00_AliasFreestandingRupee(Xflag* xflag)
{
    switch (xflag->sceneId)
    {
    case SCE_OOT_LOST_WOODS:
        if (xflag->roomId == 7)
        {
            xflag->setupId = 0;
            xflag->id = 4;
        }
        break;
    case SCE_OOT_KOKIRI_FOREST:
        if (xflag->setupId == 3)
        {
            xflag->setupId = 2;
            xflag->id += 2;
        }
        break;
    }
}

void EnItem00_AliasFreestandingHeart(Xflag* xflag)
{
}
