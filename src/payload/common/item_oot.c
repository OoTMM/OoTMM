#include <combo.h>

static void addAmmo(u16 itemId, u8 max, u8 count)
{
    gOotSave.inventory[itemId] = itemId;
    gOotSave.ammo[itemId] += count;
    if (gOotSave.ammo[itemId] > max)
        gOotSave.ammo[itemId] = max;
}

static void addSticks(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.dekuStick == 0)
        gOotSave.upgrades.dekuStick = 1;

    max = kMaxSticks[gOotSave.upgrades.dekuStick];
    addAmmo(ITEM_OOT_STICK, max, count);
}

static void addNuts(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.dekuNut == 0)
        gOotSave.upgrades.dekuNut = 1;

    max = kMaxNuts[gOotSave.upgrades.dekuStick];
    addAmmo(ITEM_OOT_NUT, max, count);
}

static void addBombs(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.bombBag == 0)
        return;
    max = kMaxBombs[gOotSave.upgrades.bombBag];
    addAmmo(ITEM_OOT_BOMB, max, count);
}

static void addArrows(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.quiver == 0)
        return;
    max = kMaxArrows[gOotSave.upgrades.quiver];
    addAmmo(ITEM_OOT_BOW, max, count);
}

static void addSeeds(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.bulletBag == 0)
        return;
    max = kMaxSeeds[gOotSave.upgrades.dekuStick];
    addAmmo(ITEM_OOT_SLINGSHOT, max, count);
}

static void addBombchus(u8 count)
{
    addAmmo(ITEM_OOT_BOMBCHU_10, 50, count);
}

static void addNewBottle(u16 itemId)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.inventory[ITS_OOT_BOTTLE + i] == ITEM_NONE)
        {
            gOotSave.inventory[ITS_OOT_BOTTLE + i] = itemId;
            return;
        }
    }
}

static void addBombBag(u8 level)
{
    gOotSave.inventory[ITS_OOT_BOMBS] = ITEM_OOT_BOMB;
    gOotSave.upgrades.bombBag = level;
    gOotSave.ammo[ITS_OOT_BOMBS] = kMaxBombs[level];
}

static void addQuiver(u8 level)
{
    gOotSave.inventory[ITS_OOT_BOW] = ITEM_OOT_BOW;
    gOotSave.upgrades.quiver = level;
    gOotSave.ammo[ITS_OOT_BOW] = kMaxArrows[level];
}

static void addBulletBag(u8 level)
{
    gOotSave.inventory[ITS_OOT_SLINGSHOT] = ITEM_OOT_SLINGSHOT;
    gOotSave.upgrades.bulletBag = level;
    gOotSave.ammo[ITS_OOT_SLINGSHOT] = kMaxSeeds[level];
}

