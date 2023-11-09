#include <combo.h>
#include <combo/item.h>

void EnJgameTsn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
        npc = NPC_MM_FISHERMAN_HEART_PIECE;
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80c145cc, EnJgameTsn_GiveItem);
PATCH_CALL(0x80c145e4, EnJgameTsn_GiveItem);

int Fisherman_ShouldEndGame(Actor* actor, GameState_Play* play, u32 timerHi, u32 timerLo)
{
    return (gSaveContext.minigameCounter[0] >= 20 || (timerHi == 0 && timerLo == 0));
}

int Fisherman_ShouldPassTimerCheck(Actor* actor, GameState_Play* play, u32 timerHi, u32 timerLo)
{
    return 1;
}