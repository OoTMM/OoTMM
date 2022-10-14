#include <combo.h>

int EnDns_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_OOT_STICK_UPGRADE:
    case GI_OOT_STICK_UPGRADE2:
        npc = NPC_OOT_SCRUB_STICKS;
        break;
    case GI_OOT_NUT_UPGRADE:
    case GI_OOT_NUT_UPGRADE2:
        npc = NPC_OOT_SCRUB_NUTS;
        break;
    case GI_OOT_HEART_PIECE:
        npc = NPC_OOT_SCRUB_HP;
        break;
    }

    if (npc >= 0)
    {
        gi = comboOverride(OV_NPC, 0, npc, gi);
    }
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a75734, EnDns_GiveItem);
PATCH_CALL(0x80a75750, EnDns_GiveItem);
PATCH_CALL(0x80a757a8, EnDns_GiveItem);
PATCH_CALL(0x80a757c4, EnDns_GiveItem);
PATCH_CALL(0x80a757e4, EnDns_GiveItem);
