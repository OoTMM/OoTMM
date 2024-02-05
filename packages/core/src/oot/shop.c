#include <combo.h>

#define SHOP_NUTS_5                 0x00
#define SHOP_ARROWS_30              0x01
#define SHOP_ARROWS_50              0x02
#define SHOP_BOMBS_5_ALT            0x03
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
#define SHOP_POTION_RED_ALT         0x30
#define SHOP_POTION_RED_ALT2        0x31

u8 comboShopItemSlot(GameState_Play* play, Actor_EnGirlA* girlA)
{
    int ageMask;

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
    else if (play->sceneId == SCE_OOT_BOMBCHU_SHOP)
    {
        /* Bombchu shop */
        switch (girlA->base.variable)
        {
        case SHOP_BOMBCHU_20_1:     return 0x08;
        case SHOP_BOMBCHU_20_2:     return 0x09;
        case SHOP_BOMBCHU_20_3:     return 0x0a;
        case SHOP_BOMBCHU_20_4:     return 0x0b;
        case SHOP_BOMBCHU_10_1:     return 0x0c;
        case SHOP_BOMBCHU_10_2:     return 0x0d;
        case SHOP_BOMBCHU_10_3:     return 0x0e;
        case SHOP_BOMBCHU_10_4:     return 0x0f;
        }
    }
    else if (play->sceneId == SCE_OOT_ZORA_SHOP)
    {
        /* Zora shop */
        switch (girlA->base.variable)
        {
        case SHOP_POTION_RED_ALT2:  return 0x10;
        case SHOP_ARROWS_50:        return 0x11;
        case SHOP_FISH:             return 0x12;
        case SHOP_NUTS_5:           return 0x13;
        case SHOP_ARROWS_10:        return 0x14;
        case SHOP_ARROWS_30:        return 0x15;
        case SHOP_TUNIC_ZORA:       return 0x16;
        case SHOP_RECOVERY_HEART:   return 0x17;
        }
    }
    else if (play->sceneId == SCE_OOT_GORON_SHOP)
    {
        /* Goron shop */
        switch (girlA->base.variable)
        {
        case SHOP_RECOVERY_HEART:   return girlA->base.world.pos.x < -60.f ? 0x18 : 0x19;
        case SHOP_POTION_RED_ALT:   return 0x1a;
        case SHOP_TUNIC_GORON:      return 0x1b;
        case SHOP_BOMBS_5_ALT:      return 0x1c;
        case SHOP_BOMBS_10:         return 0x1d;
        case SHOP_BOMBS_20:         return 0x1e;
        case SHOP_BOMBS_30:         return 0x1f;
        }
    }
    else if (play->sceneId == SCE_OOT_BAZAAR)
    {
        /* Market / Kakariko Bazaar */
        ageMask = (gSave.entrance == ENTR_OOT_KAKARIKO_BAZAAR ? 0x10 : 0x00);
        switch (girlA->base.variable)
        {
        case SHOP_ARROWS_10:        return 0x20 | ageMask;
        case SHOP_ARROWS_30:        return 0x21 | ageMask;
        case SHOP_ARROWS_50:        return 0x22 | ageMask;
        case SHOP_STICK:            return 0x23 | ageMask;
        case SHOP_BOMBS_5:          return 0x24 | ageMask;
        case SHOP_RECOVERY_HEART:   return 0x25 | ageMask;
        case SHOP_NUTS_5:           return 0x26 | ageMask;
        case SHOP_SHIELD_HYLIAN:    return 0x27 | ageMask;
        }
    }
    else if (play->sceneId == SCE_OOT_MARKET_POTION_SHOP || play->sceneId == SCE_OOT_KAKARIKO_POTION_SHOP)
    {
        /* Market / Kakriko Potion Shop */
        ageMask = (play->sceneId == SCE_OOT_KAKARIKO_POTION_SHOP) ? 0x10 : 0x00;
        switch (girlA->base.variable)
        {
        case SHOP_FISH:         return 0x28 | ageMask;
        case SHOP_BUG:          return 0x29 | ageMask;
        case SHOP_POE:          return 0x2a | ageMask;
        case SHOP_NUTS_5:       return 0x2b | ageMask;
        case SHOP_BLUE_FIRE:    return 0x2c | ageMask;
        case SHOP_FAIRY:        return 0x2d | ageMask;
        case SHOP_POTION_GREEN: return 0x2e | ageMask;
        case SHOP_POTION_RED:   return 0x2f | ageMask;
        }
    }

    UNREACHABLE();
}
