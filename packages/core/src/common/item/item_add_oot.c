#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>

const u8 kOotTradeAdult[] = {
    ITEM_OOT_POCKET_EGG,
    ITEM_OOT_POCKET_CUCCO,
    ITEM_OOT_COJIRO,
    ITEM_OOT_ODD_MUSHROOM,
    ITEM_OOT_ODD_POTION,
    ITEM_OOT_POACHER_SAW,
    ITEM_OOT_GORON_SWORD_BROKEN,
    ITEM_OOT_PRESCRIPTION,
    ITEM_OOT_EYEBALL_FROG,
    ITEM_OOT_EYE_DROPS,
    ITEM_OOT_CLAIM_CHECK
};

const u8 kOotTradeChild[] = {
    ITEM_OOT_WEIRD_EGG,
    ITEM_OOT_CHICKEN,
    ITEM_OOT_ZELDA_LETTER,
    ITEM_OOT_KEATON_MASK,
    ITEM_OOT_SKULL_MASK,
    ITEM_OOT_SPOOKY_MASK,
    ITEM_OOT_BUNNY_HOOD,
    ITEM_OOT_GORON_MASK,
    ITEM_OOT_ZORA_MASK,
    ITEM_OOT_GERUDO_MASK,
    ITEM_OOT_MASK_OF_TRUTH,
};

int comboAddSmallKeyOot(u16 dungeonId)
{
    s8 keyCount;

    /* Check for max keys */
    if (dungeonId != SCE_OOT_TREASURE_SHOP && gOotSave.inventory.dungeonItems[dungeonId].maxKeys >= g.maxKeysOot[dungeonId])
        return 0;

    keyCount = gOotSave.inventory.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gOotSave.inventory.dungeonKeys[dungeonId] = keyCount;
    if (dungeonId == SCE_OOT_TREASURE_SHOP)
        return 0;
    else
        return ++gOotSave.inventory.dungeonItems[dungeonId].maxKeys;
}

void comboAddKeyRingOot(u16 dungeonId)
{
    for (int i = 0; i < g.maxKeysOot[dungeonId]; ++i)
        comboAddSmallKeyOot(dungeonId);
}

void comboAddSkeletonKeyOot(void)
{
    for (int i = 0; i < ARRAY_SIZE(g.maxKeysOot); ++i)
        comboAddKeyRingOot(i);
}

void comboAddBossKeyOot(u16 dungeonId)
{
    gOotSave.inventory.dungeonItems[dungeonId].bossKey = 1;
}

void comboAddCompassOot(u16 dungeonId)
{
    gOotSave.inventory.dungeonItems[dungeonId].compass = 1;
}

void comboAddMapOot(u16 dungeonId)
{
    gOotSave.inventory.dungeonItems[dungeonId].map = 1;
}

static void addHealth(u8 count)
{
    u16 health;

    health = (u16)count * 0x10;
    gOotSave.playerData.health += health;
    if (gOotSave.playerData.health > gOotSave.playerData.healthMax)
        gOotSave.playerData.health = gOotSave.playerData.healthMax;
}

void comboAddMagicUpgradeOot(int level)
{
    gOotSave.playerData.magicUpgrade = 1;
    if (level >= 2)
        gOotSave.playerData.magicUpgrade2 = 1;
}

static void refillMagic(int level)
{
    gOotSave.playerData.magicSize = level;
    gOotSave.playerData.magicAmount = level * 0x30;
}


static void addAmmo(u8 slot, u16 item, u8 max, u8 count)
{
    gOotSave.inventory.items[slot] = item;
    gOotSave.inventory.ammo[slot] += count;
    if (gOotSave.inventory.ammo[slot] > max)
        gOotSave.inventory.ammo[slot] = max;
}

void comboAddSticksOot(int count)
{
    u8 max;

    if (gOotSave.inventory.upgrades.dekuStick == 0)
        gOotSave.inventory.upgrades.dekuStick = 1;

    max = kMaxSticks[gOotSave.inventory.upgrades.dekuStick];
    addAmmo(ITS_OOT_STICKS, ITEM_OOT_STICK, max, count);
}

void comboAddNutsOot(int count)
{
    u8 max;

    if (gOotSave.inventory.upgrades.dekuNut == 0)
        gOotSave.inventory.upgrades.dekuNut = 1;

    max = kMaxNuts[gOotSave.inventory.upgrades.dekuNut];
    addAmmo(ITS_OOT_NUTS, ITEM_OOT_NUT, max, count);
}

void comboAddBombsOot(int count)
{
    u8 max;

    if (gOotSave.inventory.upgrades.bombBag == 0)
        return;
    max = kMaxBombs[gOotSave.inventory.upgrades.bombBag];
    addAmmo(ITS_OOT_BOMBS, ITEM_OOT_BOMB, max, count);
}

void comboAddArrowsOot(int count)
{
    int max;

    if (gOotSave.inventory.upgrades.quiver == 0)
        return;
    max = kMaxArrows[gOotSave.inventory.upgrades.quiver];
    addAmmo(ITS_OOT_BOW, ITEM_OOT_BOW, max, count);
}

static void addSeeds(u8 count)
{
    u8 max;

    if (gOotSave.inventory.upgrades.bulletBag == 0)
        return;
    max = kMaxSeeds[gOotSave.inventory.upgrades.bulletBag];
    addAmmo(ITS_OOT_SLINGSHOT, ITEM_OOT_SLINGSHOT, max, count);
    BITMAP16_SET(gOotSave.eventsItem, EV_OOT_ITEM_DEKU_SEEDS);
}

static void addBombchu(u8 count)
{
    addAmmo(ITS_OOT_BOMBCHU, ITEM_OOT_BOMBCHU_10, 50, count);
}

static void addNewBottle(u16 itemId)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_NONE)
        {
            gOotSave.inventory.items[ITS_OOT_BOTTLE + i] = itemId;
            return;
        }
    }
}

static void fillBottle(u16 itemId)
{
    int slot;

    slot = -1;
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_OOT_BOTTLE_EMPTY)
        {
            slot = i;
            break;
        }
    }
    if (slot == -1)
        return;
    gOotSave.inventory.items[ITS_OOT_BOTTLE + slot] = itemId;
    for (int i = 0; i < 3; ++i)
    {
        if (gOotSave.equips.cButtonSlots[i] == ITS_OOT_BOTTLE + slot)
        {
            gOotSave.equips.buttonItems[1 + i] = itemId;
        }
    }
}

void comboAddBombBagOot(int level)
{
    gOotSave.inventory.items[ITS_OOT_BOMBS] = ITEM_OOT_BOMB;
    gOotSave.inventory.upgrades.bombBag = level;
    gOotSave.inventory.ammo[ITS_OOT_BOMBS] = kMaxBombs[level];
}

