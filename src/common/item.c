#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define comboAddItemNative             comboAddItemOot
# define comboAddItemSharedNative       comboAddItemSharedOot
# define comboAddItemForeign            comboAddItemMm
# define comboAddItemSharedForeign      comboAddItemSharedMm
#else
# define comboAddItemNative             comboAddItemMm
# define comboAddItemSharedNative       comboAddItemSharedMm
# define comboAddItemForeign            comboAddItemOot
# define comboAddItemSharedForeign      comboAddItemSharedOot
#endif

const u8 kMaxSticks[] = { 0, 10, 20, 30 };
const u8 kMaxNuts[] = { 0, 20, 30, 40 };
const u8 kMaxBombs[] = { 0, 20, 30, 40 };
const u8 kMaxArrows[] = { 0, 30, 40, 50 };
const u8 kMaxSeeds[] = { 0, 30, 40, 50 };
const u16 kMaxRupees[] = { 99, 200, 500 };

int comboAddItem(GameState_Play* play, s16 gi)
{
    int count;
    int count2;

    if (gi & MASK_FOREIGN_GI)
    {
        count = comboAddItemForeign(gi & ~MASK_FOREIGN_GI, 1);
        comboAddItemSharedForeign(gi & ~MASK_FOREIGN_GI, 0);
        comboAddItemSharedForeignEffect(play, gi & ~MASK_FOREIGN_GI);
    }
    else
    {
        count = comboAddItemNative(gi, 0);
        comboAddItemSharedNative(gi, 0);
        count2 = comboAddItemEffect(play, gi);
        if (!count)
            count = count2;
    }

    comboTextHijackItem(play, gi, count);
    return -1;
}

int comboAddItemNoEffect(s16 gi)
{
    if (gi & MASK_FOREIGN_GI)
    {
        comboAddItemForeign(gi & ~MASK_FOREIGN_GI, 1);
        comboAddItemSharedForeign(gi & ~MASK_FOREIGN_GI, 1);
    }
    else
    {
        comboAddItemNative(gi, 1);
        comboAddItemSharedNative(gi, 1);
    }
    return -1;
}

static int isItemUnavailableOot(s32 gi)
{
    switch (gi)
    {
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
        return gOotSave.inventory.upgrades.bombBag == 0;
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
        return gOotSave.inventory.upgrades.quiver == 0;
    case GI_OOT_DEKU_SEEDS_5:
    case GI_OOT_DEKU_SEEDS_30:
        return gOotSave.inventory.upgrades.bulletBag == 0;
    default:
        return 0;
    }
}

static int isItemUnavailableMm(s32 gi)
{
    switch (gi)
    {
    case GI_MM_BOMB:
    case GI_MM_BOMBS_5:
    case GI_MM_BOMBS_10:
    case GI_MM_BOMBS_20:
    case GI_MM_BOMBS_30:
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
        return gMmSave.inventory.upgrades.bombBag == 0;
    case GI_MM_ARROWS_10:
    case GI_MM_ARROWS_30:
    case GI_MM_ARROWS_40:
        return gMmSave.inventory.upgrades.quiver == 0;
    default:
        return 0;
    }
}

int comboIsItemUnavailable(s16 gi)
{
#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif
    if (gi & MASK_FOREIGN_GI)
        return isItemUnavailableMm(gi & ~MASK_FOREIGN_GI);
    else
        return isItemUnavailableOot(gi);
}

static int isItemMinorOot(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
    case GI_OOT_DEKU_SEEDS_5:
    case GI_OOT_DEKU_SEEDS_30:
        return 1;
    default:
        return 0;
    }
}

static int isItemMinorMm(s16 gi)
{
    switch (gi)
    {
    case GI_MM_BOMB:
    case GI_MM_BOMBS_5:
    case GI_MM_BOMBS_10:
    case GI_MM_BOMBS_20:
    case GI_MM_BOMBS_30:
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
    case GI_MM_ARROWS_10:
    case GI_MM_ARROWS_30:
    case GI_MM_ARROWS_40:
        return 1;
    default:
        return 0;
    }
}

int comboIsItemMinor(s16 gi)
{
#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif
    if (gi & MASK_FOREIGN_GI)
        return isItemMinorMm(gi & ~MASK_FOREIGN_GI);
    else
        return isItemMinorOot(gi);
}

