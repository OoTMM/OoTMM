#include <combo.h>
#include <combo/item.h>

void EnFr_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_OOT_HEART_PIECE)
    {
        if (GetEventChk(EV_OOT_CHK_FROGS_GAME))
            npc = NPC_OOT_FROGS_GAME;
        else
            npc = NPC_OOT_FROGS_STORMS;
    }
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80a279b0, EnFr_GiveItem);
PATCH_CALL(0x80a27a14, EnFr_GiveItem);
