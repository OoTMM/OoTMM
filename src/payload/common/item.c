#include <combo.h>

void DisplayTextBox(void* play, s16 textId);

#if defined(GAME_OOT)
# define ITEM_BASE_MM 0x100
#else
# define ITEM_BASE_MM 0x0
#endif

static const u8 kMaxSticks[] = { 0, 10, 20, 30 };
static const u8 kMaxNuts[] = { 0, 20, 30, 40 };
static const u8 kMaxBombs[] = { 0, 20, 30, 40 };
static const u8 kMaxArrows[] = { 0, 30, 40, 50 };
static const u8 kMaxSeeds[] = { 0, 30, 40, 50 };
static const u16 kMaxRupees[] = { 99, 200, 500 };

static void addOotAmmo(u16 itemId, u8 max, u8 count)
{
    gOotSave.inventory[itemId] = itemId;
    gOotSave.ammo[itemId] += count;
    if (gOotSave.ammo[itemId] > max)
        gOotSave.ammo[itemId] = max;
}

static void addItemOotSticks(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.dekuStick == 0)
        gOotSave.upgrades.dekuStick = 1;

    max = kMaxSticks[gOotSave.upgrades.dekuStick];
    addOotAmmo(ITEM_OOT_DEKU_STICK, max, count);
}

static void addItemOotNuts(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.dekuNut == 0)
        gOotSave.upgrades.dekuNut = 1;

    max = kMaxNuts[gOotSave.upgrades.dekuStick];
    addOotAmmo(ITEM_OOT_DEKU_NUT, max, count);
}

static void addItemOotBombs(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.bombBag == 0)
        return;
    max = kMaxBombs[gOotSave.upgrades.bombBag];
    addOotAmmo(ITEM_OOT_BOMB, max, count);
}

static void addItemOotArrows(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.quiver == 0)
        return;
    max = kMaxArrows[gOotSave.upgrades.quiver];
    addOotAmmo(ITEM_OOT_FAIRY_BOW, max, count);
}

static void addItemOotSeeds(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.bulletBag == 0)
        return;
    max = kMaxSeeds[gOotSave.upgrades.dekuStick];
    addOotAmmo(ITEM_OOT_FAIRY_SLINGSHOT, max, count);
}

static void addItemOotBombchus(u8 count)
{
    addOotAmmo(ITEM_OOT_BOMBCHU_10, 50, count);
}

