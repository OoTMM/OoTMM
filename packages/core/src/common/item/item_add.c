#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>
#include <combo/souls.h>
#include <combo/player.h>
#include <combo/magic.h>
#include <combo/config.h>
#include <combo/global.h>

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
    ITS_MM_OCARINA, /* needed padding?? */
    ITS_MM_BOMBS,
    ITS_MM_BOMBCHU,
    ITS_MM_STICKS,
    ITS_MM_NUTS,
    ITS_MM_BEANS,
    ITS_MM_OCARINA, /* needed padding?? */
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

typedef int (*AddItemFunc)(PlayState* play, u8 itemId, s16 gi, u16 param);

static void reloadSlotEquipsOot(OotItemEquips* equips, int slot)
{
    for (int i = 0; i < 3; ++i)
    {
        if (equips->cButtonSlots[i] == slot)
        {
            equips->buttonItems[1 + i] = gOotSave.info.inventory.items[slot];
        }
    }
}

void reloadSlotRawOot(int slot)
{
    reloadSlotEquipsOot(&gOotSave.info.equips, slot);
    reloadSlotEquipsOot(&gOotSave.info.childEquips, slot);
    reloadSlotEquipsOot(&gOotSave.info.adultEquips, slot);
}

static void reloadSlotEquipsMm(MmItemEquips* equips, int slot)
{
    for (int i = 0; i < 3; ++i)
    {
        if (equips->cButtonSlots[0][1 + i] == slot)
        {
            equips->buttonItems[0][1 + i] = gMmSave.info.inventory.items[slot];
        }
    }
}

static void reloadSlotRawMm(int slot)
{
    reloadSlotEquipsMm(&gMmSave.info.itemEquips, slot);
}

void reloadSlotOot(PlayState* play, int slot)
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

void reloadSlotMm(PlayState* play, int slot)
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
static void reloadHookshot(PlayState* play)
{
    Player* link;

    if (play)
    {
        link = GET_PLAYER(play);
        link->stateFlags1 &= ~(PLAYER_ACTOR_STATE_HOLD_ITEM | PLAYER_ACTOR_STATE_USE_ITEM);
    }
}
#endif

static void addRupeesRawOot(s16 delta)
{
    u16 max;

    max = gOotMaxRupees[gOotSave.info.inventory.upgrades.wallet];
    gOotSave.info.playerData.rupees += RupeeValueOot(delta);
    if (gOotSave.info.playerData.rupees > max)
        gOotSave.info.playerData.rupees = max;
    if (gOotSave.info.playerData.rupees < 0)
        gOotSave.info.playerData.rupees = 0;
}

static void addRupeesRawMm(s16 delta)
{
    u16 max;

    max = gMmMaxRupees[gMmSave.info.inventory.upgrades.wallet];
    gMmSave.info.playerData.rupees += RupeeValueMm(delta);
    if (gMmSave.info.playerData.rupees > max)
        gMmSave.info.playerData.rupees = max;
    if (gMmSave.info.playerData.rupees < 0)
        gMmSave.info.playerData.rupees = 0;
}

static void addRupeesOot(PlayState* play, s16 delta)
{
#if defined(GAME_MM)
    if (!Config_Flag(CFG_SHARED_WALLETS))
        play = NULL;
#endif

    if (play)
    {
        AddRupees(delta);
        return;
    }

    if (Config_Flag(CFG_SHARED_WALLETS))
        addRupeesRaw(delta);
    else
        addRupeesRawOot(delta);
}

static void addRupeesMm(PlayState* play, s16 delta)
{
#if defined(GAME_OOT)
    if (!Config_Flag(CFG_SHARED_WALLETS))
        play = NULL;
#endif

    if (play)
    {
        AddRupees(delta);
        return;
    }

    if (Config_Flag(CFG_SHARED_WALLETS))
        addRupeesRaw(delta);
    else
        addRupeesRawMm(delta);
}

static int addItemRupeesOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addRupeesOot(play, param);
    return 0;
}

static int addItemRupeesMm(PlayState* play, u8 itemId, s16 gi, u16 param)
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
        gOotSave.info.inventory.upgrades.wallet = (index - 1);
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
        gMmSave.info.inventory.upgrades.wallet = (index - 1);
    comboWalletRefresh();
}

static void addWalletRawShared(u16 index)
{
    addWalletRawOot(index);
    addWalletRawMm(index);
}

static int addItemWalletOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (Config_Flag(CFG_SHARED_WALLETS))
        addWalletRawShared(param);
    else
        addWalletRawOot(param);

    if (Config_Flag(CFG_FILL_WALLETS))
        addRupeesOot(play, gOotMaxRupees[gOotSave.info.inventory.upgrades.wallet]);

    return 0;
}

static int addItemWalletMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (Config_Flag(CFG_SHARED_WALLETS))
        addWalletRawShared(param);
    else
        addWalletRawMm(param);

    if (Config_Flag(CFG_FILL_WALLETS))
        addRupeesMm(play, gMmMaxRupees[gMmSave.info.inventory.upgrades.wallet]);

    return 0;
}

static void addAmmoOot(u8 slot, u16 item, u8 max, u8 count)
{
    gOotSave.info.inventory.items[slot] = item;
    gOotSave.info.inventory.ammo[slot] += count;
    if (gOotSave.info.inventory.ammo[slot] > max)
        gOotSave.info.inventory.ammo[slot] = max;
}

static void addAmmoMm(u8 slot, u16 item, u8 max, u8 count)
{
    gMmSave.info.inventory.items[slot] = item;
    gMmSave.info.inventory.ammo[slot] += count;
    if (gMmSave.info.inventory.ammo[slot] > max)
        gMmSave.info.inventory.ammo[slot] = max;
}

static void addBombsRawOot(u8 count)
{
    u8 max;

    if (gOotSave.info.inventory.upgrades.bombBag == 0)
        return;
    max = kMaxBombs[gOotSave.info.inventory.upgrades.bombBag];
    addAmmoOot(ITS_OOT_BOMBS, ITEM_OOT_BOMB, max, count);
}

