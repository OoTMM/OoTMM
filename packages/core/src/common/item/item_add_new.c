#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define addRupees  addRupeesOot
#else
# define addRupees  addRupeesMm
#endif

#define IA_RUPEE    0x00
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

static void addRupeesOot(s16 delta)
{
    u16 max;

    max = gOotMaxRupees[gOotSave.inventory.upgrades.wallet];
    gOotSave.playerData.rupees += delta;
    if (gOotSave.playerData.rupees > max)
        gOotSave.playerData.rupees = max;
}

static void addRupeesMm(s16 delta)
{
    u16 max;

    max = gMmMaxRupees[gMmSave.inventory.upgrades.wallet];
    gMmSave.playerData.rupees += delta;
    if (gMmSave.playerData.rupees > max)
        gMmSave.playerData.rupees = max;
}

static int addItemRupeesOot(GameState_Play* play, s16 gi, u16 param)
{
#if defined(GAME_MM)
    if (!comboConfig(CFG_SHARED_WALLETS))
        play = NULL;
#endif

    if (play)
    {
        addRupeesEffect(param);
        return 0;
    }

    if (comboConfig(CFG_SHARED_WALLETS))
        addRupees(param);
    else
        addRupeesOot(param);
    return 0;
}

static int addItemRupeesMm(GameState_Play* play, s16 gi, u16 param)
{
#if defined(GAME_OOT)
    if (!comboConfig(CFG_SHARED_WALLETS))
        play = NULL;
#endif

    if (play)
    {
        addRupeesEffect(param);
        return 0;
    }

    if (comboConfig(CFG_SHARED_WALLETS))
        addRupees(param);
    else
        addRupeesMm(param);
    return 0;
}

static const AddItemHandler kAddItemHandlers[] = {
    { addItemRupeesOot, addItemRupeesMm },
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
    GameState_Play* p;
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
