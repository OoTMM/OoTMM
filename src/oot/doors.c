#include <combo.h>

int comboDoorIsUnlocked(GameState_Play* play, int flag)
{
    u16 sceneId;

    sceneId = play->sceneId;

    /* Fire temple 1st door */
    if (sceneId == SCE_OOT_TEMPLE_FIRE && flag == 0x17 && !comboConfig(CFG_SMALL_KEY_SHUFFLE))
        return 1;

    /* Water temple water raise door */
    if (sceneId == SCE_OOT_TEMPLE_WATER && flag == 0x15)
        return 1;

    /* Ganondorf boss door */
    if (sceneId == SCE_OOT_GANON_TOWER && flag == 0x14 && comboConfig(CFG_GANON_NO_BOSS_KEY))
        return 1;

    return GetSwitchFlag(play, flag);
}
