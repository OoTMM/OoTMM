#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define addRupeesRaw  addRupeesRawOot
#else
# define addRupeesRaw  addRupeesRawMm
#endif

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
