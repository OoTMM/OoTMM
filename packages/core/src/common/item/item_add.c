#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>
#include <combo/souls.h>

#if defined(GAME_OOT)
# define addRupeesRaw  addRupeesRawOot
#else
# define addRupeesRaw  addRupeesRawMm
#endif

static const u16 kButtonMasks[] = {
    A_BUTTON,
    R_CBUTTONS,
    L_CBUTTONS,
    U_CBUTTONS,
    D_CBUTTONS,
};

static const u8 kMmSwords[] = {
    ITEM_NONE,
    ITEM_MM_SWORD_KOKIRI,
    ITEM_MM_SWORD_RAZOR,
    ITEM_MM_SWORD_GILDED,
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
    ITS_MM_OCARINA, // needed padding??
    ITS_MM_BOMBS,
    ITS_MM_BOMBCHU,
    ITS_MM_STICKS,
    ITS_MM_NUTS,
    ITS_MM_BEANS,
    ITS_MM_OCARINA, // needed padding??
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
    if (index == 1)
    {
        gOotExtraFlags.childWallet = 1;
    }
    else if (index == 5)
    {
        gOotExtraFlags.bottomlessWallet = 1;
    }
    else
        gOotSave.inventory.upgrades.wallet = (index - 1);
    comboWalletRefresh();
}

static void addWalletRawMm(u16 index)
{
    if (index == 1)
    {
        gMmExtraFlags2.childWallet = 1;
    }
    else if (index == 5)
    {
        gMmExtraFlags3.bottomlessWallet = 1;
    }
    else
        gMmSave.inventory.upgrades.wallet = (index - 1);
    comboWalletRefresh();
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

static void addBombchuRawOot(u8 count)
{
    if (comboConfig(CFG_OOT_BOMBCHU_BAG) && gOotSave.inventory.items[ITS_OOT_BOMBCHU] != ITEM_OOT_BOMBCHU_10)
        return;

    addAmmoOot(ITS_OOT_BOMBCHU, ITEM_OOT_BOMBCHU_10, 50, count);
}

static void addBombchuRawMm(u8 count)
{
    if (comboConfig(CFG_MM_BOMBCHU_BAG))
    {
        if (gMmSave.inventory.items[ITS_MM_BOMBCHU] == ITEM_MM_BOMBCHU)
            addAmmoMm(ITS_MM_BOMBCHU, ITEM_MM_BOMBCHU, 50, count);
    }
    else
    {
        if (gMmSave.inventory.upgrades.bombBag)
            addAmmoMm(ITS_MM_BOMBCHU, ITEM_MM_BOMBCHU, kMaxBombs[gMmSave.inventory.upgrades.bombBag], count);
    }
}

static void addBombchuOot(u8 count)
{
    addBombchuRawOot(count);
    if (comboConfig(CFG_SHARED_BOMBCHU))
        addBombchuRawMm(count);
}

static void addBombchuMm(u8 count)
{
    addBombchuRawMm(count);
    if (comboConfig(CFG_SHARED_BOMBCHU))
        addBombchuRawOot(count);
}

static int addItemBombchuOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBombchuOot(param);
    return 0;
}

static int addItemBombchuMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBombchuMm(param);
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
    addArrowsRawMm(count);
    if (comboConfig(CFG_SHARED_BOWS))
        addArrowsRawOot(count);
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
    BITMAP16_SET(gOotSave.eventsItem, EV_OOT_ITEM_DEKU_SEEDS);
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
    u8 slotIndex;

    if (itemId >= ITEM_MM_MASK_DEKU)
    {
        slots = kItemSlotsMasksMm;
        slotIndex = itemId - ITEM_MM_MASK_DEKU;
    }
    else
    {
        slots = kItemSlotsMm;
        slotIndex = itemId;
    }
    gMmSave.inventory.items[slots[slotIndex]] = itemId;
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
    addSticksOot(kMaxSticks[param]);
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
    if (itemId == ITEM_OOT_RUTO_LETTER)
        gOotExtraItems.rutoLetter = 1;

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
    if (itemId == ITEM_MM_GOLD_DUST)
        gMmExtraItems.goldDust = 1;

    for (int i = 0; i < 6; ++i)
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
    for (int i = 0; i < 6; ++i)
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

