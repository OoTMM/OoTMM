#include <combo.h>

void EnWonderTalk2_AfterInit(Actor* this, GameState_Play* play)
{
    u8 switchFlag;

    if ((this->variable & 0xf000) == 0x4000)
    {
        switchFlag = this->variable & 0x3F;
        if (switchFlag != 0x3F)
        {
            SetSwitchFlag(play, switchFlag);
        }
        ActorDestroy(this);
    }
}