/* Also Bomb Bag 1 */
static void addItemOotInventory(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_DEKU_STICK:
        addItemOotSticks(1);
        break;
    case ITEM_OOT_DEKU_STICKS_5:
        addItemOotSticks(5);
        break;
    case ITEM_OOT_DEKU_STICKS_10:
        addItemOotSticks(10);
        break;
    case ITEM_OOT_DEKU_NUT:
        addItemOotNuts(1);
        break;
    case ITEM_OOT_DEKU_NUTS_5:
        addItemOotNuts(5);
        break;
    case ITEM_OOT_DEKU_NUTS_10:
        addItemOotNuts(10);
        break;
    case ITEM_OOT_BOMB_BAG:
        gOotSave.inventory[ITS_OOT_BOMBS] = ITEM_OOT_BOMB;
        gOotSave.ammo[ITS_OOT_BOMBS] = 20;
        gOotSave.upgrades.bombBag = 1;
        break;
    case ITEM_OOT_BOMB:
        addItemOotBombs(1);
        break;
    case ITEM_OOT_BOMBS_5:
        addItemOotBombs(5);
        break;
    case ITEM_OOT_BOMBS_10:
        addItemOotBombs(10);
        break;
    case ITEM_OOT_BOMBS_20:
        addItemOotBombs(20);
        break;
    case ITEM_OOT_BOMBS_30:
        addItemOotBombs(30);
        break;
    case ITEM_OOT_FAIRY_BOW:
        gOotSave.inventory[ITS_OOT_BOW] = ITEM_OOT_FAIRY_BOW;
        gOotSave.ammo[ITS_OOT_BOW] = 30;
        gOotSave.upgrades.quiver = 1;
        break;
    case ITEM_OOT_ARROWS_5:
        addItemOotArrows(5);
        break;
    case ITEM_OOT_ARROWS_10:
        addItemOotArrows(10);
        break;
    case ITEM_OOT_ARROWS_30:
        addItemOotArrows(30);
        break;
    case ITEM_OOT_FAIRY_SLINGSHOT:
    case ITEM_OOT_BULLET_BAG:
        gOotSave.inventory[ITS_OOT_SLINGSHOT] = ITEM_OOT_FAIRY_SLINGSHOT;
        gOotSave.ammo[ITS_OOT_SLINGSHOT] = 30;
        gOotSave.upgrades.bulletBag = 1;
        break;
    case ITEM_OOT_DEKU_SEEDS_5:
        addItemOotSeeds(5);
        break;
    case ITEM_OOT_DEKU_SEEDS_30:
        addItemOotSeeds(30);
        break;
    case ITEM_OOT_FAIRY_OCARINA:
        if (gOotSave.inventory[ITS_OOT_OCARINA] == ITEM_NONE)
            gOotSave.inventory[ITS_OOT_OCARINA] = ITEM_OOT_FAIRY_OCARINA;
        gComboSave.ootOcarinas |= 0x1;
        break;
    case ITEM_OOT_OCARINA_OF_TIME:
        gOotSave.inventory[ITS_OOT_OCARINA] = ITEM_OOT_OCARINA_OF_TIME;
        gComboSave.ootOcarinas |= 0x2;
        break;
    case ITEM_OOT_BOMBCHU_5:
        addItemOotBombchus(5);
        break;
    case ITEM_OOT_BOMBCHU_10:
        addItemOotBombchus(10);
        break;
    case ITEM_OOT_BOMBCHU_20:
        addItemOotBombchus(20);
        break;
    case ITEM_OOT_HOOKSHOT:
        if (gOotSave.inventory[ITS_OOT_HOOKSHOT] == ITEM_NONE)
            gOotSave.inventory[ITS_OOT_HOOKSHOT] = ITEM_OOT_HOOKSHOT;
        gComboSave.ootHookshots |= 0x1;
        break;
    case ITEM_OOT_LONGSHOT:
        gOotSave.inventory[ITS_OOT_HOOKSHOT] = ITEM_OOT_LONGSHOT;
        gComboSave.ootHookshots |= 0x2;
        break;
    }
}

static void addItemOotChildTrade(u16 itemId)
{
    u32 bit;

    switch (itemId)
    {
    case ITEM_OOT_WEIRD_EGG:
        bit = 0;
        break;
    case ITEM_OOT_CHICKEN:
        bit = 1;
        break;
    case ITEM_OOT_ZELDA_LETTER:
        bit = 2;
        break;
    case ITEM_OOT_KEATON_MASK:
        bit = 3;
        break;
    case ITEM_OOT_SKULL_MASK:
        bit = 4;
        break;
    case ITEM_OOT_SPOOKY_MASK:
        bit = 5;
        break;
    case ITEM_OOT_BUNNY_HOOD:
        bit = 6;
        break;
    case ITEM_OOT_GORON_MASK:
        bit = 7;
        break;
    case ITEM_OOT_ZORA_MASK:
        bit = 8;
        break;
    case ITEM_OOT_GERUDO_MASK:
        bit = 9;
        break;
    case ITEM_OOT_MASK_OF_TRUTH:
        bit = 10;
        break;
    default:
        return;
    }

    if (gOotSave.inventory[ITS_OOT_TRADE_CHILD] == ITEM_NONE)
        gOotSave.inventory[ITS_OOT_TRADE_CHILD] = itemId;
    gComboSave.ootTradeChild |= (1 << bit);
}