static void addBombsRawMm(u8 count)
{
    u8 max;

    if (gMmSave.info.inventory.upgrades.bombBag == 0)
        return;
    max = kMaxBombs[gMmSave.info.inventory.upgrades.bombBag];
    addAmmoMm(ITS_MM_BOMBS, ITEM_MM_BOMB, max, count);
}

static void addBombsOot(u8 count)
{
    addBombsRawOot(count);
    if (Config_Flag(CFG_SHARED_BOMB_BAGS))
        addBombsRawMm(count);
}

static void addBombsMm(u8 count)
{
    addBombsRawMm(count);
    if (Config_Flag(CFG_SHARED_BOMB_BAGS))
        addBombsRawOot(count);
}

static int addItemBombsOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBombsOot(param);
    return 0;
}

static int addItemBombsMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBombsMm(param);
    return 0;
}

void addNutsRawOot(int count)
{
    u8 max;

    if (gOotSave.info.inventory.upgrades.dekuNut == 0)
        gOotSave.info.inventory.upgrades.dekuNut = 1;

    max = kMaxNuts[gOotSave.info.inventory.upgrades.dekuNut];
    addAmmoOot(ITS_OOT_NUTS, ITEM_OOT_NUT, max, count);
}

void addNutsRawMm(int count)
{
    u8 max;

    if (gMmSave.info.inventory.upgrades.dekuNut == 0)
        gMmSave.info.inventory.upgrades.dekuNut = 1;

    max = kMaxNuts[gMmSave.info.inventory.upgrades.dekuNut];
    addAmmoMm(ITS_MM_NUTS, ITEM_MM_NUT, max, count);
}

void addNutsOot(int count)
{
    addNutsRawOot(count);
    if (Config_Flag(CFG_SHARED_NUTS_STICKS))
        addNutsRawMm(count);
}

void addNutsMm(int count)
{
    addNutsRawMm(count);
    if (Config_Flag(CFG_SHARED_NUTS_STICKS))
        addNutsRawOot(count);
}

static int addItemNutsOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addNutsOot(param);
    return 0;
}

static int addItemNutsMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addNutsMm(param);
    return 0;
}

static int addItemNutsUpgradeOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (gOotSave.info.inventory.upgrades.dekuNut < param)
        gOotSave.info.inventory.upgrades.dekuNut = param;
    addNutsOot(kMaxNuts[param]);
    return 0;
}

static int addItemNutsUpgradeMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (gMmSave.info.inventory.upgrades.dekuNut < param)
        gMmSave.info.inventory.upgrades.dekuNut = param;
    addNutsMm(kMaxNuts[param]);
    return 0;
}

static void addBombchuRawOot(u8 count)
{
    if (Config_Flag(CFG_OOT_BOMBCHU_BAG) && gOotSave.info.inventory.items[ITS_OOT_BOMBCHU] != ITEM_OOT_BOMBCHU_10)
        return;

    addAmmoOot(ITS_OOT_BOMBCHU, ITEM_OOT_BOMBCHU_10, 50, count);
}

static void addBombchuRawMm(u8 count)
{
    if (Config_Flag(CFG_MM_BOMBCHU_BAG))
    {
        if (gMmSave.info.inventory.items[ITS_MM_BOMBCHU] == ITEM_MM_BOMBCHU)
            addAmmoMm(ITS_MM_BOMBCHU, ITEM_MM_BOMBCHU, 50, count);
    }
    else
    {
        if (gMmSave.info.inventory.upgrades.bombBag)
            addAmmoMm(ITS_MM_BOMBCHU, ITEM_MM_BOMBCHU, kMaxBombs[gMmSave.info.inventory.upgrades.bombBag], count);
    }
}

static void addBombchuOot(u8 count)
{
    addBombchuRawOot(count);
    if (Config_Flag(CFG_SHARED_BOMBCHU))
        addBombchuRawMm(count);
}

static void addBombchuMm(u8 count)
{
    addBombchuRawMm(count);
    if (Config_Flag(CFG_SHARED_BOMBCHU))
        addBombchuRawOot(count);
}

static int addItemBombchuOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBombchuOot(param);
    return 0;
}

static int addItemBombchuMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBombchuMm(param);
    return 0;
}

static void addArrowsRawOot(u8 count)
{
    u8 max;

    if (gOotSave.info.inventory.upgrades.quiver == 0)
        return;
    max = kMaxArrows[gOotSave.info.inventory.upgrades.quiver];
    addAmmoOot(ITS_OOT_BOW, ITEM_OOT_BOW, max, count);
}

static void addArrowsRawMm(u8 count)
{
    u8 max;

    if (gMmSave.info.inventory.upgrades.quiver == 0)
        return;
    max = kMaxArrows[gMmSave.info.inventory.upgrades.quiver];
    addAmmoMm(ITS_MM_BOW, ITEM_MM_BOW, max, count);
}

static void addArrowsOot(u8 count)
{
    addArrowsRawOot(count);
    if (Config_Flag(CFG_SHARED_BOWS))
        addArrowsRawMm(count);
}

static void addArrowsMm(u8 count)
{
    addArrowsRawMm(count);
    if (Config_Flag(CFG_SHARED_BOWS))
        addArrowsRawOot(count);
}

static int addItemArrowsOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addArrowsOot(param);
    return 0;
}

static int addItemArrowsMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addArrowsMm(param);
    return 0;
}

static void addBowRawOot(u8 index)
{
    if (index > gOotSave.info.inventory.upgrades.quiver)
        gOotSave.info.inventory.upgrades.quiver = index;
    addArrowsRawOot(kMaxArrows[index]);
}

static void addBowRawMm(u8 index)
{
    if (index > gMmSave.info.inventory.upgrades.quiver)
        gMmSave.info.inventory.upgrades.quiver = index;
    addArrowsRawMm(kMaxArrows[index]);
}

static int addItemBowOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBowRawOot(param);
    if (Config_Flag(CFG_SHARED_BOWS))
        addBowRawMm(param);
    return 0;
}

static int addItemBowMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBowRawMm(param);
    if (Config_Flag(CFG_SHARED_BOWS))
        addBowRawOot(param);
    return 0;
}

