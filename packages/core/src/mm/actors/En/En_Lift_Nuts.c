#include <combo.h>
#include <combo/item.h>

int EnLiftNuts_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.dekuPlayground = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80aeb5a4, EnLiftNuts_HasGivenItem);

void EnLiftNuts_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
    {
        npc = NPC_MM_DEKU_PLAYGROUND_2;
    }
    else if (!gMmExtraFlags2.dekuPlayground)
    {
        npc = NPC_MM_DEKU_PLAYGROUND_1;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80aeb64c, EnLiftNuts_GiveItem);
PATCH_CALL(0x80aeb66c, EnLiftNuts_GiveItem);