static int addItemBeansOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addAmmoOot(ITS_OOT_MAGIC_BEAN, ITEM_OOT_MAGIC_BEAN, 10, param);
    return 0;
}

static int addItemBeansMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addAmmoMm(ITS_MM_BEANS, ITEM_MM_MAGIC_BEAN, 20, param);
    return 0;
}

static int addItemSwordOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    int base;

    base = param;
    if (base > 3)
        base = 3;
    gOotSave.inventory.equipment.swords |= (1 << (base - 1));
    if (param >= 3)
    {
        gOotSave.playerData.swordHealth = 8;
        gOotSave.inventory.equipment.swords &= ~EQ_OOT_SWORD_KNIFE_BROKEN;
    }
    if (param >= 4)
        gOotSave.isBiggoronSword = 1;
    return 0;
}

static int addItemSwordMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    int shouldChangeBtn;

    shouldChangeBtn = 1;
#if defined(GAME_MM)
    if (gSave.playerForm == MM_PLAYER_FORM_FIERCE_DEITY)
        shouldChangeBtn = 0;
#endif

    if (shouldChangeBtn)
        gMmSave.itemEquips.buttonItems[0][0] = kMmSwords[param];
    gMmSave.itemEquips.sword = param;

    if (param == 2)
        gMmSave.playerData.swordHealth = 100;

#if defined(GAME_MM)
    if (play)
        Interface_LoadItemIconImpl(play, 0);
#endif

    return 0;
}

static void addBombBagRawOot(u8 index)
{
    if (index > gOotSave.inventory.upgrades.bombBag)
        gOotSave.inventory.upgrades.bombBag = index;
    addBombsRawOot(kMaxBombs[index]);
}

static void addBombBagRawMm(u8 index)
{
    if (index > gMmSave.inventory.upgrades.bombBag)
        gMmSave.inventory.upgrades.bombBag = index;
    addBombsRawMm(kMaxBombs[index]);
}

static int addItemBombBagOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBombBagRawOot(param);
    if (comboConfig(CFG_SHARED_BOMB_BAGS))
        addBombBagRawMm(param);
    return 0;
}

static int addItemBombBagMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBombBagRawMm(param);
    if (comboConfig(CFG_SHARED_BOMB_BAGS))
        addBombBagRawOot(param);
    return 0;
}

static int addItemShieldOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    u8 shieldType;
    u8 isProgressive;
    u8 mask;

    shieldType = (param & 0xff);
    isProgressive = !!((param >> 8) & 0xff);
    mask = 1 << (shieldType - 1);
    gOotSave.inventory.equipment.shields |= mask;
    if (isProgressive)
        gOotExtraItems.shield |= mask;
    return 0;
}

static int addItemShieldMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    u8 shieldType;
    u8 isProgressive;

    shieldType = (param & 0xff);
    isProgressive = !!((param >> 8) & 0xff);
    if (shieldType > gMmSave.itemEquips.shield)
        gMmSave.itemEquips.shield = shieldType;
    if (isProgressive)
        gMmExtraFlags2.progressiveShield = 1;

#if defined(GAME_MM)
    if (play)
        UpdateEquipment(play, GET_LINK(play));
#endif

    return 0;
}

static int addItemTunic(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.inventory.equipment.tunics |= (1 << (param - 1));
    return 0;
}

static int addItemBoots(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.inventory.equipment.boots |= (1 << (param - 1));
    return 0;
}

static int addItemStrengthOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (param > gOotSave.inventory.upgrades.strength)
        gOotSave.inventory.upgrades.strength = param;
    return 0;
}

static int addItemStrengthMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (param > gMmSave.inventory.upgrades.strength)
        gMmSave.inventory.upgrades.strength = param;
    return 0;
}

static int addItemScaleOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (param > gOotSave.inventory.upgrades.dive)
        gOotSave.inventory.upgrades.dive = param;
    return 0;
}

static int addItemScaleMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    if (param > gMmSave.inventory.upgrades.scale)
        gMmSave.inventory.upgrades.scale = param;
    return 0;
}

static int addItemQuestOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.inventory.quest.value |= (1 << param);
    if (param == QUEST_OOT_SONG_EPONA)
        BITMAP16_SET(gOotSave.eventsChk, EV_OOT_CHK_EPONA);
    return 0;
}

