#include <combo.h>

void EnGs_TalkedTo(Actor* this, GameState_Play* play)
{
    u8 key;

    key = this->variable & 0x1f;
    if (key == 0x18)
        key = (gSaveContext.grottoChestFlag & 0x1f) | 0x20;
    comboHintGossip(key, play);
}
