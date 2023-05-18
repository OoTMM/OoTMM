#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define SOLD_OUT (GI_MM_SOLD_OUT | MASK_FOREIGN_GI)
#else
# define SOLD_OUT GI_MM_SOLD_OUT
#endif

int shopReadFlag(int);
void shopWriteFlag(int);

int comboShopPrecond(GameState_Play* play, Actor_EnGirlA* girlA)
{
    return comboItemPrecond(girlA->gi, girlA->price);
}

void comboShopAfterBuy(GameState_Play* play, Actor_EnGirlA* girlA)
{
    shopWriteFlag(girlA->shopId);
    comboShopUpdateItem(play, girlA);
}

static void quickBuyItem(GameState_Play* play, Actor_EnGirlA* girlA)
{
    comboAddItem(play, girlA->gi);
    AddRupees(-girlA->price);
}

static void postBuyItem(GameState_Play* play, Actor_EnGirlA* girlA)
{
    AddRupees(-girlA->price);
}

void comboShopUpdateItem(GameState_Play* play, Actor_EnGirlA* girlA)
{
    girlA->precond = comboShopPrecond;
    girlA->quickBuy = quickBuyItem;
    girlA->postBuy = postBuyItem;

    /* Update GI */
    girlA->gi = comboOverrideEx(OV_SHOP, 0, girlA->shopId, girlA->gi, OVF_PROGRESSIVE);

    if (shopReadFlag(girlA->shopId))
    {
#if defined(GAME_MM)
        switch (girlA->shopId)
        {
        case 0x02:
        case 0x03:
        case 0x04:
            girlA->gi = SOLD_OUT;
            break;
        }
#endif

        girlA->gi = comboRenewable(girlA->gi, SOLD_OUT);

        if (girlA->gi == SOLD_OUT)
        {
            girlA->disabled = 1;
        }
    }
}

void comboShopSetupItem(GameState_Play* play, Actor_EnGirlA* girlA)
{
#if defined(GAME_MM)
    if (girlA->base.variable == 2)
        girlA->disabled = !(MM_GET_EVENT_WEEK(EV_MM_WEEK_WITCH_MUSHROOM));
#endif

    girlA->shopId = comboShopItemSlot(play, girlA);
    comboShopUpdateItem(play, girlA);
}

void comboShopDisplayTextBox(GameState_Play* play, Actor_EnGirlA* girlA)
{
    DisplayTextBox2(play, girlA->base.messageId);
    if (girlA->gi == SOLD_OUT)
    {
        girlA->disabled = 1;
    }
    comboTextHijackItemShop(play, girlA->gi, girlA->price, 0);
}

void comboShopDisplayTextBoxConfirm(GameState_Play* play, Actor_EnGirlA* girlA)
{
    DisplayTextBox2(play, girlA->messageId2);
    comboTextHijackItemShop(play, girlA->gi, girlA->price, 1);
}
