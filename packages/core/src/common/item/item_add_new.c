#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define addRupeesRaw  addRupeesRawOot
#else
# define addRupeesRaw  addRupeesRawMm
#endif

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

static const u8 kItemSlotsOot[] = {
    ITS_OOT_STICKS,
    ITS_OOT_NUTS,
    ITS_OOT_BOMBS,
    ITS_OOT_BOW,
    ITS_OOT_ARROW_FIRE,
    ITS_OOT_SPELL_FIRE,
    ITS_OOT_SLINGSHOT,
    ITS_OOT_OCARINA,
    ITS_OOT_OCARINA,
    ITS_OOT_BOMBCHU,
    ITS_OOT_HOOKSHOT,
    ITS_OOT_HOOKSHOT,
    ITS_OOT_ARROW_ICE,
    ITS_OOT_SPELL_WIND,
    ITS_OOT_BOOMERANG,
    ITS_OOT_LENS,
    ITS_OOT_MAGIC_BEAN,
    ITS_OOT_HAMMER,
    ITS_OOT_ARROW_LIGHT,
    ITS_OOT_SPELL_LOVE,
};

static const u8 kItemSlotsMm[] = {
    ITS_MM_OCARINA,
    ITS_MM_BOW,
    ITS_MM_ARROW_FIRE,
    ITS_MM_ARROW_ICE,
    ITS_MM_ARROW_LIGHT,
    ITS_MM_OCARINA,
    ITS_MM_BOMBS,
    ITS_MM_BOMBCHU,
    ITS_MM_STICKS,
    ITS_MM_NUTS,
    ITS_MM_BEANS,
    ITS_MM_KEG,
    ITS_MM_PICTOBOX,
    ITS_MM_LENS,
    ITS_MM_HOOKSHOT,
    ITS_MM_GREAT_FAIRY_SWORD,
};

static const u8 kItemSlotsMasksMm[] = {
    ITS_MM_MASK_DEKU,
    ITS_MM_MASK_GORON,
    ITS_MM_MASK_ZORA,
    ITS_MM_MASK_FIERCE_DEITY,
    ITS_MM_MASK_TRUTH,
    ITS_MM_MASK_KAFEI,
    ITS_MM_MASK_ALL_NIGHT,
    ITS_MM_MASK_BUNNY,
    ITS_MM_MASK_KEATON,
    ITS_MM_MASK_GARO,
    ITS_MM_MASK_ROMANI,
    ITS_MM_MASK_TROUPE_LEADER,
    ITS_MM_MASK_POSTMAN,
    ITS_MM_MASK_COUPLE,
    ITS_MM_MASK_GREAT_FAIRY,
    ITS_MM_MASK_GIBDO,
    ITS_MM_MASK_DON_GERO,
    ITS_MM_MASK_KAMARO,
    ITS_MM_MASK_CAPTAIN,
    ITS_MM_MASK_STONE,
    ITS_MM_MASK_BREMEN,
    ITS_MM_MASK_BLAST,
    ITS_MM_MASK_SCENTS,
    ITS_MM_MASK_GIANT,
};

typedef int (*AddItemFunc)(GameState_Play* play, u8 itemId, s16 gi, u16 param);

static void reloadSlotEquipsOot(OotItemEquips* equips, int slot)
{
    for (int i = 0; i < 3; ++i)
    {
        if (equips->cButtonSlots[i] == slot)
        {
            equips->buttonItems[1 + i] = gOotSave.inventory.items[slot];
        }
    }
}

void reloadSlotRawOot(int slot)
{
    reloadSlotEquipsOot(&gOotSave.equips, slot);
    reloadSlotEquipsOot(&gOotSave.childEquips, slot);
    reloadSlotEquipsOot(&gOotSave.adultEquips, slot);
}

static void reloadSlotEquipsMm(MmItemEquips* equips, int slot)
{
    for (int i = 0; i < 3; ++i)
    {
        if (equips->cButtonSlots[0][1 + i] == slot)
        {
            equips->buttonItems[0][1 + i] = gMmSave.inventory.items[slot];
        }
    }
}

