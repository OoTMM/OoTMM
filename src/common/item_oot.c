#include <combo.h>

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

void comboAddSmallKeyOot(u16 dungeonId)
{
    s8 keyCount;

    keyCount = gOotSave.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gOotSave.dungeonKeys[dungeonId] = keyCount;
}

void comboAddBossKeyOot(u16 dungeonId)
{
    gOotSave.dungeonItems[dungeonId].bossKey = 1;
}

void comboAddCompassOot(u16 dungeonId)
{
    gOotSave.dungeonItems[dungeonId].compass = 1;
}

void comboAddMapOot(u16 dungeonId)
{
    gOotSave.dungeonItems[dungeonId].map = 1;
}

static void addHealth(u8 count)
{
    u16 health;

    health = (u16)count * 0x10;
    gOotSave.health += health;
    if (gOotSave.health > gOotSave.healthMax)
        gOotSave.health = gOotSave.healthMax;
}

static void addMagicUpgrade(int level)
{
    gOotSave.magicUpgrade = 1;
    if (level >= 2)
        gOotSave.magicUpgrade2 = 1;
}

static void refillMagic(int level)
{
    gOotSave.magicSize = level;
    gOotSave.magicAmount = level * 0x30;
}


static void addAmmo(u8 slot, u16 item, u8 max, u8 count)
{
    gOotSave.inventory[slot] = item;
    gOotSave.ammo[slot] += count;
    if (gOotSave.ammo[slot] > max)
        gOotSave.ammo[slot] = max;
}

static void addSticks(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.dekuStick == 0)
        gOotSave.upgrades.dekuStick = 1;

    max = kMaxSticks[gOotSave.upgrades.dekuStick];
    addAmmo(ITS_OOT_STICKS, ITEM_OOT_STICK, max, count);
}

static void addNuts(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.dekuNut == 0)
        gOotSave.upgrades.dekuNut = 1;

    max = kMaxNuts[gOotSave.upgrades.dekuNut];
    addAmmo(ITS_OOT_NUTS, ITEM_OOT_NUT, max, count);
}

static void addBombs(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.bombBag == 0)
        return;
    max = kMaxBombs[gOotSave.upgrades.bombBag];
    addAmmo(ITS_OOT_BOMBS, ITEM_OOT_BOMB, max, count);
}

static void addArrows(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.quiver == 0)
        return;
    max = kMaxArrows[gOotSave.upgrades.quiver];
    addAmmo(ITS_OOT_BOW, ITEM_OOT_BOW, max, count);
}

