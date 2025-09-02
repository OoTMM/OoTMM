#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>

static int hasFreeBottleOot(void)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_OOT_BOTTLE_EMPTY)
            return 1;
    }
    return 0;
}

static int hasFreeBottleMm(void)
{
    for (int i = 0; i < 6; ++i)
    {
        if (gMmSave.info.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_BOTTLE_EMPTY)
            return 1;
    }
    return 0;
}

static int canBuyShieldOotRaw(int level)
{
    switch (level)
    {
    case 0:
        return !(gOotSave.info.inventory.equipment.shields & EQ_OOT_SHIELD_DEKU);
    case 1:
        return !(gOotSave.info.inventory.equipment.shields & EQ_OOT_SHIELD_HYLIAN);
    default:
        return 1;
    }
}

static int canBuyShieldMmRaw(int level)
{
    switch (level)
    {
    case 0:
        return gMmSave.info.itemEquips.shield == 0;
    case 1:
        return gMmSave.info.itemEquips.shield == 0 || gSharedCustomSave.mmShieldIsDeku;
    default:
        return 1;
    }
}

static int canBuyShieldOot(int level)
{
    return canBuyShieldOotRaw(level) || (Config_Flag(CFG_SHARED_SHIELDS) && canBuyShieldMmRaw(level));
}

static int canBuyShieldMm(int level)
{
    return canBuyShieldMmRaw(level) || (Config_Flag(CFG_SHARED_SHIELDS) && canBuyShieldOotRaw(level));
}

int isItemBuyable(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_RECOVERY_HEART:
        return gOotSave.info.playerData.health < gOotSave.info.playerData.healthCapacity;
    case GI_OOT_MAGIC_JAR_SMALL:
    case GI_OOT_MAGIC_JAR_LARGE:
        return (gOotSave.info.playerData.isMagicAcquired && (gOotSave.info.playerData.magic < (gOotSave.info.playerData.isDoubleMagicAcquired ? 0x60 : 0x30)));
    case GI_OOT_FAIRY_BIG:
        return gOotSave.info.playerData.health < gOotSave.info.playerData.healthCapacity || (gOotSave.info.playerData.isMagicAcquired && (gOotSave.info.playerData.magic < (gOotSave.info.playerData.isDoubleMagicAcquired ? 0x60 : 0x30)));
    case GI_MM_FAIRY_BIG:
        return gMmSave.info.playerData.health < gMmSave.info.playerData.healthCapacity || (gMmSave.info.playerData.isMagicAcquired && (gMmSave.info.playerData.magic < (gMmSave.info.playerData.isDoubleMagicAcquired ? 0x60 : 0x30)));
    case GI_OOT_STICK:
    case GI_OOT_STICKS_5:
    case GI_OOT_STICKS_10:
        return (gOotSave.info.inventory.upgrades.dekuStick == 0) || (gOotSave.info.inventory.ammo[ITS_OOT_STICKS] < kMaxSticks[gOotSave.info.inventory.upgrades.dekuStick]);
    case GI_OOT_NUTS_5:
    case GI_OOT_NUTS_5_ALT:
    case GI_OOT_NUTS_10:
        return (gOotSave.info.inventory.upgrades.dekuNut == 0) || (gOotSave.info.inventory.ammo[ITS_OOT_NUTS] < kMaxNuts[gOotSave.info.inventory.upgrades.dekuNut]);
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
        return (gOotSave.info.inventory.upgrades.bombBag > 0) && (gOotSave.info.inventory.ammo[ITS_OOT_BOMBS] < kMaxBombs[gOotSave.info.inventory.upgrades.bombBag]);
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
        return (gOotSave.info.inventory.ammo[ITS_OOT_BOMBCHU] < gMaxBombchuOot);
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
        return (gOotSave.info.inventory.upgrades.quiver > 0) && (gOotSave.info.inventory.ammo[ITS_OOT_BOW] < kMaxArrows[gOotSave.info.inventory.upgrades.quiver]);
    case GI_OOT_DEKU_SEEDS_5:
    case GI_OOT_DEKU_SEEDS_30:
        return (gOotSave.info.inventory.upgrades.bulletBag > 0) && (gOotSave.info.inventory.ammo[ITS_OOT_SLINGSHOT] < kMaxSeeds[gOotSave.info.inventory.upgrades.bulletBag]);
    case GI_OOT_RUPEE_GREEN:
    case GI_OOT_RUPEE_BLUE:
    case GI_OOT_RUPEE_RED:
    case GI_OOT_RUPEE_PURPLE:
    case GI_OOT_RUPEE_HUGE:
        return gOotSave.info.playerData.rupees < gMaxRupees[gOotSave.info.inventory.upgrades.wallet];
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
        return canBuyShieldOot(0);
    case GI_OOT_SHIELD_HYLIAN:
        return canBuyShieldOot(1);
    case GI_MM_RECOVERY_HEART:
        return gMmSave.info.playerData.health < gMmSave.info.playerData.healthCapacity;
    case GI_MM_MAGIC_JAR_SMALL:
    case GI_MM_MAGIC_JAR_LARGE:
        return (gMmSave.info.playerData.isMagicAcquired && (gMmSave.info.playerData.magic < (gMmSave.info.playerData.isDoubleMagicAcquired ? 0x60 : 0x30)));
    case GI_MM_STICK:
        return (gMmSave.info.inventory.upgrades.dekuStick == 0) || (gMmSave.info.inventory.ammo[ITS_MM_STICKS] < kMaxSticks[gMmSave.info.inventory.upgrades.dekuStick]);
    case GI_MM_NUT:
    case GI_MM_NUTS_5:
    case GI_MM_NUTS_10:
        return (gMmSave.info.inventory.upgrades.dekuNut == 0) || (gMmSave.info.inventory.ammo[ITS_MM_NUTS] < kMaxNuts[gMmSave.info.inventory.upgrades.dekuNut]);
    case GI_MM_BOMB:
    case GI_MM_BOMBS_5:
    case GI_MM_BOMBS_10:
    case GI_MM_BOMBS_20:
    case GI_MM_BOMBS_30:
        return (gMmSave.info.inventory.upgrades.bombBag > 0) && (gMmSave.info.inventory.ammo[ITS_MM_BOMBS] < kMaxBombs[gMmSave.info.inventory.upgrades.bombBag]);
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
        return (gMmSave.info.inventory.ammo[ITS_MM_BOMBCHU] < gMaxBombchuMm);
    case GI_MM_ARROWS_10:
    case GI_MM_ARROWS_30:
    case GI_MM_ARROWS_40:
        return (gMmSave.info.inventory.upgrades.quiver > 0) && (gMmSave.info.inventory.ammo[ITS_MM_BOW] < kMaxArrows[gMmSave.info.inventory.upgrades.quiver]);
    case GI_MM_RUPEE_GREEN:
    case GI_MM_RUPEE_BLUE:
    case GI_MM_RUPEE_RED:
    case GI_MM_RUPEE_PURPLE:
    case GI_MM_RUPEE_SILVER:
    case GI_MM_RUPEE_GOLD:
        return gMmSave.info.playerData.rupees < gMaxRupees[gMmSave.info.inventory.upgrades.wallet];
    case GI_MM_MAGIC_BEAN:
        return gMmSave.info.inventory.items[ITS_MM_BEANS] < 20;
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
    case GI_MM_SHIELD_DEKU:
        return canBuyShieldMm(0);
    case GI_MM_SHIELD_HERO:
        return canBuyShieldMm(1);
    default:
        return 1;
    }
}
