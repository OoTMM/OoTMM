#include <combo.h>

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
    if (gMmSave.playerData.health > gMmSave.playerData.healthCapacity)
        gMmSave.playerData.health = gMmSave.playerData.healthCapacity;
}

static void addSword(int index)
{
    gMmSave.itemEquips.buttonItems[0][0] = kSwords[index];
    gMmSave.itemEquips.sword = index;
}

static void addShield(int index)
{
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

static void addNuts(int count)
{
    u16 max;

    max = 20;
    gMmSave.inventory.items[ITS_MM_NUTS] = ITEM_MM_NUT;
    gMmSave.inventory.ammo[ITS_MM_NUTS] += count;
    if (gMmSave.inventory.ammo[ITS_MM_NUTS] > max)
        gMmSave.inventory.ammo[ITS_MM_NUTS] = max;
}

static void addSticks(int count)
{
    u16 max;

    max = 10;
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
        if (gMmSave.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_EMPTY_BOTTLE)
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

    max = kMaxRupees[gMmSave.inventory.upgrades.wallet];
    gMmSave.playerData.rupees += count;
    if (gMmSave.playerData.rupees > max)
        gMmSave.playerData.rupees = max;
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

void comboAddCommonItemMm(int sid)
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
    }
}

static void addItemShared(s16 gi, int noEffect)
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

    if (comboConfig(CFG_SHARED_MAGIC_ARROWS))
    {
        switch (gi)
        {
        case GI_MM_ARROW_FIRE:
            comboAddCommonItemOot(SITEM_ARROW_FIRE);
            break;
        case GI_MM_ARROW_ICE:
            comboAddCommonItemOot(SITEM_ARROW_ICE);
            break;
        case GI_MM_ARROW_LIGHT:
            comboAddCommonItemOot(SITEM_ARROW_LIGHT);
            break;
        }
    }
}

