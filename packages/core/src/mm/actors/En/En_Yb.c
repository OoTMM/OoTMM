#include <combo.h>
#include <combo/item.h>

int EnYb_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.maskKamaro = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80bfa940, EnYb_HasGivenItem);

void EnYb_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gMmExtraFlags2.maskKamaro)
        gi = GI_MM_RECOVERY_HEART;
    else
        npc = NPC_MM_MASK_KAMARO;
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80bfa9b0, EnYb_GiveItem);
