#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

static void Message_ForceCancel(GameState_Play* play)
{
    PlayerDisplayTextBox(play, 0x1000, NULL);
    Message_Close(play);
}

void EnRz_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    Actor_Player* link;

    link = GET_PLAYER(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    Message_ForceCancel(play);
    comboGiveItemNpc(this, play, gi, NPC_MM_ROSA_HEART_PIECE, a, b);
}

PATCH_CALL(0x80bfc2d8, EnRz_GiveItem);
PATCH_CALL(0x80bfc344, EnRz_GiveItem);

int EnRz_DanceCutscene(Actor* this, GameState_Play* play)
{
    Message_ForceCancel(play);
    return 1;
}
