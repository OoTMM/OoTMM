#include <combo.h>

void DisplayTextBox(void* play, s16 textId);

#if defined(GAME_OOT)
# define ITEM_BASE_MM 0x100
#else
# define ITEM_BASE_MM 0x0
#endif

static void addItemOot(u16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_BOMB_BAG:
        gOotSave.inventory[ITS_OOT_BOMBS] = ITEM_OOT_BOMB;
        gOotSave.ammo[ITS_OOT_BOMBS] = 20;
        gOotSave.upgrades.bombBag = 1;
        break;
    case ITEM_OOT_FAIRY_BOW:
        gOotSave.inventory[ITS_OOT_BOW] = ITEM_OOT_FAIRY_BOW;
        gOotSave.ammo[ITS_OOT_BOW] = 30;
        gOotSave.upgrades.quiver = 1;
        break;
    case ITEM_OOT_OCARINA_OF_TIME:
        gOotSave.inventory[ITS_OOT_OCARINA] = ITEM_OOT_OCARINA_OF_TIME;
        gComboSave.ootOcarinas |= 0x2;
        break;
    case ITEM_OOT_FAIRY_OCARINA:
        if (gOotSave.inventory[ITS_OOT_OCARINA] == ITEM_NONE)
            gOotSave.inventory[ITS_OOT_OCARINA] = ITEM_OOT_FAIRY_OCARINA;
        gComboSave.ootOcarinas |= 0x1;
        break;
    case ITEM_OOT_FAIRY_SLINGSHOT:
    case ITEM_OOT_BULLET_BAG:
        gOotSave.inventory[ITS_OOT_SLINGSHOT] = ITEM_OOT_FAIRY_SLINGSHOT;
        gOotSave.ammo[ITS_OOT_SLINGSHOT] = 30;
        gOotSave.upgrades.bulletBag = 1;
        break;
    /* Strength */
    case ITEM_OOT_GORON_BRACELET:
        gOotSave.upgrades.strength = 1;
        break;
    case ITEM_OOT_SILVER_GAUNTLETS:
        gOotSave.upgrades.strength = 2;
        break;
    case ITEM_OOT_GOLDEN_GAUNTLETS:
        gOotSave.upgrades.strength = 3;
        break;
    /* Dive */
    case ITEM_OOT_SILVER_SCALE:
        gOotSave.upgrades.dive = 1;
        break;
    case ITEM_OOT_GOLDEN_SCALE:
        gOotSave.upgrades.dive = 2;
        break;
    /* Bullet Bag */
    case ITEM_OOT_BULLET_BAG2:
        gOotSave.upgrades.bulletBag = 2;
        break;
    case ITEM_OOT_BULLET_BAG3:
        gOotSave.upgrades.bulletBag = 3;
        break;
    /* Quiver */
    case ITEM_OOT_QUIVER2:
        gOotSave.upgrades.quiver = 2;
        break;
    case ITEM_OOT_QUIVER3:
        gOotSave.upgrades.quiver = 3;
        break;
    /* Bomb Bag */
    case ITEM_OOT_BOMB_BAG2:
        gOotSave.upgrades.bombBag = 2;
        break;
    case ITEM_OOT_BOMB_BAG3:
        gOotSave.upgrades.bombBag = 3;
        break;
    }
}

static void addItemMm(u16 itemId)
{

}

int comboAddItem(void* play, u16 itemId)
{
    if ((itemId & 0x100) == ITEM_BASE_MM)
        addItemMm(itemId & 0xff);
    else
        addItemOot(itemId & 0xff);

    return -1;
}

int comboAddItemFromChest(void* play, s16 chestId)
{
    u16 itemId;
    u16 textId;

    chestId--;
    itemId = kExtendedGetItems[chestId].itemId;
    itemId |= (chestId & 0x100);

    comboTextHijackItem(play, itemId);
    return comboAddItem(play, itemId);
}
