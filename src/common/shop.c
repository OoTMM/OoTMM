#include <combo.h>

#if defined(GAME_OOT)
# define SOLD_OUT (GI_MM_SOLD_OUT | MASK_FOREIGN_GI)
#else
# define SOLD_OUT GI_MM_SOLD_OUT
#endif

#if defined(GAME_OOT)
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
#endif

#if defined(GAME_OOT)
#define SHOP_NUTS_5                 0x00
#define SHOP_ARROWS_30              0x01
#define SHOP_ARROWS_50              0x02
#define SHOP_BOMBS_5                0x03
#define SHOP_NUTS_10                0x04
#define SHOP_STICK                  0x05
#define SHOP_BOMBS_10               0x06
#define SHOP_FISH                   0x07
#define SHOP_POTION_RED             0x08
#define SHOP_POTION_GREEN           0x09
#define SHOP_POTION_BLUE            0x0a
#define SHOP_GIANT_KNIFE            0x0b
#define SHOP_SHIELD_HYLIAN          0x0c
#define SHOP_SHIELD_DEKU            0x0d
#define SHOP_TUNIC_GORON            0x0e
#define SHOP_TUNIC_ZORA             0x0f
#define SHOP_RECOVERY_HEART         0x10
#define SHOP_BOMBCHU_10_1           0x15
#define SHOP_BOMBCHU_20_1           0x16
#define SHOP_BOMBCHU_20_2           0x17
#define SHOP_BOMBCHU_10_2           0x18
#define SHOP_BOMBCHU_10_3           0x19
#define SHOP_BOMBCHU_20_3           0x1a
#define SHOP_BOMBCHU_20_4           0x1b
#define SHOP_BOMBCHU_10_4           0x1c
#define SHOP_SEEDS_30               0x1d
#define SHOP_MASK_KEATON            0x1e
#define SHOP_MASK_SPOOKY            0x1f
#define SHOP_MASK_SKULL             0x20
#define SHOP_MASK_BUNNY             0x21
#define SHOP_MASK_TRUTH             0x22
#define SHOP_MASK_ZORA              0x23
#define SHOP_MASK_GORON             0x24
#define SHOP_MASK_GERUDO            0x25
#define SHOP_SOLD_OUT               0x26
#define SHOP_BLUE_FIRE              0x27
#define SHOP_BUG                    0x28
#define SHOP_BIG_POE                0x29
#define SHOP_POE                    0x2a
#define SHOP_FAIRY                  0x2b
#define SHOP_ARROWS_10              0x2c
#define SHOP_BOMBS_20               0x2d
#define SHOP_BOMBS_30               0x2e
#define SHOP_BOMBS_5                0x2f
#define SHOP_POTION_RED             0x30
#define SHOP_POTION_RED             0x31

int comboShopItemSlot(GameState_Play* play, Actor_EnGirlA* girlA)
{
    if (play->sceneId == SCE_OOT_KOKIRI_SHOP)
    {
        /* Kokiri shop */
        switch (girlA->base.variable)
        {
        case SHOP_RECOVERY_HEART:   return 0x00;
        case SHOP_ARROWS_10:        return 0x01;
        case SHOP_ARROWS_30:        return 0x02;
        case SHOP_SEEDS_30:         return 0x03;
        case SHOP_NUTS_5:           return 0x04;
        case SHOP_NUTS_10:          return 0x05;
        case SHOP_STICK:            return 0x06;
        case SHOP_SHIELD_DEKU:      return 0x07;
        }
    }

    return 7;

    UNREACHABLE();
}
#endif

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