void comboAddQuiverOot(int level)
{
    gOotSave.inventory.items[ITS_OOT_BOW] = ITEM_OOT_BOW;
    gOotSave.inventory.upgrades.quiver = level;
    gOotSave.inventory.ammo[ITS_OOT_BOW] = kMaxArrows[level];
}

static void addBulletBag(u8 level)
{
    gOotSave.inventory.items[ITS_OOT_SLINGSHOT] = ITEM_OOT_SLINGSHOT;
    gOotSave.inventory.upgrades.bulletBag = level;
    gOotSave.inventory.ammo[ITS_OOT_SLINGSHOT] = kMaxSeeds[level];
    BITMAP16_SET(gOotSave.eventsItem, EV_OOT_ITEM_DEKU_SEEDS);
}

static void addNutUpgrade(int level)
{
    gOotSave.inventory.upgrades.dekuNut = level;
    comboAddNutsOot(kMaxNuts[level]);
}

static void addStickUpgrade(int level)
{
    gOotSave.inventory.upgrades.dekuStick = level;
    comboAddSticksOot(kMaxSticks[level]);
}

static void addNutUpgradeMm(int level)
{
    gMmSave.inventory.upgrades.dekuNut = level;
    comboAddNutsMm(kMaxNuts[level]);
}

static void addStickUpgradeMm(int level)
{
    gMmSave.inventory.upgrades.dekuStick = level;
    comboAddSticksMm(kMaxSticks[level]);
}

static void addTradeChild(u8 index)
{
    u16 itemId;
    u16 mask;

    itemId = kOotTradeChild[index];
    mask = 1 << (u16)index;
    if (gOotExtraTradeSave.child & mask)
        return;
    if (gOotSave.inventory.items[ITS_OOT_TRADE_CHILD] == ITEM_NONE)
        gOotSave.inventory.items[ITS_OOT_TRADE_CHILD] = itemId;
    gOotExtraTrade.child |= mask;
    gOotExtraTradeSave.child |= mask;
}

static void addTradeAdult(u8 index)
{
    u16 itemId;
    u16 mask;

    itemId = kOotTradeAdult[index];
    mask = 1 << (u16)index;
    if (gOotExtraTradeSave.adult & mask)
        return;
    if (gOotSave.inventory.items[ITS_OOT_TRADE_ADULT] == ITEM_NONE)
        gOotSave.inventory.items[ITS_OOT_TRADE_ADULT] = itemId;
    gOotExtraTrade.adult |= mask;
    gOotExtraTradeSave.adult |= mask;
}

static void addRupees(u16 count)
{
    u16 max;

    max = gOotMaxRupees[gOotSave.inventory.upgrades.wallet];
    gOotSave.playerData.rupees += count;
    if (gOotSave.playerData.rupees > max)
        gOotSave.playerData.rupees = max;
}

static int addSilverRupees(int id, int noEffect)
{
    if (noEffect)
        addRupees(5);
    return comboSilverRupeesIncCount(id, noEffect);
}

static void addWallet(int index, int noEffect)
{
    if (index == 0)
    {
        gOotExtraFlags.childWallet = 1;
        gOotMaxRupees[0] = 99;
    }
    else if (index == 4)
    {
        gOotExtraFlags.bottomlessWallet = 1;
        gOotMaxRupees[3] = 9999;
#if defined(GAME_OOT)
        gWalletDigits[3] = 4;
#endif
    }
    else
        gOotSave.inventory.upgrades.wallet = index;
    if (noEffect && comboConfig(CFG_FILL_WALLETS))
        addRupees(gOotMaxRupees[gOotSave.inventory.upgrades.wallet]);
}

static void reloadSlotEquips(OotItemEquips* equips, int slot)
{
    for (int i = 0; i < 3; ++i)
    {
        if (equips->cButtonSlots[i] == slot)
        {
            equips->buttonItems[1 + i] = gOotSave.inventory.items[slot];
        }
    }
}

static void reloadSlot(int slot)
{
    reloadSlotEquips(&gOotSave.equips, slot);
    reloadSlotEquips(&gOotSave.childEquips, slot);
    reloadSlotEquips(&gOotSave.adultEquips, slot);
}

static void addHookshot(int level)
{
    u16 itemId;

    if (level >= 2)
        itemId = ITEM_OOT_LONGSHOT;
    else
        itemId = ITEM_OOT_HOOKSHOT;
    gOotSave.inventory.items[ITS_OOT_HOOKSHOT] = itemId;
    gOotExtraItems.hookshot |= (1 << (level - 1));
    reloadSlot(ITS_OOT_HOOKSHOT);
}

static void addOcarina(int level)
{
    u16 itemId;

    if (level >= 2)
        itemId = ITEM_OOT_OCARINA_TIME;
    else
        itemId = ITEM_OOT_OCARINA_FAIRY;
    gOotSave.inventory.items[ITS_OOT_OCARINA] = itemId;
    gOotExtraItems.ocarina |= (1 << (level - 1));
    reloadSlot(ITS_OOT_OCARINA);
}

void comboAddSoulOot(s16 gi)
{
    int index;

    index = gi - GI_OOT_SOUL_STALFOS;
    if (index >= 32)
        gOotSouls2 |= (1 << (index - 32));
    else
        gOotSouls1 |= (1 << index);
}

