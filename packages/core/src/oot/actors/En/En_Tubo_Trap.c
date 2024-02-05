#include <combo.h>

void EnTuboTrap_SpawnShuffledDrop(Actor_EnTuboTrap* this, GameState_Play* play)
{
    u16 var;
    u16 unk;

    if (EnTuboTrap_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->base.world.pos, &this->xflag);
        return;
    }

    var = this->base.variable;
    unk = (var >> 6) & 0x3ff;
    if (unk < 0x1a)
    {
        Item_DropCollectible(play, &this->base.world.pos, unk | ((var & 0x3f) << 8));
    }
}