static void reloadSlotRawMm(int slot)
{
    reloadSlotEquipsMm(&gMmSave.itemEquips, slot);
}

void reloadSlotOot(GameState_Play* play, int slot)
{
    reloadSlotRawOot(slot);

#if defined(GAME_OOT)
    if (play)
    {
        for (int i = 1; i < 4; ++i)
            Interface_LoadItemIconImpl(play, i);
    }
#endif
}

void reloadSlotMm(GameState_Play* play, int slot)
{
    reloadSlotRawMm(slot);

#if defined(GAME_MM)
    if (play)
    {
        for (int i = 1; i < 4; ++i)
            Interface_LoadItemIconImpl(play, i);
    }
#endif
}

#if defined(GAME_MM)
static void reloadHookshot(GameState_Play* play)
{
    Actor_Player* link;

    if (play)
    {
        link = GET_LINK(play);
        link->state &= ~(PLAYER_ACTOR_STATE_HOLD_ITEM | PLAYER_ACTOR_STATE_USE_ITEM);
    }
}
#endif

static void addRupeesEffect(s16 delta)
{
#if defined(GAME_OOT)
    gSaveContext.rupeesDelta += delta;
#else
    gSaveContext.save.rupeesDelta += delta;
#endif
}

static void addRupeesRawOot(s16 delta)
{
    u16 max;

    max = gOotMaxRupees[gOotSave.inventory.upgrades.wallet];
    gOotSave.playerData.rupees += delta;
    if (gOotSave.playerData.rupees > max)
        gOotSave.playerData.rupees = max;
}

static void addRupeesRawMm(s16 delta)
{
    u16 max;

    max = gMmMaxRupees[gMmSave.inventory.upgrades.wallet];
    gMmSave.playerData.rupees += delta;
    if (gMmSave.playerData.rupees > max)
        gMmSave.playerData.rupees = max;
}

static void addRupeesOot(GameState_Play* play, s16 delta)
{
#if defined(GAME_MM)
    if (!comboConfig(CFG_SHARED_WALLETS))
        play = NULL;
#endif

    if (play)
    {
        addRupeesEffect(delta);
        return;
    }

    if (comboConfig(CFG_SHARED_WALLETS))
        addRupeesRaw(delta);
    else
        addRupeesRawOot(delta);
}

static void addRupeesMm(GameState_Play* play, s16 delta)
{
#if defined(GAME_OOT)
    if (!comboConfig(CFG_SHARED_WALLETS))
        play = NULL;
#endif

    if (play)
    {
        addRupeesEffect(delta);
        return;
    }

    if (comboConfig(CFG_SHARED_WALLETS))
        addRupeesRaw(delta);
    else
        addRupeesRawMm(delta);
}

static int addItemRupeesOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addRupeesOot(play, param);
    return 0;
}

static int addItemRupeesMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addRupeesMm(play, param);
    return 0;

}

static void addWalletRawOot(u16 index)
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
}

static void addWalletRawMm(u16 index)
{
    if (index == 0)
    {
        gMmExtraFlags2.childWallet = 1;
        gMmMaxRupees[0] = 99;
    }
    else if (index == 4)
    {
        gMmExtraFlags3.bottomlessWallet = 1;
        gMmMaxRupees[3] = 9999;
#if defined(GAME_MM)
        gWalletDigits[3] = 4;
#endif
    }
    else
        gMmSave.inventory.upgrades.wallet = index;
}

static void addWalletRawShared(u16 index)
{
    addWalletRawOot(index);
    addWalletRawMm(index);
}

static int addItemWalletOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (comboConfig(CFG_SHARED_WALLETS))
        addWalletRawShared(param);
    else
        addWalletRawOot(param);

    if (comboConfig(CFG_FILL_WALLETS))
        addRupeesOot(play, gOotMaxRupees[gOotSave.inventory.upgrades.wallet]);

    return 0;
}