int comboAddItemMm(s16 gi, int noEffect)
{
    int count;

    addItemShared(gi, noEffect);

    count = 0;
    switch (gi)
    {
    case GI_MM_OCARINA_OF_TIME:
        gMmSave.inventory.items[ITS_MM_OCARINA] = ITEM_MM_OCARINA_OF_TIME;
        break;
    case GI_MM_ARROW_FIRE:
        comboAddCommonItemMm(SITEM_ARROW_FIRE);
        break;
    case GI_MM_ARROW_ICE:
        comboAddCommonItemMm(SITEM_ARROW_ICE);
        break;
    case GI_MM_ARROW_LIGHT:
        comboAddCommonItemMm(SITEM_ARROW_LIGHT);
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
        gMmSave.inventory.items[ITS_MM_LENS] = ITEM_MM_LENS_OF_TRUTH;
        break;
    case GI_MM_HOOKSHOT:
        gMmSave.inventory.items[ITS_MM_HOOKSHOT] = ITEM_MM_HOOKSHOT;
        break;
    case GI_MM_GREAT_FAIRY_SWORD:
        gMmSave.inventory.items[ITS_MM_GREAT_FAIRY_SWORD] = ITEM_MM_GREAT_FAIRY_SWORD;
        break;
    case GI_MM_EMPTY_BOTTLE:
        addNewBottle(ITEM_MM_EMPTY_BOTTLE);
        break;
    case GI_MM_BOTTLED_MILK:
        addNewBottle(ITEM_MM_BOTTLED_MILK);
        break;
    case GI_MM_BOTTLED_GOLD_DUST:
        addNewBottle(ITEM_MM_BOTTLED_GOLD_DUST);
        break;
    case GI_MM_BOTTLED_CHATEAU_ROMANI:
        addNewBottle(ITEM_MM_BOTTLED_CHATEAU_ROMANI);
        break;
    case GI_MM_BOTTLED_POTION_RED:
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
        fillBottle(ITEM_MM_BOTTLED_MILK);
        break;
    case GI_MM_CHATEAU_ROMANI:
        fillBottle(ITEM_MM_BOTTLED_CHATEAU_ROMANI);
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
    case GI_MM_DEKU_NUT:
        addNuts(1);
        break;
    case GI_MM_DEKU_NUTS_5:
        addNuts(5);
        break;
    case GI_MM_DEKU_NUTS_10:
        addNuts(10);
        break;
    case GI_MM_DEKU_STICK:
        addSticks(1);
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
        gMmSave.inventory.items[ITS_MM_MASK_KEATON] = ITEM_MM_MASK_KEATON;
        break;
    case GI_MM_MASK_BREMEN:
        gMmSave.inventory.items[ITS_MM_MASK_BREMEN] = ITEM_MM_MASK_BREMEN;
        break;
    case GI_MM_MASK_BUNNY:
        gMmSave.inventory.items[ITS_MM_MASK_BUNNY] = ITEM_MM_MASK_BUNNY;
        break;
    case GI_MM_MASK_DON_GERO:
        gMmSave.inventory.items[ITS_MM_MASK_DON_GERO] = ITEM_MM_MASK_DON_GERO;
        break;
    case GI_MM_MASK_SCENTS:
        gMmSave.inventory.items[ITS_MM_MASK_SCENTS] = ITEM_MM_MASK_SCENTS;
        break;
    case GI_MM_MASK_GORON:
        gMmSave.inventory.items[ITS_MM_MASK_GORON] = ITEM_MM_MASK_GORON;
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
        gMmSave.inventory.items[ITS_MM_MASK_TRUTH] = ITEM_MM_MASK_TRUTH;
        break;
    case GI_MM_MASK_ZORA:
        gMmSave.inventory.items[ITS_MM_MASK_ZORA] = ITEM_MM_MASK_ZORA;
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
    case GI_MM_WALLET2:
        gMmSave.inventory.upgrades.wallet = 1;
        break;
    case GI_MM_WALLET3:
        gMmSave.inventory.upgrades.wallet = 2;
        break;
    case GI_MM_GS_TOKEN_SWAMP:
        count = ++gMmSave.skullCountSwamp;
        break;
    case GI_MM_GS_TOKEN_OCEAN:
        count = ++gMmSave.skullCountOcean;
        break;
    case GI_MM_SONG_AWAKENING:
        gMmSave.inventory.questItems.songAwakening = 1;
        break;
    case GI_MM_SONG_GORON:
        gMmSave.inventory.questItems.songLullaby = 1;
        break;
    case GI_MM_SONG_ZORA:
        gMmSave.inventory.questItems.songNewWave = 1;
        break;
    case GI_MM_SONG_EMPTINESS:
        gMmSave.inventory.questItems.songEmpty = 1;
        break;
    case GI_MM_SONG_ORDER:
        gMmSave.inventory.questItems.songOrder = 1;
        break;
    case GI_MM_SONG_TIME:
        gMmSave.inventory.questItems.songTime = 1;
        break;
    case GI_MM_SONG_HEALING:
        gMmSave.inventory.questItems.songHealing = 1;
        break;
    case GI_MM_SONG_EPONA:
        gMmSave.inventory.questItems.songEpona = 1;
        break;
    case GI_MM_SONG_SOARING:
        gMmSave.inventory.questItems.songSoaring = 1;
        break;
    case GI_MM_SONG_STORMS:
        gMmSave.inventory.questItems.songStorms = 1;
        break;
    case GI_MM_SONG_GORON_HALF:
        gMmSave.inventory.questItems.songLullabyIntro = 1;
        break;
    case GI_MM_BOMBER_NOTEBOOK:
        gMmSave.inventory.questItems.notebook = 1;
        break;
    case GI_MM_REMAINS_ODOLWA:
        gMmSave.inventory.questItems.remainsOdolwa = 1;
        break;
    case GI_MM_REMAINS_GOHT:
        gMmSave.inventory.questItems.remainsGoht = 1;
        break;
    case GI_MM_REMAINS_GYORG:
        gMmSave.inventory.questItems.remainsGyorg = 1;
        break;
    case GI_MM_REMAINS_TWINMOLD:
        gMmSave.inventory.questItems.remainsTwinmold = 1;
        break;
    case GI_MM_HEART_PIECE:
        gMmSave.inventory.questItems.heartPieces += 1;
        if (gMmSave.inventory.questItems.heartPieces == 4)
        {
            gMmSave.inventory.questItems.heartPieces = 0;
            gMmSave.playerData.healthCapacity += 0x10;
        }
        if (noEffect)
            addHealth(20);
        break;
    case GI_MM_HEART_CONTAINER:
        gMmSave.playerData.healthCapacity += 0x10;
        if (noEffect)
            addHealth(20);
        break;
    case GI_MM_RECOVERY_HEART:
        if (noEffect)
            addHealth(1);
        break;
    case GI_MM_RUPEE_GREEN:
        if (noEffect)
            addRupees(1);
        break;
    case GI_MM_RUPEE_BLUE:
        if (noEffect)
            addRupees(5);
        break;
    case GI_MM_RUPEE_RED:
        if (noEffect)
            addRupees(20);
        break;
    case GI_MM_RUPEE_PURPLE:
        if (noEffect)
            addRupees(50);
        break;
    case GI_MM_RUPEE_SILVER:
        if (noEffect)
            addRupees(100);
        break;
    case GI_MM_RUPEE_GOLD:
        if (noEffect)
            addRupees(200);
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
        gMmSave.playerData.doubleDefense = 1;
        gMmSave.inventory.defenseHearts = 20;
        if (noEffect)
            addHealth(20);
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
    }

    return count;
}