static void addSeeds(u8 count)
{
    u8 max;

    if (gOotSave.upgrades.bulletBag == 0)
        return;
    max = kMaxSeeds[gOotSave.upgrades.bulletBag];
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
        if (gOotSave.inventory[ITS_OOT_BOTTLE + i] == ITEM_NONE)
        {
            gOotSave.inventory[ITS_OOT_BOTTLE + i] = itemId;
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
        if (gOotSave.inventory[ITS_OOT_BOTTLE + i] == ITEM_OOT_EMPTY_BOTTLE)
        {
            slot = i;
            break;
        }
    }
    if (slot == -1)
        return;
    gOotSave.inventory[ITS_OOT_BOTTLE + slot] = itemId;
    for (int i = 0; i < 3; ++i)
    {
        if (gOotSave.buttons[4 + i] == ITS_OOT_BOTTLE + slot)
        {
            gOotSave.buttons[1 + i] = itemId;
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

static void addNutUpgrade(u8 level)
{
    gOotSave.inventory[ITS_OOT_NUTS] = ITEM_OOT_NUT;
    gOotSave.upgrades.dekuNut = level;
    gOotSave.ammo[ITS_OOT_NUTS] = kMaxNuts[level];
}

static void addStickUpgrade(u8 level)
{
    gOotSave.inventory[ITS_OOT_STICKS] = ITEM_OOT_STICK;
    gOotSave.upgrades.dekuStick = level;
    gOotSave.ammo[ITS_OOT_STICKS] = kMaxSticks[level];
}

static void addTradeChild(u8 index)
{
    u16 itemId;

    itemId = kOotTradeChild[index];
    if (gOotSave.inventory[ITS_OOT_TRADE_CHILD] == ITEM_NONE)
        gOotSave.inventory[ITS_OOT_TRADE_CHILD] = itemId;
    gOotExtraTrade.child |= (1 << (u16)index);
}

static void addTradeAdult(u8 index)
{
    u16 itemId;

    itemId = kOotTradeAdult[index];
    if (gOotSave.inventory[ITS_OOT_TRADE_ADULT] == ITEM_NONE)
        gOotSave.inventory[ITS_OOT_TRADE_ADULT] = itemId;
    gOotExtraTrade.adult |= (1 << (u16)index);
}

static void addRupees(u16 count)
{
    u16 max;

    max = kMaxRupees[gOotSave.upgrades.wallet];
    gOotSave.rupees += count;
    if (gOotSave.rupees > max)
        gOotSave.rupees = max;
}

static void addHookshot(int level)
{
    u16 itemId;

    if (level >= 2)
        itemId = ITEM_OOT_LONGSHOT;
    else
        itemId = ITEM_OOT_HOOKSHOT;
    gOotSave.inventory[ITS_OOT_HOOKSHOT] = itemId;
    gOotExtraItems.hookshot |= (1 << (level - 1));

    /* Reload the items */
    for (int i = 0; i < 3; ++i)
    {
        if (gOotSave.buttons[4 + i] == ITS_OOT_HOOKSHOT)
        {
            gOotSave.buttons[1 + i] = itemId;
        }
    }
}

static void addOcarina(int level)
{
    u16 itemId;

    if (level >= 2)
        itemId = ITEM_OOT_OCARINA_TIME;
    else
        itemId = ITEM_OOT_OCARINA_FAIRY;
    gOotSave.inventory[ITS_OOT_OCARINA] = itemId;
    gOotExtraItems.ocarina |= (1 << (level - 1));

    /* Reload the items */
    for (int i = 0; i < 3; ++i)
    {
        if (gOotSave.buttons[4 + i] == ITS_OOT_OCARINA)
        {
            gOotSave.buttons[1 + i] = itemId;
        }
    }
}

void comboAddItemOot(s16 gi, int noEffect)
{
    u16 dungeonId;

    (void)dungeonId;
    switch (gi)
    {
    case GI_OOT_STICK:
        addSticks(1);
        break;
    case GI_OOT_STICKS_5:
        addSticks(5);
        break;
    case GI_OOT_STICKS_10:
        addSticks(10);
        break;
    case GI_OOT_NUTS_5:
    case GI_OOT_NUTS_5_ALT:
        addNuts(5);
        break;
    case GI_OOT_NUTS_10:
        addNuts(10);
        break;
    case GI_OOT_BOMB:
        addBombs(1);
        break;
    case GI_OOT_BOMBS_5:
        addBombs(5);
        break;
    case GI_OOT_BOMBS_10:
        addBombs(10);
        break;
    case GI_OOT_BOMBS_20:
        addBombs(20);
        break;
    case GI_OOT_BOMBS_30:
        addBombs(30);
        break;
    case GI_OOT_BOW:
        addQuiver(1);
        break;
    case GI_OOT_ARROWS_5:
        addArrows(5);
        break;
    case GI_OOT_ARROWS_10:
        addArrows(10);
        break;
    case GI_OOT_ARROWS_30:
        addArrows(30);
        break;
    case GI_OOT_ARROW_FIRE:
        gOotSave.inventory[ITS_OOT_ARROW_FIRE] = ITEM_OOT_ARROW_FIRE;
        break;
    case GI_OOT_SPELL_FIRE:
        gOotSave.inventory[ITS_OOT_SPELL_FIRE] = ITEM_OOT_SPELL_FIRE;
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
        addOcarina(2);
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
        addHookshot(1);
        break;
    case GI_OOT_LONGSHOT:
        addHookshot(2);
        break;
    case GI_OOT_ARROW_ICE:
        gOotSave.inventory[ITS_OOT_ARROW_ICE] = ITEM_OOT_ARROW_ICE;
        break;
    case GI_OOT_SPELL_WIND:
        gOotSave.inventory[ITS_OOT_SPELL_WIND] = ITEM_OOT_SPELL_WIND;
        break;
    case GI_OOT_BOOMERANG:
        gOotSave.inventory[ITS_OOT_BOOMERANG] = ITEM_OOT_BOOMERANG;
        break;
    case GI_OOT_LENS:
        gOotSave.inventory[ITS_OOT_LENS] = ITEM_OOT_LENS;
        break;
    case GI_OOT_MAGIC_BEAN:
        gOotSave.inventory[ITS_OOT_MAGIC_BEAN] = ITEM_OOT_MAGIC_BEAN;
        gOotSave.ammo[ITS_OOT_MAGIC_BEAN] = 10;
        break;
    case GI_OOT_HAMMER:
        gOotSave.inventory[ITS_OOT_HAMMER] = ITEM_OOT_HAMMER;
        break;
    case GI_OOT_ARROW_LIGHT:
        gOotSave.inventory[ITS_OOT_ARROW_LIGHT] = ITEM_OOT_ARROW_LIGHT;
        break;
    case GI_OOT_SPELL_LOVE:
        gOotSave.inventory[ITS_OOT_SPELL_LOVE] = ITEM_OOT_SPELL_LOVE;
        break;
    case GI_OOT_EMPTY_BOTTLE:
        addNewBottle(ITEM_OOT_EMPTY_BOTTLE);
        break;
    case GI_OOT_RUTO_LETTER:
        addNewBottle(ITEM_OOT_RUTO_LETTER);
        break;
    case GI_OOT_MILK_BOTTLE:
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
    case GI_OOT_LON_LON_MILK:
        fillBottle(ITEM_OOT_MILK);
        break;
    case GI_OOT_SWORD_KOKIRI:
        gOotSave.equipment.swords |= EQ_OOT_SWORD_KOKIRI;
        break;
    case GI_OOT_SWORD_MASTER:
        gOotSave.equipment.swords |= EQ_OOT_SWORD_MASTER;
        break;
    case GI_OOT_SWORD_KNIFE:
        gOotSave.equipment.swords |= EQ_OOT_SWORD_KNIFE;
        gOotSave.swordHealth = 8;
        break;
    case GI_OOT_SWORD_BIGGORON:
        gOotSave.equipment.swords |= EQ_OOT_SWORD_KNIFE;
        gOotSave.equipment.swords &= ~(EQ_OOT_SWORD_KNIFE_BROKEN);
        gOotSave.isBiggoronSword = 1;
        break;
    case GI_OOT_PROGRESSIVE_SHIELD_DEKU:
        gOotExtraItems.shield |= EQ_OOT_SHIELD_DEKU;
        /* Fallthrough */
    case GI_OOT_SHIELD_DEKU:
        gOotSave.equipment.shields |= EQ_OOT_SHIELD_DEKU;
        break;
    case GI_OOT_PROGRESSIVE_SHIELD_HYLIAN:
        gOotExtraItems.shield |= EQ_OOT_SHIELD_HYLIAN;
        /* Fallthrough */
    case GI_OOT_SHIELD_HYLIAN:
        gOotSave.equipment.shields |= EQ_OOT_SHIELD_HYLIAN;
        break;
    case GI_OOT_SHIELD_MIRROR:
        gOotSave.equipment.shields |= EQ_OOT_SHIELD_MIRROR;
        gOotExtraItems.shield |= EQ_OOT_SHIELD_MIRROR;
        break;
    case GI_OOT_TUNIC_GORON:
        gOotSave.equipment.tunics |= EQ_OOT_TUNIC_GORON;
        break;
    case GI_OOT_TUNIC_ZORA:
        gOotSave.equipment.tunics |= EQ_OOT_TUNIC_ZORA;
        break;
    case GI_OOT_BOOTS_IRON:
        gOotSave.equipment.boots |= EQ_OOT_BOOTS_IRON;
        break;
    case GI_OOT_BOOTS_HOVER:
        gOotSave.equipment.boots |= EQ_OOT_BOOTS_HOVER;
        break;
    case GI_OOT_GORON_BRACELET:
        gOotSave.upgrades.strength = 1;
        break;
    case GI_OOT_SILVER_GAUNTLETS:
        gOotSave.upgrades.strength = 2;
        break;
    case GI_OOT_GOLDEN_GAUNTLETS:
        gOotSave.upgrades.strength = 3;
        break;
    case GI_OOT_SCALE_SILVER:
        gOotSave.upgrades.dive = 1;
        break;
    case GI_OOT_SCALE_GOLDEN:
        gOotSave.upgrades.dive = 2;
        break;
    case GI_OOT_BULLET_BAG2:
        addBulletBag(2);
        break;
    case GI_OOT_BULLET_BAG3:
        addBulletBag(3);
        break;
    case GI_OOT_QUIVER2:
        addQuiver(2);
        break;
    case GI_OOT_QUIVER3:
        addQuiver(3);
        break;
    case GI_OOT_BOMB_BAG:
        addBombBag(1);
        break;
    case GI_OOT_BOMB_BAG2:
        addBombBag(2);
        break;
    case GI_OOT_BOMB_BAG3:
        addBombBag(3);
        break;
    case GI_OOT_WALLET2:
        gOotSave.upgrades.wallet = 1;
        break;
    case GI_OOT_WALLET3:
        gOotSave.upgrades.wallet = 2;
        break;
    case GI_OOT_MAGIC_UPGRADE:
        addMagicUpgrade(1);
        if (noEffect)
            refillMagic(1);
        break;
    case GI_OOT_MAGIC_UPGRADE2:
        addMagicUpgrade(2);
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
        gOotSave.doubleDefense = 1;
        gOotSave.doubleDefenseHearts = 20;
        if (noEffect)
            addHealth(20);
        break;
    case GI_OOT_HEART_PIECE:
    case GI_OOT_TC_HEART_PIECE:
        gOotSave.quest.heartPieces++;
        if (noEffect)
        {
            if (gOotSave.quest.heartPieces >= 4)
            {
                gOotSave.quest.heartPieces -= 4;
                gOotSave.healthMax += 0x10;
            }
            addHealth(20);
        }
        break;
    case GI_OOT_HEART_CONTAINER:
    case GI_OOT_HEART_CONTAINER2:
        gOotSave.healthMax += 0x10;
        if (noEffect)
            addHealth(20);
        break;
    case GI_OOT_GS_TOKEN:
        gOotSave.quest.goldToken = 1;
        gOotSave.goldTokens++;
        break;
    case GI_OOT_GERUDO_CARD:
        gOotSave.quest.gerudoCard = 1;
        break;
    case GI_OOT_STONE_OF_AGONY:
        gOotSave.quest.agonyStone = 1;
        break;
    case GI_OOT_STONE_SAPPHIRE:
        gOotSave.quest.stoneSapphire = 1;
        break;
    case GI_OOT_STONE_RUBY:
        gOotSave.quest.stoneRuby = 1;
        break;
    case GI_OOT_STONE_EMERALD:
        gOotSave.quest.stoneEmerald = 1;
        break;
    case GI_OOT_SONG_STORMS:
        gOotSave.quest.songStorms = 1;
        break;
    case GI_OOT_SONG_TIME:
        gOotSave.quest.songTime = 1;
        break;
    case GI_OOT_SONG_SUN:
        gOotSave.quest.songSun = 1;
        break;
    case GI_OOT_SONG_SARIA:
        gOotSave.quest.songSaria = 1;
        break;
    case GI_OOT_SONG_EPONA:
        gOotSave.quest.songEpona = 1;
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_EPONA);
        break;
    case GI_OOT_SONG_ZELDA:
        gOotSave.quest.songZelda = 1;
        break;
    case GI_OOT_SONG_TP_LIGHT:
        gOotSave.quest.songTpLight = 1;
        break;
    case GI_OOT_SONG_TP_SHADOW:
        gOotSave.quest.songTpShadow = 1;
        break;
    case GI_OOT_SONG_TP_SPIRIT:
        gOotSave.quest.songTpSpirit = 1;
        break;
    case GI_OOT_SONG_TP_WATER:
        gOotSave.quest.songTpWater = 1;
        break;
    case GI_OOT_SONG_TP_FIRE:
        gOotSave.quest.songTpFire = 1;
        break;
    case GI_OOT_SONG_TP_FOREST:
        gOotSave.quest.songTpForest = 1;
        break;
    case GI_OOT_MEDALLION_LIGHT:
        gOotSave.quest.medallionLight = 1;
        break;
    case GI_OOT_MEDALLION_SHADOW:
        gOotSave.quest.medallionShadow = 1;
        break;
    case GI_OOT_MEDALLION_SPIRIT:
        gOotSave.quest.medallionSpirit = 1;
        break;
    case GI_OOT_MEDALLION_WATER:
        gOotSave.quest.medallionWater = 1;
        break;
    case GI_OOT_MEDALLION_FIRE:
        gOotSave.quest.medallionFire = 1;
        break;
    case GI_OOT_MEDALLION_FOREST:
        gOotSave.quest.medallionForest = 1;
        break;
    case GI_OOT_RUPEE_GREEN:
    case GI_OOT_TC_RUPEE_GREEN:
        if (noEffect)
            addRupees(1);
        break;
    case GI_OOT_RUPEE_BLUE:
    case GI_OOT_TC_RUPEE_BLUE:
        if (noEffect)
            addRupees(5);
        break;
    case GI_OOT_RUPEE_RED:
    case GI_OOT_TC_RUPEE_RED:
        if (noEffect)
            addRupees(20);
        break;
    case GI_OOT_RUPEE_PURPLE:
    case GI_OOT_TC_RUPEE_PURPLE:
        if (noEffect)
            addRupees(50);
        break;
    case GI_OOT_RUPEE_HUGE:
        if (noEffect)
            addRupees(200);
        break;
    case GI_OOT_RECOVERY_HEART:
        if (noEffect)
            addHealth(1);
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
        addTradeChild(3);
        break;
    case GI_OOT_MASK_SKULL:
        addTradeChild(4);
        break;
    case GI_OOT_MASK_SPOOKY:
        addTradeChild(5);
        break;
    case GI_OOT_MASK_BUNNY:
        addTradeChild(6);
        break;
    case GI_OOT_MASK_GORON:
        addTradeChild(7);
        break;
    case GI_OOT_MASK_ZORA:
        addTradeChild(8);
        break;
    case GI_OOT_MASK_GERUDO:
        addTradeChild(9);
        break;
    case GI_OOT_MASK_TRUTH:
        addTradeChild(10);
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
        comboAddSmallKeyOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_SMALL_KEY_FIRE:
        comboAddSmallKeyOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_SMALL_KEY_WATER:
        comboAddSmallKeyOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_SMALL_KEY_SPIRIT:
        comboAddSmallKeyOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_SMALL_KEY_SHADOW:
        comboAddSmallKeyOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_SMALL_KEY_GANON:
        comboAddSmallKeyOot(SCE_OOT_INSIDE_GANON_CASTLE);
        break;
    case GI_OOT_SMALL_KEY_BOTW:
        comboAddSmallKeyOot(SCE_OOT_BOTTOM_OF_THE_WELL);
        break;
    case GI_OOT_SMALL_KEY_GF:
        comboAddSmallKeyOot(SCE_OOT_THIEVES_HIDEOUT);
        break;
    case GI_OOT_SMALL_KEY_GTG:
        comboAddSmallKeyOot(SCE_OOT_GERUDO_TRAINING_GROUND);
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
    }
}