static void addSeeds(u8 count)
{
    u8 max;

    if (gOotSave.info.inventory.upgrades.bulletBag == 0)
        return;
    max = kMaxSeeds[gOotSave.info.inventory.upgrades.bulletBag];
    addAmmoOot(ITS_OOT_SLINGSHOT, ITEM_OOT_SLINGSHOT, max, count);
    BITMAP16_SET(gOotSave.info.eventsItem, EV_OOT_ITEM_DEKU_SEEDS);
}

static int addItemSeeds(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addSeeds(param);
    return 0;
}

static int addItemSlingshot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (gOotSave.info.inventory.upgrades.bulletBag < param)
        gOotSave.info.inventory.upgrades.bulletBag = param;
    addSeeds(kMaxSeeds[param]);
    return 0;
}

static int addItemNormalOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.info.inventory.items[kItemSlotsOot[itemId]] = itemId;
    return 0;
}

static int addItemNormalMm(PlayState* play, u8 itemId, s16 gi, u16 param)
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
    gMmSave.info.inventory.items[slots[slotIndex]] = itemId;
    return 0;
}

void addSticksRawOot(int count)
{
    u8 max;

    if (gOotSave.info.inventory.upgrades.dekuStick == 0)
        gOotSave.info.inventory.upgrades.dekuStick = 1;

    max = kMaxSticks[gOotSave.info.inventory.upgrades.dekuStick];
    addAmmoOot(ITS_OOT_STICKS, ITEM_OOT_STICK, max, count);
}

void addSticksRawMm(int count)
{
    u8 max;

    if (gMmSave.info.inventory.upgrades.dekuStick == 0)
        gMmSave.info.inventory.upgrades.dekuStick = 1;

    max = kMaxSticks[gMmSave.info.inventory.upgrades.dekuStick];
    addAmmoMm(ITS_MM_STICKS, ITEM_MM_STICK, max, count);
}

void addSticksOot(int count)
{
    addSticksRawOot(count);
    if (Config_Flag(CFG_SHARED_NUTS_STICKS))
        addSticksRawMm(count);
}

void addSticksMm(int count)
{
    addSticksRawMm(count);
    if (Config_Flag(CFG_SHARED_NUTS_STICKS))
        addSticksRawOot(count);
}

static int addItemSticksOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addSticksOot(param);
    return 0;
}

static int addItemSticksMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addSticksMm(param);
    return 0;
}

static int addItemSticksUpgradeOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (gOotSave.info.inventory.upgrades.dekuStick < param)
        gOotSave.info.inventory.upgrades.dekuStick = param;
    addSticksOot(kMaxSticks[param]);
    return 0;
}

static int addItemSticksUpgradeMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (gMmSave.info.inventory.upgrades.dekuStick < param)
        gMmSave.info.inventory.upgrades.dekuStick = param;
    addSticksMm(kMaxSticks[param]);
    return 0;
}

static void addHookshotRawOot(PlayState* play, int level)
{
    u16 itemId;

    if (level >= 2)
        itemId = ITEM_OOT_LONGSHOT;
    else
        itemId = ITEM_OOT_HOOKSHOT;
    gOotSave.info.inventory.items[ITS_OOT_HOOKSHOT] = itemId;
    gOotExtraItems.hookshot |= (1 << (level - 1));
    reloadSlotOot(play, ITS_OOT_HOOKSHOT);
}

static void addHookshotRawMm(PlayState* play, int level)
{
    u8 itemId;

    if (level >= 2)
        itemId = ITEM_MM_HOOKSHOT;
    else
        itemId = 0x11; /* ITEM_MM_BOTTLE_POTION_RED but that enum is wrong */
    gMmSave.info.inventory.items[ITS_MM_HOOKSHOT] = itemId;
    gMmExtraItems.hookshot |= (1 << (level - 1));
    reloadSlotMm(play, ITS_MM_HOOKSHOT);
#if defined(GAME_MM)
    if (Config_Flag(CFG_MM_HOOKSHOT_SHORT) && level >= 2)
        reloadHookshot(play);
#endif
}

static void addHookshotOot(PlayState* play, int level)
{
    addHookshotRawOot(play, level);
    if (Config_Flag(CFG_SHARED_HOOKSHOT))
    {
        if (Config_Flag(CFG_MM_HOOKSHOT_SHORT))
            addHookshotRawMm(play, level);
        else
            addHookshotRawMm(play, 2);
    }
}

static void addHookshotMm(PlayState* play, int level)
{
    addHookshotRawMm(play, level);
    if (Config_Flag(CFG_SHARED_HOOKSHOT))
    {
        if (Config_Flag(CFG_MM_HOOKSHOT_SHORT))
            addHookshotRawOot(play, level);
        else
            addHookshotRawOot(play, 1);
    }
}

static int addItemHookshotOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addHookshotOot(play, param);
    return 0;
}

static int addItemHookshotMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addHookshotMm(play, param);
    return 0;
}

static int addItemTradeOotChild(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    u16 mask;

    itemId = kOotTradeChild[param];
    mask = 1 << (u16)param;
    if (gOotExtraTradeSave.child & mask)
        return 0;
    if (gOotSave.info.inventory.items[ITS_OOT_TRADE_CHILD] == ITEM_NONE)
        gOotSave.info.inventory.items[ITS_OOT_TRADE_CHILD] = itemId;
    gOotExtraTrade.child |= mask;
    gOotExtraTradeSave.child |= mask;
    return 0;
}

static int addItemTradeOotAdult(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    u16 mask;

    itemId = kOotTradeAdult[param];
    mask = 1 << (u16)param;
    if (gOotExtraTradeSave.adult & mask)
        return 0;
    if (gOotSave.info.inventory.items[ITS_OOT_TRADE_ADULT] == ITEM_NONE)
        gOotSave.info.inventory.items[ITS_OOT_TRADE_ADULT] = itemId;
    gOotExtraTrade.adult |= mask;
    gOotExtraTradeSave.adult |= mask;
    return 0;
}

static int addItemTradeMm1(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    itemId = kMmTrade1[param];
    if (gMmSave.info.inventory.items[ITS_MM_TRADE1] == ITEM_NONE)
        gMmSave.info.inventory.items[ITS_MM_TRADE1] = itemId;
    gMmExtraTrade.trade1 |= (1 << (u16)param);
    gMmExtraTrade.tradeObtained1 |= (1 << (u16)param);
    return 0;
}

