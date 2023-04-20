#include <combo.h>

static s16 moonTearGI(GameState_Play* play)
{
    if (gMmExtraFlags2.moonTear && play->sceneId == SCE_MM_TERMINA_FIELD)
        return GI_MM_RECOVERY_HEART;
    return comboOverride(OV_NPC, 0, NPC_MM_MOON_TEAR, GI_MM_MOON_TEAR);
}

void ObjMoonStone_Draw(Actor* this, GameState_Play* play)
{
    s16 gi;

    gi = moonTearGI(play);
    comboDrawGI(play, this, gi, 0);
}

PATCH_FUNC(0x80c06910, ObjMoonStone_Draw);

int ObjMoonStone_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.moonTear = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80c067ec, ObjMoonStone_HasGivenItem);

int ObjMoonStone_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = moonTearGI(play);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80c06840, ObjMoonStone_GiveItem);