static int isItemConsumableOot(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_RECOVERY_HEART:
    case GI_OOT_STICK:
    case GI_OOT_STICKS_5:
    case GI_OOT_STICKS_10:
    case GI_OOT_NUTS_5:
    case GI_OOT_NUTS_5_ALT:
    case GI_OOT_NUTS_10:
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
    case GI_OOT_DEKU_SEEDS_5:
    case GI_OOT_DEKU_SEEDS_30:
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
    case GI_OOT_SHIELD_DEKU:
    case GI_OOT_SHIELD_HYLIAN:
    case GI_OOT_MILK:
    case GI_OOT_POTION_RED:
    case GI_OOT_POTION_GREEN:
    case GI_OOT_POTION_BLUE:
    case GI_OOT_FAIRY:
    case GI_OOT_FISH:
    case GI_OOT_BUG:
    case GI_OOT_BLUE_FIRE:
    case GI_OOT_POE:
        return 1;
    default:
        return 0;
    }
}

static int isItemConsumableMm(s16 gi)
{
    switch (gi)
    {
    case GI_MM_RECOVERY_HEART:
    case GI_MM_STICK:
    case GI_MM_NUT:
    case GI_MM_NUTS_5:
    case GI_MM_NUTS_10:
    case GI_MM_BOMB:
    case GI_MM_BOMBS_5:
    case GI_MM_BOMBS_10:
    case GI_MM_BOMBS_20:
    case GI_MM_BOMBS_30:
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
    case GI_MM_ARROWS_10:
    case GI_MM_ARROWS_30:
    case GI_MM_ARROWS_40:
    case GI_MM_SHIELD_HERO:
    case GI_MM_MILK:
    case GI_MM_CHATEAU:
    case GI_MM_FISH:
    case GI_MM_BUGS:
    case GI_MM_FAIRY:
    case GI_MM_POTION_RED:
    case GI_MM_POTION_GREEN:
    case GI_MM_POTION_BLUE:
        return 1;
    default:
        return 0;
    }
}

int comboIsItemConsumable(s16 gi)
{
#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif
    if (gi & MASK_FOREIGN_GI)
        return isItemConsumableMm(gi ^ MASK_FOREIGN_GI);
    else
        return isItemConsumableOot(gi);
}


void comboSyncItems(void)
{
    if (comboConfig(CFG_SHARED_BOWS))
        gForeignSave.inventory.ammo[ITS_FOREIGN_BOW] = gSave.inventory.ammo[ITS_NATIVE_BOW];

    if (comboConfig(CFG_SHARED_BOMB_BAGS))
        gForeignSave.inventory.ammo[ITS_FOREIGN_BOMBS] = gSave.inventory.ammo[ITS_NATIVE_BOMBS];

    if (comboConfig(CFG_SHARED_MAGIC))
       gForeignSave.playerData.magicAmount = gSave.playerData.magicAmount;

    if (comboConfig(CFG_SHARED_NUTS_STICKS))
    {
        gForeignSave.inventory.ammo[ITS_FOREIGN_NUTS] = gSave.inventory.ammo[ITS_NATIVE_NUTS];
        gForeignSave.inventory.ammo[ITS_FOREIGN_STICKS] = gSave.inventory.ammo[ITS_NATIVE_STICKS];
    }

    if (comboConfig(CFG_SHARED_WALLETS))
        gForeignSave.playerData.rupees = gSave.playerData.rupees;

    if (comboConfig(CFG_SHARED_HEALTH))
    {
        gForeignSave.playerData.healthMax = gSave.playerData.healthMax;
        gForeignSave.playerData.health = gSave.playerData.health;
        gForeignSave.inventory.quest.heartPieces = gSave.inventory.quest.heartPieces;
    }
}

static int isItemBuyableOot(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
        if (gOotSave.inventory.upgrades.bombBag == 0 && gOotSave.inventory.items[ITS_OOT_BOMBCHU] != ITEM_OOT_BOMBCHU_10)
            return 0;
        break;
    }
    return 1;
}

static int isItemBuyableMm(s16 gi)
{
    return 1;
}

static int isItemBuyable(s16 gi)
{
#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif

    if (gi & MASK_FOREIGN_GI)
        return isItemBuyableMm(gi ^ MASK_FOREIGN_GI);
    else
        return isItemBuyableOot(gi);
}

int comboItemPrecond(s16 gi, s16 price)
{
    if (comboIsItemUnavailable(gi) || !isItemBuyable(gi))
        return SC_ERR_CANNOTBUY;

    if (gSave.playerData.rupees < price)
        return SC_ERR_NORUPEES;

    if (comboIsItemMinor(gi))
        return SC_OK_NOCUTSCENE;

    return SC_OK;
}
