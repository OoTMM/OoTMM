#include <combo.h>

int comboZoraWaterfallDown(GameState_Play* play, int flag)
{
    if (Flags_GetSwitch(play, flag))
        Flags_SetSwitch(play, 0x0);
    return Flags_GetSwitch(play, 0x0);
}