static int addItemQuestMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gMmSave.inventory.quest.value |= (1 << param);
    return 0;
}

static void addHealthEffect(u8 count)
{
    gSaveContext.healthDelta += count * 0x10;
}

static void addHealthRawOot(u8 count)
{
    gOotSave.playerData.health += count * 0x10;
    if (gOotSave.playerData.health > gOotSave.playerData.healthMax)
        gOotSave.playerData.health = gOotSave.playerData.healthMax;
}

static void addHealthRawMm(u8 count)
{
    gMmSave.playerData.health += count * 0x10;
    if (gMmSave.playerData.health > gMmSave.playerData.healthMax)
        gMmSave.playerData.health = gMmSave.playerData.healthMax;
}

static void addHealthOot(GameState_Play* play, u8 count)
{
#if defined(GAME_MM)
    if (!comboConfig(CFG_SHARED_HEALTH))
        play = NULL;
#endif

    if (play)
    {
        addHealthEffect(count);
        return;
    }

    if (comboConfig(CFG_SHARED_HEALTH))
        addHealthRawMm(count);
    addHealthRawOot(count);
}

static void addHealthMm(GameState_Play* play, u8 count)
{
#if defined(GAME_OOT)
    if (!comboConfig(CFG_SHARED_HEALTH))
        play = NULL;
#endif

    if (play)
    {
        addHealthEffect(count);
        return;
    }

    if (comboConfig(CFG_SHARED_HEALTH))
        addHealthRawOot(count);
    addHealthRawMm(count);
}

static int addItemHeartOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addHealthOot(play, param);
    return 0;
}

static int addItemHeartMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addHealthMm(play, param);
    return 0;
}

static void addDefenseUpgradeRawOot(void)
{
    gOotSave.playerData.doubleDefense = 1;
    gOotSave.inventory.doubleDefenseHearts = 20;
}

static void addDefenseUpgradeRawMm(void)
{
    gMmSave.playerData.doubleDefense = 1;
    gMmSave.inventory.defenseHearts = 20;
}

static int addItemDefenseUpgradeOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addDefenseUpgradeRawOot();
    if (comboConfig(CFG_SHARED_HEALTH))
        addDefenseUpgradeRawMm();
    addHealthOot(play, 20);
    return 0;
}

static int addItemDefenseUpgradeMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addDefenseUpgradeRawMm();
    if (comboConfig(CFG_SHARED_HEALTH))
        addDefenseUpgradeRawOot();
    addHealthMm(play, 20);
    return 0;
}

static void addHeartPieceRawOot(void)
{
    gOotSave.inventory.quest.heartPieces++;
    if (gOotSave.inventory.quest.heartPieces >= 4)
    {
        gOotSave.inventory.quest.heartPieces -= 4;
        gOotSave.playerData.healthMax += 0x10;
    }
}

static void addHeartPieceRawMm(void)
{
    gMmSave.inventory.quest.heartPieces++;
    if (gMmSave.inventory.quest.heartPieces >= 4)
    {
        gMmSave.inventory.quest.heartPieces -= 4;
        gMmSave.playerData.healthMax += 0x10;
    }
}

static int addItemHeartPieceOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < param; ++i)
    {
        addHeartPieceRawOot();
        if (comboConfig(CFG_SHARED_HEALTH))
            addHeartPieceRawMm();
    }
    addHealthOot(play, 20);
    return 0;
}

static int addItemHeartPieceMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < param; ++i)
    {
        addHeartPieceRawMm();
        if (comboConfig(CFG_SHARED_HEALTH))
            addHeartPieceRawOot();
    }
    addHealthMm(play, 20);
    return 0;
}

static int addSmallKeyOot(u16 dungeonId)
{
    s8 keyCount;

    /* Check for max keys */
    if ((dungeonId != SCE_OOT_TREASURE_SHOP || comboConfig(CFG_OOT_CHEST_GAME_SHUFFLE)) && gOotSave.inventory.dungeonItems[dungeonId].maxKeys >= g.maxKeysOot[dungeonId])
        return 0;

    keyCount = gOotSave.inventory.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gOotSave.inventory.dungeonKeys[dungeonId] = keyCount;
    if (dungeonId == SCE_OOT_TREASURE_SHOP && !comboConfig(CFG_OOT_CHEST_GAME_SHUFFLE))
        return 0;
    else
        return ++gOotSave.inventory.dungeonItems[dungeonId].maxKeys;
}

