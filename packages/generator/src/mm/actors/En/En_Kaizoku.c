#include <combo.h>
#include <combo/entrance.h>

void EnKaizoku_ThrowOut(PlayState* play)
{
    u32 entrance;
    u32 override;

    entrance = ENTR_MM_PIRATE_FORTRESS_EXTERIOR_FROM_INTERIOR;
    override = comboEntranceOverride(entrance);
    if (override != ENTR_INVALID && override != entrance)
        comboTransition(play, override);
    else
        play->transitionType = 0x26;
}

PATCH_CALL(0x80b86ae0, EnKaizoku_ThrowOut);