static int addItemWalletMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (comboConfig(CFG_SHARED_WALLETS))
        addWalletRawShared(param);
    else
        addWalletRawMm(param);

    if (comboConfig(CFG_FILL_WALLETS))
        addRupeesMm(play, gMmMaxRupees[gMmSave.inventory.upgrades.wallet]);

    return 0;
}

static void addAmmoOot(u8 slot, u16 item, u8 max, u8 count)
{
    gOotSave.inventory.items[slot] = item;
    gOotSave.inventory.ammo[slot] += count;
    if (gOotSave.inventory.ammo[slot] > max)
        gOotSave.inventory.ammo[slot] = max;
}

static void addAmmoMm(u8 slot, u16 item, u8 max, u8 count)
{
    gMmSave.inventory.items[slot] = item;
    gMmSave.inventory.ammo[slot] += count;
    if (gMmSave.inventory.ammo[slot] > max)
        gMmSave.inventory.ammo[slot] = max;
}

static void addBombsRawOot(u8 count)
{
    u8 max;

    if (gOotSave.inventory.upgrades.bombBag == 0)
        return;
    max = kMaxBombs[gOotSave.inventory.upgrades.bombBag];
    addAmmoOot(ITS_OOT_BOMBS, ITEM_OOT_BOMB, max, count);
}

static void addBombsRawMm(u8 count)
{
    u8 max;

    if (gMmSave.inventory.upgrades.bombBag == 0)
        return;
    max = kMaxBombs[gMmSave.inventory.upgrades.bombBag];
    addAmmoMm(ITS_MM_BOMBS, ITEM_MM_BOMB, max, count);
}

static void addBombsOot(u8 count)
{
    addBombsRawOot(count);
    if (comboConfig(CFG_SHARED_BOMB_BAGS))
        addBombsRawMm(count);
}

static void addBombsMm(u8 count)
{
    addBombsRawMm(count);
    if (comboConfig(CFG_SHARED_BOMB_BAGS))
        addBombsRawOot(count);
}

static int addItemBombsOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBombsOot(param);
    return 0;
}

static int addItemBombsMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBombsMm(param);
    return 0;
}

void addNutsRawOot(int count)
{
    u8 max;

    if (gOotSave.inventory.upgrades.dekuNut == 0)
        gOotSave.inventory.upgrades.dekuNut = 1;

    max = kMaxNuts[gOotSave.inventory.upgrades.dekuNut];
    addAmmoOot(ITS_OOT_NUTS, ITEM_OOT_NUT, max, count);
}

void addNutsRawMm(int count)
{
    u8 max;

    if (gMmSave.inventory.upgrades.dekuNut == 0)
        gMmSave.inventory.upgrades.dekuNut = 1;

    max = kMaxNuts[gMmSave.inventory.upgrades.dekuNut];
    addAmmoMm(ITS_MM_NUTS, ITEM_MM_NUT, max, count);
}

void addNutsOot(int count)
{
    addNutsRawOot(count);
    if (comboConfig(CFG_SHARED_NUTS_STICKS))
        addNutsRawMm(count);
}

void addNutsMm(int count)
{
    addNutsRawMm(count);
    if (comboConfig(CFG_SHARED_NUTS_STICKS))
        addNutsRawOot(count);
}

static int addItemNutsOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addNutsOot(param);
    return 0;
}

static int addItemNutsMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addNutsMm(param);
    return 0;
}

static int addItemNutsUpgrade(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (gOotSave.inventory.upgrades.dekuNut < param)
        gOotSave.inventory.upgrades.dekuNut = param;
    if (comboConfig(CFG_SHARED_NUTS_STICKS))
        gMmSave.inventory.upgrades.dekuNut = gOotSave.inventory.upgrades.dekuNut;
    addNutsOot(kMaxNuts[param]);
    return 0;
}

static int addItemBombchuOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addAmmoOot(ITS_OOT_BOMBCHU, ITEM_OOT_BOMBCHU_10, 50, param);
    return 0;
}

