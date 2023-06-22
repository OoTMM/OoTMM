#include <combo.h>
#include <combo/item.h>

static u8 sIsFirstReward;

int EnFu_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParent(this);
    if (ret && sIsFirstReward)
    {
        gMmExtraFlags2.honeyDarling = 1;
        sIsFirstReward = 0;
    }
    return ret;
}

PATCH_CALL(0x80963574, EnFu_HasGivenItem);

void EnFu_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_MM_RUPEE_PURPLE:
        if (!gMmExtraFlags2.honeyDarling)
        {
            npc = NPC_MM_HONEY_DARLING_1;
            sIsFirstReward = 1;
        }
        break;
    case GI_MM_HEART_PIECE:
        npc = NPC_MM_HONEY_DARLING_2;
        break;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x809635cc, EnFu_GiveItem);
PATCH_CALL(0x809635ec, EnFu_GiveItem);
