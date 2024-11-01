#include <combo.h>

void EnWonderTalk2_AfterInit(Actor* this, PlayState* play)
{
    if ((this->params & 0xf000) == 0x4000)
    {
        Actor_Kill(this);
    }
}
