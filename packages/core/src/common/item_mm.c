#include <combo.h>
#include <combo/item.h>

static const u16 kSwords[] = {
    ITEM_NONE,
    ITEM_MM_SWORD_KOKIRI,
    ITEM_MM_SWORD_RAZOR,
    ITEM_MM_SWORD_GILDED,
};

const u8 kMmTrade1[] = {
    ITEM_MM_MOON_TEAR,
    ITEM_MM_DEED_LAND,
    ITEM_MM_DEED_SWAMP,
    ITEM_MM_DEED_MOUNTAIN,
    ITEM_MM_DEED_OCEAN,
};

const u8 kMmTrade2[] = {
    ITEM_MM_ROOM_KEY,
    ITEM_MM_LETTER_TO_MAMA,
};

const u8 kMmTrade3[] = {
    ITEM_MM_LETTER_TO_KAFEI,
    ITEM_MM_PENDANT_OF_MEMORIES,
};

int comboAddSmallKeyMm(u16 dungeonId)
{
    s8 keyCount;

    keyCount = gMmSave.inventory.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gMmSave.inventory.dungeonKeys[dungeonId] = keyCount;
    gMmSave.inventory.dungeonItems[dungeonId].maxKeys++;

    return gMmSave.inventory.dungeonItems[dungeonId].maxKeys;
}

void comboAddBossKeyMm(u16 dungeonId)
{
    gMmSave.inventory.dungeonItems[dungeonId].bossKey = 1;
}

int comboAddStrayFairyMm(u16 dungeonId)
{
    if (dungeonId == 4)
    {
        MM_SET_EVENT_WEEK(EV_MM_WEEK_TOWN_FAIRY);
        return 0;
    }
    else
    {
        gMmSave.inventory.strayFairies[dungeonId]++;
        return gMmSave.inventory.strayFairies[dungeonId];
    }
}

void comboAddMapMm(u16 dungeonId)
{
    gMmSave.inventory.dungeonItems[dungeonId].map = 1;
}

void comboAddCompassMm(u16 dungeonId)
{
    gMmSave.inventory.dungeonItems[dungeonId].compass = 1;
}

static void addHealth(u8 count)
{
    u16 health;

    health = (u16)count * 0x10;
    gMmSave.playerData.health += health;
    if (gMmSave.playerData.health > gMmSave.playerData.healthMax)
        gMmSave.playerData.health = gMmSave.playerData.healthMax;
}

static void addSword(int index)
{
    gMmSave.itemEquips.buttonItems[0][0] = kSwords[index];
    gMmSave.itemEquips.sword = index;
}

static void addShield(int index)
{
    if (index > gMmSave.itemEquips.shield)
        gMmSave.itemEquips.shield = index;
}

void comboAddBombBagMm(int index)
{
    gMmSave.inventory.items[ITS_MM_BOMBS] = ITEM_MM_BOMB;
    gMmSave.inventory.upgrades.bombBag = index;
    gMmSave.inventory.ammo[ITS_MM_BOMBS] = kMaxBombs[index];
}

void comboAddQuiverMm(int index)
{
    gMmSave.inventory.items[ITS_MM_BOW] = ITEM_MM_BOW;
    gMmSave.inventory.upgrades.quiver = index;
    gMmSave.inventory.ammo[ITS_MM_BOW] = kMaxArrows[index];
}

void comboAddBombsMm(int count)
{
    u16 max;

    max = kMaxBombs[gMmSave.inventory.upgrades.bombBag];
    gMmSave.inventory.items[ITS_MM_BOMBS] = ITEM_MM_BOMB;
    gMmSave.inventory.ammo[ITS_MM_BOMBS] += count;
    if (gMmSave.inventory.ammo[ITS_MM_BOMBS] > max)
        gMmSave.inventory.ammo[ITS_MM_BOMBS] = max;
}

void comboAddArrowsMm(int count)
{
    int max;

    max = kMaxArrows[gMmSave.inventory.upgrades.quiver];
    gMmSave.inventory.items[ITS_MM_BOW] = ITEM_MM_BOW;
    gMmSave.inventory.ammo[ITS_MM_BOW] += count;
    if (gMmSave.inventory.ammo[ITS_MM_BOW] > max)
        gMmSave.inventory.ammo[ITS_MM_BOW] = max;
}

void comboAddNutsMm(int count)
{
    u16 max;

    if (gMmSave.inventory.upgrades.dekuNut == 0)
        gMmSave.inventory.upgrades.dekuNut = 1;

    max = kMaxNuts[gMmSave.inventory.upgrades.dekuNut];
    gMmSave.inventory.items[ITS_MM_NUTS] = ITEM_MM_NUT;
    gMmSave.inventory.ammo[ITS_MM_NUTS] += count;
    if (gMmSave.inventory.ammo[ITS_MM_NUTS] > max)
        gMmSave.inventory.ammo[ITS_MM_NUTS] = max;
}

