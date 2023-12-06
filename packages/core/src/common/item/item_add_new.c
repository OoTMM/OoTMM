#include <combo.h>
#include <combo/item.h>

#define IA_RUPEE    0x00
#define IA_NONE     0xff

typedef int (*AddItemFunc)(GameState_Play* play, s16 gi, u16 param);
typedef struct
{
    AddItemFunc oot;
    AddItemFunc mm;
}
AddItemHandler;

static int addItemRupeesOot(GameState_Play* play, s16 gi, u16 param)
{
    u16 max;

#if defined(GAME_OOT)
    if (play)
    {
        gSaveContext.rupeesDelta += param;
        return 0;
    }
#endif

    max = gOotMaxRupees[gOotSave.inventory.upgrades.wallet];
    gOotSave.playerData.rupees += param;
    if (gOotSave.playerData.rupees > max)
        gOotSave.playerData.rupees = max;
    return 0;
}

static int addItemRupeesMm(GameState_Play* play, s16 gi, u16 param)
{
    u16 max;

#if defined(GAME_MM)
    if (play)
    {
        gSaveContext.save.rupeesDelta += param;
        return 0;
    }
#endif

    max = gMmMaxRupees[gMmSave.inventory.upgrades.wallet];
    gMmSave.playerData.rupees += param;
    if (gMmSave.playerData.rupees > max)
        gMmSave.playerData.rupees = max;
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
    int isForeign;
    int isMm;
    s16 legacyGi;
    u16 addParam;
    u8 addId;

    legacyGi = gi;
    isForeign = !!(gi & MASK_FOREIGN_GI);
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
        p = isForeign ? NULL : play;
        ret = func(p, gi, addParam);
    }

    return ret;
}