int addSmallKeyMm(u16 dungeonId)
{
    s8 keyCount;

    /* Max keys */
    if (gMmSave.inventory.dungeonItems[dungeonId].maxKeys >= g.maxKeysMm[dungeonId])
        return 0;

    keyCount = gMmSave.inventory.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gMmSave.inventory.dungeonKeys[dungeonId] = keyCount;
    gMmSave.inventory.dungeonItems[dungeonId].maxKeys++;

    return gMmSave.inventory.dungeonItems[dungeonId].maxKeys;
}

static int addItemSmallKeyOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 0);
#endif
    return addSmallKeyOot(param);
}

static int addItemSmallKeyMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif
    return addSmallKeyMm(param);
}

static int addItemKeyRingOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 0);
#endif
    for (int i = 0; i < g.maxKeysOot[param]; ++i)
        addSmallKeyOot(param);
    return 0;
}

static int addItemKeyRingMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif
    for (int i = 0; i < g.maxKeysMm[param]; ++i)
        addSmallKeyMm(param);
    return 0;
}

static int addItemSkeletonKeyOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    for (int j = 0; j < ARRAY_SIZE(g.maxKeysOot); ++j)
    {
        for (int i = 0; i < g.maxKeysOot[j]; ++i)
            addSmallKeyOot(j);
    }
    return 0;
}

static int addItemSkeletonKeyMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    for (int j = 0; j < ARRAY_SIZE(g.maxKeysMm); ++j)
    {
        for (int i = 0; i < g.maxKeysMm[j]; ++i)
            addSmallKeyMm(j);
    }
    return 0;
}

static int addItemBossKeyOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 1);
#endif

    gOotSave.inventory.dungeonItems[param].bossKey = 1;
    return 0;
}

static int addItemBossKeyMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif

    gMmSave.inventory.dungeonItems[param].bossKey = 1;
    return 0;
}

static int addItemCompassOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 0);
#endif

    gOotSave.inventory.dungeonItems[param].compass = 1;
    return 0;
}

static int addItemCompassMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif

    gMmSave.inventory.dungeonItems[param].compass = 1;
    return 0;
}

static int addItemMapOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 0);
#endif

    gOotSave.inventory.dungeonItems[param].map = 1;
    return 0;
}

static int addItemMapMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif

    gMmSave.inventory.dungeonItems[param].map = 1;
    return 0;
}

static int addSilverRupee(GameState_Play* play, int id)
{
    addRupeesOot(play, 5);
    return comboSilverRupeesIncCount(play, id);
}

static void addSilverPouch(GameState_Play* play, int id)
{
    int max;

    max = gSilverRupeeData[id].count;
    for (int i = 0; i < max; ++i)
        addSilverRupee(play, id);
}

static int addItemSilverRupee(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    return addSilverRupee(play, param);
}

static int addItemSilverPouch(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addSilverPouch(play, param);
    return 0;
}

static int addItemMagicalRupee(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < ARRAY_SIZE(gSilverRupeeData); ++i)
        addSilverPouch(play, i);
    return 0;
}

static int addItemGsToken(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.inventory.quest.goldToken = 1;
    return ++gOotSave.inventory.goldTokens;
}

static int addItemGsTokenSwamp(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    return ++gMmSave.skullCountSwamp;
}

static int addItemGsTokenOcean(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    return ++gMmSave.skullCountOcean;
}

static void fillMagicOot(GameState_Play* play)
{
    int level;
    int max;

    if (!gOotSave.playerData.magicUpgrade)
        return;
    level = gOotSave.playerData.magicUpgrade2 ? 2 : 1;
    max = level * 0x30;

    /* Handle the effect */
#if defined(GAME_OOT)
    if (play)
    {
        gOotSave.playerData.magicSize = 0;
        gSaveContext.magicFillTarget = max;
        return;
    }
#endif

    /* No effect - add the magic directly */
    gOotSave.playerData.magicSize = level;
    gOotSave.playerData.magicAmount = max;
}

