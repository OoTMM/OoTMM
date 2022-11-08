#include <combo.h>

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

int EnGinkoMan_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
        gi = comboOverride(OV_NPC, 0, NPC_MM_BANK_1, gi);
        break;
    case GI_MM_RUPEE_BLUE:
        if (!gMmExtraFlags.bankReward2)
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_BANK_2, gi);
            sIsReward2 = 1;
        }
        break;
    case GI_MM_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_MM_BANK_3, gi);
        break;
    }

    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80a65618, EnGinkoMan_GiveItem);
PATCH_CALL(0x80a65658, EnGinkoMan_GiveItem);
PATCH_CALL(0x80a65694, EnGinkoMan_GiveItem);
