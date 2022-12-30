#include <combo.h>

void EnWonderTalk2_AfterInit(Actor* this, GameState_Play* play)
{
    if ((this->variable & 0xf000) == 0x4000)
    {
        ActorDestroy(this);
    }
}
