#include <combo.h>

int EnMaYto_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParent(this);
    if (ret)
        gMmExtraFlags.maskRomani = 1;
    return ret;
}

PATCH_CALL(0x80b900d4, EnMaYto_HasGivenItem);

void EnMaYto_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags.maskRomani)
    {
        gi = GI_MM_RUPEE_GOLD;
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_CREMIA_ESCORT, GI_MM_MASK_ROMANI);
    }
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b9012c, EnMaYto_GiveItem);
PATCH_CALL(0x80b9014c, EnMaYto_GiveItem);
