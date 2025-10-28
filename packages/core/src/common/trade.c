#include <combo.h>
#include <combo/item.h>
#include <combo/inventory.h>

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
    ITEM_OOT_CLAIM_CHECK,
    ITEM_OOT_BOTTLE_EMPTY,
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
    ITEM_OOT_MASK_BLAST,
    ITEM_OOT_MASK_STONE,
    ITEM_OOT_BOTTLE_EMPTY,
};

const u8 kOotOcarina[] = {
    ITEM_OOT_OCARINA_FAIRY,
    ITEM_OOT_OCARINA_TIME,
};

const u8 kOotHookshot[] = {
    ITEM_OOT_HOOKSHOT,
    ITEM_OOT_LONGSHOT,
};

const u8 kMmTrade1[] = {
    ITEM_MM_SPELL_FIRE,
    ITEM_MM_MOON_TEAR,
    ITEM_MM_DEED_LAND,
    ITEM_MM_DEED_SWAMP,
    ITEM_MM_DEED_MOUNTAIN,
    ITEM_MM_DEED_OCEAN,
};

const u8 kMmTrade2[] = {
    ITEM_MM_SPELL_WIND,
    ITEM_MM_BOOTS_IRON,
    ITEM_MM_TUNIC_GORON,
    ITEM_MM_ROOM_KEY,
    ITEM_MM_LETTER_TO_MAMA,
};

const u8 kMmTrade3[] = {
    ITEM_MM_SPELL_LOVE,
    ITEM_MM_BOOTS_HOVER,
    ITEM_MM_TUNIC_ZORA,
    ITEM_MM_LETTER_TO_KAFEI,
    ITEM_MM_PENDANT_OF_MEMORIES,
};

const u8 kMmHookshot[] = {
    ITEM_MM_HOOKSHOT_SHORT,
    ITEM_MM_HOOKSHOT,
};

const u8 kMmOcarina[] = {
    ITEM_MM_OCARINA_FAIRY,
    ITEM_MM_OCARINA_OF_TIME,
};

const u8 kMmGFSHammer[] = {
    ITEM_MM_GREAT_FAIRY_SWORD,
    ITEM_MM_HAMMER,
};

/* Return value is used to index gVertex */
s32 comboGetSlotExtras(u32 slot, u8** outItemPtr, u32* outFlags, const u8** outTable, u32* outTableSize)
{
    s32 result = -1;
    *outItemPtr = gSave.info.inventory.items + slot;
    switch (slot)
    {
#if defined(GAME_MM)
    case ITS_MM_TRADE1:
        *outFlags = gMmExtraTrade.trade1;
        *outTable = kMmTrade1;
        *outTableSize = ARRAY_COUNT(kMmTrade1);
        result = 0;
        break;
    case ITS_MM_TRADE2:
        *outFlags = gMmExtraTrade.trade2;
        *outTable = kMmTrade2;
        *outTableSize = ARRAY_COUNT(kMmTrade2);
        result = 1;
        break;
    case ITS_MM_TRADE3:
        *outFlags = gMmExtraTrade.trade3;
        *outTable = kMmTrade3;
        *outTableSize = ARRAY_COUNT(kMmTrade3);
        result = 2;
        break;
    case ITS_MM_HOOKSHOT:
        *outFlags = gMmExtraItems.hookshot;
        *outTable = kMmHookshot;
        *outTableSize = ARRAY_COUNT(kMmHookshot);
        result = 3;
        break;
    case ITS_MM_OCARINA:
        *outFlags = gMmExtraItems.ocarina;
        *outTable = kMmOcarina;
        *outTableSize = ARRAY_COUNT(kMmOcarina);
        result = 4;
        break;
    case ITS_MM_GREAT_FAIRY_SWORD:
        *outFlags = gMmExtraItems.hammerGFS;
        *outTable = kMmGFSHammer;
        *outTableSize = ARRAY_COUNT(kMmGFSHammer);
        result = 5;
        break;
#else
    case ITS_OOT_TRADE_ADULT:
        *outFlags = gOotExtraTrade.adult;
        *outTable = kOotTradeAdult;
        *outTableSize = ARRAY_COUNT(kOotTradeAdult);
        result = 0;
        break;
    case ITS_OOT_TRADE_CHILD:
        *outFlags = gOotExtraTrade.child;
        *outTable = kOotTradeChild;
        *outTableSize = ARRAY_COUNT(kOotTradeChild);
        result = 1;
        break;
    case ITS_OOT_HOOKSHOT:
        *outFlags = gOotExtraItems.hookshot;
        *outTable = kOotHookshot;
        *outTableSize = ARRAY_COUNT(kOotHookshot);
        result = 2;
        break;
    case ITS_OOT_OCARINA:
        *outFlags = gOotExtraItems.ocarina;
        *outTable = kOotOcarina;
        *outTableSize = ARRAY_COUNT(kOotOcarina);
        result = 3;
        break;
#endif
    default:
        return -1;
    }
    if (result >= 0)
    {
        s32 found = 0;
        u32 tableSize = *outTableSize;
        const u8* table = *outTable;
        u8 item = **outItemPtr;
#if defined (GAME_OOT)
        if ((slot == ITS_OOT_TRADE_ADULT || slot == ITS_OOT_TRADE_CHILD) && comboIsTradeBottleOot(item))
        {
            item = ITEM_OOT_BOTTLE_EMPTY;
        }
#endif
        for (int i = 0; i < tableSize; i++)
        {
            if (table[i] == item)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            return -1;
        }
    }
    return result;
}

