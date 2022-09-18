#include <combo.h>

int ShotSun_GiveFireArrow(Actor* actor, GameState_Play* play)
{
    PlaySound(0x4802);
    return GetEventChk(EV_OOT_CHK_ARROW_FIRE) ? 0 : 1;
}
