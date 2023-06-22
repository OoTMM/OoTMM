#include <combo.h>
#include <combo/item.h>

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
    int npc;

    npc = -1;
    if (gi == GI_MM_MASK_POSTMAN)
    {
        if (gMmExtraFlags2.maskPostman)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            npc = NPC_MM_MASK_POSTMAN;
            sIsMaksPostman = 1;
        }
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}