void comboAddSticksMm(int count)
{
    u16 max;

    if (gMmSave.inventory.upgrades.dekuStick == 0)
        gMmSave.inventory.upgrades.dekuStick = 1;

    max = kMaxSticks[gMmSave.inventory.upgrades.dekuStick];
    gMmSave.inventory.items[ITS_MM_STICKS] = ITEM_MM_STICK;
    gMmSave.inventory.ammo[ITS_MM_STICKS] += count;
    if (gMmSave.inventory.ammo[ITS_MM_STICKS] > max)
        gMmSave.inventory.ammo[ITS_MM_STICKS] = max;
}

static void addBombchu(int count)
{
    u16 max;

    max = 40;
    gMmSave.inventory.items[ITS_MM_BOMBCHU] = ITEM_MM_BOMBCHU;
    gMmSave.inventory.ammo[ITS_MM_BOMBCHU] += count;
    if (gMmSave.inventory.ammo[ITS_MM_BOMBCHU] > max)
        gMmSave.inventory.ammo[ITS_MM_BOMBCHU] = max;
}

static void addNewBottle(u16 itemId)
{
    int i;

    for (i = 0; i < 6; i++)
    {
        if (gMmSave.inventory.items[ITS_MM_BOTTLE + i] == ITEM_NONE)
        {
            gMmSave.inventory.items[ITS_MM_BOTTLE + i] = itemId;
            break;
        }
    }
}

static void fillBottle(u16 itemId)
{
    int slot;

    slot = -1;
    for (int i = 0; i < 6; ++i)
    {
        if (gMmSave.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_BOTTLE_EMPTY)
        {
            slot = i;
            break;
        }
    }
    if (slot == -1)
        return;
    gMmSave.inventory.items[ITS_MM_BOTTLE + slot] = itemId;
    for (int i = 1; i <= 3; ++i)
    {
        if (gMmSave.itemEquips.cButtonSlots[0][i] == ITS_MM_BOTTLE + slot)
        {
            gMmSave.itemEquips.buttonItems[0][i] = itemId;
        }
    }
}

static void addRupees(s16 count)
{
    u16 max;

    max = gMmMaxRupees[gMmSave.inventory.upgrades.wallet];
    gMmSave.playerData.rupees += count;
    if (gMmSave.playerData.rupees > max)
        gMmSave.playerData.rupees = max;
}

static void addWallet(int index, int noEffect)
{
    if (index == 0)
    {
        gMmExtraFlags2.childWallet = 1;
        gMmMaxRupees[0] = 99;
    }
    else
        gMmSave.inventory.upgrades.wallet = index;
    if (!noEffect && comboConfig(CFG_FILL_WALLETS))
        addRupees(999);
}

static void addTrade1(u8 index)
{
    u16 itemId;

    itemId = kMmTrade1[index];
    if (gMmSave.inventory.items[ITS_MM_TRADE1] == ITEM_NONE)
        gMmSave.inventory.items[ITS_MM_TRADE1] = itemId;
    gMmExtraTrade.trade1 |= (1 << (u16)index);
    gMmExtraTrade.tradeObtained1 |= (1 << (u16)index);
}

static void addTrade2(u8 index)
{
    u16 itemId;

    itemId = kMmTrade2[index];
    if (gMmSave.inventory.items[ITS_MM_TRADE2] == ITEM_NONE)
        gMmSave.inventory.items[ITS_MM_TRADE2] = itemId;
    gMmExtraTrade.trade2 |= (1 << (u16)index);
    gMmExtraTrade.tradeObtained2 |= (1 << (u16)index);
}

static void addTrade3(u8 index)
{
    u16 itemId;

    itemId = kMmTrade3[index];
    if (gMmSave.inventory.items[ITS_MM_TRADE3] == ITEM_NONE)
        gMmSave.inventory.items[ITS_MM_TRADE3] = itemId;
    gMmExtraTrade.trade3 |= (1 << (u16)index);
    gMmExtraTrade.tradeObtained3 |= (1 << (u16)index);
}

static void reloadSlotEquips(MmItemEquips* equips, int slot)
{
    for (int i = 0; i < 3; ++i)
    {
        if (equips->cButtonSlots[0][1 + i] == slot)
        {
            equips->buttonItems[0][1 + i] = gMmSave.inventory.items[slot];
        }
    }
}

static void reloadSlot(int slot)
{
    reloadSlotEquips(&gMmSave.itemEquips, slot);
}

static void addHookshot(int level)
{
    u8 itemId;

    if (level >= 2)
        itemId = ITEM_MM_HOOKSHOT;
    else
        itemId = 0x11; // ITEM_MM_BOTTLE_POTION_RED but that enum is wrong
    gMmSave.inventory.items[ITS_MM_HOOKSHOT] = itemId;
    gMmExtraItems.hookshot |= (1 << (level - 1));
    reloadSlot(ITS_MM_HOOKSHOT);
}

static void addOcarina(int level)
{
    u8 itemId;

    if (level >= 2)
        itemId = ITEM_MM_OCARINA_OF_TIME;
    else
        itemId = ITEM_MM_OCARINA_FAIRY;
    gMmSave.inventory.items[ITS_MM_OCARINA] = itemId;
    gMmExtraItems.ocarina |= (1 << (level - 1));
    reloadSlot(ITS_MM_OCARINA);
}

void comboAddMagicUpgradeMm(int level)
{
    gMmSave.playerData.magicAcquired = 1;
    if (level >= 2)
        gMmSave.playerData.doubleMagic = 1;
}

