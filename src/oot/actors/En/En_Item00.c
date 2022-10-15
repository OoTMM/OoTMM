#include <combo.h>

int EnItem00_GiveItemDefaultRange(Actor_EnItem00* this, GameState_Play* play, s16 gi)
{
    switch (this->base.variable)
    {
    case 0x6:
        gi = comboOverride(OV_COLLECTIBLE, play->sceneId, this->collectibleFlag, gi);
        break;
    }

    return GiveItemDefaultRange(&this->base, play, gi);
}

PATCH_CALL(0x80012174, EnItem00_GiveItemDefaultRange);
PATCH_CALL(0x80012e4c, EnItem00_GiveItemDefaultRange);

void EnItem00_DrawHeartPiece(Actor_EnItem00* this, GameState_Play* play)
{
    s16 gi;

    gi = comboOverride(OV_COLLECTIBLE, play->sceneId, this->collectibleFlag, GI_OOT_HEART_PIECE);
    comboDrawGI(play, &this->base, gi, 12.5f);
}

PATCH_FUNC(0x80013498, EnItem00_DrawHeartPiece);
