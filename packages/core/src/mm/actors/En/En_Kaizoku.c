#include <combo.h>
#include <combo/entrance.h>

void EnKaizoku_ThrowOut(GameState_Play* play)
{
    u32 entrance;
    s32 override;

    entrance = ENTR_MM_PIRATE_FORTRESS_EXTERIOR_FROM_INTERIOR;
    override = comboEntranceOverride(entrance);
    if (override != -1 && (u32)override != entrance)
        comboTransition(play, (u32)override);
    else
        play->transitionType = 0x26;
}

PATCH_CALL(0x80b86ae0, EnKaizoku_ThrowOut);