static void fillMagicMm(GameState_Play* play)
{
    int level;
    int max;

    if (!gMmSave.playerData.magicAcquired)
        return;
    level = gMmSave.playerData.doubleMagic ? 2 : 1;
    max = level * 0x30;

    /* Handle the effect */
#if defined(GAME_MM)
    if (play)
    {
        gMmSave.playerData.magicLevel = 0;
        gMmSave.playerData.magicAmount = max;
        gSaveContext.magicFillTarget = max;
        return;
    }
#endif

    /* No effect - add the magic directly */
    gMmSave.playerData.magicLevel = level;
    gMmSave.playerData.magicAmount = max;
}

static int addItemMagicUpgradeOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.playerData.magicUpgrade = 1;
    if (param >= 2)
        gOotSave.playerData.magicUpgrade2 = 1;
    fillMagicOot(play);
    return 0;
}

static int addItemMagicUpgradeMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gMmSave.playerData.magicAcquired = 1;
    if (param >= 2)
        gMmSave.playerData.doubleMagic = 1;
    fillMagicMm(play);
    return 0;
}

static int addItemStrayFairy(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = comboStrayFairyIndex();
#endif

    addHealthMm(play, 3);
    if (param == 4)
    {
        MM_SET_EVENT_WEEK(EV_MM_WEEK_TOWN_FAIRY);
        return 0;
    }
    else
    {
        gMmSave.inventory.strayFairies[param]++;
        return gMmSave.inventory.strayFairies[param];
    }
}

static int addItemTriforce(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    return ++gTriforceCount;
}

static int addItemCoin(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    return ++gSharedCustomSave.coins[param];
}

static int addItemButtonOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gSharedCustomSave.ocarinaButtonMaskOot |= kButtonMasks[param];
    return 0;
}

static int addItemButtonMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gSharedCustomSave.ocarinaButtonMaskMm |= kButtonMasks[param];
    return 0;
}

static int addItemKeg(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addAmmoMm(ITS_MM_KEG, ITEM_MM_POWDER_KEG, 1, param);
    return 0;
}

static int addItemSpinUpgrade(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    MM_SET_EVENT_WEEK(EV_MM_WEEK_SPIN_UPGRADE);
    return 0;
}

static int addItemSoulOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    comboAddSoulOot(gi);
    return 0;
}

static int addItemSoulMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    comboAddSoulMm(gi);
    return 0;
}

static int addItemPondFish(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    u8 caughtListCapacity;
    u8 caughtListLength;
    u8  weight;
    u8* caughtList;

    weight = param & 0xff;
    if (param & 0x2000)
        weight |= 0x80;
    if (param & 0x1000)
    {
        caughtList = gSharedCustomSave.caughtAdultFishWeight;
        caughtListCapacity = (ARRAY_SIZE(gSharedCustomSave.caughtAdultFishWeight) - 1);
    }
    else
    {
        caughtList = gSharedCustomSave.caughtChildFishWeight;
        caughtListCapacity = (ARRAY_SIZE(gSharedCustomSave.caughtChildFishWeight) - 1);
    }

    caughtListLength = caughtList[0];
    if (caughtListLength < caughtListCapacity)
    {
        caughtListLength++;
        caughtList[0] = caughtListLength;
        caughtList[caughtListLength] = weight;
    }

    return 0;
}

static int addItemWorldMap(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    Inventory_SetWorldMapCloudVisibility(param);
    return 0;
}

static int addItemOwl(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gMmOwlFlags |= (1 << param);
    return 0;
}

static void addMagicRawOot(u8 count)
{
    s16 acc;
    s16 max;

    if (!gOotSave.playerData.magicUpgrade)
        return;
    acc = gOotSave.playerData.magicAmount;
    acc += count;
    max = gOotSave.playerData.magicUpgrade2 ? 0x60 : 0x30;
    if (acc > max)
        acc = max;
    gOotSave.playerData.magicAmount = acc;
}

static void addMagicRawMm(u8 count)
{
    s16 acc;
    s16 max;

    if (!gMmSave.playerData.magicAcquired)
        return;
    acc = gMmSave.playerData.magicAmount;
    acc += count;
    max = gMmSave.playerData.doubleMagic ? 0x60 : 0x30;
    if (acc > max)
        acc = max;
    gMmSave.playerData.magicAmount = acc;
}

static void addMagicEffect(GameState_Play* play, u8 count)
{
#if defined(GAME_OOT)
    Magic_Refill(play);
    Magic_RequestChange(play, count, 5);
#else
    AddMagic(play, count);
#endif
}

