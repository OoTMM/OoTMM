#include <combo.h>

int EnGs_TalkedTo(Actor* this, GameState_Play* play)
{
    if (ActorTalkedTo(this))
    {
        comboHintGossip(this->variable & 0x1f, play);
    }
    return 0;
}

PATCH_CALL(0x80b6c3ec, EnGs_TalkedTo);
