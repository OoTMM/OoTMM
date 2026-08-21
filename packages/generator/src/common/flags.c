#include <combo.h>
#include <combo/flags.h>

int Flags_GetPermSwitchOot(PlayState* play, int sceneId, int flag)
{
#if defined(GAME_OOT)
    if (play && play->sceneId == sceneId)
        return Flags_GetSwitch(play, flag);
#endif

    if (flag < 0 || flag >= 32)
        return 0;

    return gOotSave.info.perm[sceneId].switches & (1 << flag);
}

void Flags_SetPermSwitchOot(PlayState* play, int sceneId, int flag)
{
#if defined(GAME_OOT)
    if (play && play->sceneId == sceneId)
    {
        Flags_SetSwitch(play, flag);
        return;
    }
#endif

    if (flag < 0 || flag >= 32)
        return;

    gOotSave.info.perm[sceneId].switches |= (1 << flag);
}
