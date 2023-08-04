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

void EnItem00_InitWrapper(Actor_EnItem00* this, GameState_Play* play)
{
    this->isExtended = g.spawnExtended;
    EnItem00_Init(this, play);

    /* Disable blinking */
    if (this->isExtended)
        *(s16*)((char*)this + 0x148) = 0;
}

static int EnItem00_GetCollectibleFlag(GameState_Play* play, u32 flag)
{
    if (g.spawnExtended)
        return 0;

    return GetCollectibleFlag(play, flag);
}

PATCH_CALL(0x80011ba0, EnItem00_GetCollectibleFlag);

void EnItem00_SetExtendedFlag(Actor_EnItem00* this)
{
    if (this->isExtended)
    {
        /* TODO: Mark the potsanity item as collected */
    }
    else
        SetCollectibleFlag(gPlay, this->collectibleFlag);
}

static void EnItem00_ExtendedItemQuery(ComboItemQuery* q, Actor_EnItem00* this)
{
    bzero(q, sizeof(*q));
    q->ovType = OV_EXTENDED;
    q->sceneId = this->xflag.sceneId;
    q->roomId = (this->xflag.roomId | ((this->xflag.setupId & 3) << 6));
    q->id = this->xflag.id;
    q->gi = this->extendedGi;
}

static void EnItem00_DrawExtended(Actor_EnItem00* this, GameState_Play* play)
{
    static const float scale = 12.5f;

    if (this->extendedGiDraw == 0)
        return;
    ModelViewScale(scale, scale, scale, MAT_MUL);
    comboDrawGI(play, &this->base, this->extendedGiDraw, 0);
}

static void EnItem00_DrawExtendedOrRupee(Actor_EnItem00* item, GameState_Play* play)
{
    if (item->isExtended)
        EnItem00_DrawExtended(item, play);
    else
        EnItem00_DrawRupee(item, play);
}

PATCH_CALL(0x80013004, EnItem00_DrawExtendedOrRupee);

void EnItem00_GiveExtendedOrRupee(GameState_Play* play, Actor_EnItem00* this)
{
    ComboItemQuery q;
    ComboItemOverride o;
    int major;

    /* Check for an actual extended item */
    if (!this->isExtended)
    {
        AddItem(play, 0x84);
        return;
    }

    /* Query the item */
    EnItem00_ExtendedItemQuery(&q, this);
    comboItemOverride(&o, &q);
    major = !isItemFastBuy(o.gi);
    if (major)
    {
        PlayerDisplayTextBox(play, 0xb4, NULL);
        FreezePlayer(play);
        this->extendedMajor = 1;
    }
    else
    {
        this->extendedMajor = 0;
    }
    comboAddItemEx(play, &q, major);
    comboXflagsSet(&this->xflag);
}

void EnItem00_PlayExtendedCollectedSound(Actor_EnItem00* this)
{
    /* Check for an actual extended item */
    if (!this->isExtended)
    {
        PlaySound(0x4803);
        return;
    }

    /* Play the regular get item sound */
    PlaySound(0x4824);
}

void EnItem00_ExtendedCollectedHandler(Actor_EnItem00* this, GameState_Play* play)
{
    /* Check for an extended item */
    if (!this->isExtended)
    {
        EnItem00_SetHandler(this, EnItem00_CollectedHandler);
        EnItem00_CollectedHandler(this, play);
        return;
    }

    /* Invoke the default handler */
    EnItem00_CollectedHandler(this, play);

    /* Check for the message box */
    if (!this->extendedMajor || Message_IsClosed(&this->base, play))
    {
        UnfreezePlayer(play);
        EnItem00_SetHandler(this, EnItem00_CollectedHandler);
    }
    else
    {
        FreezePlayer(play);
    }
}

void EnItem00_UpdateWrapper(Actor_EnItem00* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;

    if (this->isExtended)
    {
        if (*(u32*)((char*)this + 0x13c) != 0x800127e0)
        {
            if (*(void**)((char*)this + 0x13c) != EnItem00_ExtendedCollectedHandler)
            {
                EnItem00_ExtendedItemQuery(&q, this);
                comboItemOverride(&o, &q);
                this->extendedGiDraw = o.gi;
            }
            *(s16*)((char*)this + 0x14a) += 1;
        }
    }
    EnItem00_Update(this, play);
}