static void addMagicOot(GameState_Play* play, u8 count)
{
#if defined(GAME_MM)
    if (!comboConfig(CFG_SHARED_MAGIC))
        play = NULL;
#endif

    if (play)
    {
        addMagicEffect(play, count);
        return;
    }

    if (comboConfig(CFG_SHARED_MAGIC))
        addMagicRawMm(count);
    addMagicRawOot(count);
}

static void addMagicMm(GameState_Play* play, u8 count)
{
#if defined(GAME_OOT)
    if (!comboConfig(CFG_SHARED_MAGIC))
        play = NULL;
#endif

    if (play)
    {
        addMagicEffect(play, count);
        return;
    }

    if (comboConfig(CFG_SHARED_MAGIC))
        addMagicRawOot(count);
    addMagicRawMm(count);
}

static int addItemMagicOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addMagicOot(play, param * (comboConfig(CFG_SHARED_MAGIC) ? 0x18 : 0x0c));
    return 0;
}

static int addItemMagicMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addMagicMm(play, param * 0x18);
    return 0;
}

static void addBombchuBagRawOot(void)
{
    gOotSave.inventory.items[ITS_OOT_BOMBCHU] = ITEM_OOT_BOMBCHU_10;
}

static void addBombchuBagRawMm(void)
{
    gMmSave.inventory.items[ITS_MM_BOMBCHU] = ITEM_MM_BOMBCHU;
}

static void addBombchuBagOot(u8 count)
{
    addBombchuBagRawOot();
    if (comboConfig(CFG_SHARED_BOMBCHU))
        addBombchuBagRawMm();
    addBombchuOot(count);
}

static void addBombchuBagMm(u8 count)
{
    addBombchuBagRawMm();
    if (comboConfig(CFG_SHARED_BOMBCHU))
        addBombchuBagRawOot();
    addBombchuMm(count);
}

static int addItemBombchuBagOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBombchuBagOot(param);
    return 0;
}

static int addItemBombchuBagMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addBombchuBagMm(param);
    return 0;
}

static int addItemBigFairyOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addHealthOot(play, 8);
    addMagicOot(play, 0x60);
    return 0;
}

static int addItemBigFairyMm(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    addHealthMm(play, 8);
    addMagicMm(play, 0x60);
    return 0;
}

static int addItemClock(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gSharedCustomSave.mm.halfDays |= (1 << param);
    return 0;
}

static int addItemEndgame(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    switch (param)
    {
    case 0:
        gOotExtraFlags.ganon = 1;
        break;
    case 1:
        gMmExtraFlags2.majora = 1;
        break;
    }

    if (comboGoalCond())
        gOotExtraFlags.endgameItemIsWin = 1;

    return 0;
}