static int addItemTradeMm2(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    itemId = kMmTrade2[param];
    if (gMmSave.info.inventory.items[ITS_MM_TRADE2] == ITEM_NONE)
        gMmSave.info.inventory.items[ITS_MM_TRADE2] = itemId;
    gMmExtraTrade.trade2 |= (1 << (u16)param);
    gMmExtraTrade.tradeObtained2 |= (1 << (u16)param);
    return 0;
}

static int addItemTradeMm3(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    itemId = kMmTrade3[param];
    if (gMmSave.info.inventory.items[ITS_MM_TRADE3] == ITEM_NONE)
        gMmSave.info.inventory.items[ITS_MM_TRADE3] = itemId;
    gMmExtraTrade.trade3 |= (1 << (u16)param);
    gMmExtraTrade.tradeObtained3 |= (1 << (u16)param);
    return 0;
}

static void addOcarinaRawOot(PlayState* play, int level)
{
    u16 itemId;

    if (level >= 2)
        itemId = ITEM_OOT_OCARINA_TIME;
    else
        itemId = ITEM_OOT_OCARINA_FAIRY;
    gOotSave.info.inventory.items[ITS_OOT_OCARINA] = itemId;
    gOotExtraItems.ocarina |= (1 << (level - 1));
    reloadSlotOot(play, ITS_OOT_OCARINA);
}

static void addOcarinaRawMm(PlayState* play, int level)
{
    u8 itemId;

    if (level >= 2)
        itemId = ITEM_MM_OCARINA_OF_TIME;
    else
        itemId = ITEM_MM_OCARINA_FAIRY;
    gMmSave.info.inventory.items[ITS_MM_OCARINA] = itemId;
    gMmExtraItems.ocarina |= (1 << (level - 1));
    reloadSlotMm(play, ITS_MM_OCARINA);
}

static int addItemOcarinaOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addOcarinaRawOot(play, param);
    if (Config_Flag(CFG_SHARED_OCARINA))
    {
        if (param >= 2 || Config_Flag(CFG_MM_OCARINA_FAIRY))
            addOcarinaRawMm(play, param);
    }
    return 0;
}

static int addItemOcarinaMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addOcarinaRawMm(play, param);
    if (Config_Flag(CFG_SHARED_OCARINA))
        addOcarinaRawOot(play, param);
    return 0;
}

static int addItemBottleNewOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (itemId == ITEM_OOT_RUTO_LETTER)
        gOotExtraItems.rutoLetter = 1;

    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_NONE)
        {
            gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i] = itemId;
            break;
        }
    }

    return 0;
}

static int addItemBottleRefillOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_OOT_BOTTLE_EMPTY)
        {
            gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i] = itemId;
            reloadSlotOot(play, ITS_OOT_BOTTLE + i);
            break;
        }
    }

    return 0;
}

static int addItemBottleNewMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (itemId == ITEM_MM_GOLD_DUST)
        gMmExtraItems.goldDust = 1;

    for (int i = 0; i < 6; ++i)
    {
        if (gMmSave.info.inventory.items[ITS_MM_BOTTLE + i] == ITEM_NONE)
        {
            gMmSave.info.inventory.items[ITS_MM_BOTTLE + i] = itemId;
            break;
        }
    }

    return 0;
}

static int addItemBottleRefillMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < 6; ++i)
    {
        if (gMmSave.info.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_BOTTLE_EMPTY)
        {
            gMmSave.info.inventory.items[ITS_MM_BOTTLE + i] = itemId;
            reloadSlotMm(play, ITS_MM_BOTTLE + i);
            break;
        }
    }

    return 0;
}

static int addItemBeansOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addAmmoOot(ITS_OOT_MAGIC_BEAN, ITEM_OOT_MAGIC_BEAN, 10, param);
    return 0;
}

static int addItemBeansMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addAmmoMm(ITS_MM_BEANS, ITEM_MM_MAGIC_BEAN, 20, param);
    return 0;
}

static int addItemSwordOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    int base;

    base = param;
    if (base > 3)
        base = 3;
    gOotSave.info.inventory.equipment.swords |= (1 << (base - 1));
    if (param >= 3)
    {
        gOotSave.info.playerData.swordHealth = 8;
        gOotSave.info.inventory.equipment.swords &= ~EQ_OOT_SWORD_KNIFE_BROKEN;
    }
    if (param >= 4)
        gOotSave.info.isBiggoronSword = 1;
    if (param == 2)
        gSharedCustomSave.foundMasterSword = 1;
    return 0;
}

static int addItemSwordMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    int shouldChangeBtn;

    shouldChangeBtn = 1;
#if defined(GAME_MM)
    if (gSave.playerForm == MM_PLAYER_FORM_FIERCE_DEITY)
        shouldChangeBtn = 0;
#endif

    if (shouldChangeBtn)
        gMmSave.info.itemEquips.buttonItems[0][0] = kMmSwords[param];
    gMmSave.info.itemEquips.sword = param;

    if (param == 2)
        gMmSave.info.playerData.swordHealth = 100;

#if defined(GAME_MM)
    if (play)
        Interface_LoadItemIconImpl(play, 0);
#endif

    return 0;
}

static void addBombBagRawOot(u8 index)
{
    if (index > gOotSave.info.inventory.upgrades.bombBag)
        gOotSave.info.inventory.upgrades.bombBag = index;
    addBombsRawOot(kMaxBombs[index]);
}

static void addBombBagRawMm(u8 index)
{
    if (index > gMmSave.info.inventory.upgrades.bombBag)
        gMmSave.info.inventory.upgrades.bombBag = index;
    addBombsRawMm(kMaxBombs[index]);
}

static int addItemBombBagOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBombBagRawOot(param);
    if (Config_Flag(CFG_SHARED_BOMB_BAGS))
        addBombBagRawMm(param);
    return 0;
}

static int addItemBombBagMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBombBagRawMm(param);
    if (Config_Flag(CFG_SHARED_BOMB_BAGS))
        addBombBagRawOot(param);
    return 0;
}