static void refillMagic(int level)
{
    gMmSave.playerData.magicLevel = level;
    gMmSave.playerData.magicAmount = level * 0x30;
}

void comboAddCommonItemMm(int sid, int noEffect)
{
    switch (sid)
    {
    case SITEM_ARROW_FIRE:
        gMmSave.inventory.items[ITS_MM_ARROW_FIRE] = ITEM_MM_ARROW_FIRE;
        break;
    case SITEM_ARROW_ICE:
        gMmSave.inventory.items[ITS_MM_ARROW_ICE] = ITEM_MM_ARROW_ICE;
        break;
    case SITEM_ARROW_LIGHT:
        gMmSave.inventory.items[ITS_MM_ARROW_LIGHT] = ITEM_MM_ARROW_LIGHT;
        break;
    case SITEM_SONG_EPONA:
        gMmSave.inventory.quest.songEpona = 1;
        break;
    case SITEM_SONG_TIME:
        gMmSave.inventory.quest.songTime = 1;
        break;
    case SITEM_SONG_STORMS:
        gMmSave.inventory.quest.songStorms = 1;
        break;
    case SITEM_HOOKSHOT:
        addHookshot(1);
        break;
    case SITEM_LONGSHOT:
        addHookshot(2);
        break;
    case SITEM_LENS:
        gMmSave.inventory.items[ITS_MM_LENS] = ITEM_MM_LENS_OF_TRUTH;
        break;
    case SITEM_OCARINA_FAIRY:
        addOcarina(1);
        break;
    case SITEM_OCARINA_TIME:
        addOcarina(2);
        break;
    case SITEM_MASK_KEATON:
        gMmSave.inventory.items[ITS_MM_MASK_KEATON] = ITEM_MM_MASK_KEATON;
        break;
    case SITEM_MASK_BUNNY:
        gMmSave.inventory.items[ITS_MM_MASK_BUNNY] = ITEM_MM_MASK_BUNNY;
        break;
    case SITEM_MASK_TRUTH:
        gMmSave.inventory.items[ITS_MM_MASK_TRUTH] = ITEM_MM_MASK_TRUTH;
        break;
    case SITEM_MASK_GORON:
        gMmSave.inventory.items[ITS_MM_MASK_GORON] = ITEM_MM_MASK_GORON;
        break;
    case SITEM_MASK_ZORA:
        gMmSave.inventory.items[ITS_MM_MASK_ZORA] = ITEM_MM_MASK_ZORA;
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
        gMmSave.inventory.quest.heartPieces += 1;
        if (gMmSave.inventory.quest.heartPieces == 4)
        {
            gMmSave.inventory.quest.heartPieces = 0;
            gMmSave.playerData.healthMax += 0x10;
        }
        if (noEffect)
            addHealth(20);
        break;
    case SITEM_HEART_CONTAINER:
        gMmSave.playerData.healthMax += 0x10;
        if (noEffect)
            addHealth(20);
        break;
    case SITEM_RECOVERY_HEART:
        if (noEffect)
            addHealth(1);
        break;
    case SITEM_DEFENSE_UPGRADE:
        gMmSave.playerData.doubleDefense = 1;
        gMmSave.inventory.defenseHearts = 20;
        if (noEffect)
            addHealth(20);
        break;
    case SITEM_SONG_SUN:
        gMmSave.inventory.quest.songSun = 1;
        break;
    }
}