static int addElegyOot(GameState_Play* play, u8 itemId, s16 gi, u16 param)
{
    gSharedCustomSave.oot.hasElegy = 1;
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
    addItemBeansOot,
    addItemBeansMm,
    addItemSwordOot,
    addItemSwordMm,
    addItemBombBagOot,
    addItemBombBagMm,
    addItemShieldOot,
    addItemShieldMm,
    addItemTunic,
    addItemBoots,
    addItemStrengthOot,
    addItemScaleOot,
    addItemQuestOot,
    addItemQuestMm,
    addItemHeartOot,
    addItemHeartMm,
    addItemDefenseUpgradeOot,
    addItemDefenseUpgradeMm,
    addItemHeartPieceOot,
    addItemHeartPieceMm,
    addItemSmallKeyOot,
    addItemSmallKeyMm,
    addItemKeyRingOot,
    addItemKeyRingMm,
    addItemSkeletonKeyOot,
    addItemSkeletonKeyMm,
    addItemBossKeyOot,
    addItemBossKeyMm,
    addItemCompassOot,
    addItemCompassMm,
    addItemMapOot,
    addItemMapMm,
    addItemSilverRupee,
    addItemSilverPouch,
    addItemMagicalRupee,
    addItemGsToken,
    addItemGsTokenSwamp,
    addItemGsTokenOcean,
    addItemMagicUpgradeOot,
    addItemMagicUpgradeMm,
    addItemStrayFairy,
    addItemTriforce,
    addItemCoin,
    addItemButtonOot,
    addItemButtonMm,
    addItemKeg,
    addItemSpinUpgrade,
    addItemSoulOot,
    addItemSoulMm,
    addItemPondFish,
    addItemWorldMap,
    addItemOwl,
    addItemMagicOot,
    addItemMagicMm,
    addItemBombchuBagOot,
    addItemBombchuBagMm,
    addItemBigFairyOot,
    addItemBigFairyMm,
    addItemScaleMm,
    addItemStrengthMm,
    addItemClock,
    addItemEndgame,
    addElegyOot,
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
    { CFG_SHARED_LENS, GI_OOT_LENS, GI_MM_LENS },
    { CFG_SHARED_MAGIC_ARROW_FIRE, GI_OOT_ARROW_FIRE, GI_MM_ARROW_FIRE },
    { CFG_SHARED_MAGIC_ARROW_ICE, GI_OOT_ARROW_ICE, GI_MM_ARROW_ICE },
    { CFG_SHARED_MAGIC_ARROW_LIGHT, GI_OOT_ARROW_LIGHT, GI_MM_ARROW_LIGHT },
    { CFG_SHARED_MASK_BUNNY, GI_OOT_MASK_BUNNY, GI_MM_MASK_BUNNY },
    { CFG_SHARED_MASK_TRUTH, GI_OOT_MASK_TRUTH, GI_MM_MASK_TRUTH },
    { CFG_SHARED_MASK_KEATON, GI_OOT_MASK_KEATON, GI_MM_MASK_KEATON },
    { CFG_SHARED_MASK_GORON, GI_OOT_MASK_GORON, GI_MM_MASK_GORON },
    { CFG_SHARED_MASK_ZORA, GI_OOT_MASK_ZORA, GI_MM_MASK_ZORA },
    { CFG_SHARED_SONG_EPONA, GI_OOT_SONG_EPONA, GI_MM_SONG_EPONA },
    { CFG_SHARED_SONG_TIME, GI_OOT_SONG_TIME, GI_MM_SONG_TIME },
    { CFG_SHARED_SONG_STORMS, GI_OOT_SONG_STORMS, GI_MM_SONG_STORMS },
    { CFG_SHARED_SONG_SUN, GI_OOT_SONG_SUN, GI_MM_SONG_SUN },
    { CFG_SHARED_SKELETON_KEY, GI_OOT_SKELETON_KEY, GI_MM_SKELETON_KEY },
    { CFG_SHARED_MAGIC, GI_OOT_MAGIC_UPGRADE, GI_MM_MAGIC_UPGRADE },
    { CFG_SHARED_MAGIC, GI_OOT_MAGIC_UPGRADE2, GI_MM_MAGIC_UPGRADE2 },
    { CFG_SHARED_OCARINA_BUTTONS, GI_OOT_BUTTON_A, GI_MM_BUTTON_A },
    { CFG_SHARED_OCARINA_BUTTONS, GI_OOT_BUTTON_C_RIGHT, GI_MM_BUTTON_C_RIGHT },
    { CFG_SHARED_OCARINA_BUTTONS, GI_OOT_BUTTON_C_LEFT, GI_MM_BUTTON_C_LEFT },
    { CFG_SHARED_OCARINA_BUTTONS, GI_OOT_BUTTON_C_UP, GI_MM_BUTTON_C_UP },
    { CFG_SHARED_OCARINA_BUTTONS, GI_OOT_BUTTON_C_DOWN, GI_MM_BUTTON_C_DOWN },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_OCTOROK, GI_MM_SOUL_ENEMY_OCTOROK },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_WALLMASTER, GI_MM_SOUL_ENEMY_WALLMASTER },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_DODONGO, GI_MM_SOUL_ENEMY_DODONGO },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_KEESE, GI_MM_SOUL_ENEMY_KEESE },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_TEKTITE, GI_MM_SOUL_ENEMY_TEKTITE },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_PEAHAT, GI_MM_SOUL_ENEMY_PEAHAT },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_LIZALFOS_DINALFOS, GI_MM_SOUL_ENEMY_LIZALFOS_DINALFOS },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_SKULLTULA, GI_MM_SOUL_ENEMY_SKULLTULA },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_ARMOS, GI_MM_SOUL_ENEMY_ARMOS },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_DEKU_BABA, GI_MM_SOUL_ENEMY_DEKU_BABA },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_DEKU_SCRUB, GI_MM_SOUL_ENEMY_DEKU_SCRUB },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_BUBBLE, GI_MM_SOUL_ENEMY_BUBBLE },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_BEAMOS, GI_MM_SOUL_ENEMY_BEAMOS },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_REDEAD_GIBDO, GI_MM_SOUL_ENEMY_REDEAD_GIBDO },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_SKULLWALLTULA, GI_MM_SOUL_ENEMY_SKULLWALLTULA },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_SHELL_BLADE, GI_MM_SOUL_ENEMY_SHELL_BLADE },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_LIKE_LIKE, GI_MM_SOUL_ENEMY_LIKE_LIKE },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_IRON_KNUCKLE, GI_MM_SOUL_ENEMY_IRON_KNUCKLE },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_FREEZARD, GI_MM_SOUL_ENEMY_FREEZARD },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_WOLFOS, GI_MM_SOUL_ENEMY_WOLFOS },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_GUAY, GI_MM_SOUL_ENEMY_GUAY },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_FLYING_POT, GI_MM_SOUL_ENEMY_FLYING_POT },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_FLOORMASTER, GI_MM_SOUL_ENEMY_FLOORMASTER },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_LEEVER, GI_MM_SOUL_ENEMY_LEEVER },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_STALCHILD, GI_MM_SOUL_ENEMY_STALCHILD },
    { CFG_SHARED_SHIELDS, GI_OOT_SHIELD_HYLIAN, GI_MM_SHIELD_HERO },
    { CFG_SHARED_SHIELDS, GI_OOT_SHIELD_MIRROR, GI_MM_SHIELD_MIRROR },
    { CFG_SHARED_SHIELDS, GI_OOT_PROGRESSIVE_SHIELD_HYLIAN, GI_MM_PROGRESSIVE_SHIELD_HERO },
    { CFG_SHARED_SPELL_FIRE, GI_OOT_SPELL_FIRE, GI_MM_SPELL_FIRE },
    { CFG_SHARED_SPELL_WIND, GI_OOT_SPELL_WIND, GI_MM_SPELL_WIND },
    { CFG_SHARED_SPELL_LOVE, GI_OOT_SPELL_LOVE, GI_MM_SPELL_LOVE },
    { CFG_SHARED_BOOTS_IRON, GI_OOT_BOOTS_IRON, GI_MM_BOOTS_IRON },
    { CFG_SHARED_BOOTS_HOVER, GI_OOT_BOOTS_HOVER, GI_MM_BOOTS_HOVER },
    { CFG_SHARED_TUNIC_GORON, GI_OOT_TUNIC_GORON, GI_MM_TUNIC_GORON },
    { CFG_SHARED_TUNIC_ZORA, GI_OOT_TUNIC_ZORA, GI_MM_TUNIC_ZORA },
    { CFG_SHARED_MASK_BLAST, GI_OOT_MASK_BLAST, GI_MM_MASK_BLAST },
    { CFG_SHARED_MASK_STONE, GI_OOT_MASK_STONE, GI_MM_MASK_STONE },
    { CFG_SHARED_SCALES, GI_OOT_SCALE_SILVER, GI_MM_SCALE_SILVER },
    { CFG_SHARED_SCALES, GI_OOT_SCALE_GOLDEN, GI_MM_SCALE_GOLDEN },
    { CFG_SHARED_STRENGTH, GI_OOT_GORON_BRACELET, GI_MM_GORON_BRACELET },
    { CFG_SHARED_STRENGTH, GI_OOT_SILVER_GAUNTLETS, GI_MM_SILVER_GAUNTLETS },
    { CFG_SHARED_STRENGTH, GI_OOT_GOLDEN_GAUNTLETS, GI_MM_GOLDEN_GAUNTLETS },
    { CFG_SHARED_SONG_EMPTINESS, GI_OOT_SONG_EMPTINESS, GI_MM_SONG_EMPTINESS },
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
        return 0;

    func = kAddItemHandlers[addId];
    ret = func(play, kExtendedGetItems[gi - 1].itemId, gi, addParam);

    return ret;
}

int comboAddItemRaw(GameState_Play* play, s16 gi)
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
            addItem(play, otherGi);
        break;
    }
    return count;
}