void comboAddCommonItemOot(int sid, int noEffect)
{
    switch (sid)
    {
    case SITEM_ARROW_FIRE:
        gOotSave.inventory.items[ITS_OOT_ARROW_FIRE] = ITEM_OOT_ARROW_FIRE;
        break;
    case SITEM_ARROW_ICE:
        gOotSave.inventory.items[ITS_OOT_ARROW_ICE] = ITEM_OOT_ARROW_ICE;
        break;
    case SITEM_ARROW_LIGHT:
        gOotSave.inventory.items[ITS_OOT_ARROW_LIGHT] = ITEM_OOT_ARROW_LIGHT;
        break;
    case SITEM_SONG_EPONA:
        gOotSave.inventory.quest.songEpona = 1;
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_EPONA);
        break;
    case SITEM_SONG_TIME:
        gOotSave.inventory.quest.songTime = 1;
        break;
    case SITEM_SONG_STORMS:
        gOotSave.inventory.quest.songStorms = 1;
        break;
    case SITEM_SONG_SUN:
        gOotSave.inventory.quest.songSun = 1;
        break;
    case SITEM_HOOKSHOT:
        addHookshot(1);
        break;
    case SITEM_LONGSHOT:
        addHookshot(2);
        break;
    case SITEM_LENS:
        gOotSave.inventory.items[ITS_OOT_LENS] = ITEM_OOT_LENS;
        break;
    case SITEM_OCARINA_FAIRY:
        addOcarina(1);
        break;
    case SITEM_OCARINA_TIME:
        addOcarina(2);
        break;
    case SITEM_MASK_KEATON:
        addTradeChild(3);
        break;
    case SITEM_MASK_BUNNY:
        addTradeChild(6);
        break;
    case SITEM_MASK_GORON:
        addTradeChild(7);
        break;
    case SITEM_MASK_ZORA:
        addTradeChild(8);
        break;
    case SITEM_MASK_TRUTH:
        addTradeChild(10);
        break;
    case SITEM_WALLET:
        addWallet(0, noEffect);
        break;
    case SITEM_WALLET2:
        addWallet(1, noEffect);
        break;
    case SITEM_WALLET3:
        addWallet(2, noEffect);
        break;
    case SITEM_WALLET4:
        addWallet(3, noEffect);
        break;
    case SITEM_WALLET5:
        addWallet(4, noEffect);
        break;
    case SITEM_RUPEE_GREEN:
        if (noEffect)
            addRupees(1);
        break;
    case SITEM_RUPEE_BLUE:
        if (noEffect)
            addRupees(5);
        break;
    case SITEM_RUPEE_RED:
        if (noEffect)
            addRupees(20);
        break;
    case SITEM_RUPEE_PURPLE:
        if (noEffect)
            addRupees(50);
        break;
    case SITEM_RUPEE_SILVER:
        if (noEffect)
            addRupees(100);
        break;
    case SITEM_RUPEE_GOLD:
        if (noEffect)
            addRupees(200);
        break;
    case SITEM_HEART_PIECE:
        gOotSave.inventory.quest.heartPieces++;
        if (noEffect)
        {
            if (gOotSave.inventory.quest.heartPieces >= 4)
            {
                gOotSave.inventory.quest.heartPieces -= 4;
                gOotSave.playerData.healthMax += 0x10;
            }
            addHealth(20);
        }
        break;
    case SITEM_HEART_CONTAINER:
        gOotSave.playerData.healthMax += 0x10;
        if (noEffect)
            addHealth(20);
        break;
    case SITEM_RECOVERY_HEART:
        if (noEffect)
            addHealth(1);
        break;
    case SITEM_DEFENSE_UPGRADE:
        gOotSave.playerData.doubleDefense = 1;
        gOotSave.inventory.doubleDefenseHearts = 20;
        if (noEffect)
            addHealth(20);
        break;
    }
}

