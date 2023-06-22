#include <combo.h>
#include <combo/item.h>

int EnBaba_HasGiveItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.maskBlast = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80ba9ac8, EnBaba_HasGiveItem);

void EnBaba_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gMmExtraFlags2.maskBlast)
    {
        gi = GI_MM_RECOVERY_HEART;
    }
    else
    {
        npc = NPC_MM_MASK_BLAST;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80ba9b0c, EnBaba_GiveItem);
