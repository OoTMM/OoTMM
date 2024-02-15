#include <combo.h>

void EnDekubaba_GiveItemDefaultRange(Actor* this, GameState_Play* play, s16 gi)
{
    Actor_Player* link;
    float dist;

    link = GET_LINK(play);

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
        dist = this->world.pos.y - link->base.world.pos.y;
        if (dist < 0.f)
            dist = -dist;
        if (dist < 10.f)
        {
            this->parent = &link->base;
            AddItemWithIcon(play, link, &kExtendedGetItems[gi - 1]);
        }
    }
}

PATCH_CALL(0x808fdde4, EnDekubaba_GiveItemDefaultRange);
PATCH_CALL(0x809baebc, EnDekubaba_GiveItemDefaultRange); /* En_Karebaba */
