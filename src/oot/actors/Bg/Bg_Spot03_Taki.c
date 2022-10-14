#include <combo.h>

int comboZoraWaterfallDown(GameState_Play* play, int flag)
{
    if (GetSwitchFlag(play, flag))
        SetSwitchFlag(play, 0x0);
    return GetSwitchFlag(play, 0x0);
}
