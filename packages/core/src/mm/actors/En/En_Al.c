#include <combo.h>
#include <combo/item.h>

void EnAl_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_MM_MASK_KAFEI:
        npc = NPC_MM_MASK_KAFEI;
        gMmExtraFlags2.maskKafei = 1;
        break;
    case GI_MM_BOTTLE_CHATEAU:
        npc = NPC_MM_CHATEAU_ROMANI;
        break;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}
