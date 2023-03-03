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

int comboShopItemSlot(GameState_Play* play, Actor_EnGirlA* girlA)
{
    static int rhBit;

    int slot;
    int ageMask;

    slot = 0;
    ageMask = (gSave.age == AGE_ADULT) ? 0x10 : 0x00;

    if (play->sceneId == SCE_OOT_KOKIRI_SHOP)
    {
        /* Kokiri shop */
        switch (girlA->base.variable)
        {
        case SHOP_RECOVERY_HEART:   slot = 0x00; break;
        case SHOP_ARROWS_10:        slot = 0x01; break;
        case SHOP_ARROWS_30:        slot = 0x02; break;
        case SHOP_SEEDS_30:         slot = 0x03; break;
        case SHOP_NUTS_5:           slot = 0x04; break;
        case SHOP_NUTS_10:          slot = 0x05; break;
        case SHOP_STICK:            slot = 0x06; break;
        case SHOP_SHIELD_DEKU:      slot = 0x07; break;
        }
    }
    else if (play->sceneId == SCE_OOT_BOMBCHU_SHOP)
    {
        /* Bombchu shop */
        switch (girlA->base.variable)
        {
        case SHOP_BOMBCHU_20_1:     slot = 0x08; break;
        case SHOP_BOMBCHU_20_2:     slot = 0x09; break;
        case SHOP_BOMBCHU_20_3:     slot = 0x0a; break;
        case SHOP_BOMBCHU_20_4:     slot = 0x0b; break;
        case SHOP_BOMBCHU_10_1:     slot = 0x0c; break;
        case SHOP_BOMBCHU_10_2:     slot = 0x0d; break;
        case SHOP_BOMBCHU_10_3:     slot = 0x0e; break;
        case SHOP_BOMBCHU_10_4:     slot = 0x0f; break;
        }
    }
    else if (play->sceneId == SCE_OOT_GORON_SHOP)
    {
        /* Goron shop */
        switch (girlA->base.variable)
        {
        case SHOP_RECOVERY_HEART:   slot = rhBit ? 0x19 : 0x18; rhBit = !rhBit; break;
        case SHOP_POTION_RED_ALT:   slot = 0x1a; break;
        case SHOP_TUNIC_GORON:      slot = 0x1b; break;
        case SHOP_BOMBS_5_ALT:      slot = 0x1c; break;
        case SHOP_BOMBS_10:         slot = 0x1d; break;
        case SHOP_BOMBS_20:         slot = 0x1e; break;
        case SHOP_BOMBS_30:         slot = 0x1f; break;
        }
    }
    else if (play->sceneId == SCE_OOT_ZORA_SHOP)
    {
        /* Zora shop */
        switch (girlA->base.variable)
        {
        case SHOP_POTION_RED_ALT2:  slot = 0x10; break;
        case SHOP_ARROWS_50:        slot = 0x11; break;
        case SHOP_FISH:             slot = 0x12; break;
        case SHOP_NUTS_5:           slot = 0x13; break;
        case SHOP_ARROWS_10:        slot = 0x14; break;
        case SHOP_ARROWS_30:        slot = 0x15; break;
        case SHOP_TUNIC_ZORA:       slot = 0x16; break;
        case SHOP_RECOVERY_HEART:   slot = 0x17; break;
        }
    }
    else if (play->sceneId == SCE_OOT_BAZAAR)
    {
        /* Market / Kakariko Bazaar */
        switch (girlA->base.variable)
        {
        case SHOP_ARROWS_10:        slot = 0x20 | ageMask; break;
        case SHOP_ARROWS_30:        slot = 0x21 | ageMask; break;
        case SHOP_ARROWS_50:        slot = 0x22 | ageMask; break;
        case SHOP_STICK:            slot = 0x23 | ageMask; break;
        case SHOP_BOMBS_5:          slot = 0x24 | ageMask; break;
        case SHOP_RECOVERY_HEART:   slot = 0x25 | ageMask; break;
        case SHOP_NUTS_5:           slot = 0x26 | ageMask; break;
        case SHOP_SHIELD_HYLIAN:    slot = 0x27 | ageMask; break;
        }
    }
    else if (play->sceneId == SCE_OOT_MARKET_POTION_SHOP || play->sceneId == SCE_OOT_KAKARIKO_POTION_SHOP)
    {
        /* Market / Kakriko Potion Shop */
        switch (girlA->base.variable)
        {
        case SHOP_FISH:         slot = 0x28 | ageMask; break;
        case SHOP_BUG:          slot = 0x29 | ageMask; break;
        case SHOP_POE:          slot = 0x2a | ageMask; break;
        case SHOP_NUTS_5:       slot = 0x2b | ageMask; break;
        case SHOP_BLUE_FIRE:    slot = 0x2c | ageMask; break;
        case SHOP_FAIRY:        slot = 0x2d | ageMask; break;
        case SHOP_POTION_GREEN: slot = 0x2e | ageMask; break;
        case SHOP_POTION_RED:   slot = 0x2f | ageMask; break;
        }
    }

    return slot;
}
