#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define addRupeesRaw  addRupeesRawOot
#else
# define addRupeesRaw  addRupeesRawMm
#endif

#define IA_RUPEE    0x00
#define IA_WALLET   0x01
#define IA_NONE     0xff

typedef int (*AddItemFunc)(GameState_Play* play, s16 gi, u16 param);
typedef struct
{
    AddItemFunc oot;
    AddItemFunc mm;
}
AddItemHandler;

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

static int addItemRupeesOot(GameState_Play* play, s16 gi, u16 param)
{
    addRupeesOot(play, param);
    return 0;
}

static int addItemRupeesMm(GameState_Play* play, s16 gi, u16 param)
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

static int addItemWalletOot(GameState_Play* play, s16 gi, u16 param)
{
    if (comboConfig(CFG_SHARED_WALLETS))
        addWalletRawShared(param);
    else
        addWalletRawOot(param);

    if (comboConfig(CFG_FILL_WALLETS))
        addRupeesOot(play, gOotMaxRupees[gOotSave.inventory.upgrades.wallet]);

    return 0;
}

static int addItemWalletMm(GameState_Play* play, s16 gi, u16 param)
{
    if (comboConfig(CFG_SHARED_WALLETS))
        addWalletRawShared(param);
    else
        addWalletRawMm(param);

    if (comboConfig(CFG_FILL_WALLETS))
        addRupeesMm(play, gMmMaxRupees[gMmSave.inventory.upgrades.wallet]);

    return 0;
}

static const AddItemHandler kAddItemHandlers[] = {
    { addItemRupeesOot, addItemRupeesMm },
    { addItemWalletOot, addItemWalletMm },
};

#define X(a, b, c, drawGiParam, addItemId, addItemParam, d, e, text) addItemId
static const u8 kAddItemIdsOot[] = {
#include "../data/oot/gi.inc"
};
static const u8 kAddItemIdsMm[] = {
#include "../data/mm/gi.inc"
};
#undef X

#define X(a, b, c, drawGiParam, addItemId, addItemParam, d, e, text) addItemParam
static const u16 kAddItemParamsOot[] = {
#include "../data/oot/gi.inc"
};
static const u16 kAddItemParamsMm[] = {
#include "../data/mm/gi.inc"
};
#undef X

int comboAddItem(GameState_Play* play, s16 gi)
{
    AddItemFunc func;
    int ret;
    int isMm;
    s16 legacyGi;
    u16 addParam;
    u8 addId;

    legacyGi = gi;
#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif
    isMm = !!(gi & MASK_FOREIGN_GI);
    gi &= ~MASK_FOREIGN_GI;
    addId = isMm ? kAddItemIdsMm[gi - 1] : kAddItemIdsOot[gi - 1];
    addParam = isMm ? kAddItemParamsMm[gi - 1] : kAddItemParamsOot[gi - 1];

    if (addId == IA_NONE)
    {
        /* Legacy item add */
        if (play == NULL)
            ret = comboAddItemLegacyNoEffect(legacyGi);
        else
            ret = comboAddItemLegacy(play, legacyGi);
    }
    else
    {
        /* New item handlers */
        func = isMm ? kAddItemHandlers[addId].mm : kAddItemHandlers[addId].oot;
        ret = func(play, gi, addParam);
    }

    return ret;
}
