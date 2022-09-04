#include <combo.h>

static const u16 kSwords[] = {
    ITEM_NONE,
    ITEM_MM_SWORD_KOKIRI,
    ITEM_MM_SWORD_RAZOR,
    ITEM_MM_SWORD_GILDED,
};

static void addSword(int index)
{
    gMmSave.itemEquips.buttonItems[0][0] = kSwords[index];
    gMmSave.itemEquips.sword = index;
}

static void addShield(int index)
{
    gMmSave.itemEquips.shield = index;
}

static void addBombBag(int index)
{
    gMmSave.inventory.items[ITS_MM_BOMBS] = ITEM_MM_BOMB;
    gMmSave.inventory.upgrades.bombBag = index;
    gMmSave.inventory.ammo[ITS_MM_BOMBS] = kMaxBombs[index];
}

static void addBowQuiver(int index)
{
    gMmSave.inventory.items[ITS_MM_BOW] = ITEM_MM_BOW;
    gMmSave.inventory.upgrades.quiver = index;
    gMmSave.inventory.ammo[ITS_MM_BOW] = kMaxArrows[index];
}

void comboAddItemMm(u16 itemId)
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
    case ITEM_MM_SWORD_KOKIRI:
        addSword(1);
        break;
    case ITEM_MM_SWORD_RAZOR:
        addSword(2);
        break;
    case ITEM_MM_SWORD_GILDED:
        addSword(3);
        break;
    case ITEM_MM_SHIELD_HERO:
        addShield(1);
        break;
    case ITEM_MM_SHIELD_MIRROR:
        addShield(2);
        break;
    case ITEM_MM_BOMB_BAG:
        addBombBag(1);
        break;
    case ITEM_MM_BOMB_BAG2:
        addBombBag(2);
        break;
    case ITEM_MM_BOMB_BAG3:
        addBombBag(3);
        break;
    case ITEM_MM_BOW:
    case ITEM_MM_QUIVER:
        addBowQuiver(1);
        break;
    case ITEM_MM_QUIVER2:
        addBowQuiver(2);
        break;
    case ITEM_MM_QUIVER3:
        addBowQuiver(3);
        break;
    }
}