static void addInventory(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_STICK:
        addSticks(1);
        break;
    case ITEM_OOT_STICKS_5:
        addSticks(5);
        break;
    case ITEM_OOT_STICKS_10:
        addSticks(10);
        break;
    case ITEM_OOT_NUT:
        addNuts(1);
        break;
    case ITEM_OOT_NUTS_5:
        addNuts(5);
        break;
    case ITEM_OOT_NUTS_10:
        addNuts(10);
        break;
    case ITEM_OOT_BOMB:
        addBombs(1);
        break;
    case ITEM_OOT_BOMBS_5:
        addBombs(5);
        break;
    case ITEM_OOT_BOMBS_10:
        addBombs(10);
        break;
    case ITEM_OOT_BOMBS_20:
        addBombs(20);
        break;
    case ITEM_OOT_BOMBS_30:
        addBombs(30);
        break;
    case ITEM_OOT_BOW:
        addQuiver(1);
        break;
    case ITEM_OOT_ARROWS_5:
        addArrows(5);
        break;
    case ITEM_OOT_ARROWS_10:
        addArrows(10);
        break;
    case ITEM_OOT_ARROWS_30:
        addArrows(30);
        break;
    case ITEM_OOT_ARROW_FIRE:
        gOotSave.inventory[ITS_OOT_ARROW_FIRE] = ITEM_OOT_ARROW_FIRE;
        break;
    case ITEM_OOT_SPELL_FIRE:
        gOotSave.inventory[ITS_OOT_SPELL_FIRE] = ITEM_OOT_SPELL_FIRE;
        break;
    case ITEM_OOT_SLINGSHOT:
    case ITEM_OOT_BULLET_BAG:
        addBulletBag(1);
        break;
    case ITEM_OOT_SEEDS_5:
        addSeeds(5);
        break;
    case ITEM_OOT_SEEDS_30:
        addSeeds(30);
        break;
    case ITEM_OOT_OCARINA_FAIRY:
        if (gOotSave.inventory[ITS_OOT_OCARINA] == ITEM_NONE)
            gOotSave.inventory[ITS_OOT_OCARINA] = ITEM_OOT_OCARINA_FAIRY;
        gOotExtraItems.ocarina |= 0x1;
        break;
    case ITEM_OOT_OCARINA_TIME:
        gOotSave.inventory[ITS_OOT_OCARINA] = ITEM_OOT_OCARINA_TIME;
        gOotExtraItems.ocarina |= 0x2;
        break;
    case ITEM_OOT_BOMBCHU_5:
        addBombchus(5);
        break;
    case ITEM_OOT_BOMBCHU_10:
        addBombchus(10);
        break;
    case ITEM_OOT_BOMBCHU_20:
        addBombchus(20);
        break;
    case ITEM_OOT_HOOKSHOT:
        if (gOotSave.inventory[ITS_OOT_HOOKSHOT] == ITEM_NONE)
            gOotSave.inventory[ITS_OOT_HOOKSHOT] = ITEM_OOT_HOOKSHOT;
        gOotExtraItems.hookshot |= 0x1;
        break;
    case ITEM_OOT_LONGSHOT:
        gOotSave.inventory[ITS_OOT_HOOKSHOT] = ITEM_OOT_LONGSHOT;
        gOotExtraItems.hookshot |= 0x2;
        break;
    case ITEM_OOT_ARROW_ICE:
        gOotSave.inventory[ITS_OOT_ARROW_ICE] = ITEM_OOT_ARROW_ICE;
        break;
    case ITEM_OOT_SPELL_WIND:
        gOotSave.inventory[ITS_OOT_SPELL_WIND] = ITEM_OOT_SPELL_WIND;
        break;
    case ITEM_OOT_BOOMERANG:
        gOotSave.inventory[ITS_OOT_BOOMERANG] = ITEM_OOT_BOOMERANG;
        break;
    case ITEM_OOT_LENS:
        gOotSave.inventory[ITS_OOT_LENS] = ITEM_OOT_LENS;
        break;
    case ITEM_OOT_MAGIC_BEAN:
        gOotSave.inventory[ITS_OOT_MAGIC_BEAN] = ITEM_OOT_MAGIC_BEAN;
        gOotSave.ammo[ITS_OOT_MAGIC_BEAN]++;
        break;
    case ITEM_OOT_HAMMER:
        gOotSave.inventory[ITS_OOT_HAMMER] = ITEM_OOT_HAMMER;
        break;
    case ITEM_OOT_ARROW_LIGHT:
        gOotSave.inventory[ITS_OOT_ARROW_LIGHT] = ITEM_OOT_ARROW_LIGHT;
        break;
    case ITEM_OOT_SPELL_LOVE:
        gOotSave.inventory[ITS_OOT_SPELL_LOVE] = ITEM_OOT_SPELL_LOVE;
        break;
    case ITEM_OOT_EMPTY_BOTTLE:
    case ITEM_OOT_RUTO_LETTER:
    case ITEM_OOT_MILK_BOTTLE:
        addNewBottle(itemId);
        break;
    }
}

static void addChildTrade(u16 itemId)
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
    gOotExtraTrade.child |= (1 << bit);
}

static void addEquipment(u16 itemId)
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
        gOotExtraItems.shield |= EQ_OOT_SHIELD_DEKU;
        break;
    case ITEM_OOT_HYLIAN_SHIELD:
        gOotSave.equipment.shields |= EQ_OOT_SHIELD_HYLIAN;
        gOotExtraItems.shield |= EQ_OOT_SHIELD_HYLIAN;
        break;
    case ITEM_OOT_MIRROR_SHIELD:
        gOotSave.equipment.shields |= EQ_OOT_SHIELD_MIRROR;
        gOotExtraItems.shield |= EQ_OOT_SHIELD_MIRROR;
        break;
    }
}

static void addUpgrade(u16 itemId)
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
        addBulletBag(2);
        break;
    case ITEM_OOT_BULLET_BAG3:
        addBulletBag(3);
        break;
    case ITEM_OOT_QUIVER2:
        addQuiver(2);
        break;
    case ITEM_OOT_QUIVER3:
        addQuiver(3);
        break;
    case ITEM_OOT_BOMB_BAG:
        addBombBag(1);
        break;
    case ITEM_OOT_BOMB_BAG2:
        addBombBag(2);
        break;
    case ITEM_OOT_BOMB_BAG3:
        addBombBag(3);
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

static void addQuest(u16 itemId)
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

static void addRupee(u16 itemId)
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
    (void)max;
    (void)kMaxRupees;
    gSaveContext.rupeesDelta += count;
#else
    max = kMaxRupees[gOotSave.upgrades.wallet];
    gOotSave.rupees += count;
    if (gOotSave.rupees > max)
        gOotSave.rupees = max;
#endif
}

void comboAddItemOot(u16 itemId)
{
    addInventory(itemId);
    addChildTrade(itemId);
    addEquipment(itemId);
    addUpgrade(itemId);
    addQuest(itemId);
    addRupee(itemId);
}
