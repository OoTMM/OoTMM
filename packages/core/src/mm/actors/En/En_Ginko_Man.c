#include <combo.h>
#include <combo/item.h>

static u8 sIsReward2;

int EnGinkoMan_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParent(this);
    if (ret)
    {
        if (sIsReward2)
        {
            gMmExtraFlags.bankReward2 = 1;
            sIsReward2 = 0;
        }
    }
    return ret;
}

PATCH_CALL(0x80a6559c, EnGinkoMan_HasGivenItem);

void EnGinkoMan_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
        npc = NPC_MM_BANK_1;
        break;
    case GI_MM_RUPEE_BLUE:
        if (!gMmExtraFlags.bankReward2)
        {
            npc = NPC_MM_BANK_2;
            sIsReward2 = 1;
        }
        break;
    case GI_MM_HEART_PIECE:
        npc = NPC_MM_BANK_3;
        break;
    }

    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80a65618, EnGinkoMan_GiveItem);
PATCH_CALL(0x80a65658, EnGinkoMan_GiveItem);
PATCH_CALL(0x80a65694, EnGinkoMan_GiveItem);
