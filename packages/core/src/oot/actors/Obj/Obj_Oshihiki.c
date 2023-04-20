#include <combo.h>

static int ObjOshihiki_GetSwitchFlag(GameState_Play* play, int flag)
{
    /* MQ Spirit fix */
    if (play->sceneId == SCE_OOT_TEMPLE_SPIRIT && (gComboData.mq & (1 << MQ_TEMPLE_SPIRIT)) && gSave.age == AGE_CHILD && flag == 0x09)
        return 0;
    return GetSwitchFlag(play, flag);
}

PATCH_CALL(0x80a4f574, ObjOshihiki_GetSwitchFlag);