static int addItemShieldOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    u8 shieldType;
    u8 isProgressive;
    u8 mask;

    shieldType = (param & 0xff);
    isProgressive = !!((param >> 8) & 0xff);
    mask = 1 << (shieldType - 1);
    gOotSave.info.inventory.equipment.shields |= mask;
    if (isProgressive)
        gOotExtraItems.shield |= mask;
    return 0;
}

static int addItemShieldMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    u8 shieldType;
    u8 isProgressive;

    shieldType = (param & 0xff);
    isProgressive = !!((param >> 8) & 0xff);
    if (shieldType > gMmSave.info.itemEquips.shield)
        gMmSave.info.itemEquips.shield = shieldType;
    if (isProgressive)
        gMmExtraFlags2.progressiveShield = 1;

#if defined(GAME_MM)
    if (play)
        UpdateEquipment(play, GET_PLAYER(play));
#endif

    return 0;
}

static int addItemTunic(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.info.inventory.equipment.tunics |= (1 << (param - 1));
    return 0;
}

static int addItemBoots(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.info.inventory.equipment.boots |= (1 << (param - 1));
    return 0;
}

static int addItemStrengthOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (param > gOotSave.info.inventory.upgrades.strength)
        gOotSave.info.inventory.upgrades.strength = param;
    return 0;
}

static int addItemStrengthMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (param > gMmSave.info.inventory.upgrades.strength)
        gMmSave.info.inventory.upgrades.strength = param;
    return 0;
}

static int addItemScaleOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (param > gOotSave.info.inventory.upgrades.dive)
        gOotSave.info.inventory.upgrades.dive = param;
    return 0;
}

static int addItemScaleMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (param > gMmSave.info.inventory.upgrades.scale)
        gMmSave.info.inventory.upgrades.scale = param;
    return 0;
}

static int addItemQuestOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.info.inventory.quest.value |= (1 << param);
    if (param == QUEST_OOT_SONG_EPONA)
        BITMAP16_SET(gOotSave.info.eventsChk, EV_OOT_CHK_EPONA);
    return 0;
}

static int addItemQuestMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gMmSave.info.inventory.quest.value |= (1 << param);
    return 0;
}

static void addHealthEffect(u8 count)
{
    gSaveContext.healthDelta += count * 0x10;
}

static void addHealthRawOot(u8 count)
{
    gOotSave.info.playerData.health += count * 0x10;
    if (gOotSave.info.playerData.health > gOotSave.info.playerData.healthCapacity)
        gOotSave.info.playerData.health = gOotSave.info.playerData.healthCapacity;
}

static void addHealthRawMm(u8 count)
{
    gMmSave.info.playerData.health += count * 0x10;
    if (gMmSave.info.playerData.health > gMmSave.info.playerData.healthCapacity)
        gMmSave.info.playerData.health = gMmSave.info.playerData.healthCapacity;
}

static void addHealthOot(PlayState* play, u8 count)
{
#if defined(GAME_MM)
    if (!Config_Flag(CFG_SHARED_HEALTH))
        play = NULL;
#endif

    if (play)
    {
        addHealthEffect(count);
        return;
    }

    if (Config_Flag(CFG_SHARED_HEALTH))
        addHealthRawMm(count);
    addHealthRawOot(count);
}

static void addHealthMm(PlayState* play, u8 count)
{
#if defined(GAME_OOT)
    if (!Config_Flag(CFG_SHARED_HEALTH))
        play = NULL;
#endif

    if (play)
    {
        addHealthEffect(count);
        return;
    }

    if (Config_Flag(CFG_SHARED_HEALTH))
        addHealthRawOot(count);
    addHealthRawMm(count);
}

static int addItemHeartOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addHealthOot(play, param);
    return 0;
}

static int addItemHeartMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addHealthMm(play, param);
    return 0;
}

static void addDefenseUpgradeRawOot(void)
{
    gOotSave.info.playerData.doubleDefense = 1;
    gOotSave.info.inventory.doubleDefenseHearts = 20;
}

static void addDefenseUpgradeRawMm(void)
{
    gMmSave.info.playerData.doubleDefense = 1;
    gMmSave.info.inventory.defenseHearts = 20;
}

static int addItemDefenseUpgradeOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addDefenseUpgradeRawOot();
    if (Config_Flag(CFG_SHARED_HEALTH))
        addDefenseUpgradeRawMm();
    addHealthOot(play, 20);
    return 0;
}

static int addItemDefenseUpgradeMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addDefenseUpgradeRawMm();
    if (Config_Flag(CFG_SHARED_HEALTH))
        addDefenseUpgradeRawOot();
    addHealthMm(play, 20);
    return 0;
}

static void addHeartPieceRawOot(void)
{
    gOotSave.info.inventory.quest.heartPieces++;
    if (gOotSave.info.inventory.quest.heartPieces >= 4)
    {
        gOotSave.info.inventory.quest.heartPieces -= 4;
        gOotSave.info.playerData.healthCapacity += 0x10;
    }
}

static void addHeartPieceRawMm(void)
{
    gMmSave.info.inventory.quest.heartPieces++;
    if (gMmSave.info.inventory.quest.heartPieces >= 4)
    {
        gMmSave.info.inventory.quest.heartPieces -= 4;
        gMmSave.info.playerData.healthCapacity += 0x10;
    }
}

static int addItemHeartPieceOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < param; ++i)
    {
        addHeartPieceRawOot();
        if (Config_Flag(CFG_SHARED_HEALTH))
            addHeartPieceRawMm();
    }
    addHealthOot(play, 20);
    return param > 1 ? 0: gOotSave.info.inventory.quest.heartPieces;
}

static int addItemHeartPieceMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < param; ++i)
    {
        addHeartPieceRawMm();
        if (Config_Flag(CFG_SHARED_HEALTH))
            addHeartPieceRawOot();
    }
    addHealthMm(play, 20);
    return param > 1 ? 0: gMmSave.info.inventory.quest.heartPieces;
}

