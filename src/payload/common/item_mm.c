#include <combo.h>

static void addUpgrade(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_MM_MAGIC_UPGRADE:
        gMmSave.playerData.magicAcquired = 1;
#if defined(GAME_MM)
        gSave.playerData.magicLevel = 0;
        gSave.playerData.magic = 0x30;
        gSaveContext.magicTarget = 0x30;
#else
        gMmSave.playerData.magicLevel = 1;
        gMmSave.playerData.magic = 0x30;
#endif
        break;
    case ITEM_MM_MAGIC_UPGRADE2:
        gMmSave.playerData.doubleMagic = 1;
#if defined(GAME_MM)
        gSave.playerData.magicLevel = 0;
        gSave.playerData.magic = 0x60;
        gSaveContext.magicTarget = 0x60;
#else
        gMmSave.playerData.magicLevel = 2;
        gMmSave.playerData.magic = 0x60;
#endif
        break;
    }
}

static void addInventory(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_MM_MASK_POSTMAN:
        gMmSave.inventory.items[ITS_MM_MASK_POSTMAN] = ITEM_MM_MASK_POSTMAN;
        break;
    case ITEM_MM_MASK_ALL_NIGHT:
        gMmSave.inventory.items[ITS_MM_MASK_ALL_NIGHT] = ITEM_MM_MASK_ALL_NIGHT;
        break;
    case ITEM_MM_MASK_BLAST:
        gMmSave.inventory.items[ITS_MM_MASK_BLAST] = ITEM_MM_MASK_BLAST;
        break;
    case ITEM_MM_MASK_STONE:
        gMmSave.inventory.items[ITS_MM_MASK_STONE] = ITEM_MM_MASK_STONE;
        break;
    case ITEM_MM_MASK_GREAT_FAIRY:
        gMmSave.inventory.items[ITS_MM_MASK_GREAT_FAIRY] = ITEM_MM_MASK_GREAT_FAIRY;
        break;
    case ITEM_MM_MASK_DEKU:
        gMmSave.inventory.items[ITS_MM_MASK_DEKU] = ITEM_MM_MASK_DEKU;
        break;
    case ITEM_MM_MASK_KEATON:
        gMmSave.inventory.items[ITS_MM_MASK_KEATON] = ITEM_MM_MASK_KEATON;
        break;
    case ITEM_MM_MASK_BREMEN:
        gMmSave.inventory.items[ITS_MM_MASK_BREMEN] = ITEM_MM_MASK_BREMEN;
        break;
    case ITEM_MM_MASK_BUNNY:
        gMmSave.inventory.items[ITS_MM_MASK_BUNNY] = ITEM_MM_MASK_BUNNY;
        break;
    case ITEM_MM_MASK_DON_GERO:
        gMmSave.inventory.items[ITS_MM_MASK_DON_GERO] = ITEM_MM_MASK_DON_GERO;
        break;
    case ITEM_MM_MASK_SCENTS:
        gMmSave.inventory.items[ITS_MM_MASK_SCENTS] = ITEM_MM_MASK_SCENTS;
        break;
    case ITEM_MM_MASK_GORON:
        gMmSave.inventory.items[ITS_MM_MASK_GORON] = ITEM_MM_MASK_GORON;
        break;
    case ITEM_MM_MASK_ROMANI:
        gMmSave.inventory.items[ITS_MM_MASK_ROMANI] = ITEM_MM_MASK_ROMANI;
        break;
    case ITEM_MM_MASK_CIRCUS:
        gMmSave.inventory.items[ITS_MM_MASK_CIRCUS] = ITEM_MM_MASK_CIRCUS;
        break;
    case ITEM_MM_MASK_KAFEI:
        gMmSave.inventory.items[ITS_MM_MASK_KAFEI] = ITEM_MM_MASK_KAFEI;
        break;
    case ITEM_MM_MASK_COUPLE:
        gMmSave.inventory.items[ITS_MM_MASK_COUPLE] = ITEM_MM_MASK_COUPLE;
        break;
    case ITEM_MM_MASK_TRUTH:
        gMmSave.inventory.items[ITS_MM_MASK_TRUTH] = ITEM_MM_MASK_TRUTH;
        break;
    case ITEM_MM_MASK_ZORA:
        gMmSave.inventory.items[ITS_MM_MASK_ZORA] = ITEM_MM_MASK_ZORA;
        break;
    case ITEM_MM_MASK_KAMARO:
        gMmSave.inventory.items[ITS_MM_MASK_KAMARO] = ITEM_MM_MASK_KAMARO;
        break;
    case ITEM_MM_MASK_GIBDO:
        gMmSave.inventory.items[ITS_MM_MASK_GIBDO] = ITEM_MM_MASK_GIBDO;
        break;
    case ITEM_MM_MASK_GARO:
        gMmSave.inventory.items[ITS_MM_MASK_GARO] = ITEM_MM_MASK_GARO;
        break;
    case ITEM_MM_MASK_CAPTAIN:
        gMmSave.inventory.items[ITS_MM_MASK_CAPTAIN] = ITEM_MM_MASK_CAPTAIN;
        break;
    case ITEM_MM_MASK_GIANT:
        gMmSave.inventory.items[ITS_MM_MASK_GIANT] = ITEM_MM_MASK_GIANT;
        break;
    case ITEM_MM_MASK_FIERCE_DEITY:
        gMmSave.inventory.items[ITS_MM_MASK_FIERCE_DEITY] = ITEM_MM_MASK_FIERCE_DEITY;
        break;
    default:
        break;
    }
}

void comboAddItemMm(u16 itemId)
{
    addInventory(itemId);
    addUpgrade(itemId);
}
