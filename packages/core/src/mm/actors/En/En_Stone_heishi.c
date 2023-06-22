#include <combo.h>
#include <combo/item.h>

int EnStoneheishi_HasGivenItem(Actor* actor)
{
    int ret;

    ret = Actor_HasParent(actor);
    if (ret)
    {
        gMmExtraFlags.maskStone = 1;
    }
    return ret;
}

PATCH_CALL(0x80bc9d4c, EnStoneheishi_HasGivenItem);

void EnStoneheishi_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gMmExtraFlags.maskStone)
        gi = GI_MM_RUPEE_BLUE;
    else
        npc = NPC_MM_MASK_STONE;
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80bc9cd4, EnStoneheishi_GiveItem);
PATCH_CALL(0x80bc9cf8, EnStoneheishi_GiveItem);
PATCH_CALL(0x80bc9e10, EnStoneheishi_GiveItem);
PATCH_CALL(0x80bc9e34, EnStoneheishi_GiveItem);
