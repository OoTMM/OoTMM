#include <combo.h>
#include <combo/item.h>

void EnHy_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_OOT_HEART_PIECE)
        npc = NPC_OOT_DOG_LADY;
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80ae5138, EnHy_GiveItem);
PATCH_CALL(0x80ae6e84, EnHy_GiveItem);