void comboToggleSlot(u32 slot)
{
    u32 flags;
    const u8* table;
    u32 tableSize;
    u8* itemPtr;
    comboGetSlotExtras(slot, &itemPtr, &flags, &table, &tableSize);
    comboToggleTrade(itemPtr, flags, table, tableSize);
#if defined(GAME_MM)
    reloadSlotMm(gPlay, slot);
#else
    reloadSlotOot(gPlay, slot);
#endif
}

u8 comboGetNextTrade(u8 currentItem, u32 flags, const u8* table, u32 tableSize)
{
    int bitPos;
    int initialBitPos;
    /* We need to get the bit index of the current item */

#if defined (GAME_OOT)
    if (comboIsTradeBottleOot(currentItem))
    {
        currentItem = ITEM_OOT_BOTTLE_EMPTY;
    }
#endif

    bitPos = -1;
    for (u32 i = 0; i < tableSize; ++i)
    {
        if (currentItem == table[i])
        {
            bitPos = i;
            break;
        }
    }
    if (bitPos == -1)
    {
        return ITEM_NONE;
    }
    initialBitPos = bitPos;
    for (;;)
    {
        bitPos++;
        if (bitPos >= tableSize)
            bitPos = 0;
        if (flags & (1 << bitPos) || bitPos == initialBitPos)
        {
            u8 result = table[bitPos];
#if defined (GAME_OOT)
            if (comboIsTradeBottleOot(result))
            {
                result = table == kOotTradeAdult ? gOotExtraItems.bottleAdultSlot : gOotExtraItems.bottleChildSlot;
            }
#endif
            return result;
        }
    }
}

void comboToggleTrade(u8* slot, u32 flags, const u8* table, u32 tableSize)
{
#if defined (GAME_OOT)
    u8 oldItem = *slot;
    if (comboIsTradeBottleOot(oldItem))
    {
        if (table == kOotTradeAdult)
        {
            gOotExtraItems.bottleAdultSlot = oldItem;
        }
        else
        {
            gOotExtraItems.bottleChildSlot = oldItem;
        }
    }
#endif
    u8 newItem = comboGetNextTrade(*slot, flags, table, tableSize);
    if (newItem != ITEM_NONE)
        *slot = newItem;
}

s32 comboIsTradeBottleOot(u8 itemId)
{
    return (itemId >= ITEM_OOT_BOTTLE_EMPTY && itemId <= ITEM_OOT_POE) || (itemId >= ITEM_OOT_MAGIC_MUSHROOM && itemId <= ITEM_OOT_ZORA_EGG);
}
