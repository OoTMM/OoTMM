#include <combo.h>

int EnGs_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_GOSSIP_HEART_PIECE, gi);
    }
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x809989cc, EnGs_GiveItem);
PATCH_CALL(0x80998a30, EnGs_GiveItem);

void EnGs_MessageBox(GameState_Play* play, Actor* this)
{
    u8 key;

    key = (*(u8*)((char*)this + 0x195) & 0x1f);
    PlayerDisplayTextBox(play, 0x20d0, this);
    comboHintGossip(key, play);
}

PATCH_CALL(0x80997e98, EnGs_MessageBox);
