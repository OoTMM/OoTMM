#include <combo.h>
#include <combo/item.h>

static int hasFreeBottleOot(void)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_OOT_BOTTLE_EMPTY)
            return 1;
    }
    return 0;
}

static int hasFreeBottleMm(void)
{
    for (int i = 0; i < 6; ++i)
    {
        if (gMmSave.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_BOTTLE_EMPTY)
            return 1;
    }
    return 0;
}

int isItemBuyable(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_RECOVERY_HEART:
        return gOotSave.playerData.health < gOotSave.playerData.healthMax;
    case GI_OOT_STICK:
    case GI_OOT_STICKS_5:
    case GI_OOT_STICKS_10:
        return (gOotSave.inventory.upgrades.dekuStick == 0) || (gOotSave.inventory.ammo[ITS_OOT_STICKS] < kMaxSticks[gOotSave.inventory.upgrades.dekuStick]);
    case GI_OOT_NUTS_5:
    case GI_OOT_NUTS_5_ALT:
    case GI_OOT_NUTS_10:
        return (gOotSave.inventory.upgrades.dekuNut == 0) || (gOotSave.inventory.ammo[ITS_OOT_NUTS] < kMaxNuts[gOotSave.inventory.upgrades.dekuNut]);
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
        return (gOotSave.inventory.upgrades.bombBag > 0) && (gOotSave.inventory.ammo[ITS_OOT_BOMBS] < kMaxBombs[gOotSave.inventory.upgrades.bombBag]);
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
        return (gOotSave.inventory.ammo[ITS_OOT_BOMBCHU] < 50);
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
        return (gOotSave.inventory.upgrades.quiver > 0) && (gOotSave.inventory.ammo[ITS_OOT_BOW] < kMaxArrows[gOotSave.inventory.upgrades.quiver]);
    case GI_OOT_DEKU_SEEDS_5:
    case GI_OOT_DEKU_SEEDS_30:
        return (gOotSave.inventory.upgrades.bulletBag > 0) && (gOotSave.inventory.ammo[ITS_OOT_SLINGSHOT] < kMaxSeeds[gOotSave.inventory.upgrades.bulletBag]);
    case GI_OOT_RUPEE_GREEN:
    case GI_OOT_RUPEE_BLUE:
    case GI_OOT_RUPEE_RED:
    case GI_OOT_RUPEE_PURPLE:
    case GI_OOT_RUPEE_HUGE:
        return gOotSave.playerData.rupees < gMaxRupees[gOotSave.inventory.upgrades.wallet];
    case GI_OOT_POTION_RED:
    case GI_OOT_POTION_GREEN:
    case GI_OOT_POTION_BLUE:
    case GI_OOT_FISH:
    case GI_OOT_BUG:
    case GI_OOT_BLUE_FIRE:
    case GI_OOT_POE:
    case GI_OOT_BIG_POE:
    case GI_OOT_FAIRY:
    case GI_OOT_MILK:
        return hasFreeBottleOot();
    case GI_OOT_SHIELD_DEKU:
        return !(gOotSave.inventory.equipment.shields & EQ_OOT_SHIELD_DEKU);
    case GI_OOT_SHIELD_HYLIAN:
        return !(gOotSave.inventory.equipment.shields & EQ_OOT_SHIELD_HYLIAN);
    case GI_MM_RECOVERY_HEART:
        return gMmSave.playerData.health < gMmSave.playerData.healthMax;
    case GI_MM_STICK:
        return (gMmSave.inventory.upgrades.dekuStick == 0) || (gMmSave.inventory.ammo[ITS_MM_STICKS] < kMaxSticks[gMmSave.inventory.upgrades.dekuStick]);
    case GI_MM_NUT:
    case GI_MM_NUTS_5:
    case GI_MM_NUTS_10:
        return (gMmSave.inventory.upgrades.dekuNut == 0) || (gMmSave.inventory.ammo[ITS_MM_NUTS] < kMaxNuts[gMmSave.inventory.upgrades.dekuNut]);
    case GI_MM_BOMB:
    case GI_MM_BOMBS_5:
    case GI_MM_BOMBS_10:
    case GI_MM_BOMBS_20:
    case GI_MM_BOMBS_30:
        return (gMmSave.inventory.upgrades.bombBag > 0) && (gMmSave.inventory.ammo[ITS_MM_BOMBS] < kMaxBombs[gMmSave.inventory.upgrades.bombBag]);
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
        return (gMmSave.inventory.upgrades.bombBag > 0) && (gMmSave.inventory.ammo[ITS_MM_BOMBCHU] < 40);
    case GI_MM_ARROWS_10:
    case GI_MM_ARROWS_30:
    case GI_MM_ARROWS_40:
        return (gMmSave.inventory.upgrades.quiver > 0) && (gMmSave.inventory.ammo[ITS_MM_BOW] < kMaxArrows[gMmSave.inventory.upgrades.quiver]);
    case GI_MM_RUPEE_GREEN:
    case GI_MM_RUPEE_BLUE:
    case GI_MM_RUPEE_RED:
    case GI_MM_RUPEE_PURPLE:
    case GI_MM_RUPEE_SILVER:
    case GI_MM_RUPEE_GOLD:
        return gMmSave.playerData.rupees < gMaxRupees[gMmSave.inventory.upgrades.wallet];
    case GI_MM_MAGIC_BEAN:
        return gMmSave.inventory.items[ITS_MM_BEANS] < 20;
    case GI_MM_POTION_RED:
    case GI_MM_POTION_GREEN:
    case GI_MM_POTION_BLUE:
    case GI_MM_FAIRY:
    case GI_MM_MILK:
    case GI_MM_CHATEAU:
    case GI_MM_BUGS:
    case GI_MM_FISH:
    case GI_MM_POE:
    case GI_MM_BIG_POE:
        return hasFreeBottleMm();
    case GI_MM_SHIELD_HERO:
        return gMmSave.itemEquips.shield == 0;
    case GI_NOTHING:
        return 0;
    default:
        return 1;
    }
}