static int addSmallKeyOot(u16 dungeonId)
{
    s8 keyCount;

    /* Check for max keys */
    if ((dungeonId != SCE_OOT_TREASURE_SHOP || Config_Flag(CFG_OOT_CHEST_GAME_SHUFFLE)) && gOotSave.info.inventory.dungeonItems[dungeonId].maxKeys >= g.maxKeysOot[dungeonId])
        return 0;

    keyCount = gOotSave.info.inventory.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gOotSave.info.inventory.dungeonKeys[dungeonId] = keyCount;
    if (dungeonId == SCE_OOT_TREASURE_SHOP && !Config_Flag(CFG_OOT_CHEST_GAME_SHUFFLE))
        return 0;
    else
        return ++gOotSave.info.inventory.dungeonItems[dungeonId].maxKeys;
}

int addSmallKeyMm(u16 dungeonId)
{
    s8 keyCount;

    /* Max keys */
    if (gMmSave.info.inventory.dungeonItems[dungeonId].maxKeys >= g.maxKeysMm[dungeonId])
        return 0;

    keyCount = gMmSave.info.inventory.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gMmSave.info.inventory.dungeonKeys[dungeonId] = keyCount;
    gMmSave.info.inventory.dungeonItems[dungeonId].maxKeys++;

    return gMmSave.info.inventory.dungeonItems[dungeonId].maxKeys;
}

static int addItemSmallKeyOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 0);
#endif
    return addSmallKeyOot(param);
}

static int addItemSmallKeyMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif
    return addSmallKeyMm(param);
}

static int addItemKeyRingOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 0);
#endif
    for (int i = 0; i < g.maxKeysOot[param]; ++i)
        addSmallKeyOot(param);
    return 0;
}

static int addItemKeyRingMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif
    for (int i = 0; i < g.maxKeysMm[param]; ++i)
        addSmallKeyMm(param);
    return 0;
}

static int addItemSkeletonKeyOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    for (int j = 0; j < ARRAY_COUNT(g.maxKeysOot); ++j)
    {
        for (int i = 0; i < g.maxKeysOot[j]; ++i)
            addSmallKeyOot(j);
    }
    return 0;
}

static int addItemSkeletonKeyMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    for (int j = 0; j < ARRAY_COUNT(g.maxKeysMm); ++j)
    {
        for (int i = 0; i < g.maxKeysMm[j]; ++i)
            addSmallKeyMm(j);
    }
    return 0;
}

static int addItemBossKeyOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 1);
#endif

    gOotSave.info.inventory.dungeonItems[param].bossKey = 1;
    return 0;
}

static int addItemBossKeyMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif

    gMmSave.info.inventory.dungeonItems[param].bossKey = 1;
    return 0;
}

static int addItemCompassOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 0);
#endif

    gOotSave.info.inventory.dungeonItems[param].compass = 1;
    return 0;
}

static int addItemCompassMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif

    gMmSave.info.inventory.dungeonItems[param].compass = 1;
    return 0;
}

static int addItemMapOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (param == 0xffff)
        param = comboOotDungeonScene(play, 0);
#endif

    gOotSave.info.inventory.dungeonItems[param].map = 1;
    return 0;
}

static int addItemMapMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (param == 0xffff)
        param = gSaveContext.dungeonId;
#endif

    gMmSave.info.inventory.dungeonItems[param].map = 1;
    return 0;
}

static int addSilverRupee(PlayState* play, int id)
{
    addRupeesOot(play, 5);
    return comboSilverRupeesIncCount(play, id);
}

static void addSilverPouch(PlayState* play, int id)
{
    int max;

    max = gSilverRupeeData[id].count;
    for (int i = 0; i < max; ++i)
        addSilverRupee(play, id);
}

static int addItemSilverRupee(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    return addSilverRupee(play, param);
}

static int addItemSilverPouch(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addSilverPouch(play, param);
    return 0;
}

static int addItemMagicalRupee(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    for (int i = 0; i < ARRAY_COUNT(gSilverRupeeData); ++i)
        addSilverPouch(play, i);
    return 0;
}

static int addItemGsToken(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.info.inventory.quest.goldToken = 1;
    return ++gOotSave.info.inventory.goldTokens;
}

static int addItemGsTokenSwamp(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    return ++gMmSave.info.skullCountSwamp;
}

static int addItemGsTokenOcean(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    return ++gMmSave.info.skullCountOcean;
}

static void fillMagicOot(PlayState* play)
{
    int level;
    int max;

    if (!gOotSave.info.playerData.isMagicAcquired)
        return;
    level = gOotSave.info.playerData.isDoubleMagicAcquired ? 2 : 1;
    max = level * 0x30;

    /* Handle the effect */
#if defined(GAME_OOT)
    if (play)
    {
        gOotSave.info.playerData.magicLevel = 0;
        gSaveContext.magicFillTarget = max;
        return;
    }
#endif

    /* No effect - add the magic directly */
    gOotSave.info.playerData.magicLevel = level;
    gOotSave.info.playerData.magic = max;
}

static void fillMagicMm(PlayState* play)
{
    int level;
    int max;

    if (!gMmSave.info.playerData.isMagicAcquired)
        return;
    level = gMmSave.info.playerData.isDoubleMagicAcquired ? 2 : 1;
    max = level * 0x30;

    /* Handle the effect */
#if defined(GAME_MM)
    if (play)
    {
        gMmSave.info.playerData.magicLevel = 0;
        gMmSave.info.playerData.magic = max;
        gSaveContext.magicFillTarget = max;
        return;
    }
#endif

    /* No effect - add the magic directly */
    gMmSave.info.playerData.magicLevel = level;
    gMmSave.info.playerData.magic = max;
}

static int addItemMagicUpgradeOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gOotSave.info.playerData.isMagicAcquired = 1;
    if (param >= 2)
        gOotSave.info.playerData.isDoubleMagicAcquired = 1;
    fillMagicOot(play);
    return 0;
}

static int addItemMagicUpgradeMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gMmSave.info.playerData.isMagicAcquired = 1;
    if (param >= 2)
        gMmSave.info.playerData.isDoubleMagicAcquired = 1;
    fillMagicMm(play);
    return 0;
}

static int addItemStrayFairy(PlayState* play, u8 itemId, s16 gi, u16 param)
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
        gMmSave.info.inventory.strayFairies[param]++;
        return gMmSave.info.inventory.strayFairies[param];
    }
}

static int addItemTriforce(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    return ++gTriforceCount;
}

static int addItemCoin(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    return ++gSharedCustomSave.coins[param];
}

