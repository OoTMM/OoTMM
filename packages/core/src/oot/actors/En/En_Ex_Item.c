#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>
#include <combo/draw.h>

void EnExItem_RewardByIndex(ComboItemQuery* q, int index)
{
    memset(q, 0, sizeof(*q));

    switch (index)
    {
    case 0x00:
    case 0x05:
        /* Bomb pouch */
        q->ovType = OV_NPC;
        q->gi = GI_OOT_BOMB_BAG;
        q->id = NPC_OOT_BOMBCHU_BOWLING_1;
        break;
    case 0x01:
    case 0x06:
        /* Piece of Heart */
        q->ovType = OV_NPC;
        q->gi = GI_OOT_HEART_PIECE;
        q->id = NPC_OOT_BOMBCHU_BOWLING_2;
        break;
    case 0x02:
    case 0x07:
        /* Bombchu */
        q->gi = GI_OOT_BOMBCHU_10;
        if (gComboConfig.bombchuBehaviorOot == BOMBCHU_BEHAVIOR_BAG_FIRST && !gSharedCustomSave.bombchuBagOot)
            q->gi = GI_OOT_RUPEE_PURPLE;
        break;
    case 0x03:
    case 0x08:
        /* Bombs */
        q->gi = GI_OOT_BOMBS_10;
        break;
    case 0x04:
    case 0x09:
        /* Purple Rupee */
        q->gi = GI_OOT_RUPEE_PURPLE;
        break;
    case 0x0a:
        /* Treasure Game: Green Rupee */
        q->gi = GI_OOT_RUPEE_GREEN;
        break;
    case 0x0b:
        /* Treasure Game: Blue Rupee */
        q->gi = GI_OOT_RUPEE_BLUE;
        break;
    case 0x0c:
        /* Treasure Game: Red Rupee */
        q->gi = GI_OOT_RUPEE_RED;
        break;
    case 0x0f:
        /* Treasure Game: Small Key */
        q->gi = GI_OOT_SMALL_KEY;
        break;
    case 0x13:
        /* Target: Slingshot */
        q->ovType = OV_NPC;
        q->gi = GI_OOT_SLINGSHOT;
        q->id = NPC_OOT_LOST_WOODS_TARGET;
        break;
    default:
        break;
    }
}

void EnExItem_Reward(ComboItemQuery* q, const Actor* actor)
{
    EnExItem_RewardByIndex(q, actor->params & 0x1f);
}

void EnExItem_Draw(Actor* actor, PlayState* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
    float scale;
    s16 gi;

    EnExItem_Reward(&q, actor);
    comboItemOverride(&o, &q);
    scale = *(float*)(((char*)actor) + 0x154);
    Actor_SetScale(actor, scale);

    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        Matrix_RotateYS(-actor->shape.rot.y * 2, MTXMODE_APPLY);
    }
    Draw_Gi(play, actor, gi, 0);
}

void EnExItem_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnExItem_Reward(&q, actor);
    comboGiveItem(actor, play, &q, a, b);
}

PATCH_FUNC(0x80ad9f78, EnExItem_Draw);
PATCH_CALL(0x80ad9e10, EnExItem_GiveItem);
PATCH_CALL(0x80ad9eac, EnExItem_GiveItem);