static int addItemBombchuMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (gMmSave.inventory.upgrades.bombBag)
        addAmmoMm(ITS_MM_BOMBCHU, ITEM_MM_BOMBCHU, 40, param);
    return 0;
}

static void addArrowsRawOot(u8 count)
{
    u8 max;

    if (gOotSave.inventory.upgrades.quiver == 0)
        return;
    max = kMaxArrows[gOotSave.inventory.upgrades.quiver];
    addAmmoOot(ITS_OOT_BOW, ITEM_OOT_BOW, max, count);
}

static void addArrowsRawMm(u8 count)
{
    u8 max;

    if (gMmSave.inventory.upgrades.quiver == 0)
        return;
    max = kMaxArrows[gMmSave.inventory.upgrades.quiver];
    addAmmoMm(ITS_MM_BOW, ITEM_MM_BOW, max, count);
}

static void addArrowsOot(u8 count)
{
    addArrowsRawOot(count);
    if (comboConfig(CFG_SHARED_BOWS))
        addArrowsRawMm(count);
}

static void addArrowsMm(u8 count)
{
    addBombsRawMm(count);
    if (comboConfig(CFG_SHARED_BOWS))
        addBombsRawOot(count);
}

static int addItemArrowsOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addArrowsOot(param);
    return 0;
}

static int addItemArrowsMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addArrowsMm(param);
    return 0;
}

static void addBowRawOot(u8 index)
{
    if (index > gOotSave.inventory.upgrades.quiver)
        gOotSave.inventory.upgrades.quiver = index;
    addArrowsRawOot(kMaxArrows[index]);
}

static void addBowRawMm(u8 index)
{
    if (index > gMmSave.inventory.upgrades.quiver)
        gMmSave.inventory.upgrades.quiver = index;
    addArrowsRawMm(kMaxArrows[index]);
}

static int addItemBowOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBowRawOot(param);
    if (comboConfig(CFG_SHARED_BOWS))
        addBowRawMm(param);
    return 0;
}

static int addItemBowMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBowRawMm(param);
    if (comboConfig(CFG_SHARED_BOWS))
        addBowRawOot(param);
    return 0;
}

static void addSeeds(u8 count)
{
    u8 max;

    if (gOotSave.inventory.upgrades.bulletBag == 0)
        return;
    max = kMaxSeeds[gOotSave.inventory.upgrades.bulletBag];
    addAmmoOot(ITS_OOT_SLINGSHOT, ITEM_OOT_SLINGSHOT, max, count);
}

static int addItemSeeds(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addSeeds(param);
    return 0;
}

static int addItemSlingshot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (gOotSave.inventory.upgrades.bulletBag < param)
        gOotSave.inventory.upgrades.bulletBag = param;
    addSeeds(kMaxSeeds[param]);
    return 0;
}

static int addItemNormalOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.inventory.items[kItemSlotsOot[itemId]] = itemId;
    return 0;
}

static int addItemNormalMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    const u8* slots;

    if (itemId >= ITEM_MM_MASK_DEKU)
        slots = kItemSlotsMasksMm;
    else
        slots = kItemSlotsMm;
    gMmSave.inventory.items[slots[itemId]] = itemId;
    return 0;
}

void addSticksRawOot(int count)
{
    u8 max;

    if (gOotSave.inventory.upgrades.dekuStick == 0)
        gOotSave.inventory.upgrades.dekuStick = 1;

    max = kMaxSticks[gOotSave.inventory.upgrades.dekuStick];
    addAmmoOot(ITS_OOT_STICKS, ITEM_OOT_STICK, max, count);
}

void addSticksRawMm(int count)
{
    u8 max;

    if (gMmSave.inventory.upgrades.dekuStick == 0)
        gMmSave.inventory.upgrades.dekuStick = 1;

    max = kMaxSticks[gMmSave.inventory.upgrades.dekuStick];
    addAmmoMm(ITS_MM_STICKS, ITEM_MM_STICK, max, count);
}