static void addItemOotEquipment(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_KOKIRI_SWORD:
        gOotSave.equipment.swords |= EQ_OOT_SWORD_KOKIRI;
        break;
    case ITEM_OOT_MASTER_SWORD:
        gOotSave.equipment.swords |= EQ_OOT_SWORD_MASTER;
        break;
    case ITEM_OOT_GIANT_KNIFE_BS:
        gOotSave.equipment.swords |= EQ_OOT_SWORD_KNIFE;
        break;
    case ITEM_OOT_DEKU_SHIELD:
        gOotSave.equipment.shields |= EQ_OOT_SHIELD_DEKU;
        gComboSave.ootShields |= EQ_OOT_SHIELD_DEKU;
        break;
    case ITEM_OOT_HYLIAN_SHIELD:
        gOotSave.equipment.shields |= EQ_OOT_SHIELD_HYLIAN;
        gComboSave.ootShields |= EQ_OOT_SHIELD_HYLIAN;
        break;
    case ITEM_OOT_MIRROR_SHIELD:
        gOotSave.equipment.shields |= EQ_OOT_SHIELD_MIRROR;
        gComboSave.ootShields |= EQ_OOT_SHIELD_MIRROR;
        break;
    }
}

static void addItemOotUpgrade(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_GORON_BRACELET:
        gOotSave.upgrades.strength = 1;
        break;
    case ITEM_OOT_SILVER_GAUNTLETS:
        gOotSave.upgrades.strength = 2;
        break;
    case ITEM_OOT_GOLDEN_GAUNTLETS:
        gOotSave.upgrades.strength = 3;
        break;
    case ITEM_OOT_SILVER_SCALE:
        gOotSave.upgrades.dive = 1;
        break;
    case ITEM_OOT_GOLDEN_SCALE:
        gOotSave.upgrades.dive = 2;
        break;
    case ITEM_OOT_BULLET_BAG2:
        gOotSave.upgrades.bulletBag = 2;
        break;
    case ITEM_OOT_BULLET_BAG3:
        gOotSave.upgrades.bulletBag = 3;
        break;
    case ITEM_OOT_QUIVER2:
        gOotSave.upgrades.quiver = 2;
        break;
    case ITEM_OOT_QUIVER3:
        gOotSave.upgrades.quiver = 3;
        break;
    case ITEM_OOT_BOMB_BAG2:
        gOotSave.upgrades.bombBag = 2;
        break;
    case ITEM_OOT_BOMB_BAG3:
        gOotSave.upgrades.bombBag = 3;
        break;
    case ITEM_OOT_WALLET2:
        gOotSave.upgrades.wallet = 1;
        break;
    case ITEM_OOT_WALLET3:
        gOotSave.upgrades.wallet = 2;
        break;
    case ITEM_OOT_MAGIC_UPGRADE:
        gOotSave.magicUpgrade = 1;
#if defined(GAME_OOT)
        gOotSave.magicSize = 0;
        gSaveContext.magicTarget = 0x30;
#else
        gOotSave.magicSize = 1;
        gOotSave.magicAmount = 0x30;
#endif
        break;
    case ITEM_OOT_MAGIC_UPGRADE2:
        gOotSave.magicUpgrade2 = 1;
#if defined(GAME_OOT)
        gOotSave.magicSize = 0;
        gSaveContext.magicTarget = 0x60;
#else
        gOotSave.magicSize = 2;
        gOotSave.magicAmount = 0x60;
#endif
        break;
    }
}

