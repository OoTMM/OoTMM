#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define SOLD_OUT (GI_MM_SOLD_OUT | MASK_FOREIGN_GI)
# define PRICES_SHOPS PRICES_OOT_SHOPS
#else
# define SOLD_OUT GI_MM_SOLD_OUT
# define PRICES_SHOPS PRICES_MM_SHOPS
#endif

int shopReadFlag(int);
void shopWriteFlag(int);

int comboShopPrecond(GameState_Play* play, Actor_EnGirlA* girlA)
{
    ComboItemQuery q;

    EnGirlA_ItemQuery(&q, girlA, 0);
    return comboItemPrecondEx(&q, girlA->price);
}

void comboShopAfterBuy(GameState_Play* play, Actor_EnGirlA* girlA)
{
    shopWriteFlag(girlA->shopId);
    comboShopUpdateItem(play, girlA);
}

static void quickBuyItem(GameState_Play* play, Actor_EnGirlA* girlA)
{
    ComboItemQuery q;

    EnGirlA_ItemQuery(&q, girlA, OVF_PROGRESSIVE);
    comboAddItemEx(play, &q);
    AddRupees(-girlA->price);
}

static void postBuyItem(GameState_Play* play, Actor_EnGirlA* girlA)
{
    AddRupees(-girlA->price);
}

void comboShopUpdateItem(GameState_Play* play, Actor_EnGirlA* girlA)
{
    ComboItemOverride o;

    EnGirlA_ItemOverride(&o, girlA, OVF_PROGRESSIVE);
    if (o.gi == SOLD_OUT)
        girlA->disabled = 1;

    girlA->precond = comboShopPrecond;
    girlA->quickBuy = quickBuyItem;
    girlA->postBuy = postBuyItem;
}

void comboShopSetupItem(GameState_Play* play, Actor_EnGirlA* girlA)
{
#if defined(GAME_MM)
    if (girlA->base.variable == 2)
        girlA->disabled = !(MM_GET_EVENT_WEEK(EV_MM_WEEK_WITCH_MUSHROOM));
#endif

    girlA->shopId = comboShopItemSlot(play, girlA);
    girlA->price = (s16)gComboData.prices[PRICES_SHOPS + girlA->shopId];

#if defined(GAME_MM)
    if (girlA->shopId == 0x03 && play->sceneId == SCE_MM_CURIOSITY_SHOP)
        girlA->price = (s16)gComboData.prices[PRICES_MM_SHOPS_EX + 0x00];
#endif

    comboShopUpdateItem(play, girlA);
}

void comboShopDisplayTextBox(GameState_Play* play, Actor_EnGirlA* girlA)
{
    ComboItemOverride o;

    EnGirlA_ItemOverride(&o, girlA, OVF_PROGRESSIVE);
    DisplayTextBox2(play, girlA->base.messageId);
    if (o.gi == SOLD_OUT)
    {
        girlA->disabled = 1;
    }
    comboTextHijackItemShop(play, &o, girlA->price, 0);
}

void comboShopDisplayTextBoxConfirm(GameState_Play* play, Actor_EnGirlA* girlA)
{
    ComboItemOverride o;

    EnGirlA_ItemOverride(&o, girlA, OVF_PROGRESSIVE);
    DisplayTextBox2(play, girlA->messageId2);
    comboTextHijackItemShop(play, &o, girlA->price, 1);
}
