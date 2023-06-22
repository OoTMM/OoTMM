#include <combo.h>
#include <combo/item.h>

void EnZos_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);

    if (gi == GI_MM_HEART_PIECE)
    {
        npc = NPC_MM_ZORA_EVAN;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80bbb3fc, EnZos_GiveItem);
