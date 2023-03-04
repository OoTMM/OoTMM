#include <combo.h>

#if defined(GAME_OOT)
# define SOLD_OUT (GI_MM_SOLD_OUT | MASK_FOREIGN_GI)
#else
# define SOLD_OUT GI_MM_SOLD_OUT
#endif

int comboShopPrecond(GameState_Play* play, Actor_EnGirlA* girlA)
{
    if (comboIsItemUnavailable(girlA->gi))
        return SC_ERR_CANNOTBUY;

    if (gSave.playerData.rupees < girlA->price)
        return SC_ERR_NORUPEES;

    if (comboIsItemMinor(girlA->gi))
        return SC_OK_NOCUTSCENE;

    return SC_OK;
}

#if defined(GAME_OOT)
void shopWriteFlag(int flag)
{
    u32* ptr;

    ptr = &gOotExtraShopsLo;
    if (flag >= 32)
    {
        ptr = &gOotExtraShopsHi;
        flag -= 32;
    }
    (*ptr) |= (1 << flag);
}

int shopReadFlag(int flag)
{
    u32* ptr;

    ptr = &gOotExtraShopsLo;
    if (flag >= 32)
    {
        ptr = &gOotExtraShopsHi;
        flag -= 32;
    }
    return ((*ptr) & (1 << flag)) != 0;
}
#endif

#if defined(GAME_MM)
u8 comboShopItemSlot(GameState_Play* play, Actor_EnGirlA* girlA)
{
    return 0;
}

void shopWriteFlag(int flag)
{
}

int shopReadFlag(int flag)
{
    return 0;
}
#endif

void comboShopAfterBuy(GameState_Play* play, Actor_EnGirlA* girlA)
{
    shopWriteFlag(girlA->shopId);
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
    girlA->gi = comboOverrideEx(OV_SHOP, 0, girlA->shopId, girlA->gi, OVF_PROGRESSIVE);

    if (!comboIsItemConsumable(girlA->gi) && shopReadFlag(girlA->shopId))
    {
        girlA->gi = SOLD_OUT;
        girlA->disabled = 1;
    }
}

void comboShopSetupItem(GameState_Play* play, Actor_EnGirlA* girlA)
{
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
