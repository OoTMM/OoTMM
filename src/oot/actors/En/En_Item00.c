#include <combo.h>

int EnItem00_GiveItemDefaultRange(Actor_EnItem00* this, GameState_Play* play, s16 gi)
{
    s16 itemId;

    itemId = -1;
    switch (this->base.variable)
    {
    case 0x06:
    case 0x11:
        gi = comboOverride(OV_COLLECTIBLE, play->sceneId, this->collectibleFlag, gi);
        break;
    default:
        itemId = kExtendedGetItems[gi - 1].itemId;
        if (GetItemCollectBehavior(itemId) == 0xff)
            itemId = -1;
        break;
    }

    if (itemId >= 0)
    {
        this->base.attachedA = &LINK.base;
        PlaySound(0x4824);
        return AddItem(play, itemId);
    }
    else
    {
        return GiveItemDefaultRange(&this->base, play, gi);
    }
}

PATCH_CALL(0x80012174, EnItem00_GiveItemDefaultRange);
PATCH_CALL(0x80012e4c, EnItem00_GiveItemDefaultRange);

void EnItem00_DrawHeartPieceSmallKey(Actor_EnItem00* this, GameState_Play* play)
{
    s16 gi;
    float scale;

    switch (this->base.variable)
    {
    case 0x06:
        gi = GI_OOT_HEART_PIECE;
        scale = 12.5f;
        break;
    case 0x11:
        gi = GI_OOT_SMALL_KEY;
        scale = 8.3333f;
        break;
    default:
        UNREACHABLE();
    }
    gi = comboOverride(OV_COLLECTIBLE, play->sceneId, this->collectibleFlag, gi);
    ModelViewScale(scale, scale, scale, MAT_MUL);
    comboDrawGI(play, &this->base, gi, 0);
}

PATCH_FUNC(0x80013498, EnItem00_DrawHeartPieceSmallKey);
