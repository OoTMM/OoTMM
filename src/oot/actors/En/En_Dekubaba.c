#include <combo.h>

void EnDekubaba_GiveItemDefaultRange(Actor* this, GameState_Play* play, s16 gi)
{
    float dist;

    if (GetItemCollectBehavior(ITEM_OOT_STICK) == 0xff)
    {
        GiveItemDefaultRange(this, play, gi);
        return;
    }

    dist = this->xzDistanceFromLink;
    if (dist < 0.f)
        dist = -dist;
    if (dist < 50.f)
    {
        dist = this->position.y - GET_LINK(play)->base.position.y;
        if (dist < 0.f)
            dist = -dist;
        if (dist < 10.f)
        {
            this->attachedA = &GET_LINK(play)->base;
            PlaySound(0x4824);
            AddItem(play, ITEM_OOT_STICK);
        }
    }
}

PATCH_CALL(0x808fdde4, EnDekubaba_GiveItemDefaultRange);
PATCH_CALL(0x809baebc, EnDekubaba_GiveItemDefaultRange); /* En_Karebaba */
