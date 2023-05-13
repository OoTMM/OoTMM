#include <combo.h>

static u8 sIsMaksPostman;

void EnPm_AfterGivingItem(Actor* this)
{
    if (sIsMaksPostman)
    {
        gMmExtraFlags2.maskPostman = 1;
        sIsMaksPostman = 0;
    }
}

void EnPm_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_MASK_POSTMAN)
    {
        if (gMmExtraFlags2.maskPostman)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_POSTMAN, gi);
            sIsMaksPostman = 1;
        }
    }
    GiveItem(this, play, gi, a, b);
}