void addSticksOot(int count)
{
    addSticksRawOot(count);
    if (comboConfig(CFG_SHARED_NUTS_STICKS))
        addSticksRawMm(count);
}

void addSticksMm(int count)
{
    addSticksRawMm(count);
    if (comboConfig(CFG_SHARED_NUTS_STICKS))
        addSticksRawOot(count);
}

static int addItemSticksOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addSticksOot(param);
    return 0;
}

static int addItemSticksMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addSticksMm(param);
    return 0;
}

static int addItemSticksUpgrade(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (gOotSave.inventory.upgrades.dekuStick < param)
        gOotSave.inventory.upgrades.dekuStick = param;
    if (comboConfig(CFG_SHARED_NUTS_STICKS))
        gMmSave.inventory.upgrades.dekuStick = gOotSave.inventory.upgrades.dekuStick;
    addSticksOot(kMaxNuts[param]);
    return 0;
}

static void addHookshotRawOot(GameState_Play* play, int level)
{
    u16 itemId;

    if (level >= 2)
        itemId = ITEM_OOT_LONGSHOT;
    else
        itemId = ITEM_OOT_HOOKSHOT;
    gOotSave.inventory.items[ITS_OOT_HOOKSHOT] = itemId;
    gOotExtraItems.hookshot |= (1 << (level - 1));
    reloadSlotOot(play, ITS_OOT_HOOKSHOT);
}

static void addHookshotRawMm(GameState_Play* play, int level)
{
    u8 itemId;

    if (level >= 2)
        itemId = ITEM_MM_HOOKSHOT;
    else
        itemId = 0x11; // ITEM_MM_BOTTLE_POTION_RED but that enum is wrong
    gMmSave.inventory.items[ITS_MM_HOOKSHOT] = itemId;
    gMmExtraItems.hookshot |= (1 << (level - 1));
    reloadSlotMm(play, ITS_MM_HOOKSHOT);
#if defined(GAME_MM)
    if (comboConfig(CFG_MM_HOOKSHOT_SHORT) && level >= 2)
        reloadHookshot(play);
#endif
}

static void addHookshotOot(GameState_Play* play, int level)
{
    addHookshotRawOot(play, level);
    if (comboConfig(CFG_SHARED_HOOKSHOT))
    {
        if (comboConfig(CFG_MM_HOOKSHOT_SHORT))
            addHookshotRawMm(play, level);
        else
            addHookshotRawMm(play, 2);
    }
}

static void addHookshotMm(GameState_Play* play, int level)
{
    addHookshotRawMm(play, level);
    if (comboConfig(CFG_SHARED_HOOKSHOT))
    {
        if (comboConfig(CFG_MM_HOOKSHOT_SHORT))
            addHookshotRawOot(play, level);
        else
            addHookshotRawOot(play, 1);
    }
}

static int addItemHookshotOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addHookshotOot(play, param);
    return 0;
}

static int addItemHookshotMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addHookshotMm(play, param);
    return 0;
}

static int addItemTradeOotChild(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    u16 mask;

    itemId = kOotTradeChild[param];
    mask = 1 << (u16)param;
    if (gOotExtraTradeSave.child & mask)
        return 0;
    if (gOotSave.inventory.items[ITS_OOT_TRADE_CHILD] == ITEM_NONE)
        gOotSave.inventory.items[ITS_OOT_TRADE_CHILD] = itemId;
    gOotExtraTrade.child |= mask;
    gOotExtraTradeSave.child |= mask;
    return 0;
}

static int addItemTradeOotAdult(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    u16 mask;

    itemId = kOotTradeAdult[param];
    mask = 1 << (u16)param;
    if (gOotExtraTradeSave.adult & mask)
        return 0;
    if (gOotSave.inventory.items[ITS_OOT_TRADE_ADULT] == ITEM_NONE)
        gOotSave.inventory.items[ITS_OOT_TRADE_ADULT] = itemId;
    gOotExtraTrade.adult |= mask;
    gOotExtraTradeSave.adult |= mask;
    return 0;
}


