#include <combo.h>

#define SHOP_MASK_ALL_NIGHT     0x13
#define SHOP_BOMB_BAG           0x14
#define SHOP_BOMB_BAG2          0x15
#define SHOP_BOMB_BAG_ALT       0x17
#define SHOP_BOMB_BAG2_ALT      0x18
#define SHOP_BOMBCHU_10         0x19
#define SHOP_BOMBS_10           0x1a

void shopWriteFlag(int flag)
{
    gMmExtraShops |= 1 << flag;
}

int shopReadFlag(int flag)
{
    return !!(gMmExtraShops & (1 << flag));
}

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
    }

    /* UNREACHABLE(); */
    return 0;
}
