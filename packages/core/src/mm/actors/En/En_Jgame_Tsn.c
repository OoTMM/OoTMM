#include <combo.h>
#include <combo/item.h>

void EnJgameTsn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
        npc = NPC_MM_FISHERMAN_HEART_PIECE;
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80c145cc, EnJgameTsn_GiveItem);
PATCH_CALL(0x80c145e4, EnJgameTsn_GiveItem);