static void addItemOotQuest(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_GERUDO_CARD:
        gOotSave.quest.gerudoCard = 1;
        break;
    case ITEM_OOT_STONE_OF_AGONY:
        gOotSave.quest.agonyStone = 1;
        break;
    case ITEM_OOT_STONE_SAPPHIRE:
        gOotSave.quest.stoneSapphire = 1;
        break;
    case ITEM_OOT_STONE_RUBY:
        gOotSave.quest.stoneRuby = 1;
        break;
    case ITEM_OOT_STONE_EMERALD:
        gOotSave.quest.stoneEmerald = 1;
        break;
    case ITEM_OOT_SONG_STORMS:
        gOotSave.quest.songStorms = 1;
        break;
    case ITEM_OOT_SONG_TIME:
        gOotSave.quest.songTime = 1;
        break;
    case ITEM_OOT_SONG_SUN:
        gOotSave.quest.songSun = 1;
        break;
    case ITEM_OOT_SONG_SARIA:
        gOotSave.quest.songSaria = 1;
        break;
    case ITEM_OOT_SONG_EPONA:
        gOotSave.quest.songEpona = 1;
        break;
    case ITEM_OOT_SONG_ZELDA:
        gOotSave.quest.songZelda = 1;
        break;
    case ITEM_OOT_SONG_TP_LIGHT:
        gOotSave.quest.songTpLight = 1;
        break;
    case ITEM_OOT_SONG_TP_SHADOW:
        gOotSave.quest.songTpShadow = 1;
        break;
    case ITEM_OOT_SONG_TP_SPIRIT:
        gOotSave.quest.songTpSpirit = 1;
        break;
    case ITEM_OOT_SONG_TP_WATER:
        gOotSave.quest.songTpWater = 1;
        break;
    case ITEM_OOT_SONG_TP_FIRE:
        gOotSave.quest.songTpFire = 1;
        break;
    case ITEM_OOT_SONG_TP_FOREST:
        gOotSave.quest.songTpForest = 1;
        break;
    case ITEM_OOT_MEDALLION_LIGHT:
        gOotSave.quest.medallionLight = 1;
        break;
    case ITEM_OOT_MEDALLION_SHADOW:
        gOotSave.quest.medallionShadow = 1;
        break;
    case ITEM_OOT_MEDALLION_SPIRIT:
        gOotSave.quest.medallionSpirit = 1;
        break;
    case ITEM_OOT_MEDALLION_WATER:
        gOotSave.quest.medallionWater = 1;
        break;
    case ITEM_OOT_MEDALLION_FIRE:
        gOotSave.quest.medallionFire = 1;
        break;
    case ITEM_OOT_MEDALLION_FOREST:
        gOotSave.quest.medallionForest = 1;
        break;
    }
}

static void addItemOotRupee(u16 itemId)
{
    u16 count;
    u16 max;

    switch (itemId)
    {
    case ITEM_OOT_RUPEE_GREEN:
        count = 1;
        break;
    case ITEM_OOT_RUPEE_BLUE:
        count = 5;
        break;
    case ITEM_OOT_RUPEE_RED:
        count = 20;
        break;
    case ITEM_OOT_RUPEE_PURPLE:
        count = 50;
        break;
    case ITEM_OOT_RUPEE_HUGE:
        count = 200;
        break;
    default:
        return;
    }

#if defined(GAME_OOT)
    gSaveContext.rupeesDelta += count;
#else
    max = kMaxRupees[gOotSave.upgrades.wallet];
    gOotSave.rupees += count;
    if (gOotSave.rupees > max)
        gOotSave.rupees = max;
#endif
}

static void addItemOot(u16 itemId)
{
    addItemOotInventory(itemId);
    addItemOotChildTrade(itemId);
    addItemOotEquipment(itemId);
    addItemOotUpgrade(itemId);
    addItemOotQuest(itemId);
    addItemOotRupee(itemId);
}

static void addItemMm(u16 itemId)
{

}

int comboAddItem(void* play, u16 itemId)
{
    if ((itemId & 0x100) == ITEM_BASE_MM)
        addItemMm(itemId & 0xff);
    else
        addItemOot(itemId & 0xff);

    return -1;
}

int comboAddItemFromChest(void* play, s16 chestId)
{
    u16 itemId;
    u16 textId;

    chestId--;
    itemId = kExtendedGetItems[chestId].itemId;
    itemId |= (chestId & 0x100);

    comboTextHijackItem(play, itemId);
    return comboAddItem(play, itemId);
}
