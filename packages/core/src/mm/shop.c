#include <combo.h>

#define SHOP_POTION_RED             0x00
#define SHOP_POTION_GREEN           0x01
#define SHOP_POTION_BLUE            0x02
#define SHOP_FAIRY                  0x03
#define SHOP_ARROWS_50              0x04
#define SHOP_POTION_GREEN_ALT       0x05
#define SHOP_SHIELD_HERO            0x06
#define SHOP_STICK                  0x07
#define SHOP_ARROWS_30              0x08
#define SHOP_NUTS_10                0x09
#define SHOP_POTION_RED_ALT         0x0a
#define SHOP_FAIRY_ALT              0x0b
#define SHOP_ARROWS_30_ALT          0x0c
#define SHOP_ARROWS_50_ALT          0x0d
#define SHOP_POTION_GREEN_ALT2      0x0e
#define SHOP_NUTS_10_ALT            0x0f
#define SHOP_STICK_ALT              0x10
#define SHOP_SHIELD_HERO_ALT        0x11
#define SHOP_POTION_RED_ALT2        0x12
#define SHOP_MASK_ALL_NIGHT         0x13
#define SHOP_BOMB_BAG               0x14
#define SHOP_BOMB_BAG2              0x15
#define SHOP_BOMB_BAG3              0x16
#define SHOP_BOMB_BAG_ALT           0x17
#define SHOP_BOMB_BAG2_ALT          0x18
#define SHOP_BOMBCHU_10             0x19
#define SHOP_BOMBS_10               0x1a
#define SHOP_SHIELD_HERO_ALT2       0x1b
#define SHOP_ARROWS_10              0x1c
#define SHOP_POTION_RED_ALT3        0x1d
#define SHOP_BOMBS_10_ALT           0x1e
#define SHOP_ARROWS_10_ALT          0x1f
#define SHOP_POTION_RED_ALT4        0x20
#define SHOP_BOMBS_10_ALT2          0x21
#define SHOP_ARROWS_10_ALT2         0x22
#define SHOP_POTION_RED_ALT5        0x23
#define SHOP_ITEM_A9                0x24
#define SHOP_ITEM_9B                0x25
#define SHOP_ITEM_9C                0x26
#define SHOP_ITEM_9D                0x27
#define SHOP_ITEM_9E                0x28
#define SHOP_ITEM_9F                0x29
#define SHOP_ITEM_SHIELD_MIRROR     0x2a

u8 comboShopItemSlot(GameState_Play* play, Actor_EnGirlA* girlA)
{
    switch (play->sceneId)
    {
    case SCE_MM_BOMB_SHOP:
        switch (girlA->base.variable)
        {
        case SHOP_BOMBS_10: return 0x00;
        case SHOP_BOMBCHU_10: return 0x01;
        case SHOP_BOMB_BAG: return 0x02;
        case SHOP_BOMB_BAG_ALT: return 0x02;
        case SHOP_BOMB_BAG2: return 0x03;
        case SHOP_BOMB_BAG2_ALT: return 0x03;
        }
        break;
    case SCE_MM_CURIOSITY_SHOP:
        switch (girlA->base.variable)
        {
        case SHOP_MASK_ALL_NIGHT: return 0x04;
        case SHOP_BOMB_BAG2: return 0x03;
        case SHOP_BOMB_BAG2_ALT: return 0x03;
        }
        break;
    case SCE_MM_TRADING_POST:
        switch (girlA->base.variable)
        {
        case SHOP_POTION_RED:
        case SHOP_POTION_RED_ALT:
        case SHOP_POTION_RED_ALT2:
        case SHOP_POTION_RED_ALT3:
        case SHOP_POTION_RED_ALT4:
        case SHOP_POTION_RED_ALT5:
            return 0x05;
        case SHOP_POTION_GREEN:
        case SHOP_POTION_GREEN_ALT:
        case SHOP_POTION_GREEN_ALT2:
            return 0x06;
        case SHOP_SHIELD_HERO:
        case SHOP_SHIELD_HERO_ALT:
        case SHOP_SHIELD_HERO_ALT2:
            return 0x07;
        case SHOP_FAIRY:
        case SHOP_FAIRY_ALT:
            return 0x08;
        case SHOP_STICK:
        case SHOP_STICK_ALT:
            return 0x09;
        case SHOP_ARROWS_30:
        case SHOP_ARROWS_30_ALT:
            return 0x0a;
        case SHOP_NUTS_10:
        case SHOP_NUTS_10_ALT:
            return 0x0b;
        case SHOP_ARROWS_50:
        case SHOP_ARROWS_50_ALT:
            return 0x0c;
        }
        break;
    case SCE_MM_POTION_SHOP:
        switch (girlA->base.variable)
        {
        case SHOP_POTION_BLUE:
            return 0x0d;
        case SHOP_POTION_GREEN:
        case SHOP_POTION_GREEN_ALT:
        case SHOP_POTION_GREEN_ALT2:
            return 0x0e;
        case SHOP_POTION_RED:
        case SHOP_POTION_RED_ALT:
        case SHOP_POTION_RED_ALT2:
        case SHOP_POTION_RED_ALT3:
        case SHOP_POTION_RED_ALT4:
        case SHOP_POTION_RED_ALT5:
            return 0x0f;
        }
        break;
    case SCE_MM_GORON_SHOP:
        switch (girlA->base.variable)
        {
        case SHOP_BOMBS_10:
        case SHOP_BOMBS_10_ALT:
        case SHOP_BOMBS_10_ALT2:
            return 0x10;
        case SHOP_ARROWS_10:
        case SHOP_ARROWS_10_ALT:
        case SHOP_ARROWS_10_ALT2:
            return 0x11;
        case SHOP_POTION_RED:
        case SHOP_POTION_RED_ALT:
        case SHOP_POTION_RED_ALT2:
        case SHOP_POTION_RED_ALT3:
        case SHOP_POTION_RED_ALT4:
        case SHOP_POTION_RED_ALT5:
            return 0x12;
        }
        break;
    case SCE_MM_ZORA_HALL_ROOMS:
        switch (girlA->base.variable)
        {
        case SHOP_SHIELD_HERO:
        case SHOP_SHIELD_HERO_ALT:
        case SHOP_SHIELD_HERO_ALT2:
            return 0x13;
        case SHOP_ARROWS_10:
        case SHOP_ARROWS_10_ALT:
        case SHOP_ARROWS_10_ALT2:
            return 0x14;
        case SHOP_POTION_RED:
        case SHOP_POTION_RED_ALT:
        case SHOP_POTION_RED_ALT2:
        case SHOP_POTION_RED_ALT3:
        case SHOP_POTION_RED_ALT4:
        case SHOP_POTION_RED_ALT5:
            return 0x15;
        }
        break;
    }

    UNREACHABLE();
}
