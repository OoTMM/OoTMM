#include <combo.h>

int EnGs_TalkedTo(Actor* this, GameState_Play* play)
{
    u8 key;

    if (ActorTalkedTo(this))
    {
        key = this->variable & 0x1f;
        if (key == 0x18)
            key = (gSaveContext.grottoChestFlag & 0x1f) | 0x20;
        comboHintGossip(key, play);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80b6c3ec, EnGs_TalkedTo);
