#include <combo.h>

int ShotSun_GiveFireArrow(Actor* actor, GameState_Play* play)
{
    PlaySound(0x4802);
    return gOotExtraFlags.fireArrow ? 0 : 1;
}
