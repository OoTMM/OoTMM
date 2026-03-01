#include <combo.h>
#include <combo/entrance.h>

void EnDns_AfterDetection(void)
{
    PlayState* play;
    u32 entrance;
    s32 override;

    play = gPlay;
    if (!play->nextEntrance)
        return;

    override = comboEntranceOverride(ENTR_MM_DEKU_PALACE_EXTERIOR_FROM_THRONE);
    if (override != -1)
        comboTransitionOverride(play, (u32)override);
}
