#include <combo.h>
#include <combo/item.h>

void EnSyatekiMan_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_OOT_BULLET_BAG2:
    case GI_OOT_BULLET_BAG3:
        npc = NPC_OOT_SHOOTING_GAME_CHILD;
        break;
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
        npc = NPC_OOT_SHOOTING_GAME_ADULT;
        break;
    }
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x809a130c, EnSyatekiMan_GiveItem);
PATCH_CALL(0x809a140c, EnSyatekiMan_GiveItem);
