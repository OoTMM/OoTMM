#include <combo.h>

int comboDoorIsUnlocked(GameState_Play* play, int flag)
{
    u16 sceneId;

    sceneId = play->sceneId;

    /* Ganondorf boss door */
    if (sceneId == SCE_OOT_GANON_TOWER && flag == 0x14 && comboConfig(CFG_GANON_NO_BOSS_KEY))
        return 1;

    return GetSwitchFlag(play, flag);
}
