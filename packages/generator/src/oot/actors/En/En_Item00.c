#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/draw.h>

static void EnItem00_ItemQuery(ComboItemQuery* q, Actor_EnItem00* this, PlayState* play, s16 gi)
{
    memset(q, 0, sizeof(*q));

    switch (this->actor.params)
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

void EnItem00_GiveItemDefaultRange(Actor_EnItem00* this, PlayState* play, s16 gi)
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
    }

    if (itemId >= 0)
    {
        this->actor.parent = &link->actor;
        AddItemWithIcon(play, link, &kExtendedGetItems[gi - 1]);
        return;
    }

     comboGiveItem(&this->actor, play, &q, 50.f, 10.f);
}

PATCH_CALL(0x80012e4c, EnItem00_GiveItemDefaultRange);

void EnItem00_DrawHeartPieceSmallKey(Actor_EnItem00* this, PlayState* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
    float scale;
    s16 gi;

    switch (this->actor.params)
    {
    case 0x06:
        scale = 17.5f;
        break;
    case 0x11:
        scale = 11.666f;
        break;
    default:
        UNREACHABLE();
    }

    EnItem00_ItemQuery(&q, this, play, -1);
    comboItemOverride(&o, &q);
    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        Matrix_RotateYS(-this->actor.shape.rot.y * 2, MTXMODE_APPLY);
    }
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Draw_Gi(play, &this->actor, gi, 0);
}

PATCH_FUNC(0x80013498, EnItem00_DrawHeartPieceSmallKey);

static int fixDropBomb(int dropId)
{
    int hasChuBag;
    int hasBombBag;
    int hasBombBagMm;
    u8  bombCount;
    u8  bombCountMm;
    u8  bombchuCount;

    hasChuBag = (gSharedCustomSave.bombchuBagOot > 0);
    hasBombBag = (gSave.info.inventory.upgrades.bombBag > 0);
    hasBombBagMm = (gOotExtraItems.bombSlot & 4);

    if (!hasChuBag && !hasBombBagMm)
    {
        if (!hasBombBag)
            return -1;
        return dropId;
    }

    if (!hasBombBag && !hasBombBagMm)
        return ITEM00_BOMBCHU;

    if (!hasBombBag && !hasChuBag)
        return ITEM00_BOMBS_5_ALT;

    /* We have at least two, check for ammo */
    bombCount = hasBombBag ? gSave.info.inventory.ammo[ITS_OOT_BOMBS] : 15;
    bombchuCount = hasChuBag ? gSave.info.inventory.ammo[ITS_OOT_BOMBCHU] : 15;
    bombCountMm = hasBombBagMm ? gOotExtraAmmo.mmBombAmmo : 15;

    /* Low on ammo */
    if (bombCount < 15 || bombchuCount < 15 || bombCountMm < 15)
    {
        if (bombchuCount < bombCount && bombchuCount < bombCountMm)
            return ITEM00_BOMBCHU;
        if (bombCountMm < bombCount && bombCountMm < bombchuCount)
            return ITEM00_BOMBS_5_ALT;
        return dropId;
    }

    /* Not low, return at random */

    if (!hasBombBag)
    {
        if (Rand_ZeroOne() < 0.5f)
            return ITEM00_BOMBCHU;
        return ITEM00_BOMBS_5_ALT;
    }

    if (!hasChuBag)
    {
        if (Rand_ZeroOne() < 0.5f)
            return ITEM00_BOMBS_5_ALT;
        return dropId;
    }

    if (!hasBombBagMm)
    {
        if (Rand_ZeroOne() < 0.5f)
            return ITEM00_BOMBCHU;
        return dropId;
    }

    f32 rand = Rand_ZeroFloat(3.0f);

    if (rand < 1.0f)
        return ITEM00_BOMBCHU;
    if (rand < 2.0f)
        return ITEM00_BOMBS_5_ALT;
    return dropId;
}

static int fixDropArrowSeeds(int size)
{
    static const s8 kDrops[] = { ITEM00_ARROWS_5, ITEM00_ARROWS_10, ITEM00_ARROWS_30, ITEM00_SEEDS, ITEM00_SEEDS, ITEM00_SEEDS };

    int isAllowedArrows;
    int isAllowedSeeds;
    int index;

    isAllowedArrows = TRUE;
    isAllowedSeeds = TRUE;

    if ((gSave.age == AGE_CHILD && !Config_Flag(CFG_OOT_AGELESS_BOW)) || gSave.info.inventory.upgrades.quiver == 0)
        isAllowedArrows = FALSE;
    if ((gSave.age == AGE_ADULT && !Config_Flag(CFG_OOT_AGELESS_SLINGSHOT)) || gSave.info.inventory.upgrades.bulletBag == 0)
        isAllowedSeeds = FALSE;

    if (!isAllowedArrows && !isAllowedSeeds)
        return -1;

    if (!isAllowedSeeds)
        index = 0;
    else if (!isAllowedArrows)
        index = 1;
    else
        index = (Rand_ZeroOne() < 0.5f) ? 0 : 1;

    return kDrops[index * 3 + size];
}

/* TODO: Flexible drops would ideally need to be patched on top of this */
int EnItem00_FixDrop(int dropId)
{
    switch (dropId)
    {
    case ITEM00_STICK:
        if (gSave.age == AGE_ADULT && !Config_Flag(CFG_OOT_AGELESS_STICKS))
            dropId = ITEM00_RUPEE_GREEN;
        break;
    case ITEM00_BOMB:
    case ITEM00_BOMBS_5:
    case ITEM00_BOMBS_5_ALT:
        dropId = fixDropBomb(dropId);
        break;
    case ITEM00_ARROWS_5: dropId = fixDropArrowSeeds(0); break;
    case ITEM00_ARROWS_10: dropId = fixDropArrowSeeds(1); break;
    case ITEM00_ARROWS_30: dropId = fixDropArrowSeeds(2); break;
    case ITEM00_SEEDS: dropId = fixDropArrowSeeds(0); break;
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

PATCH_CALL(0x8001376c, EnItem00_FixDrop);
PATCH_CALL(0x80013998, EnItem00_FixDrop);
PATCH_CALL(0x80013dec, EnItem00_FixDrop);

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
    }
}

void EnItem00_AliasFreestandingHeart(Xflag* xflag)
{
}

void EnItem00_DrawShieldDeku(PlayState* play)
{
    Draw_Gi(play, NULL, GI_OOT_SHIELD_DEKU, DRAW_RAW);
}

PATCH_CALL(0x800130fc, EnItem00_DrawShieldDeku);

void EnItem00_DrawShieldHylian(PlayState* play)
{
    Draw_Gi(play, NULL, GI_OOT_SHIELD_HYLIAN, DRAW_RAW);
}

PATCH_CALL(0x80013110, EnItem00_DrawShieldHylian);

void EnItem00_GiveItem(Actor_EnItem00* this, PlayState* play)
{
    comboAddItemRaw(play, GI_OOT_BOMBS_MM_5);
}
