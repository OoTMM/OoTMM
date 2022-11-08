#include <combo.h>

int DmChar02_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        if (!gMmExtraFlags2.ocarina)
        {
            gMmExtraFlags2.ocarina = 1;
            this->attachedA = NULL;
            return 0;
        }
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80aab1d4, DmChar02_HasGivenItem);

int DmChar02_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (!gMmExtraFlags2.ocarina)
        gi = comboOverride(OV_NPC, 0, NPC_MM_SKULL_KID_OCARINA, GI_MM_OCARINA_OF_TIME);
    else
        gi = comboOverride(OV_NPC, 0, NPC_MM_SKULL_KID_SONG, GI_MM_SONG_TIME);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80aab1fc, DmChar02_GiveItem);

void DmChar02_DrawOcarina(Actor* this, GameState_Play* play)
{
    s16 gi;
    static const float scale = 25.0f;

    gi = comboOverride(OV_NPC, 0, NPC_MM_SKULL_KID_OCARINA, GI_MM_OCARINA_OF_TIME);
    ModelViewScale(scale, scale, scale, MAT_MUL);
    comboDrawGI(play, this, gi, DRAW_RAW);
}
