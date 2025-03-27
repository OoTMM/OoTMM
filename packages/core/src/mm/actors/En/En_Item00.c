#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/draw.h>

static void EnItem00_ItemQuery(ComboItemQuery* q, Actor_EnItem00* this, PlayState* play, s16 gi)
{
    memset(q, 0, sizeof(*q));

    if (this->base.params == 0x06)
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

void EnItem00_GiveItem(Actor_EnItem00* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Player* link;
    s16 itemId;

    EnItem00_ItemQuery(&q, this, play, gi);
    link = GET_PLAYER(play);
    itemId = -1;

    if (q.ovType == OV_NONE)
    {
        itemId = kExtendedGetItems[gi - 1].itemId;
        if (GetItemCollectBehavior(itemId) == 0xff)
            itemId = -1;

        if (gi == GI_MM_SHIELD_HERO && gSharedCustomSave.mmShieldIsDeku)
            q.gi = GI_MM_SHIELD_DEKU;
    }

    if (itemId >= 0)
    {
        this->base.parent = &link->actor;
        AddItemWithIcon(play, link, &kExtendedGetItems[gi - 1]);
        return;
    }

    comboGiveItem(&this->base, play, &q, a, b);
}

PATCH_CALL(0x800a700c, EnItem00_GiveItem);
PATCH_CALL(0x800a6a8c, EnItem00_GiveItem);

void EnItem00_DrawHeartPiece(Actor_EnItem00* this, PlayState* play)
{
    static const float scale = 17.5f;

    ComboItemQuery q;
    ComboItemOverride o;

    EnItem00_ItemQuery(&q, this, play, GI_OOT_HEART_PIECE);
    comboItemOverride(&o, &q);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Draw_GiCloaked(play, &this->base, o.gi, o.cloakGi, 0);
}

PATCH_FUNC(0x800a75b8, EnItem00_DrawHeartPiece);

static int dropBombs(int dropId)
{
    int hasChuBag;
    int hasBombBag;
    u8  bombCount;
    u8  bombchuCount;

    hasChuBag = (gSharedCustomSave.bombchuBagMm > 0);
    hasBombBag = (gMmSave.info.inventory.upgrades.bombBag > 0);

    if (!hasChuBag)
    {
        if (!hasBombBag)
            return -1;
        return dropId;
    }

    if (!hasBombBag)
        return ITEM00_BOMBCHU;

    /* We have both, check for ammo */
    bombCount = gMmSave.info.inventory.ammo[ITS_MM_BOMBS];
    bombchuCount = gMmSave.info.inventory.ammo[ITS_MM_BOMBCHU];

    /* Low on ammo */
    if (bombCount < 15 || bombchuCount < 15)
    {
        if (bombchuCount < bombCount)
            return ITEM00_BOMBCHU;
        return dropId;
    }

    /* Not low, return at random */
    if (Rand_ZeroOne() < 0.5f)
        return ITEM00_BOMBCHU;
    return dropId;
}

/* TODO: Flexible drops would ideally need to be patched on top of this */
int EnItem00_FixDrop(int dropId)
{
    switch (dropId)
    {
    case ITEM00_ARROWS_10:
    case ITEM00_ARROWS_30:
    case ITEM00_ARROWS_40:
    case ITEM00_ARROWS_50:
        if (gSave.info.inventory.upgrades.quiver == 0)
            dropId = -1;
        break;
    case ITEM00_BOMB:
    case ITEM00_BOMBS_5:
        dropId = dropBombs(dropId);
        break;
    case ITEM00_RECOVERY_HEART:
        if (gSave.info.playerData.health >= gSave.info.playerData.healthCapacity)
            dropId = ITEM00_RUPEE_GREEN;
        break;
    case ITEM00_MAGIC_SMALL:
    case ITEM00_MAGIC_LARGE:
        if (!gSave.info.playerData.isMagicAcquired)
            dropId = -1;
        break;
    default:
        break;
    }

    return dropId;
}

PATCH_CALL(0x800a7994, EnItem00_FixDrop);
PATCH_CALL(0x800a7c44, EnItem00_FixDrop);
PATCH_CALL(0x800a8024, EnItem00_FixDrop);

void EnItem00_AliasFreestandingRupee(Xflag* xflag)
{
    switch (xflag->sceneId)
    {
    case SCE_MM_WOODFALL:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            xflag->id -= 37;
        }
        break;
    case SCE_MM_LAUNDRY_POOL:
        switch (xflag->id)
        {
        case 0x14:
        case 0x15:
            xflag->id = 0x13;
            break;
        case 0x17:
        case 0x18:
            xflag->id = 0x16;
            break;
        case 0x1a:
        case 0x1b:
            xflag->id = 0x19;
            break;
        }
        break;
    case SCE_MM_TWIN_ISLANDS_WINTER:
        xflag->sceneId = SCE_MM_TWIN_ISLANDS_SPRING;
        xflag->id -= 18;
        break;
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        if (xflag->roomId == 0x03)
        {
            switch (xflag->id)
            {
            case 0: xflag->roomId = 0x01; xflag->id = 0; break;
            case 3: xflag->roomId = 0x01; xflag->id = 1; break;
            case 4: xflag->roomId = 0x01; xflag->id = 2; break;
            }
        }
        break;
    }
}

void EnItem00_AliasFreestandingHeart(Xflag* xflag)
{
}

void EnItem00_DrawShield(PlayState* play)
{
    s16 gi;

    if (gSharedCustomSave.mmShieldIsDeku)
        gi = GI_MM_SHIELD_DEKU;
    else
        gi = GI_MM_SHIELD_HERO;


    Draw_Gi(play, NULL, gi, DRAW_RAW);
}

PATCH_CALL(0x800a726c, EnItem00_DrawShield);
