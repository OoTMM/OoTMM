#include <combo.h>

static int EnMd_ShouldSpawn(Actor* this, GameState_Play* play)
{
    int midoInHouse;

    if (play->sceneId == SCE_OOT_LOST_WOODS)
        return 1;

    midoInHouse =
        GetEventChk(EV_OOT_CHK_DEKU_MIDO_SWORD_SHIELD)
        && GetEventChk(EV_OOT_CHK_MIDO_TREE_DEAD)
        && (GetEventChk(EV_OOT_CHK_TREE_DEAD) || GetEventChk(EV_OOT_CHK_ZELDA_LETTER));

    if (play->sceneId == SCE_OOT_KOKIRI_MIDO)
    {
        return (gSave.age == AGE_CHILD && midoInHouse);
    }
    else
    {
        return !midoInHouse;
    }
}

PATCH_FUNC(0x80ae3668, EnMd_ShouldSpawn);
