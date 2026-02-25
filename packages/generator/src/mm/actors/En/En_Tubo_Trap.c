#include <combo.h>

void EnTuboTrap_SpawnShuffledDrop(Actor_EnTuboTrap* this, PlayState* play)
{
    if (EnTuboTrap_IsShuffled(this))
        EnItem00_DropCustom(play, &this->base.world.pos, &this->xflag);
}