void comboAddItemSharedOot(s16 gi, int noEffect)
{
    if (comboConfig(CFG_SHARED_BOWS))
    {
        switch (gi)
        {
        case GI_OOT_BOW:
            comboAddQuiverMm(1);
            break;
        case GI_OOT_QUIVER2:
            comboAddQuiverMm(2);
            break;
        case GI_OOT_QUIVER3:
            comboAddQuiverMm(3);
            break;
        case GI_OOT_ARROWS_5:
            comboAddArrowsMm(5);
            break;
        case GI_OOT_ARROWS_10:
            comboAddArrowsMm(10);
            break;
        case GI_OOT_ARROWS_30:
            comboAddArrowsMm(30);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_BOMB_BAGS))
    {
        switch (gi)
        {
        case GI_OOT_BOMB_BAG:
            comboAddBombBagMm(1);
            break;
        case GI_OOT_BOMB_BAG2:
            comboAddBombBagMm(2);
            break;
        case GI_OOT_BOMB_BAG3:
            comboAddBombBagMm(3);
            break;
        case GI_OOT_BOMB:
            comboAddBombsMm(1);
            break;
        case GI_OOT_BOMBS_5:
            comboAddBombsMm(5);
            break;
        case GI_OOT_BOMBS_10:
            comboAddBombsMm(10);
            break;
        case GI_OOT_BOMBS_20:
            comboAddBombsMm(20);
            break;
        case GI_OOT_BOMBS_30:
            comboAddBombsMm(30);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_MAGIC))
    {
        switch (gi)
        {
        case GI_OOT_MAGIC_UPGRADE:
            comboAddMagicUpgradeMm(1);
            break;
        case GI_OOT_MAGIC_UPGRADE2:
            comboAddMagicUpgradeMm(2);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_MAGIC_ARROW_FIRE) && gi == GI_OOT_ARROW_FIRE)
    {
        comboAddCommonItemMm(SITEM_ARROW_FIRE, noEffect);
    }

    if (comboConfig(CFG_SHARED_MAGIC_ARROW_ICE) && gi == GI_OOT_ARROW_ICE)
    {
        comboAddCommonItemMm(SITEM_ARROW_ICE, noEffect);
    }

    if (comboConfig(CFG_SHARED_MAGIC_ARROW_LIGHT) && gi == GI_OOT_ARROW_LIGHT)
    {
        comboAddCommonItemMm(SITEM_ARROW_LIGHT, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_EPONA) && gi == GI_OOT_SONG_EPONA)
    {
        comboAddCommonItemMm(SITEM_SONG_EPONA, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_STORMS) && gi == GI_OOT_SONG_STORMS)
    {
        comboAddCommonItemMm(SITEM_SONG_STORMS, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_TIME) && gi == GI_OOT_SONG_TIME)
    {
        comboAddCommonItemMm(SITEM_SONG_TIME, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_SUN) && comboConfig(CFG_MM_SONG_SUN) && gi == GI_OOT_SONG_SUN)
    {
        comboAddCommonItemMm(SITEM_SONG_SUN, noEffect);
    }

    if (comboConfig(CFG_SHARED_NUTS_STICKS))
    {
        switch (gi)
        {
        case GI_OOT_NUTS_5:
        case GI_OOT_NUTS_5_ALT:
            comboAddNutsMm(5);
            break;
        case GI_OOT_NUTS_10:
            comboAddNutsMm(10);
            break;
        case GI_OOT_STICK:
            comboAddSticksMm(1);
            break;
        case GI_OOT_STICKS_5:
            comboAddSticksMm(5);
            break;
        case GI_OOT_STICKS_10:
            comboAddSticksMm(10);
            break;
        case GI_OOT_NUT_UPGRADE:
            addNutUpgradeMm(2);
            break;
        case GI_OOT_NUT_UPGRADE2:
            addNutUpgradeMm(3);
            break;
        case GI_OOT_STICK_UPGRADE:
            addStickUpgradeMm(2);
            break;
        case GI_OOT_STICK_UPGRADE2:
            addStickUpgradeMm(3);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_HOOKSHOT))
    {
        switch (gi)
        {
        case GI_OOT_HOOKSHOT:
            if (comboConfig(CFG_MM_HOOKSHOT_SHORT))
                comboAddCommonItemMm(SITEM_HOOKSHOT, noEffect);
            else
                comboAddCommonItemMm(SITEM_LONGSHOT, noEffect);
            break;
        case GI_OOT_LONGSHOT:
            comboAddCommonItemMm(SITEM_LONGSHOT, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_LENS) && gi == GI_OOT_LENS)
    {
        comboAddCommonItemMm(SITEM_LENS, noEffect);
    }

    if (comboConfig(CFG_SHARED_OCARINA))
    {
        switch (gi)
        {
        case GI_OOT_OCARINA_FAIRY:
            if (comboConfig(CFG_MM_OCARINA_FAIRY))
                comboAddCommonItemMm(SITEM_OCARINA_FAIRY, noEffect);
            break;
        case GI_OOT_OCARINA_TIME:
            comboAddCommonItemMm(SITEM_OCARINA_TIME, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_MASK_KEATON) && gi == GI_OOT_MASK_KEATON)
        comboAddCommonItemMm(SITEM_MASK_KEATON, noEffect);

    if (comboConfig(CFG_SHARED_MASK_BUNNY) && gi == GI_OOT_MASK_BUNNY)
        comboAddCommonItemMm(SITEM_MASK_BUNNY, noEffect);

    if (comboConfig(CFG_SHARED_MASK_TRUTH) && gi == GI_OOT_MASK_TRUTH)
        comboAddCommonItemMm(SITEM_MASK_TRUTH, noEffect);

    if (comboConfig(CFG_SHARED_MASK_GORON) && gi == GI_OOT_MASK_GORON)
        comboAddCommonItemMm(SITEM_MASK_GORON, noEffect);

    if (comboConfig(CFG_SHARED_MASK_ZORA) && gi == GI_OOT_MASK_ZORA)
        comboAddCommonItemMm(SITEM_MASK_ZORA, noEffect);

    if (comboConfig(CFG_SHARED_WALLETS))
    {
        switch (gi)
        {
        case GI_OOT_WALLET:
            comboAddCommonItemMm(SITEM_WALLET, noEffect);
            break;
        case GI_OOT_WALLET2:
            comboAddCommonItemMm(SITEM_WALLET2, noEffect);
            break;
        case GI_OOT_WALLET3:
            comboAddCommonItemMm(SITEM_WALLET3, noEffect);
            break;
        case GI_OOT_WALLET4:
            comboAddCommonItemMm(SITEM_WALLET4, noEffect);
            break;
        case GI_OOT_WALLET5:
            comboAddCommonItemMm(SITEM_WALLET5, noEffect);
            break;
        case GI_OOT_RUPEE_GREEN:
        case GI_OOT_TC_RUPEE_GREEN:
            comboAddCommonItemMm(SITEM_RUPEE_GREEN, noEffect);
            break;
        case GI_OOT_RUPEE_BLUE:
        case GI_OOT_TC_RUPEE_BLUE:
            comboAddCommonItemMm(SITEM_RUPEE_BLUE, noEffect);
            break;
        case GI_OOT_RUPEE_RED:
        case GI_OOT_TC_RUPEE_RED:
            comboAddCommonItemMm(SITEM_RUPEE_RED, noEffect);
            break;
        case GI_OOT_RUPEE_PURPLE:
        case GI_OOT_TC_RUPEE_PURPLE:
            comboAddCommonItemMm(SITEM_RUPEE_PURPLE, noEffect);
            break;
        case GI_OOT_RUPEE_HUGE:
            comboAddCommonItemMm(SITEM_RUPEE_GOLD, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_HEALTH))
    {
        switch (gi)
        {
        case GI_OOT_RECOVERY_HEART:
            comboAddCommonItemMm(SITEM_RECOVERY_HEART, noEffect);
            break;
        case GI_OOT_HEART_CONTAINER:
        case GI_OOT_HEART_CONTAINER2:
            comboAddCommonItemMm(SITEM_HEART_CONTAINER, noEffect);
            break;
        case GI_OOT_HEART_PIECE:
        case GI_OOT_TC_HEART_PIECE:
            comboAddCommonItemMm(SITEM_HEART_PIECE, noEffect);
            break;
        case GI_OOT_DEFENSE_UPGRADE:
            comboAddCommonItemMm(SITEM_DEFENSE_UPGRADE, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_SOULS))
    {
        switch (gi)
        {
        case GI_OOT_SOUL_OCTOROK:
            comboAddSoulMm(GI_MM_SOUL_OCTOROK);
            break;
        case GI_OOT_SOUL_WALLMASTER:
            comboAddSoulMm(GI_MM_SOUL_WALLMASTER);
            break;
        case GI_OOT_SOUL_DODONGO:
            comboAddSoulMm(GI_MM_SOUL_DODONGO);
            break;
        case GI_OOT_SOUL_KEESE:
            comboAddSoulMm(GI_MM_SOUL_KEESE);
            break;
        case GI_OOT_SOUL_TEKTITE:
            comboAddSoulMm(GI_MM_SOUL_TEKTITE);
            break;
        case GI_OOT_SOUL_PEAHAT:
            comboAddSoulMm(GI_MM_SOUL_PEAHAT);
            break;
        case GI_OOT_SOUL_LIZALFOS_DINALFOS:
            comboAddSoulMm(GI_MM_SOUL_LIZALFOS_DINALFOS);
            break;
        case GI_OOT_SOUL_SKULLTULA:
            comboAddSoulMm(GI_MM_SOUL_SKULLTULA);
            break;
        case GI_OOT_SOUL_ARMOS:
            comboAddSoulMm(GI_MM_SOUL_ARMOS);
            break;
        case GI_OOT_SOUL_DEKU_BABA:
            comboAddSoulMm(GI_MM_SOUL_DEKU_BABA);
            break;
        case GI_OOT_SOUL_DEKU_SCRUB:
            comboAddSoulMm(GI_MM_SOUL_DEKU_SCRUB);
            break;
        case GI_OOT_SOUL_BUBBLE:
            comboAddSoulMm(GI_MM_SOUL_BUBBLE);
            break;
        case GI_OOT_SOUL_BEAMOS:
            comboAddSoulMm(GI_MM_SOUL_BEAMOS);
            break;
        case GI_OOT_SOUL_REDEAD_GIBDO:
            comboAddSoulMm(GI_MM_SOUL_REDEAD_GIBDO);
            break;
        case GI_OOT_SOUL_SKULLWALLTULA:
            comboAddSoulMm(GI_MM_SOUL_SKULLWALLTULA);
            break;
        case GI_OOT_SOUL_SHELL_BLADE:
            comboAddSoulMm(GI_MM_SOUL_SHELL_BLADE);
            break;
        case GI_OOT_SOUL_LIKE_LIKE:
            comboAddSoulMm(GI_MM_SOUL_LIKE_LIKE);
            break;
        case GI_OOT_SOUL_IRON_KNUCKLE:
            comboAddSoulMm(GI_MM_SOUL_IRON_KNUCKLE);
            break;
        case GI_OOT_SOUL_FREEZARD:
            comboAddSoulMm(GI_MM_SOUL_FREEZARD);
            break;
        case GI_OOT_SOUL_WOLFOS:
            comboAddSoulMm(GI_MM_SOUL_WOLFOS);
            break;
        case GI_OOT_SOUL_GUAY:
            comboAddSoulMm(GI_MM_SOUL_GUAY);
            break;
        case GI_OOT_SOUL_FLYING_POT:
            comboAddSoulMm(GI_MM_SOUL_FLYING_POT);
            break;
        case GI_OOT_SOUL_FLOORMASTER:
            comboAddSoulMm(GI_MM_SOUL_FLOORMASTER);
            break;
        case GI_OOT_SOUL_LEEVER:
            comboAddSoulMm(GI_MM_SOUL_LEEVER);
            break;
        case GI_OOT_SOUL_STALCHILD:
            comboAddSoulMm(GI_MM_SOUL_STALCHILD);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_SKELETON_KEY) && gi == GI_OOT_SKELETON_KEY)
        comboAddSkeletonKeyMm();
}

int comboAddItemOot(s16 gi, int noEffect)
{
    int count;
    u16 dungeonId;

    count = 0;
    (void)dungeonId;
    switch (gi)
    {
    case GI_OOT_STICK:
        comboAddSticksOot(1);
        break;
    case GI_OOT_STICKS_5:
        comboAddSticksOot(5);
        break;
    case GI_OOT_STICKS_10:
        comboAddSticksOot(10);
        break;
    case GI_OOT_NUTS_5:
    case GI_OOT_NUTS_5_ALT:
        comboAddNutsOot(5);
        break;
    case GI_OOT_NUTS_10:
        comboAddNutsOot(10);
        break;
    case GI_OOT_BOMB:
        comboAddBombsOot(1);
        break;
    case GI_OOT_BOMBS_5:
        comboAddBombsOot(5);
        break;
    case GI_OOT_BOMBS_10:
        comboAddBombsOot(10);
        break;
    case GI_OOT_BOMBS_20:
        comboAddBombsOot(20);
        break;
    case GI_OOT_BOMBS_30:
        comboAddBombsOot(30);
        break;
    case GI_OOT_BOW:
        comboAddQuiverOot(1);
        break;
    case GI_OOT_ARROWS_5:
        comboAddArrowsOot(5);
        break;
    case GI_OOT_ARROWS_10:
        comboAddArrowsOot(10);
        break;
    case GI_OOT_ARROWS_30:
        comboAddArrowsOot(30);
        break;
    case GI_OOT_ARROW_FIRE:
        comboAddCommonItemOot(SITEM_ARROW_FIRE, noEffect);
        break;
    case GI_OOT_SPELL_FIRE:
        gOotSave.inventory.items[ITS_OOT_SPELL_FIRE] = ITEM_OOT_SPELL_FIRE;
        break;
    case GI_OOT_SLINGSHOT:
    case GI_OOT_BULLET_BAG:
        addBulletBag(1);
        break;
    case GI_OOT_DEKU_SEEDS_5:
        addSeeds(5);
        break;
    case GI_OOT_DEKU_SEEDS_30:
        addSeeds(30);
        break;
    case GI_OOT_OCARINA_FAIRY:
        addOcarina(1);
        break;
    case GI_OOT_OCARINA_TIME:
        comboAddCommonItemOot(SITEM_OCARINA_TIME, noEffect);
        break;
    case GI_OOT_BOMBCHU_5:
        addBombchu(5);
        break;
    case GI_OOT_BOMBCHU_10:
        addBombchu(10);
        break;
    case GI_OOT_BOMBCHU_20:
        addBombchu(20);
        break;
    case GI_OOT_HOOKSHOT:
        comboAddCommonItemOot(SITEM_HOOKSHOT, noEffect);
        break;
    case GI_OOT_LONGSHOT:
        comboAddCommonItemOot(SITEM_LONGSHOT, noEffect);
        break;
    case GI_OOT_ARROW_ICE:
        comboAddCommonItemOot(SITEM_ARROW_ICE, noEffect);
        break;
    case GI_OOT_SPELL_WIND:
        gOotSave.inventory.items[ITS_OOT_SPELL_WIND] = ITEM_OOT_SPELL_WIND;
        break;
    case GI_OOT_BOOMERANG:
        gOotSave.inventory.items[ITS_OOT_BOOMERANG] = ITEM_OOT_BOOMERANG;
        break;
    case GI_OOT_LENS:
        comboAddCommonItemOot(SITEM_LENS, noEffect);
        break;
    case GI_OOT_MAGIC_BEAN:
        gOotSave.inventory.items[ITS_OOT_MAGIC_BEAN] = ITEM_OOT_MAGIC_BEAN;
        gOotSave.inventory.ammo[ITS_OOT_MAGIC_BEAN] = 10;
        break;
    case GI_OOT_HAMMER:
        gOotSave.inventory.items[ITS_OOT_HAMMER] = ITEM_OOT_HAMMER;
        break;
    case GI_OOT_ARROW_LIGHT:
        comboAddCommonItemOot(SITEM_ARROW_LIGHT, noEffect);
        break;
    case GI_OOT_SPELL_LOVE:
        gOotSave.inventory.items[ITS_OOT_SPELL_LOVE] = ITEM_OOT_SPELL_LOVE;
        break;
    case GI_OOT_BOTTLE_EMPTY:
        addNewBottle(ITEM_OOT_BOTTLE_EMPTY);
        break;
    case GI_OOT_RUTO_LETTER:
        addNewBottle(ITEM_OOT_RUTO_LETTER);
        gOotExtraItems.rutoLetter = 1;
        break;
    case GI_OOT_BOTTLE_MILK:
        addNewBottle(ITEM_OOT_MILK);
        break;
    case GI_OOT_POTION_RED:
        fillBottle(ITEM_OOT_POTION_RED);
        break;
    case GI_OOT_POTION_BLUE:
        fillBottle(ITEM_OOT_POTION_BLUE);
        break;
    case GI_OOT_POTION_GREEN:
        fillBottle(ITEM_OOT_POTION_GREEN);
        break;
    case GI_OOT_FISH:
        fillBottle(ITEM_OOT_FISH);
        break;
    case GI_OOT_BLUE_FIRE:
        fillBottle(ITEM_OOT_BLUE_FIRE);
        break;
    case GI_OOT_BUG:
        fillBottle(ITEM_OOT_BUG);
        break;
    case GI_OOT_FAIRY:
        fillBottle(ITEM_OOT_FAIRY);
        break;
    case GI_OOT_POE:
        fillBottle(ITEM_OOT_POE);
        break;
    case GI_OOT_MILK:
        fillBottle(ITEM_OOT_MILK);
        break;
    case GI_OOT_SWORD_KOKIRI:
        gOotSave.inventory.equipment.swords |= EQ_OOT_SWORD_KOKIRI;
        break;
    case GI_OOT_SWORD_MASTER:
        gOotSave.inventory.equipment.swords |= EQ_OOT_SWORD_MASTER;
        break;
    case GI_OOT_SWORD_KNIFE:
        gOotSave.inventory.equipment.swords |= EQ_OOT_SWORD_KNIFE;
        gOotSave.playerData.swordHealth = 8;
        break;
    case GI_OOT_SWORD_BIGGORON:
        gOotSave.inventory.equipment.swords |= EQ_OOT_SWORD_KNIFE;
        gOotSave.inventory.equipment.swords &= ~(EQ_OOT_SWORD_KNIFE_BROKEN);
        gOotSave.isBiggoronSword = 1;
        break;
    case GI_OOT_PROGRESSIVE_SHIELD_DEKU:
        gOotExtraItems.shield |= EQ_OOT_SHIELD_DEKU;
        /* Fallthrough */
    case GI_OOT_SHIELD_DEKU:
        gOotSave.inventory.equipment.shields |= EQ_OOT_SHIELD_DEKU;
        break;
    case GI_OOT_PROGRESSIVE_SHIELD_HYLIAN:
        gOotExtraItems.shield |= EQ_OOT_SHIELD_HYLIAN;
        /* Fallthrough */
    case GI_OOT_SHIELD_HYLIAN:
        gOotSave.inventory.equipment.shields |= EQ_OOT_SHIELD_HYLIAN;
        break;
    case GI_OOT_SHIELD_MIRROR:
        gOotSave.inventory.equipment.shields |= EQ_OOT_SHIELD_MIRROR;
        gOotExtraItems.shield |= EQ_OOT_SHIELD_MIRROR;
        break;
    case GI_OOT_TUNIC_GORON:
        gOotSave.inventory.equipment.tunics |= EQ_OOT_TUNIC_GORON;
        break;
    case GI_OOT_TUNIC_ZORA:
        gOotSave.inventory.equipment.tunics |= EQ_OOT_TUNIC_ZORA;
        break;
    case GI_OOT_BOOTS_IRON:
        gOotSave.inventory.equipment.boots |= EQ_OOT_BOOTS_IRON;
        break;
    case GI_OOT_BOOTS_HOVER:
        gOotSave.inventory.equipment.boots |= EQ_OOT_BOOTS_HOVER;
        break;
    case GI_OOT_GORON_BRACELET:
        gOotSave.inventory.upgrades.strength = 1;
        break;
    case GI_OOT_SILVER_GAUNTLETS:
        gOotSave.inventory.upgrades.strength = 2;
        break;
    case GI_OOT_GOLDEN_GAUNTLETS:
        gOotSave.inventory.upgrades.strength = 3;
        break;
    case GI_OOT_SCALE_SILVER:
        gOotSave.inventory.upgrades.dive = 1;
        break;
    case GI_OOT_SCALE_GOLDEN:
        gOotSave.inventory.upgrades.dive = 2;
        break;
    case GI_OOT_BULLET_BAG2:
        addBulletBag(2);
        break;
    case GI_OOT_BULLET_BAG3:
        addBulletBag(3);
        break;
    case GI_OOT_QUIVER2:
        comboAddQuiverOot(2);
        break;
    case GI_OOT_QUIVER3:
        comboAddQuiverOot(3);
        break;
    case GI_OOT_BOMB_BAG:
        comboAddBombBagOot(1);
        break;
    case GI_OOT_BOMB_BAG2:
        comboAddBombBagOot(2);
        break;
    case GI_OOT_BOMB_BAG3:
        comboAddBombBagOot(3);
        break;
    case GI_OOT_WALLET:
        comboAddCommonItemOot(SITEM_WALLET, noEffect);
        break;
    case GI_OOT_WALLET2:
        comboAddCommonItemOot(SITEM_WALLET2, noEffect);
        break;
    case GI_OOT_WALLET3:
        comboAddCommonItemOot(SITEM_WALLET3, noEffect);
        break;
    case GI_OOT_WALLET4:
        comboAddCommonItemOot(SITEM_WALLET4, noEffect);
        break;
    case GI_OOT_WALLET5:
        comboAddCommonItemOot(SITEM_WALLET5, noEffect);
        break;
    case GI_OOT_MAGIC_UPGRADE:
        comboAddMagicUpgradeOot(1);
        if (noEffect)
            refillMagic(1);
        break;
    case GI_OOT_MAGIC_UPGRADE2:
        comboAddMagicUpgradeOot(2);
        if (noEffect)
            refillMagic(2);
        break;
    case GI_OOT_NUT_UPGRADE:
        addNutUpgrade(2);
        break;
    case GI_OOT_NUT_UPGRADE2:
        addNutUpgrade(3);
        break;
    case GI_OOT_STICK_UPGRADE:
        addStickUpgrade(2);
        break;
    case GI_OOT_STICK_UPGRADE2:
        addStickUpgrade(3);
        break;
    case GI_OOT_DEFENSE_UPGRADE:
        comboAddCommonItemOot(SITEM_DEFENSE_UPGRADE, noEffect);
        break;
    case GI_OOT_HEART_PIECE:
    case GI_OOT_TC_HEART_PIECE:
        comboAddCommonItemOot(SITEM_HEART_PIECE, noEffect);
        break;
    case GI_OOT_HEART_CONTAINER:
    case GI_OOT_HEART_CONTAINER2:
        comboAddCommonItemOot(SITEM_HEART_CONTAINER, noEffect);
        break;
    case GI_OOT_GS_TOKEN:
        gOotSave.inventory.quest.goldToken = 1;
        count = ++gOotSave.inventory.goldTokens;
        break;
    case GI_OOT_GERUDO_CARD:
        gOotSave.inventory.quest.gerudoCard = 1;
        break;
    case GI_OOT_STONE_OF_AGONY:
        gOotSave.inventory.quest.agonyStone = 1;
        break;
    case GI_OOT_STONE_SAPPHIRE:
        gOotSave.inventory.quest.stoneSapphire = 1;
        break;
    case GI_OOT_STONE_RUBY:
        gOotSave.inventory.quest.stoneRuby = 1;
        break;
    case GI_OOT_STONE_EMERALD:
        gOotSave.inventory.quest.stoneEmerald = 1;
        break;
    case GI_OOT_SONG_STORMS:
        comboAddCommonItemOot(SITEM_SONG_STORMS, noEffect);
        break;
    case GI_OOT_SONG_TIME:
        comboAddCommonItemOot(SITEM_SONG_TIME, noEffect);
        break;
    case GI_OOT_SONG_SUN:
        comboAddCommonItemOot(SITEM_SONG_SUN, noEffect);
        break;
    case GI_OOT_SONG_SARIA:
        gOotSave.inventory.quest.songSaria = 1;
        break;
    case GI_OOT_SONG_EPONA:
        comboAddCommonItemOot(SITEM_SONG_EPONA, noEffect);
        break;
    case GI_OOT_SONG_ZELDA:
        gOotSave.inventory.quest.songZelda = 1;
        break;
    case GI_OOT_SONG_TP_LIGHT:
        gOotSave.inventory.quest.songTpLight = 1;
        break;
    case GI_OOT_SONG_TP_SHADOW:
        gOotSave.inventory.quest.songTpShadow = 1;
        break;
    case GI_OOT_SONG_TP_SPIRIT:
        gOotSave.inventory.quest.songTpSpirit = 1;
        break;
    case GI_OOT_SONG_TP_WATER:
        gOotSave.inventory.quest.songTpWater = 1;
        break;
    case GI_OOT_SONG_TP_FIRE:
        gOotSave.inventory.quest.songTpFire = 1;
        break;
    case GI_OOT_SONG_TP_FOREST:
        gOotSave.inventory.quest.songTpForest = 1;
        break;
    case GI_OOT_MEDALLION_LIGHT:
        gOotSave.inventory.quest.medallionLight = 1;
        break;
    case GI_OOT_MEDALLION_SHADOW:
        gOotSave.inventory.quest.medallionShadow = 1;
        break;
    case GI_OOT_MEDALLION_SPIRIT:
        gOotSave.inventory.quest.medallionSpirit = 1;
        break;
    case GI_OOT_MEDALLION_WATER:
        gOotSave.inventory.quest.medallionWater = 1;
        break;
    case GI_OOT_MEDALLION_FIRE:
        gOotSave.inventory.quest.medallionFire = 1;
        break;
    case GI_OOT_MEDALLION_FOREST:
        gOotSave.inventory.quest.medallionForest = 1;
        break;
    case GI_OOT_RUPEE_GREEN:
    case GI_OOT_TC_RUPEE_GREEN:
        comboAddCommonItemOot(SITEM_RUPEE_GREEN, noEffect);
        break;
    case GI_OOT_RUPEE_BLUE:
    case GI_OOT_TC_RUPEE_BLUE:
        comboAddCommonItemOot(SITEM_RUPEE_BLUE, noEffect);
        break;
    case GI_OOT_RUPEE_RED:
    case GI_OOT_TC_RUPEE_RED:
        comboAddCommonItemOot(SITEM_RUPEE_RED, noEffect);
        break;
    case GI_OOT_RUPEE_PURPLE:
    case GI_OOT_TC_RUPEE_PURPLE:
        comboAddCommonItemOot(SITEM_RUPEE_PURPLE, noEffect);
        break;
    case GI_OOT_RUPEE_HUGE:
        comboAddCommonItemOot(SITEM_RUPEE_GOLD, noEffect);
        break;
    case GI_OOT_RECOVERY_HEART:
        comboAddCommonItemOot(SITEM_RECOVERY_HEART, noEffect);
        break;
    case GI_OOT_WEIRD_EGG:
        addTradeChild(0);
        break;
    case GI_OOT_CHICKEN:
        addTradeChild(1);
        break;
    case GI_OOT_ZELDA_LETTER:
        addTradeChild(2);
        break;
    case GI_OOT_MASK_KEATON:
        comboAddCommonItemOot(SITEM_MASK_KEATON, noEffect);
        break;
    case GI_OOT_MASK_SKULL:
        addTradeChild(4);
        break;
    case GI_OOT_MASK_SPOOKY:
        addTradeChild(5);
        break;
    case GI_OOT_MASK_BUNNY:
        comboAddCommonItemOot(SITEM_MASK_BUNNY, noEffect);
        break;
    case GI_OOT_MASK_GORON:
        comboAddCommonItemOot(SITEM_MASK_GORON, noEffect);
        break;
    case GI_OOT_MASK_ZORA:
        comboAddCommonItemOot(SITEM_MASK_ZORA, noEffect);
        break;
    case GI_OOT_MASK_GERUDO:
        addTradeChild(9);
        break;
    case GI_OOT_MASK_TRUTH:
        comboAddCommonItemOot(SITEM_MASK_TRUTH, noEffect);
        break;
    case GI_OOT_POCKET_EGG:
        addTradeAdult(0);
        break;
    case GI_OOT_POCKET_CUCCO:
        addTradeAdult(1);
        break;
    case GI_OOT_COJIRO:
        addTradeAdult(2);
        break;
    case GI_OOT_ODD_MUSHROOM:
        addTradeAdult(3);
        break;
    case GI_OOT_ODD_POTION:
        addTradeAdult(4);
        break;
    case GI_OOT_POACHER_SAW:
        addTradeAdult(5);
        break;
    case GI_OOT_BROKEN_GORON_SWORD:
        addTradeAdult(6);
        break;
    case GI_OOT_PRESCRIPTION:
        addTradeAdult(7);
        break;
    case GI_OOT_EYEBALL_FROG:
        addTradeAdult(8);
        break;
    case GI_OOT_EYE_DROPS:
        addTradeAdult(9);
        break;
    case GI_OOT_CLAIM_CHECK:
        addTradeAdult(10);
        break;
    case GI_OOT_SMALL_KEY_FOREST:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_SMALL_KEY_FIRE:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_SMALL_KEY_WATER:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_SMALL_KEY_SPIRIT:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_SMALL_KEY_SHADOW:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_SMALL_KEY_GANON:
        count = comboAddSmallKeyOot(SCE_OOT_INSIDE_GANON_CASTLE);
        break;
    case GI_OOT_SMALL_KEY_BOTW:
        count = comboAddSmallKeyOot(SCE_OOT_BOTTOM_OF_THE_WELL);
        break;
    case GI_OOT_SMALL_KEY_GF:
        count = comboAddSmallKeyOot(SCE_OOT_THIEVES_HIDEOUT);
        break;
    case GI_OOT_SMALL_KEY_GTG:
        count = comboAddSmallKeyOot(SCE_OOT_GERUDO_TRAINING_GROUND);
        break;
    case GI_OOT_KEY_RING_FOREST:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_KEY_RING_FIRE:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_KEY_RING_WATER:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_KEY_RING_SPIRIT:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_KEY_RING_SHADOW:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_KEY_RING_GANON:
        comboAddKeyRingOot(SCE_OOT_INSIDE_GANON_CASTLE);
        break;
    case GI_OOT_KEY_RING_BOTW:
        comboAddKeyRingOot(SCE_OOT_BOTTOM_OF_THE_WELL);
        break;
    case GI_OOT_KEY_RING_GF:
        comboAddKeyRingOot(SCE_OOT_THIEVES_HIDEOUT);
        break;
    case GI_OOT_KEY_RING_GTG:
        comboAddKeyRingOot(SCE_OOT_GERUDO_TRAINING_GROUND);
        break;
    case GI_OOT_SKELETON_KEY:
        comboAddSkeletonKeyOot();
        break;
    case GI_OOT_BOSS_KEY_FOREST:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_BOSS_KEY_FIRE:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_BOSS_KEY_WATER:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_BOSS_KEY_SPIRIT:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_BOSS_KEY_SHADOW:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_BOSS_KEY_GANON:
        comboAddBossKeyOot(SCE_OOT_GANON_TOWER);
        break;
    case GI_OOT_MAP_DT:
        comboAddMapOot(SCE_OOT_INSIDE_DEKU_TREE);
        break;
    case GI_OOT_MAP_DC:
        comboAddMapOot(SCE_OOT_DODONGO_CAVERN);
        break;
    case GI_OOT_MAP_JJ:
        comboAddMapOot(SCE_OOT_INSIDE_JABU_JABU);
        break;
    case GI_OOT_MAP_FOREST:
        comboAddMapOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_MAP_FIRE:
        comboAddMapOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_MAP_WATER:
        comboAddMapOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_MAP_SPIRIT:
        comboAddMapOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_MAP_SHADOW:
        comboAddMapOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_MAP_BOTW:
        comboAddMapOot(SCE_OOT_BOTTOM_OF_THE_WELL);
        break;
    case GI_OOT_MAP_IC:
        comboAddMapOot(SCE_OOT_ICE_CAVERN);
        break;
    case GI_OOT_COMPASS_DT:
        comboAddCompassOot(SCE_OOT_INSIDE_DEKU_TREE);
        break;
    case GI_OOT_COMPASS_DC:
        comboAddCompassOot(SCE_OOT_DODONGO_CAVERN);
        break;
    case GI_OOT_COMPASS_JJ:
        comboAddCompassOot(SCE_OOT_INSIDE_JABU_JABU);
        break;
    case GI_OOT_COMPASS_FOREST:
        comboAddCompassOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_COMPASS_FIRE:
        comboAddCompassOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_COMPASS_WATER:
        comboAddCompassOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_COMPASS_SPIRIT:
        comboAddCompassOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_COMPASS_SHADOW:
        comboAddCompassOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_COMPASS_BOTW:
        comboAddCompassOot(SCE_OOT_BOTTOM_OF_THE_WELL);
        break;
    case GI_OOT_COMPASS_IC:
        comboAddCompassOot(SCE_OOT_ICE_CAVERN);
        break;
    case GI_OOT_TRIFORCE:
    case GI_OOT_TRIFORCE_POWER:
    case GI_OOT_TRIFORCE_COURAGE:
    case GI_OOT_TRIFORCE_WISDOM:
        count = ++gTriforceCount;
        break;
    case GI_OOT_COIN_RED:
        count = ++gSharedCustomSave.coins[0];
        break;
    case GI_OOT_COIN_GREEN:
        count = ++gSharedCustomSave.coins[1];
        break;
    case GI_OOT_COIN_BLUE:
        count = ++gSharedCustomSave.coins[2];
        break;
    case GI_OOT_COIN_YELLOW:
        count = ++gSharedCustomSave.coins[3];
        break;
    case GI_OOT_RUPEE_SILVER_DC:
        count = addSilverRupees(SR_DC, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_BOTW:
        count = addSilverRupees(SR_BOTW, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_CHILD:
        count = addSilverRupees(SR_SPIRIT1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_SUN:
        count = addSilverRupees(SR_SPIRIT2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_BOULDERS:
        count = addSilverRupees(SR_SPIRIT3, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_LOBBY:
        count = addSilverRupees(SR_SPIRIT1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_ADULT:
        count = addSilverRupees(SR_SPIRIT2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SHADOW_SCYTHE:
        count = addSilverRupees(SR_SHADOW1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SHADOW_PIT:
        count = addSilverRupees(SR_SHADOW3, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SHADOW_SPIKES:
        count = addSilverRupees(SR_SHADOW4, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SHADOW_BLADES:
        count = addSilverRupees(SR_SHADOW2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_IC_SCYTHE:
        count = addSilverRupees(SR_IC1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_IC_BLOCK:
        count = addSilverRupees(SR_IC2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GTG_SLOPES:
        count = addSilverRupees(SR_GTG1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GTG_LAVA:
        count = addSilverRupees(SR_GTG2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GTG_WATER:
        count = addSilverRupees(SR_GTG3, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_SPIRIT:
        count = addSilverRupees(SR_GANON1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_LIGHT:
        count = addSilverRupees(SR_GANON2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_FIRE:
        count = addSilverRupees(SR_GANON3, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_FOREST:
        count = addSilverRupees(SR_GANON4, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_SHADOW:
        count = addSilverRupees(SR_GANON1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_WATER:
        count = addSilverRupees(SR_GANON2, noEffect);
        break;
    case GI_OOT_SOUL_STALFOS:
    case GI_OOT_SOUL_OCTOROK:
    case GI_OOT_SOUL_WALLMASTER:
    case GI_OOT_SOUL_DODONGO:
    case GI_OOT_SOUL_KEESE:
    case GI_OOT_SOUL_TEKTITE:
    case GI_OOT_SOUL_LEEVER:
    case GI_OOT_SOUL_PEAHAT:
    case GI_OOT_SOUL_LIZALFOS_DINALFOS:
    case GI_OOT_SOUL_GOHMA_LARVA:
    case GI_OOT_SOUL_SHABOM:
    case GI_OOT_SOUL_BABY_DODONGO:
    case GI_OOT_SOUL_DARK_LINK:
    case GI_OOT_SOUL_BIRI_BARI:
    case GI_OOT_SOUL_TAILPASARN:
    case GI_OOT_SOUL_PARASITE:
    case GI_OOT_SOUL_SKULLTULA:
    case GI_OOT_SOUL_TORCH_SLUG:
    case GI_OOT_SOUL_MOBLIN:
    case GI_OOT_SOUL_ARMOS:
    case GI_OOT_SOUL_DEKU_BABA:
    case GI_OOT_SOUL_DEKU_SCRUB:
    case GI_OOT_SOUL_BUBBLE:
    case GI_OOT_SOUL_BEAMOS:
    case GI_OOT_SOUL_FLOORMASTER:
    case GI_OOT_SOUL_REDEAD_GIBDO:
    case GI_OOT_SOUL_SKULLWALLTULA:
    case GI_OOT_SOUL_FLARE_DANCER:
    case GI_OOT_SOUL_DEAD_HAND:
    case GI_OOT_SOUL_SHELL_BLADE:
    case GI_OOT_SOUL_LIKE_LIKE:
    case GI_OOT_SOUL_SPIKE:
    case GI_OOT_SOUL_ANUBIS:
    case GI_OOT_SOUL_IRON_KNUCKLE:
    case GI_OOT_SOUL_SKULL_KID:
    case GI_OOT_SOUL_FLYING_POT:
    case GI_OOT_SOUL_FREEZARD:
    case GI_OOT_SOUL_STINGER:
    case GI_OOT_SOUL_WOLFOS:
    case GI_OOT_SOUL_STALCHILD:
    case GI_OOT_SOUL_GUAY:
    case GI_OOT_SOUL_QUEEN_GOHMA:
    case GI_OOT_SOUL_KING_DODONGO:
    case GI_OOT_SOUL_BARINADE:
    case GI_OOT_SOUL_PHANTOM_GANON:
    case GI_OOT_SOUL_VOLVAGIA:
    case GI_OOT_SOUL_MORPHA:
    case GI_OOT_SOUL_BONGO_BONGO:
    case GI_OOT_SOUL_TWINROVA:
        comboAddSoulOot(gi);
        break;
    }

    return count;
}
