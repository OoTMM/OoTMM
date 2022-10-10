#include <combo.h>

s16 EnExItem_RewardByIndex(int index)
{
    s16 gi;
    s32 npc;

    npc = -1;
    switch (index)
    {
    case 0x00:
    case 0x05:
        /* Bomb pouch */
        gi = GI_OOT_BOMB_BAG;
        npc = 0x09;
        break;
    case 0x01:
    case 0x06:
        /* Piece of Heart */
        gi = GI_OOT_HEART_PIECE;
        npc = 0x0a;
        break;
    case 0x02:
    case 0x07:
        /* Bombchu */
        gi = GI_OOT_BOMBCHUS_10;
        break;
    case 0x03:
    case 0x08:
        /* Bombs */
        gi = GI_OOT_BOMBS_10;
        break;
    case 0x04:
    case 0x09:
        /* Purple Rupee */
        gi = GI_OOT_RUPEE_PURPLE;
        break;
    case 0x13:
        /* Target: Slingshot */
        gi = GI_OOT_SLINGSHOT;
        npc = 0x08;
        break;
    default:
        gi = GI_NONE;
        break;
    }
    if (npc >= 0)
    {
        gi = comboOverrideNpc(npc, gi);
    }
    return gi;
}

s16 EnExItem_Reward(Actor* actor)
{
    return EnExItem_RewardByIndex(actor->variable & 0x1f);
}

void EnExItem_Draw(Actor* actor, GameState_Play* play)
{
    float scale;

    scale = *(float*)(((char*)actor) + 0x154);
    ActorSetScale(actor, scale);
    comboDrawGI(play, actor, EnExItem_Reward(actor));
}

PATCH_FUNC(0x80ad9f78, EnExItem_Draw);
