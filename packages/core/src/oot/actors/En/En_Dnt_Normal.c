#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x204) = (h); } while (0)

static void EnDntNormal_Noop(Actor* this, GameState_Play* play)
{
}

static void EnDntNormal_GiveReward(Actor* this, GameState_Play* play)
{
    Actor_Player* link;

    if (Actor_HasParent(this))
    {
        gSave.eventsItem[1] |= 0x2000;
        SET_HANDLER(this, EnDntNormal_Noop);
        return;
    }

    link = GET_LINK(play);
    link->state &= ~(0x00100008);
    comboGiveItemNpc(this, play, GI_OOT_SLINGSHOT, NPC_OOT_LOST_WOODS_TARGET, 16384.f, 16384.f);
}

PATCH_FUNC(0x80b4f7e4, EnDntNormal_GiveReward);
