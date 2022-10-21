#include <combo.h>

void EnOwl_AfterInit(Actor* this)
{
    switch ((this->variable & 0xfc0) >> 6)
    {
    case 0x07:
    case 0x08:
    case 0x09:
        break;
    default:
        ActorDestroy(this);
        break;
    }
}