static int addItemTradeMm1(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    itemId = kMmTrade1[param];
    if (gMmSave.inventory.items[ITS_MM_TRADE1] == ITEM_NONE)
        gMmSave.inventory.items[ITS_MM_TRADE1] = itemId;
    gMmExtraTrade.trade1 |= (1 << (u16)param);
    gMmExtraTrade.tradeObtained1 |= (1 << (u16)param);
    return 0;
}

static int addItemTradeMm2(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    itemId = kMmTrade2[param];
    if (gMmSave.inventory.items[ITS_MM_TRADE2] == ITEM_NONE)
        gMmSave.inventory.items[ITS_MM_TRADE2] = itemId;
    gMmExtraTrade.trade2 |= (1 << (u16)param);
    gMmExtraTrade.tradeObtained2 |= (1 << (u16)param);
    return 0;
}

static int addItemTradeMm3(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    itemId = kMmTrade3[param];
    if (gMmSave.inventory.items[ITS_MM_TRADE3] == ITEM_NONE)
        gMmSave.inventory.items[ITS_MM_TRADE3] = itemId;
    gMmExtraTrade.trade3 |= (1 << (u16)param);
    gMmExtraTrade.tradeObtained3 |= (1 << (u16)param);
    return 0;
}

static void addOcarinaRawOot(GameState_Play* play, int level)
{
    u16 itemId;

    if (level >= 2)
        itemId = ITEM_OOT_OCARINA_TIME;
    else
        itemId = ITEM_OOT_OCARINA_FAIRY;
    gOotSave.inventory.items[ITS_OOT_OCARINA] = itemId;
    gOotExtraItems.ocarina |= (1 << (level - 1));
    reloadSlotOot(play, ITS_OOT_OCARINA);
}

static void addOcarinaRawMm(GameState_Play* play, int level)
{
    u8 itemId;

    if (level >= 2)
        itemId = ITEM_MM_OCARINA_OF_TIME;
    else
        itemId = ITEM_MM_OCARINA_FAIRY;
    gMmSave.inventory.items[ITS_MM_OCARINA] = itemId;
    gMmExtraItems.ocarina |= (1 << (level - 1));
    reloadSlotMm(play, ITS_MM_OCARINA);
}

static int addItemOcarinaOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addOcarinaRawOot(play, param);
    if (comboConfig(CFG_SHARED_OCARINA))
    {
        if (param >= 2 || comboConfig(CFG_MM_OCARINA_FAIRY))
            addOcarinaRawMm(play, param);
    }
    return 0;
}

static int addItemOcarinaMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addOcarinaRawMm(play, param);
    if (comboConfig(CFG_SHARED_OCARINA))
        addOcarinaRawOot(play, param);
    return 0;
}

static int addItemBottleNewOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_NONE)
        {
            gOotSave.inventory.items[ITS_OOT_BOTTLE + i] = itemId;
            break;
        }
    }

    return 0;
}

static int addItemBottleRefillOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_OOT_BOTTLE_EMPTY)
        {
            gOotSave.inventory.items[ITS_OOT_BOTTLE + i] = itemId;
            reloadSlotOot(play, ITS_OOT_BOTTLE + i);
            break;
        }
    }

    return 0;
}

static int addItemBottleNewMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gMmSave.inventory.items[ITS_MM_BOTTLE + i] == ITEM_NONE)
        {
            gMmSave.inventory.items[ITS_MM_BOTTLE + i] = itemId;
            break;
        }
    }

    return 0;
}

static int addItemBottleRefillMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gMmSave.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_BOTTLE_EMPTY)
        {
            gMmSave.inventory.items[ITS_MM_BOTTLE + i] = itemId;
            reloadSlotMm(play, ITS_MM_BOTTLE + i);
            break;
        }
    }

    return 0;
}

