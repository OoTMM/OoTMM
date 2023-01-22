#include <combo.h>

s16 EnExItem_RewardByIndex(int index)
{
    s16 gi;

    switch (index)
    {
    case 0x00:
    case 0x05:
        /* Bomb pouch */
        gi = comboOverride(OV_NPC, 0, NPC_OOT_BOMBCHU_BOWLING_1, GI_OOT_BOMB_BAG);
        break;
    case 0x01:
    case 0x06:
        /* Piece of Heart */
        gi = comboOverride(OV_NPC, 0, NPC_OOT_BOMBCHU_BOWLING_2, GI_OOT_HEART_PIECE);
        break;
    case 0x02:
    case 0x07:
        /* Bombchu */
        gi = GI_OOT_BOMBCHU_10;
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
    case 0x0a:
        /* Treasure Game: Green Rupee */
        gi = GI_OOT_RUPEE_GREEN;
        break;
    case 0x0b:
        /* Treasure Game: Blue Rupee */
        gi = GI_OOT_RUPEE_BLUE;
        break;
    case 0x0c:
        /* Treasure Game: Red Rupee */
        gi = GI_OOT_RUPEE_RED;
        break;
    case 0x0f:
        /* Treasure Game: Small Key */
        gi = GI_OOT_SMALL_KEY;
        break;
    case 0x13:
        /* Target: Slingshot */
        gi = comboOverride(OV_NPC, 0, NPC_OOT_LOST_WOODS_TARGET, GI_OOT_SLINGSHOT);
        break;
    default:
        gi = GI_NONE;
        break;
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
    comboDrawGI(play, actor, EnExItem_Reward(actor), 0);
}

int EnExItem_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = EnExItem_Reward(actor);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_FUNC(0x80ad9f78, EnExItem_Draw);
PATCH_CALL(0x80ad9e10, EnExItem_GiveItem);
PATCH_CALL(0x80ad9eac, EnExItem_GiveItem);