void comboAddItemSharedMm(s16 gi, int noEffect)
{
    if (comboConfig(CFG_SHARED_BOWS))
    {
        switch (gi)
        {
        case GI_MM_BOW:
            comboAddQuiverOot(1);
            break;
        case GI_MM_QUIVER2:
            comboAddQuiverOot(2);
            break;
        case GI_MM_QUIVER3:
            comboAddQuiverOot(3);
            break;
        case GI_MM_ARROWS_10:
            comboAddArrowsOot(10);
            break;
        case GI_MM_ARROWS_30:
            comboAddArrowsOot(30);
            break;
        case GI_MM_ARROWS_40:
            comboAddArrowsOot(40);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_BOMB_BAGS))
    {
        switch (gi)
        {
        case GI_MM_BOMB_BAG:
            comboAddBombBagOot(1);
            break;
        case GI_MM_BOMB_BAG2:
            comboAddBombBagOot(2);
            break;
        case GI_MM_BOMB_BAG3:
            comboAddBombBagOot(3);
            break;
        case GI_MM_BOMB:
            comboAddBombsOot(1);
            break;
        case GI_MM_BOMBS_5:
            comboAddBombsOot(5);
            break;
        case GI_MM_BOMBS_10:
            comboAddBombsOot(10);
            break;
        case GI_MM_BOMBS_20:
            comboAddBombsOot(20);
            break;
        case GI_MM_BOMBS_30:
            comboAddBombsOot(30);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_MAGIC))
    {
        switch (gi)
        {
        case GI_MM_MAGIC_UPGRADE:
            comboAddMagicUpgradeOot(1);
            break;
        case GI_MM_MAGIC_UPGRADE2:
            comboAddMagicUpgradeOot(2);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_MAGIC_ARROW_FIRE) && gi == GI_MM_ARROW_FIRE)
    {
        comboAddCommonItemOot(SITEM_ARROW_FIRE, noEffect);
    }

    if (comboConfig(CFG_SHARED_MAGIC_ARROW_ICE) && gi == GI_MM_ARROW_ICE)
    {
        comboAddCommonItemOot(SITEM_ARROW_ICE, noEffect);
    }

    if (comboConfig(CFG_SHARED_MAGIC_ARROW_LIGHT) && gi == GI_MM_ARROW_LIGHT)
    {
        comboAddCommonItemOot(SITEM_ARROW_LIGHT, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_EPONA) && gi == GI_MM_SONG_EPONA)
    {
        comboAddCommonItemOot(SITEM_SONG_EPONA, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_STORMS) && gi == GI_MM_SONG_STORMS)
    {
        comboAddCommonItemOot(SITEM_SONG_STORMS, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_TIME) && gi == GI_MM_SONG_TIME)
    {
        comboAddCommonItemOot(SITEM_SONG_TIME, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_SUN) && gi == GI_MM_SONG_SUN)
    {
        comboAddCommonItemOot(SITEM_SONG_SUN, noEffect);
    }

    if (comboConfig(CFG_SHARED_NUTS_STICKS))
    {
        switch (gi)
        {
        case GI_MM_NUT:
            comboAddNutsOot(1);
            break;
        case GI_MM_NUTS_5:
            comboAddNutsOot(5);
            break;
        case GI_MM_NUTS_10:
            comboAddNutsOot(10);
            break;
        case GI_MM_STICK:
            comboAddSticksOot(1);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_HOOKSHOT))
    {
        switch (gi)
        {
        case GI_MM_HOOKSHOT_SHORT:
            comboAddCommonItemOot(SITEM_HOOKSHOT, noEffect);
            break;
        case GI_MM_HOOKSHOT:
            if (comboConfig(CFG_MM_HOOKSHOT_SHORT))
                comboAddCommonItemOot(SITEM_LONGSHOT, noEffect);
            else
                comboAddCommonItemOot(SITEM_HOOKSHOT, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_LENS) && gi == GI_MM_LENS)
    {
        comboAddCommonItemOot(SITEM_LENS, noEffect);
    }

    if (comboConfig(CFG_SHARED_OCARINA))
    {
        switch (gi)
        {
        case GI_MM_OCARINA_FAIRY:
            comboAddCommonItemOot(SITEM_OCARINA_FAIRY, noEffect);
            break;
        case GI_MM_OCARINA_OF_TIME:
            comboAddCommonItemOot(SITEM_OCARINA_TIME, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_MASKS))
    {
        switch (gi)
        {
        case GI_MM_MASK_BUNNY:
            comboAddCommonItemOot(SITEM_MASK_BUNNY, noEffect);
            break;
        case GI_MM_MASK_GORON:
            comboAddCommonItemOot(SITEM_MASK_GORON, noEffect);
            break;
        case GI_MM_MASK_KEATON:
            comboAddCommonItemOot(SITEM_MASK_KEATON, noEffect);
            break;
        case GI_MM_MASK_TRUTH:
            comboAddCommonItemOot(SITEM_MASK_TRUTH, noEffect);
            break;
        case GI_MM_MASK_ZORA:
            comboAddCommonItemOot(SITEM_MASK_ZORA, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_WALLETS))
    {
        switch (gi)
        {
        case GI_MM_WALLET:
            comboAddCommonItemOot(SITEM_WALLET, noEffect);
            break;
        case GI_MM_WALLET2:
            comboAddCommonItemOot(SITEM_WALLET2, noEffect);
            break;
        case GI_MM_WALLET3:
            comboAddCommonItemOot(SITEM_WALLET3, noEffect);
            break;
        case GI_MM_WALLET4:
            comboAddCommonItemOot(SITEM_WALLET4, noEffect);
            break;
        case GI_MM_RUPEE_GREEN:
            comboAddCommonItemOot(SITEM_RUPEE_GREEN, noEffect);
            break;
        case GI_MM_RUPEE_BLUE:
            comboAddCommonItemOot(SITEM_RUPEE_BLUE, noEffect);
            break;
        case GI_MM_RUPEE_RED:
            comboAddCommonItemOot(SITEM_RUPEE_RED, noEffect);
            break;
        case GI_MM_RUPEE_PURPLE:
            comboAddCommonItemOot(SITEM_RUPEE_PURPLE, noEffect);
            break;
        case GI_MM_RUPEE_SILVER:
            comboAddCommonItemOot(SITEM_RUPEE_SILVER, noEffect);
            break;
        case GI_MM_RUPEE_GOLD:
            comboAddCommonItemOot(SITEM_RUPEE_GOLD, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_HEALTH))
    {
        switch (gi)
        {
        case GI_MM_RECOVERY_HEART:
            comboAddCommonItemOot(SITEM_RECOVERY_HEART, noEffect);
            break;
        case GI_MM_HEART_CONTAINER:
            comboAddCommonItemOot(SITEM_HEART_CONTAINER, noEffect);
            break;
        case GI_MM_HEART_PIECE:
            comboAddCommonItemOot(SITEM_HEART_PIECE, noEffect);
            break;
        case GI_MM_DEFENSE_UPGRADE:
            comboAddCommonItemOot(SITEM_DEFENSE_UPGRADE, noEffect);
            break;
        }
    }
}

int comboAddItemMm(s16 gi, int noEffect)
{
    int count;

    count = 0;
    switch (gi)
    {
    case GI_MM_OCARINA_FAIRY:
        comboAddCommonItemMm(SITEM_OCARINA_FAIRY, noEffect);
        break;
    case GI_MM_OCARINA_OF_TIME:
        comboAddCommonItemMm(SITEM_OCARINA_TIME, noEffect);
        break;
    case GI_MM_ARROW_FIRE:
        comboAddCommonItemMm(SITEM_ARROW_FIRE, noEffect);
        break;
    case GI_MM_ARROW_ICE:
        comboAddCommonItemMm(SITEM_ARROW_ICE, noEffect);
        break;
    case GI_MM_ARROW_LIGHT:
        comboAddCommonItemMm(SITEM_ARROW_LIGHT, noEffect);
        break;
    case GI_MM_MAGIC_BEAN:
        gMmSave.inventory.items[ITS_MM_BEANS] = ITEM_MM_MAGIC_BEAN;
        gMmSave.inventory.ammo[ITS_MM_BEANS]++;
        break;
    case GI_MM_POWDER_KEG:
        gMmSave.inventory.items[ITS_MM_KEG] = ITEM_MM_POWDER_KEG;
        gMmSave.inventory.ammo[ITS_MM_KEG] = 1;
        break;
    case GI_MM_PICTOGRAPH_BOX:
        gMmSave.inventory.items[ITS_MM_PICTOBOX] = ITEM_MM_PICTOGRAPH_BOX;
        break;
    case GI_MM_LENS:
        comboAddCommonItemMm(SITEM_LENS, noEffect);
        break;
    case GI_MM_HOOKSHOT_SHORT:
        comboAddCommonItemMm(SITEM_HOOKSHOT, noEffect);
        break;
    case GI_MM_HOOKSHOT:
        comboAddCommonItemMm(SITEM_LONGSHOT, noEffect);
        break;
    case GI_MM_GREAT_FAIRY_SWORD:
        gMmSave.inventory.items[ITS_MM_GREAT_FAIRY_SWORD] = ITEM_MM_GREAT_FAIRY_SWORD;
        break;
    case GI_MM_BOTTLE_EMPTY:
        addNewBottle(ITEM_MM_BOTTLE_EMPTY);
        break;
    case GI_MM_BOTTLE_MILK:
        addNewBottle(ITEM_MM_MILK);
        break;
    case GI_MM_BOTTLED_GOLD_DUST:
        addNewBottle(ITEM_MM_BOTTLED_GOLD_DUST);
        gMmExtraItems.goldDust = 1;
        break;
    case GI_MM_BOTTLE_CHATEAU:
        addNewBottle(ITEM_MM_BOTTLE_CHATEAU);
        break;
    case GI_MM_BOTTLE_POTION_RED:
        addNewBottle(ITEM_MM_POTION_RED);
        break;
    case GI_MM_POTION_RED:
        fillBottle(ITEM_MM_POTION_RED);
        break;
    case GI_MM_POTION_BLUE:
        fillBottle(ITEM_MM_POTION_BLUE);
        break;
    case GI_MM_POTION_GREEN:
        fillBottle(ITEM_MM_POTION_GREEN);
        break;
    case GI_MM_SEAHORSE:
    case GI_MM_SEAHORSE2:
        fillBottle(ITEM_MM_BOTTLED_SEAHORSE);
        break;
    case GI_MM_MILK:
        fillBottle(ITEM_MM_MILK);
        break;
    case GI_MM_CHATEAU:
        fillBottle(ITEM_MM_BOTTLE_CHATEAU);
        break;
    case GI_MM_BOMB:
        comboAddBombsMm(1);
        break;
    case GI_MM_BOMBS_5:
        comboAddBombsMm(5);
        break;
    case GI_MM_BOMBS_10:
        comboAddBombsMm(10);
        break;
    case GI_MM_BOMBS_20:
        comboAddBombsMm(20);
        break;
    case GI_MM_BOMBS_30:
        comboAddBombsMm(30);
        break;
    case GI_MM_ARROWS_10:
        comboAddArrowsMm(10);
        break;
    case GI_MM_ARROWS_30:
        comboAddArrowsMm(30);
        break;
    case GI_MM_ARROWS_40:
        comboAddArrowsMm(40);
        break;
    case GI_MM_NUT:
        comboAddNutsMm(1);
        break;
    case GI_MM_NUTS_5:
        comboAddNutsMm(5);
        break;
    case GI_MM_NUTS_10:
        comboAddNutsMm(10);
        break;
    case GI_MM_STICK:
        comboAddSticksMm(1);
        break;
    case GI_MM_BOMBCHU:
        addBombchu(1);
        break;
    case GI_MM_BOMBCHU_5:
        addBombchu(5);
        break;
    case GI_MM_BOMBCHU_10:
        addBombchu(10);
        break;
    case GI_MM_BOMBCHU_20:
        addBombchu(20);
        break;
    case GI_MM_MOON_TEAR:
        addTrade1(XITEM_MM_TRADE1_MOON_TEAR);
        break;
    case GI_MM_DEED_LAND:
        addTrade1(XITEM_MM_TRADE1_DEED_LAND);
        break;
    case GI_MM_DEED_SWAMP:
        addTrade1(XITEM_MM_TRADE1_DEED_SWAMP);
        break;
    case GI_MM_DEED_MOUNTAIN:
        addTrade1(XITEM_MM_TRADE1_DEED_MOUNTAIN);
        break;
    case GI_MM_DEED_OCEAN:
        addTrade1(XITEM_MM_TRADE1_DEED_OCEAN);
        break;
    case GI_MM_ROOM_KEY:
        addTrade2(XITEM_MM_TRADE2_ROOM_KEY);
        break;
    case GI_MM_LETTER_TO_MAMA:
        addTrade2(XITEM_MM_TRADE2_LETTER_TO_MAMA);
        break;
    case GI_MM_LETTER_TO_KAFEI:
        addTrade3(XITEM_MM_TRADE3_LETTER_TO_KAFEI);
        break;
    case GI_MM_PENDANT_OF_MEMORIES:
        addTrade3(XITEM_MM_TRADE3_PENDANT_OF_MEMORIES);
        break;
    case GI_MM_MASK_POSTMAN:
        gMmSave.inventory.items[ITS_MM_MASK_POSTMAN] = ITEM_MM_MASK_POSTMAN;
        break;
    case GI_MM_MASK_ALL_NIGHT:
        gMmSave.inventory.items[ITS_MM_MASK_ALL_NIGHT] = ITEM_MM_MASK_ALL_NIGHT;
        break;
    case GI_MM_MASK_BLAST:
        gMmSave.inventory.items[ITS_MM_MASK_BLAST] = ITEM_MM_MASK_BLAST;
        break;
    case GI_MM_MASK_STONE:
        gMmSave.inventory.items[ITS_MM_MASK_STONE] = ITEM_MM_MASK_STONE;
        break;
    case GI_MM_MASK_GREAT_FAIRY:
        gMmSave.inventory.items[ITS_MM_MASK_GREAT_FAIRY] = ITEM_MM_MASK_GREAT_FAIRY;
        break;
    case GI_MM_MASK_DEKU:
        gMmSave.inventory.items[ITS_MM_MASK_DEKU] = ITEM_MM_MASK_DEKU;
        break;
    case GI_MM_MASK_KEATON:
        comboAddCommonItemMm(SITEM_MASK_KEATON, noEffect);
        break;
    case GI_MM_MASK_BREMEN:
        gMmSave.inventory.items[ITS_MM_MASK_BREMEN] = ITEM_MM_MASK_BREMEN;
        break;
    case GI_MM_MASK_BUNNY:
        comboAddCommonItemMm(SITEM_MASK_BUNNY, noEffect);
        break;
    case GI_MM_MASK_DON_GERO:
        gMmSave.inventory.items[ITS_MM_MASK_DON_GERO] = ITEM_MM_MASK_DON_GERO;
        break;
    case GI_MM_MASK_SCENTS:
        gMmSave.inventory.items[ITS_MM_MASK_SCENTS] = ITEM_MM_MASK_SCENTS;
        break;
    case GI_MM_MASK_GORON:
        comboAddCommonItemMm(SITEM_MASK_GORON, noEffect);
        break;
    case GI_MM_MASK_ROMANI:
        gMmSave.inventory.items[ITS_MM_MASK_ROMANI] = ITEM_MM_MASK_ROMANI;
        break;
    case GI_MM_MASK_TROUPE_LEADER:
        gMmSave.inventory.items[ITS_MM_MASK_TROUPE_LEADER] = ITEM_MM_MASK_TROUPE_LEADER;
        break;
    case GI_MM_MASK_KAFEI:
        gMmSave.inventory.items[ITS_MM_MASK_KAFEI] = ITEM_MM_MASK_KAFEI;
        break;
    case GI_MM_MASK_COUPLE:
        gMmSave.inventory.items[ITS_MM_MASK_COUPLE] = ITEM_MM_MASK_COUPLE;
        break;
    case GI_MM_MASK_TRUTH:
        comboAddCommonItemMm(SITEM_MASK_TRUTH, noEffect);
        break;
    case GI_MM_MASK_ZORA:
        comboAddCommonItemMm(SITEM_MASK_ZORA, noEffect);
        break;
    case GI_MM_MASK_KAMARO:
        gMmSave.inventory.items[ITS_MM_MASK_KAMARO] = ITEM_MM_MASK_KAMARO;
        break;
    case GI_MM_MASK_GIBDO:
        gMmSave.inventory.items[ITS_MM_MASK_GIBDO] = ITEM_MM_MASK_GIBDO;
        break;
    case GI_MM_MASK_GARO:
        gMmSave.inventory.items[ITS_MM_MASK_GARO] = ITEM_MM_MASK_GARO;
        break;
    case GI_MM_MASK_CAPTAIN:
        gMmSave.inventory.items[ITS_MM_MASK_CAPTAIN] = ITEM_MM_MASK_CAPTAIN;
        break;
    case GI_MM_MASK_GIANT:
        gMmSave.inventory.items[ITS_MM_MASK_GIANT] = ITEM_MM_MASK_GIANT;
        break;
    case GI_MM_MASK_FIERCE_DEITY:
        gMmSave.inventory.items[ITS_MM_MASK_FIERCE_DEITY] = ITEM_MM_MASK_FIERCE_DEITY;
        break;
    case GI_MM_MAGIC_UPGRADE:
        comboAddMagicUpgradeMm(1);
        if (noEffect)
            refillMagic(1);
        break;
    case GI_MM_MAGIC_UPGRADE2:
        comboAddMagicUpgradeMm(2);
        if (noEffect)
            refillMagic(2);
        break;
    case GI_MM_SWORD_KOKIRI:
        addSword(1);
        break;
    case GI_MM_SWORD_RAZOR:
        addSword(2);
        gMmSave.playerData.swordHealth = 100;
        break;
    case GI_MM_SWORD_GILDED:
        addSword(3);
        break;
    case GI_MM_PROGRESSIVE_SHIELD_HERO:
        gMmExtraFlags2.progressiveShield = 1;
        /* Fallthrough */
    case GI_MM_SHIELD_HERO:
        addShield(1);
        break;
    case GI_MM_SHIELD_MIRROR:
        addShield(2);
        break;
    case GI_MM_BOMB_BAG:
        comboAddBombBagMm(1);
        break;
    case GI_MM_BOMB_BAG2:
        comboAddBombBagMm(2);
        break;
    case GI_MM_BOMB_BAG3:
        comboAddBombBagMm(3);
        break;
    case GI_MM_BOW:
        comboAddQuiverMm(1);
        break;
    case GI_MM_QUIVER2:
        comboAddQuiverMm(2);
        break;
    case GI_MM_QUIVER3:
        comboAddQuiverMm(3);
        break;
    case GI_MM_WALLET:
        comboAddCommonItemMm(SITEM_WALLET, noEffect);
        break;
    case GI_MM_WALLET2:
        comboAddCommonItemMm(SITEM_WALLET2, noEffect);
        break;
    case GI_MM_WALLET3:
        comboAddCommonItemMm(SITEM_WALLET3, noEffect);
        break;
    case GI_MM_WALLET4:
        comboAddCommonItemMm(SITEM_WALLET4, noEffect);
        break;
    case GI_MM_GS_TOKEN_SWAMP:
        count = ++gMmSave.skullCountSwamp;
        break;
    case GI_MM_GS_TOKEN_OCEAN:
        count = ++gMmSave.skullCountOcean;
        break;
    case GI_MM_SONG_AWAKENING:
        gMmSave.inventory.quest.songAwakening = 1;
        break;
    case GI_MM_SONG_GORON:
        gMmSave.inventory.quest.songLullaby = 1;
        break;
    case GI_MM_SONG_ZORA:
        gMmSave.inventory.quest.songNewWave = 1;
        break;
    case GI_MM_SONG_EMPTINESS:
        gMmSave.inventory.quest.songEmpty = 1;
        break;
    case GI_MM_SONG_ORDER:
        gMmSave.inventory.quest.songOrder = 1;
        break;
    case GI_MM_SONG_TIME:
        comboAddCommonItemMm(SITEM_SONG_TIME, noEffect);
        break;
    case GI_MM_SONG_HEALING:
        gMmSave.inventory.quest.songHealing = 1;
        break;
    case GI_MM_SONG_EPONA:
        comboAddCommonItemMm(SITEM_SONG_EPONA, noEffect);
        break;
    case GI_MM_SONG_SOARING:
        gMmSave.inventory.quest.songSoaring = 1;
        break;
    case GI_MM_SONG_STORMS:
        comboAddCommonItemMm(SITEM_SONG_STORMS, noEffect);
        break;
    case GI_MM_SONG_GORON_HALF:
        gMmSave.inventory.quest.songLullabyIntro = 1;
        break;
    case GI_MM_SONG_SUN:
        comboAddCommonItemMm(SITEM_SONG_SUN, noEffect);
        break;
    case GI_MM_BOMBER_NOTEBOOK:
        gMmSave.inventory.quest.notebook = 1;
        break;
    case GI_MM_REMAINS_ODOLWA:
        gMmSave.inventory.quest.remainsOdolwa = 1;
        break;
    case GI_MM_REMAINS_GOHT:
        gMmSave.inventory.quest.remainsGoht = 1;
        break;
    case GI_MM_REMAINS_GYORG:
        gMmSave.inventory.quest.remainsGyorg = 1;
        break;
    case GI_MM_REMAINS_TWINMOLD:
        gMmSave.inventory.quest.remainsTwinmold = 1;
        break;
    case GI_MM_HEART_PIECE:
        comboAddCommonItemMm(SITEM_HEART_PIECE, noEffect);
        break;
    case GI_MM_HEART_CONTAINER:
        comboAddCommonItemMm(SITEM_HEART_CONTAINER, noEffect);
        break;
    case GI_MM_RECOVERY_HEART:
        comboAddCommonItemMm(SITEM_RECOVERY_HEART, noEffect);
        break;
    case GI_MM_RUPEE_GREEN:
        comboAddCommonItemMm(SITEM_RUPEE_GREEN, noEffect);
        break;
    case GI_MM_RUPEE_BLUE:
        comboAddCommonItemMm(SITEM_RUPEE_BLUE, noEffect);
        break;
    case GI_MM_RUPEE_RED:
        comboAddCommonItemMm(SITEM_RUPEE_RED, noEffect);
        break;
    case GI_MM_RUPEE_PURPLE:
        comboAddCommonItemMm(SITEM_RUPEE_PURPLE, noEffect);
        break;
    case GI_MM_RUPEE_SILVER:
        comboAddCommonItemMm(SITEM_RUPEE_SILVER, noEffect);
        break;
    case GI_MM_RUPEE_GOLD:
        comboAddCommonItemMm(SITEM_RUPEE_GOLD, noEffect);
        break;
    case GI_MM_SMALL_KEY_WF:
        count = comboAddSmallKeyMm(0);
        break;
    case GI_MM_SMALL_KEY_SH:
        count = comboAddSmallKeyMm(1);
        break;
    case GI_MM_SMALL_KEY_GB:
        count = comboAddSmallKeyMm(2);
        break;
    case GI_MM_SMALL_KEY_ST:
        count = comboAddSmallKeyMm(3);
        break;
    case GI_MM_BOSS_KEY_WF:
        comboAddBossKeyMm(0);
        break;
    case GI_MM_BOSS_KEY_SH:
        comboAddBossKeyMm(1);
        break;
    case GI_MM_BOSS_KEY_GB:
        comboAddBossKeyMm(2);
        break;
    case GI_MM_BOSS_KEY_ST:
        comboAddBossKeyMm(3);
        break;
    case GI_MM_STRAY_FAIRY_WF:
        count = comboAddStrayFairyMm(0);
        break;
    case GI_MM_STRAY_FAIRY_SH:
        count = comboAddStrayFairyMm(1);
        break;
    case GI_MM_STRAY_FAIRY_GB:
        count = comboAddStrayFairyMm(2);
        break;
    case GI_MM_STRAY_FAIRY_ST:
        count = comboAddStrayFairyMm(3);
        break;
    case GI_MM_STRAY_FAIRY_TOWN:
        count = comboAddStrayFairyMm(4);
        break;
    case GI_MM_DEFENSE_UPGRADE:
        comboAddCommonItemMm(SITEM_DEFENSE_UPGRADE, noEffect);
        break;
    case GI_MM_SPIN_UPGRADE:
        MM_SET_EVENT_WEEK(EV_MM_WEEK_SPIN_UPGRADE);
        break;
    case GI_MM_MAP_WF:
        comboAddMapMm(0);
        break;
    case GI_MM_MAP_SH:
        comboAddMapMm(1);
        break;
    case GI_MM_MAP_GB:
        comboAddMapMm(2);
        break;
    case GI_MM_MAP_ST:
        comboAddMapMm(3);
        break;
    case GI_MM_COMPASS_WF:
        comboAddCompassMm(0);
        break;
    case GI_MM_COMPASS_SH:
        comboAddCompassMm(1);
        break;
    case GI_MM_COMPASS_GB:
        comboAddCompassMm(2);
        break;
    case GI_MM_COMPASS_ST:
        comboAddCompassMm(3);
        break;
    case GI_MM_WORLD_MAP_CLOCK_TOWN:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_CLOCK_TOWN);
        break;
    case GI_MM_WORLD_MAP_WOODFALL:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_WOODFALL);
        break;
    case GI_MM_WORLD_MAP_SNOWHEAD:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_SNOWHEAD);
        break;
    case GI_MM_WORLD_MAP_ROMANI_RANCH:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_ROMANI_RANCH);
        break;
    case GI_MM_WORLD_MAP_GREAT_BAY:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_GREAT_BAY);
        break;
    case GI_MM_WORLD_MAP_STONE_TOWER:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_STONE_TOWER);
        break;
    case GI_MM_OWL_GREAT_BAY:
        gMmOwlFlags |= (1 << 0);
        break;
    case GI_MM_OWL_ZORA_CAPE:
        gMmOwlFlags |= (1 << 1);
        break;
    case GI_MM_OWL_SNOWHEAD:
        gMmOwlFlags |= (1 << 2);
        break;
    case GI_MM_OWL_MOUNTAIN_VILLAGE:
        gMmOwlFlags |= (1 << 3);
        break;
    case GI_MM_OWL_CLOCK_TOWN:
        gMmOwlFlags |= (1 << 4);
        break;
    case GI_MM_OWL_MILK_ROAD:
        gMmOwlFlags |= (1 << 5);
        break;
    case GI_MM_OWL_WOODFALL:
        gMmOwlFlags |= (1 << 6);
        break;
    case GI_MM_OWL_SOUTHERN_SWAMP:
        gMmOwlFlags |= (1 << 7);
        break;
    case GI_MM_OWL_IKANA_CANYON:
        gMmOwlFlags |= (1 << 8);
        break;
    case GI_MM_OWL_STONE_TOWER:
        gMmOwlFlags |= (1 << 9);
        break;
    }

    return count;
}
