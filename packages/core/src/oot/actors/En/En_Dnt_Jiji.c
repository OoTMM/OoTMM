#include <combo.h>
#include <combo/item.h>

void EnDntJiji_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_OOT_STICK_UPGRADE:
    case GI_OOT_STICK_UPGRADE2:
        npc = NPC_OOT_THEATER_STICKS;
        break;
    case GI_OOT_NUT_UPGRADE:
    case GI_OOT_NUT_UPGRADE2:
        npc = NPC_OOT_THEATER_NUTS;
        break;
    }

    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80b4e7d8, EnDntJiji_GiveItem);
PATCH_CALL(0x80b4e854, EnDntJiji_GiveItem);
