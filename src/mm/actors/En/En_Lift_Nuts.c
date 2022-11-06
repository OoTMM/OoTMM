#include <combo.h>

int EnLiftNuts_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.dekuPlayground = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80aeb5a4, EnLiftNuts_HasGivenItem);

int EnLiftNuts_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_DEKU_PLAYGROUND_2, gi);
    }
    else if (!gMmExtraFlags2.dekuPlayground)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_DEKU_PLAYGROUND_1, gi);
    }
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80aeb64c, EnLiftNuts_GiveItem);
PATCH_CALL(0x80aeb66c, EnLiftNuts_GiveItem);
