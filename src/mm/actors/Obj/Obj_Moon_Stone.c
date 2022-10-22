#include <combo.h>

void ObjMoonStone_Draw(Actor* this, GameState_Play* play)
{
    s16 gi;

    gi = comboOverride(OV_NPC, 0, NPC_MM_MOON_TEAR, GI_MM_MOON_TEAR);
    comboDrawGI(play, this, gi, 0);
}

PATCH_FUNC(0x80c06910, ObjMoonStone_Draw);

int ObjMoonStone_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_MOON_TEAR, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_FUNC(0x80c06840, ObjMoonStone_GiveItem);
