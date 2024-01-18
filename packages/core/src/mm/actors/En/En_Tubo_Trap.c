#include <combo.h>

void EnTuboTrap_SpawnShuffledDrop(Actor_EnTuboTrap* this, GameState_Play* play)
{
    if (EnTuboTrap_IsShuffled(this))
        EnItem00_DropCustom(play, &this->base.position, &this->xflag);
}
