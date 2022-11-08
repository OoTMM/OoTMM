#include <combo.h>

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

int EnFu_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_RUPEE_PURPLE:
        if (!gMmExtraFlags2.honeyDarling)
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_HONEY_DARLING_1, gi);
            sIsFirstReward = 1;
        }
        break;
    case GI_MM_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_MM_HONEY_DARLING_2, gi);
        break;
    }
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x809635cc, EnFu_GiveItem);
PATCH_CALL(0x809635ec, EnFu_GiveItem);