static const AddItemFunc kAddItemHandlers[] = {
    addItemRupeesOot,
    addItemRupeesMm,
    addItemWalletOot,
    addItemWalletMm,
    addItemBombsOot,
    addItemBombsMm,
    addItemNutsOot,
    addItemNutsMm,
    addItemNutsUpgrade,
    addItemBombchuOot,
    addItemBombchuMm,
    addItemArrowsOot,
    addItemArrowsMm,
    addItemBowOot,
    addItemBowMm,
    addItemSeeds,
    addItemSlingshot,
    addItemNormalOot,
    addItemNormalMm,
    addItemSticksOot,
    addItemSticksMm,
    addItemSticksUpgrade,
    addItemHookshotOot,
    addItemHookshotMm,
    addItemTradeOotChild,
    addItemTradeOotAdult,
    addItemTradeMm1,
    addItemTradeMm2,
    addItemTradeMm3,
    addItemOcarinaOot,
    addItemOcarinaMm,
    addItemBottleNewOot,
    addItemBottleRefillOot,
    addItemBottleNewMm,
    addItemBottleRefillMm,
};

extern const u8 kAddItemFuncs[];
extern const u16 kAddItemParams[];

typedef struct
{
    u16 cfg;
    s16 gi;
    s16 gi2;
}
SharedItem;

/* Good for most purposes, doesn't work so well for items that can have cummulative effects like rupees */
static const SharedItem kSimpleSharedItems[] = {
    { CFG_SHARED_LENS,              GI_OOT_LENS,        GI_MM_LENS },
    { CFG_SHARED_MAGIC_ARROW_FIRE,  GI_OOT_ARROW_FIRE,  GI_MM_ARROW_FIRE },
    { CFG_SHARED_MAGIC_ARROW_ICE,   GI_OOT_ARROW_ICE,   GI_MM_ARROW_ICE },
    { CFG_SHARED_MAGIC_ARROW_LIGHT, GI_OOT_ARROW_LIGHT, GI_MM_ARROW_LIGHT },
    { CFG_SHARED_MASK_BUNNY,        GI_OOT_MASK_BUNNY,  GI_MM_MASK_BUNNY },
    { CFG_SHARED_MASK_TRUTH,        GI_OOT_MASK_TRUTH,  GI_MM_MASK_TRUTH },
    { CFG_SHARED_MASK_KEATON,       GI_OOT_MASK_KEATON, GI_MM_MASK_KEATON },
    { CFG_SHARED_MASK_GORON,        GI_OOT_MASK_GORON,  GI_MM_MASK_GORON  },
    { CFG_SHARED_MASK_ZORA,         GI_OOT_MASK_ZORA,   GI_MM_MASK_ZORA  },
};

static int addItem(GameState_Play* play, s16 gi)
{
    AddItemFunc func;
    int ret;
    u16 addParam;
    u8 addId;

    addId = kAddItemFuncs[gi - 1];
    addParam = kAddItemParams[gi - 1];

    if (addId == IA_NONE)
    {
        /* Legacy item add */
        if (play == NULL)
            ret = comboAddItemLegacyNoEffect(gi);
        else
            ret = comboAddItemLegacy(play, gi);
    }
    else
    {
        /* New item handlers */
        func = kAddItemHandlers[addId];
        ret = func(play, kExtendedGetItems[gi - 1].itemId, gi, addParam);
    }

    return ret;
}

int comboAddItem(GameState_Play* play, s16 gi)
{
    const SharedItem* si;
    int count;
    s16 otherGi;

    count = addItem(play, gi);
    for (int i = 0; i < ARRAY_SIZE(kSimpleSharedItems); ++i)
    {
        si = &kSimpleSharedItems[i];
        if (si->gi == gi)
            otherGi = si->gi2;
        else if (si->gi2 == gi)
            otherGi = si->gi;
        else
            continue;
        if (comboConfig(si->cfg))
        {
            addItem(play, otherGi);
            break;
        }
    }
    return count;
}
