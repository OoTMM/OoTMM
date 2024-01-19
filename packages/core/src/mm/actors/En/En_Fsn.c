#include <combo.h>
#include <combo/item.h>

static u8 sIsSecondReward;

void EnFsn_GiveNormalItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_MM_MASK_KEATON:
        if (gMmExtraFlags2.letterMama)
        {
            gi = GI_MM_RECOVERY_HEART;
        }
        else
        {
            npc = NPC_MM_MASK_KEATON;
        }
        break;
    case GI_MM_LETTER_TO_MAMA:
        if (gMmExtraFlags2.letterMama)
        {
            gi = GI_MM_RECOVERY_HEART;
        }
        else
        {
            npc = NPC_MM_LETTER_TO_MAMA;
            sIsSecondReward = 1;
        }
        break;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80ae3cd4, EnFsn_GiveNormalItem);

int EnFsn_HasGivenShopItem(Actor_EnFsn* this, GameState_Play* play)
{
    if (!Actor_HasParent(&this->base) || Message_GetState(&play->msgCtx) != 6)
        return 0;

    if (this->mode == 1)
    {
        /* Set item count to zero */
        *(u16*)((char*)this + 0x38c) = 0;
        MM_SET_EVENT_WEEK(EV_MM_WEEK_CURIOSITY_SHOP_BOUGHT);
        comboShopAfterBuy(play, this->items[this->itemIndex]);
    }
    else if (sIsSecondReward)
    {
        sIsSecondReward = 0;
        gMmExtraFlags2.letterMama = 1;
    }
    return 1;
}

PATCH_CALL(0x80ae3be0, EnFsn_HasGivenShopItem);

static void EnFsn_GiveItem(Actor_EnFsn* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Actor_EnGirlA* girlA;

    girlA = this->items[this->itemIndex];
    EnGirlA_ItemQuery(&q, girlA);
    comboGiveItem(&this->base, play, &q, a, b);
}

PATCH_CALL(0x80ae3cb0, EnFsn_GiveItem);
PATCH_CALL(0x80ae42e4, EnFsn_GiveItem);