static int addItemButtonOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gSharedCustomSave.ocarinaButtonMaskOot |= kButtonMasks[param];
    return 0;
}

static int addItemButtonMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gSharedCustomSave.ocarinaButtonMaskMm |= kButtonMasks[param];
    return 0;
}

static int addItemKeg(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addAmmoMm(ITS_MM_KEG, ITEM_MM_POWDER_KEG, 1, param);
    return 0;
}

static int addItemSpinUpgradeMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    MM_SET_EVENT_WEEK(EV_MM_WEEK_SPIN_UPGRADE);
    return 0;
}

static int addItemSpinUpgradeOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gOotExtraFlags.spinUpgrade = 1;
    return 0;
}

static int addItemSoulOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    comboAddSoulOot(gi);
    return 0;
}

static int addItemSoulMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    comboAddSoulMm(gi);
    return 0;
}

static int addItemPondFish(PlayState* play, u8 itemId, s16 gi, u16 param)
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
        caughtListCapacity = (ARRAY_COUNT(gSharedCustomSave.caughtAdultFishWeight) - 1);
    }
    else
    {
        caughtList = gSharedCustomSave.caughtChildFishWeight;
        caughtListCapacity = (ARRAY_COUNT(gSharedCustomSave.caughtChildFishWeight) - 1);
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

static int addItemWorldMap(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    Inventory_SetWorldMapCloudVisibility(param);
    return 0;
}

static int addItemOwl(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gMmOwlFlags |= (1 << param);
    return 0;
}

static void addMagicRawOot(u8 count)
{
    s16 acc;
    s16 max;

    if (!gOotSave.info.playerData.isMagicAcquired)
        return;
    acc = gOotSave.info.playerData.magic;
    acc += count;
    max = gOotSave.info.playerData.isDoubleMagicAcquired ? 0x60 : 0x30;
    if (acc > max)
        acc = max;
    gOotSave.info.playerData.magic = acc;
}

static void addMagicRawMm(u8 count)
{
    s16 acc;
    s16 max;

    if (!gMmSave.info.playerData.isMagicAcquired)
        return;
    acc = gMmSave.info.playerData.magic;
    acc += count;
    max = gMmSave.info.playerData.isDoubleMagicAcquired ? 0x60 : 0x30;
    if (acc > max)
        acc = max;
    gMmSave.info.playerData.magic = acc;
}

static void addMagicEffect(PlayState* play, u8 count)
{
#if defined(GAME_OOT)
    Magic_Refill(play);
    Magic_RequestChange(play, count, 5);
#else
    AddMagic(play, count);
#endif
}

static void addMagicOot(PlayState* play, u8 count)
{
#if defined(GAME_MM)
    if (!Config_Flag(CFG_SHARED_MAGIC))
        play = NULL;
#endif

    if (play)
    {
        addMagicEffect(play, count);
        return;
    }

    if (Config_Flag(CFG_SHARED_MAGIC))
        addMagicRawMm(count);
    addMagicRawOot(count);
}

static void addMagicMm(PlayState* play, u8 count)
{
#if defined(GAME_OOT)
    if (!Config_Flag(CFG_SHARED_MAGIC))
        play = NULL;
#endif

    if (play)
    {
        addMagicEffect(play, count);
        return;
    }

    if (Config_Flag(CFG_SHARED_MAGIC))
        addMagicRawOot(count);
    addMagicRawMm(count);
}

static int addItemMagicOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addMagicOot(play, param * (Config_Flag(CFG_SHARED_MAGIC) ? 0x18 : 0x0c));
    return 0;
}

static int addItemMagicMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addMagicMm(play, param * 0x18);
    return 0;
}

static void addBombchuBagRawOot(void)
{
    gOotSave.info.inventory.items[ITS_OOT_BOMBCHU] = ITEM_OOT_BOMBCHU_10;
}

static void addBombchuBagRawMm(void)
{
    gMmSave.info.inventory.items[ITS_MM_BOMBCHU] = ITEM_MM_BOMBCHU;
}

static void addBombchuBagOot(u8 count)
{
    addBombchuBagRawOot();
    if (Config_Flag(CFG_SHARED_BOMBCHU))
        addBombchuBagRawMm();
    addBombchuOot(count);
}

static void addBombchuBagMm(u8 count)
{
    addBombchuBagRawMm();
    if (Config_Flag(CFG_SHARED_BOMBCHU))
        addBombchuBagRawOot();
    addBombchuMm(count);
}

static int addItemBombchuBagOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBombchuBagOot(param);
    return 0;
}

static int addItemBombchuBagMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addBombchuBagMm(param);
    return 0;
}

static int addItemBigFairyOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addHealthOot(play, 8);
    addMagicOot(play, 0x60);
    return 0;
}

static int addItemBigFairyMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    addHealthMm(play, 8);
    addMagicMm(play, 0x60);
    return 0;
}

static int addItemClock(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gSharedCustomSave.mm.halfDays |= (1 << param);
    return 0;
}

static int addItemEndgame(PlayState* play, u8 itemId, s16 gi, u16 param)
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

    if (Config_IsGoal())
        gOotExtraFlags.endgameItemIsWin = 1;

    return 0;
}

static int addElegyOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gSharedCustomSave.oot.hasElegy = 1;
    return 0;
}

static int addItemSwordExtraOot(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    if (gSharedCustomSave.extraSwordsOot < param)
        gSharedCustomSave.extraSwordsOot = (u8)param;

#if defined(GAME_OOT)
    if (play)
        Interface_LoadItemIconImpl(play, 0);
#endif
    return 0;
}

static int addItemGFSHammer(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    itemId = kMmGFSHammer[param];
    if (gMmSave.info.inventory.items[ITS_MM_GREAT_FAIRY_SWORD] == ITEM_NONE)
        gMmSave.info.inventory.items[ITS_MM_GREAT_FAIRY_SWORD] = itemId;
    gMmExtraItems.hammerGFS |= (1 << (u8)param);
    return 0;
}

