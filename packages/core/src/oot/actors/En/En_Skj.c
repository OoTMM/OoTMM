#include <combo.h>

void EnSkj_GiveItem_TreeHP(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_LOST_WOODS_SKULL_KID, gi);
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a6e550, EnSkj_GiveItem_TreeHP);
PATCH_CALL(0x80a6e5e8, EnSkj_GiveItem_TreeHP);

void EnSkj_GiveItem_MemoryGame(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_OOT_HEART_PIECE)
        gi = comboOverride(OV_NPC, 0, NPC_OOT_LOST_WOODS_MEMORY, gi);
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a6f7dc, EnSkj_GiveItem_MemoryGame);
PATCH_CALL(0x80a6f860, EnSkj_GiveItem_MemoryGame);
