#include <combo.h>

void EnTuboTrap_SpawnShuffledDrop(Actor_EnTuboTrap* this, GameState_Play* play)
{
    u16 var;

    if (EnTuboTrap_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->base.position, &this->xflag);
        return;
    }

    var = this->base.variable;
    if ((var & 0xff) < 0x1a)
    {
        Item_DropCollectible(play, &this->base.position, (var & 0xff) | (((var >> 9) & 0x3f) << 8));
    }
}
