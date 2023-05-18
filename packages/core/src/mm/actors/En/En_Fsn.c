#include <combo.h>

static u8 sIsSecondReward;

void EnFsn_GiveNormalItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_MASK_KEATON:
        if (gMmExtraFlags2.letterMama)
        {
            gi = GI_MM_RECOVERY_HEART;
        }
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_KEATON, gi);
        }
        break;
    case GI_MM_LETTER_TO_MAMA:
        if (gMmExtraFlags2.letterMama)
        {
            gi = GI_MM_RECOVERY_HEART;
        }
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_LETTER_TO_MAMA, gi);
            sIsSecondReward = 1;
        }
        break;
    }
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80ae3cd4, EnFsn_GiveNormalItem);

int EnFsn_HasGivenShopItem(Actor_EnFsn* this, GameState_Play* play)
{
    if (Actor_HasParent(&this->base))
    {
        if (this->mode == 1)
            comboShopAfterBuy(play, this->items[this->itemIndex]);
        else if (sIsSecondReward)
        {
            sIsSecondReward = 0;
            gMmExtraFlags2.letterMama = 1;
        }
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80ae3be0, EnFsn_HasGivenShopItem);
