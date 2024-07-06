#include <combo.h>
#include <combo/player.h>

void EnDekubaba_GiveItemDefaultRange(Actor* this, GameState_Play* play, s16 gi)
{
    Actor_Player* link;
    float dist;

    link = GET_PLAYER(play);
    if (GetItemCollectBehavior(ITEM_MM_STICK) == 0xff)
    {
        GiveItemDefaultRange(this, play, gi);
        return;
    }

    dist = this->xzDistToPlayer;
    if (dist < 0.f)
        dist = -dist;
    if (dist < 50.f)
    {
        dist = this->world.pos.y - link->actor.world.pos.y;
        if (dist < 0.f)
            dist = -dist;
        if (dist < 10.f)
        {
            this->parent = &link->actor;
            AddItemWithIcon(play, link, &kExtendedGetItems[gi - 1]);
        }
    }
}

PATCH_CALL(0x808b3f38, EnDekubaba_GiveItemDefaultRange);
PATCH_CALL(0x808f21f4, EnDekubaba_GiveItemDefaultRange); /* En_Karebaba */