static int addItemStoneAgonyMm(PlayState* play, u8 itemId, s16 gi, u16 param)
{
    gMmExtraFlags3.stoneAgony = 1;
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
    addItemNutsUpgradeOot,
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
    addItemSticksUpgradeOot,
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
    addItemSpinUpgradeMm,
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
    addItemSwordExtraOot,
    addItemGFSHammer,
    addItemSticksUpgradeMm,
    addItemNutsUpgradeMm,
    addItemStoneAgonyMm,
    addItemSpinUpgradeOot,
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
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_SHOOTING_GALLERY_OWNER, GI_MM_SOUL_NPC_SHOOTING_GALLERY_OWNER },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_BAZAAR_SHOPKEEPER, GI_MM_SOUL_NPC_BAZAAR_SHOPKEEPER },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_GORON, GI_MM_SOUL_NPC_GORON },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_GORON_CHILD, GI_MM_SOUL_NPC_GORON_CHILD },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_BOMBCHU_SHOPKEEPER, GI_MM_SOUL_NPC_BOMBCHU_SHOPKEEPER },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_BOMBERS, GI_MM_SOUL_NPC_BOMBERS },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_CITIZEN, GI_MM_SOUL_NPC_CITIZEN },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_COMPOSER_BROS, GI_MM_SOUL_NPC_COMPOSER_BROS },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_DAMPE, GI_MM_SOUL_NPC_DAMPE },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_CHEST_GAME_OWNER, GI_MM_SOUL_NPC_CHEST_GAME_OWNER },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_GORON_SHOPKEEPER, GI_MM_SOUL_NPC_GORON_SHOPKEEPER },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_HONEY_DARLING, GI_MM_SOUL_NPC_HONEY_DARLING },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_RUTO, GI_MM_SOUL_NPC_RUTO },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_MEDIGORON, GI_MM_SOUL_NPC_MEDIGORON },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_BIGGORON, GI_MM_SOUL_NPC_BIGGORON },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_TALON, GI_MM_SOUL_NPC_TALON },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_ASTRONOMER, GI_MM_SOUL_NPC_ASTRONOMER },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_POE_COLLECTOR, GI_MM_SOUL_NPC_POE_COLLECTOR },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_BOMBCHU_BOWLING_LADY, GI_MM_SOUL_NPC_BOMBCHU_BOWLING_LADY },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_FISHING_POND_OWNER, GI_MM_SOUL_NPC_FISHING_POND_OWNER },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_ROOFTOP_MAN, GI_MM_SOUL_NPC_ROOFTOP_MAN },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_ZORA, GI_MM_SOUL_NPC_ZORA },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_ZORA_SHOPKEEPER, GI_MM_SOUL_NPC_ZORA_SHOPKEEPER },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_MALON, GI_MM_SOUL_NPC_MALON },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_BEAN_SALESMAN, GI_MM_SOUL_NPC_BEAN_SALESMAN },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_CARPENTERS, GI_MM_SOUL_NPC_CARPENTERS },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_ANJU, GI_MM_SOUL_NPC_ANJU },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_GURU_GURU, GI_MM_SOUL_NPC_GURU_GURU },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_SCIENTIST, GI_MM_SOUL_NPC_SCIENTIST },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_GORMAN, GI_MM_SOUL_NPC_GORMAN },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_GROG, GI_MM_SOUL_NPC_GROG },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_DOG_LADY, GI_MM_SOUL_NPC_DOG_LADY },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_CARPET_MAN, GI_MM_SOUL_NPC_CARPET_MAN },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_OLD_HAG, GI_MM_SOUL_NPC_OLD_HAG },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_BANKER, GI_MM_SOUL_NPC_BANKER },
    { CFG_SHARED_SOULS_MISC,  GI_OOT_SOUL_MISC_GS, GI_MM_SOUL_MISC_GS },
    { CFG_SHARED_SOULS_MISC,  GI_OOT_SOUL_MISC_BUSINESS_SCRUB, GI_MM_SOUL_MISC_BUSINESS_SCRUB },
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
    { CFG_SHARED_SWORDS, GI_OOT_SWORD_KOKIRI, GI_MM_SWORD_KOKIRI },
    { CFG_SHARED_SWORDS, GI_OOT_SWORD_RAZOR, GI_MM_SWORD_RAZOR },
    { CFG_SHARED_SWORDS, GI_OOT_SWORD_GILDED, GI_MM_SWORD_GILDED },
    { CFG_SHARED_SOULS_NPC, GI_OOT_SOUL_NPC_THIEVES, GI_MM_SOUL_NPC_THIEVES },
    { CFG_SHARED_SOULS_ENEMY, GI_OOT_SOUL_ENEMY_THIEVES, GI_MM_SOUL_ENEMY_THIEVES },
    { CFG_SHARED_HAMMER, GI_OOT_HAMMER, GI_MM_HAMMER },
    { CFG_SHARED_NUTS_STICKS, GI_OOT_STICK_UPGRADE, GI_MM_STICK_UPGRADE },
    { CFG_SHARED_NUTS_STICKS, GI_OOT_STICK_UPGRADE2, GI_MM_STICK_UPGRADE2 },
    { CFG_SHARED_NUTS_STICKS, GI_OOT_NUT_UPGRADE,  GI_MM_NUT_UPGRADE },
    { CFG_SHARED_NUTS_STICKS, GI_OOT_NUT_UPGRADE2, GI_MM_NUT_UPGRADE2 },
    { CFG_SHARED_STONE_OF_AGONY, GI_OOT_STONE_OF_AGONY, GI_MM_STONE_OF_AGONY },
    { CFG_SHARED_SPIN_UPGRADE, GI_OOT_SPIN_UPGRADE, GI_MM_SPIN_UPGRADE },
};

static int addItem(PlayState* play, s16 gi)
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

int comboAddItemRaw(PlayState* play, s16 gi)
{
    const SharedItem* si;
    int count;
    s16 otherGi;

    count = addItem(play, gi);
    for (int i = 0; i < ARRAY_COUNT(kSimpleSharedItems); ++i)
    {
        si = &kSimpleSharedItems[i];
        if (si->gi == gi)
            otherGi = si->gi2;
        else if (si->gi2 == gi)
            otherGi = si->gi;
        else
            continue;
        if (Config_Flag(si->cfg))
            addItem(play, otherGi);
        break;
    }
    return count;
}
