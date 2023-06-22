#include <combo.h>
#include <combo/item.h>

void EnAob01_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
    {
        npc = NPC_MM_DOG_LADY_HEART_PIECE;
    }
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x809c2ba8, EnAob01_GiveItem);
PATCH_CALL(0x809c2bc8, EnAob01_GiveItem);
