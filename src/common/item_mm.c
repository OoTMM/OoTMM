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

void comboAddSmallKeyMm(u16 dungeonId)
{
    s8 keyCount;

    keyCount = gMmSave.inventory.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gMmSave.inventory.dungeonKeys[dungeonId] = keyCount;
}

void comboAddBossKeyMm(u16 dungeonId)
{
    gMmSave.inventory.dungeonItems[dungeonId].bossKey = 1;
}

void comboAddStrayFairyMm(u16 dungeonId)
{
    if (dungeonId == 4)
        MM_SET_EVENT_WEEK(MM_EV(8, 7));
    else
        gMmSave.inventory.strayFairies[dungeonId]++;
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

static void addBombBag(int index)
{
    gMmSave.inventory.items[ITS_MM_BOMBS] = ITEM_MM_BOMB;
    gMmSave.inventory.upgrades.bombBag = index;
    gMmSave.inventory.ammo[ITS_MM_BOMBS] = kMaxBombs[index];
}

static void addBowQuiver(int index)
{
    gMmSave.inventory.items[ITS_MM_BOW] = ITEM_MM_BOW;
    gMmSave.inventory.upgrades.quiver = index;
    gMmSave.inventory.ammo[ITS_MM_BOW] = kMaxArrows[index];
}

static void addBombs(int count)
{
    u16 max;

    max = kMaxBombs[gMmSave.inventory.upgrades.bombBag];
    gMmSave.inventory.items[ITS_MM_BOMBS] = ITEM_MM_BOMB;
    gMmSave.inventory.ammo[ITS_MM_BOMBS] += count;
    if (gMmSave.inventory.ammo[ITS_MM_BOMBS] > max)
        gMmSave.inventory.ammo[ITS_MM_BOMBS] = max;
}

static void addArrows(int count)
{
    u16 max;

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

static void addBombchus(int count)
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
    for (int i = 0; i < 6; ++i)
    {
        if (gMmSave.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_EMPTY_BOTTLE)
        {
            gMmSave.inventory.items[ITS_MM_BOTTLE + i] = itemId;
            break;
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

static void addMagicUpgrade(int level)
{
    gMmSave.playerData.magicAcquired = 1;
    if (level >= 2)
        gMmSave.playerData.doubleMagic = 1;
}

static void refillMagic(int level)
{
    gMmSave.playerData.magicLevel = level;
    gMmSave.playerData.magic = level * 0x30;
}

void comboAddItemMm(u16 itemId, int noEffect)
{
    switch (itemId)
    {
    case ITEM_MM_OCARINA_OF_TIME:
        gMmSave.inventory.items[ITS_MM_OCARINA] = ITEM_MM_OCARINA_OF_TIME;
        break;
    case ITEM_MM_ARROW_FIRE:
        gMmSave.inventory.items[ITS_MM_ARROW_FIRE] = ITEM_MM_ARROW_FIRE;
        break;
    case ITEM_MM_ARROW_ICE:
        gMmSave.inventory.items[ITS_MM_ARROW_ICE] = ITEM_MM_ARROW_ICE;
        break;
    case ITEM_MM_ARROW_LIGHT:
        gMmSave.inventory.items[ITS_MM_ARROW_LIGHT] = ITEM_MM_ARROW_LIGHT;
        break;
    case ITEM_MM_MAGIC_BEAN:
        gMmSave.inventory.items[ITS_MM_BEANS] = ITEM_MM_MAGIC_BEAN;
        gMmSave.inventory.ammo[ITS_MM_BEANS]++;
        break;
    case ITEM_MM_POWDER_KEG:
        gMmSave.inventory.items[ITS_MM_KEG] = ITEM_MM_POWDER_KEG;
        gMmSave.inventory.ammo[ITS_MM_KEG] = 1;
        break;
    case ITEM_MM_PICTOGRAPH_BOX:
        gMmSave.inventory.items[ITS_MM_PICTOBOX] = ITEM_MM_PICTOGRAPH_BOX;
        break;
    case ITEM_MM_LENS_OF_TRUTH:
        gMmSave.inventory.items[ITS_MM_LENS] = ITEM_MM_LENS_OF_TRUTH;
        break;
    case ITEM_MM_HOOKSHOT:
        gMmSave.inventory.items[ITS_MM_HOOKSHOT] = ITEM_MM_HOOKSHOT;
        break;
    case ITEM_MM_GREAT_FAIRY_SWORD:
        gMmSave.inventory.items[ITS_MM_GREAT_FAIRY_SWORD] = ITEM_MM_GREAT_FAIRY_SWORD;
        break;
    case ITEM_MM_EMPTY_BOTTLE:
    case ITEM_MM_BOTTLED_MILK:
    case ITEM_MM_BOTTLED_GOLD_DUST:
    case ITEM_MM_BOTTLED_CHATEAU_ROMANI:
        addNewBottle(itemId);
        break;
    case ITEM_MM_RED_POTION_WITH_BOTTLE:
        addNewBottle(ITEM_MM_POTION_RED);
        break;
    case ITEM_MM_POTION_RED:
    case ITEM_MM_POTION_BLUE:
    case ITEM_MM_POTION_GREEN:
    case ITEM_MM_BOTTLED_MILK_HALF:
        fillBottle(itemId);
        break;
    case ITEM_MM_SEAHORSE:
    case ITEM_MM_BOTTLED_SEAHORSE:
        fillBottle(ITEM_MM_BOTTLED_SEAHORSE);
        break;
    case ITEM_MM_MILK:
        fillBottle(ITEM_MM_BOTTLED_MILK);
        break;
    case ITEM_MM_CHATEAU_ROMANI:
        fillBottle(ITEM_MM_BOTTLED_CHATEAU_ROMANI);
        break;
    case ITEM_MM_BOMB:
        addBombs(1);
        break;
    case ITEM_MM_BOMBS_5:
        addBombs(5);
        break;
    case ITEM_MM_BOMBS_10:
        addBombs(10);
        break;
    case ITEM_MM_BOMBS_20:
        addBombs(20);
        break;
    case ITEM_MM_BOMBS_30:
        addBombs(30);
        break;
    case ITEM_MM_ARROWS_10:
        addArrows(10);
        break;
    case ITEM_MM_ARROWS_30:
        addArrows(30);
        break;
    case ITEM_MM_ARROWS_40:
        addArrows(40);
        break;
    case ITEM_MM_NUT:
        addNuts(1);
        break;
    case ITEM_MM_NUTS_5:
        addNuts(5);
        break;
    case ITEM_MM_NUTS_10:
        addNuts(10);
        break;
    case ITEM_MM_NUTS_30:
        addNuts(30);
        break;
    case ITEM_MM_NUTS_40:
        addNuts(40);
        break;
    case ITEM_MM_STICK:
        addSticks(1);
        break;
    case ITEM_MM_STICKS_20:
        addSticks(20);
        break;
    case ITEM_MM_STICKS_30:
        addSticks(30);
        break;
    case ITEM_MM_BOMBCHU:
    case ITEM_MM_BOMBCHU_ALT:
        addBombchus(1);
        break;
    case ITEM_MM_BOMBCHUS_5:
        addBombchus(5);
        break;
    case ITEM_MM_BOMBCHUS_10:
        addBombchus(10);
        break;
    case ITEM_MM_BOMBCHUS_20:
        addBombchus(20);
        break;
    case ITEM_MM_MOON_TEAR:
        addTrade1(XITEM_MM_TRADE1_MOON_TEAR);
        break;
    case ITEM_MM_DEED_LAND:
        addTrade1(XITEM_MM_TRADE1_DEED_LAND);
        break;
    case ITEM_MM_DEED_SWAMP:
        addTrade1(XITEM_MM_TRADE1_DEED_SWAMP);
        break;
    case ITEM_MM_DEED_MOUNTAIN:
        addTrade1(XITEM_MM_TRADE1_DEED_MOUNTAIN);
        break;
    case ITEM_MM_DEED_OCEAN:
        addTrade1(XITEM_MM_TRADE1_DEED_OCEAN);
        break;
    case ITEM_MM_ROOM_KEY:
        addTrade2(XITEM_MM_TRADE2_ROOM_KEY);
        break;
    case ITEM_MM_LETTER_TO_MAMA:
        addTrade2(XITEM_MM_TRADE2_LETTER_TO_MAMA);
        break;
    case ITEM_MM_LETTER_TO_KAFEI:
        addTrade3(XITEM_MM_TRADE3_LETTER_TO_KAFEI);
        break;
    case ITEM_MM_PENDANT_OF_MEMORIES:
        addTrade3(XITEM_MM_TRADE3_PENDANT_OF_MEMORIES);
        break;
    case ITEM_MM_MASK_POSTMAN:
        gMmSave.inventory.items[ITS_MM_MASK_POSTMAN] = ITEM_MM_MASK_POSTMAN;
        break;
    case ITEM_MM_MASK_ALL_NIGHT:
        gMmSave.inventory.items[ITS_MM_MASK_ALL_NIGHT] = ITEM_MM_MASK_ALL_NIGHT;
        break;
    case ITEM_MM_MASK_BLAST:
        gMmSave.inventory.items[ITS_MM_MASK_BLAST] = ITEM_MM_MASK_BLAST;
        break;
    case ITEM_MM_MASK_STONE:
        gMmSave.inventory.items[ITS_MM_MASK_STONE] = ITEM_MM_MASK_STONE;
        break;
    case ITEM_MM_MASK_GREAT_FAIRY:
        gMmSave.inventory.items[ITS_MM_MASK_GREAT_FAIRY] = ITEM_MM_MASK_GREAT_FAIRY;
        break;
    case ITEM_MM_MASK_DEKU:
        gMmSave.inventory.items[ITS_MM_MASK_DEKU] = ITEM_MM_MASK_DEKU;
        break;
    case ITEM_MM_MASK_KEATON:
        gMmSave.inventory.items[ITS_MM_MASK_KEATON] = ITEM_MM_MASK_KEATON;
        break;
    case ITEM_MM_MASK_BREMEN:
        gMmSave.inventory.items[ITS_MM_MASK_BREMEN] = ITEM_MM_MASK_BREMEN;
        break;
    case ITEM_MM_MASK_BUNNY:
        gMmSave.inventory.items[ITS_MM_MASK_BUNNY] = ITEM_MM_MASK_BUNNY;
        break;
    case ITEM_MM_MASK_DON_GERO:
        gMmSave.inventory.items[ITS_MM_MASK_DON_GERO] = ITEM_MM_MASK_DON_GERO;
        break;
    case ITEM_MM_MASK_SCENTS:
        gMmSave.inventory.items[ITS_MM_MASK_SCENTS] = ITEM_MM_MASK_SCENTS;
        break;
    case ITEM_MM_MASK_GORON:
        gMmSave.inventory.items[ITS_MM_MASK_GORON] = ITEM_MM_MASK_GORON;
        break;
    case ITEM_MM_MASK_ROMANI:
        gMmSave.inventory.items[ITS_MM_MASK_ROMANI] = ITEM_MM_MASK_ROMANI;
        break;
    case ITEM_MM_MASK_TROUPE_LEADER:
        gMmSave.inventory.items[ITS_MM_MASK_TROUPE_LEADER] = ITEM_MM_MASK_TROUPE_LEADER;
        break;
    case ITEM_MM_MASK_KAFEI:
        gMmSave.inventory.items[ITS_MM_MASK_KAFEI] = ITEM_MM_MASK_KAFEI;
        break;
    case ITEM_MM_MASK_COUPLE:
        gMmSave.inventory.items[ITS_MM_MASK_COUPLE] = ITEM_MM_MASK_COUPLE;
        break;
    case ITEM_MM_MASK_TRUTH:
        gMmSave.inventory.items[ITS_MM_MASK_TRUTH] = ITEM_MM_MASK_TRUTH;
        break;
    case ITEM_MM_MASK_ZORA:
        gMmSave.inventory.items[ITS_MM_MASK_ZORA] = ITEM_MM_MASK_ZORA;
        break;
    case ITEM_MM_MASK_KAMARO:
        gMmSave.inventory.items[ITS_MM_MASK_KAMARO] = ITEM_MM_MASK_KAMARO;
        break;
    case ITEM_MM_MASK_GIBDO:
        gMmSave.inventory.items[ITS_MM_MASK_GIBDO] = ITEM_MM_MASK_GIBDO;
        break;
    case ITEM_MM_MASK_GARO:
        gMmSave.inventory.items[ITS_MM_MASK_GARO] = ITEM_MM_MASK_GARO;
        break;
    case ITEM_MM_MASK_CAPTAIN:
        gMmSave.inventory.items[ITS_MM_MASK_CAPTAIN] = ITEM_MM_MASK_CAPTAIN;
        break;
    case ITEM_MM_MASK_GIANT:
        gMmSave.inventory.items[ITS_MM_MASK_GIANT] = ITEM_MM_MASK_GIANT;
        break;
    case ITEM_MM_MASK_FIERCE_DEITY:
        gMmSave.inventory.items[ITS_MM_MASK_FIERCE_DEITY] = ITEM_MM_MASK_FIERCE_DEITY;
        break;
    case ITEM_MM_MAGIC_UPGRADE:
        addMagicUpgrade(1);
        if (noEffect)
            refillMagic(1);
        break;
    case ITEM_MM_MAGIC_UPGRADE2:
        addMagicUpgrade(2);
        if (noEffect)
            refillMagic(2);
        break;
    case ITEM_MM_SWORD_KOKIRI:
        addSword(1);
        break;
    case ITEM_MM_SWORD_RAZOR:
        addSword(2);
        gMmSave.playerData.swordHealth = 100;
        break;
    case ITEM_MM_SWORD_GILDED:
        addSword(3);
        break;
    case ITEM_MM_PROGRESSIVE_SHIELD_HERO:
        gMmExtraFlags2.progressiveShield = 1;
        /* Fallthrough */
    case ITEM_MM_SHIELD_HERO:
        addShield(1);
        break;
    case ITEM_MM_SHIELD_MIRROR:
        addShield(2);
        break;
    case ITEM_MM_BOMB_BAG:
        addBombBag(1);
        break;
    case ITEM_MM_BOMB_BAG2:
        addBombBag(2);
        break;
    case ITEM_MM_BOMB_BAG3:
        addBombBag(3);
        break;
    case ITEM_MM_BOW:
    case ITEM_MM_QUIVER:
        addBowQuiver(1);
        break;
    case ITEM_MM_QUIVER2:
        addBowQuiver(2);
        break;
    case ITEM_MM_QUIVER3:
        addBowQuiver(3);
        break;
    case ITEM_MM_WALLET2:
        gMmSave.inventory.upgrades.wallet = 1;
        break;
    case ITEM_MM_WALLET3:
        gMmSave.inventory.upgrades.wallet = 2;
        break;
    case ITEM_MM_GS_TOKEN_SWAMP:
        gMmSave.skullCountSwamp++;
        break;
    case ITEM_MM_GS_TOKEN_OCEAN:
        gMmSave.skullCountOcean++;
        break;
    case ITEM_MM_SONG_AWAKENING:
        gMmSave.inventory.questItems.songAwakening = 1;
        break;
    case ITEM_MM_SONG_GORON:
        gMmSave.inventory.questItems.songLullaby = 1;
        break;
    case ITEM_MM_SONG_ZORA:
        gMmSave.inventory.questItems.songNewWave = 1;
        break;
    case ITEM_MM_SOMG_EMPTINESS:
        gMmSave.inventory.questItems.songEmpty = 1;
        break;
    case ITEM_MM_SONG_ORDER:
        gMmSave.inventory.questItems.songOrder = 1;
        break;
    case ITEM_MM_SONG_TIME:
        gMmSave.inventory.questItems.songTime = 1;
        break;
    case ITEM_MM_SONG_HEALING:
        gMmSave.inventory.questItems.songHealing = 1;
        break;
    case ITEM_MM_SONG_EPONA:
        gMmSave.inventory.questItems.songEpona = 1;
        break;
    case ITEM_MM_SONG_SOARING:
        gMmSave.inventory.questItems.songSoaring = 1;
        break;
    case ITEM_MM_SONG_STORMS:
        gMmSave.inventory.questItems.songStorms = 1;
        break;
    case ITEM_MM_SONG_GORON_HALF:
        gMmSave.inventory.questItems.songLullabyIntro = 1;
        break;
    case ITEM_MM_BOMBER_NOTEBOOK:
        gMmSave.inventory.questItems.notebook = 1;
        break;
    case ITEM_MM_REMAINS_ODOLWA:
        gMmSave.inventory.questItems.remainsOdolwa = 1;
        break;
    case ITEM_MM_REMAINS_GOHT:
        gMmSave.inventory.questItems.remainsGoht = 1;
        break;
    case ITEM_MM_REMAINS_GYORG:
        gMmSave.inventory.questItems.remainsGyorg = 1;
        break;
    case ITEM_MM_REMAINS_TWINMOLD:
        gMmSave.inventory.questItems.remainsTwinmold = 1;
        break;
    case ITEM_MM_HEART_PIECE:
    case ITEM_MM_HEART_PIECE2:
        gMmSave.inventory.questItems.heartPieces += 1;
        if (gMmSave.inventory.questItems.heartPieces == 4)
        {
            gMmSave.inventory.questItems.heartPieces = 0;
            gMmSave.playerData.healthCapacity += 0x10;
        }
        if (noEffect)
            addHealth(20);
        break;
    case ITEM_MM_HEART_CONTAINER:
        gMmSave.playerData.healthCapacity += 0x10;
        if (noEffect)
            addHealth(20);
        break;
    case ITEM_MM_RECOVERY_HEART:
        if (noEffect)
            addHealth(1);
        break;
    case ITEM_MM_RUPEE_GREEN:
        if (noEffect)
            addRupees(1);
        break;
    case ITEM_MM_RUPEE_BLUE:
        if (noEffect)
            addRupees(5);
        break;
    case ITEM_MM_RUPEE_RED10:
        if (noEffect)
            addRupees(10);
        break;
    case ITEM_MM_RUPEE_RED:
        if (noEffect)
            addRupees(20);
        break;
    case ITEM_MM_RUPEE_PURPLE:
        if (noEffect)
            addRupees(50);
        break;
    case ITEM_MM_RUPEE_SILVER:
        if (noEffect)
            addRupees(100);
        break;
    case ITEM_MM_RUPEE_GOLD:
        if (noEffect)
            addRupees(200);
        break;
    case ITEM_MM_SMALL_KEY_WF:
        comboAddSmallKeyMm(0);
        break;
    case ITEM_MM_SMALL_KEY_SH:
        comboAddSmallKeyMm(1);
        break;
    case ITEM_MM_SMALL_KEY_GB:
        comboAddSmallKeyMm(2);
        break;
    case ITEM_MM_SMALL_KEY_ST:
        comboAddSmallKeyMm(3);
        break;
    case ITEM_MM_BOSS_KEY_WF:
        comboAddBossKeyMm(0);
        break;
    case ITEM_MM_BOSS_KEY_SH:
        comboAddBossKeyMm(1);
        break;
    case ITEM_MM_BOSS_KEY_GB:
        comboAddBossKeyMm(2);
        break;
    case ITEM_MM_BOSS_KEY_ST:
        comboAddBossKeyMm(3);
        break;
    case ITEM_MM_STRAY_FAIRY_WF:
        comboAddStrayFairyMm(0);
        break;
    case ITEM_MM_STRAY_FAIRY_SH:
        comboAddStrayFairyMm(1);
        break;
    case ITEM_MM_STRAY_FAIRY_GB:
        comboAddStrayFairyMm(2);
        break;
    case ITEM_MM_STRAY_FAIRY_ST:
        comboAddStrayFairyMm(3);
        break;
    case ITEM_MM_STRAY_FAIRY_TOWN:
        comboAddStrayFairyMm(4);
        break;
    case ITEM_MM_DEFENSE_UPGRADE:
        gMmSave.playerData.doubleDefense = 1;
        gMmSave.inventory.defenseHearts = 20;
        if (noEffect)
            addHealth(20);
        break;
    case ITEM_MM_SPIN_UPGRADE:
        MM_SET_EVENT_WEEK(EV_MM_WEEK_SPIN_UPGRADE);
        break;
    }
}
