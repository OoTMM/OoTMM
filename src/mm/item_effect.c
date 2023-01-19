#include <combo.h>

static void addHealth(u8 count)
{
    gSaveContext.healthDelta += count * 0x10;
}

static void refillMagic(int level)
{
    gSave.playerData.magicLevel = 0;
    gSave.playerData.magic = level * 0x30;
    gSaveContext.magicTarget = level * 0x30;
}

static void addRupees(s16 count)
{
    gSaveContext.save.rupeesDelta += count;
}

static void reloadIconsC(GameState_Play* play)
{
    for (int i = 1; i < 4; i++)
        Interface_LoadItemIconImpl(play, i);
}

void comboAddItemEffect(GameState_Play* play, u16 itemId)
{
    switch (itemId)
    {
    case ITEM_MM_SWORD_KOKIRI:
    case ITEM_MM_SWORD_RAZOR:
    case ITEM_MM_SWORD_GILDED:
        Interface_LoadItemIconImpl(play, 0);
        break;
    case ITEM_MM_PROGRESSIVE_SHIELD_HERO:
    case ITEM_MM_SHIELD_HERO:
    case ITEM_MM_SHIELD_MIRROR:
        UpdateEquipment(play, GET_LINK(play));
        break;
    case ITEM_MM_MAGIC_UPGRADE:
        refillMagic(1);
        break;
    case ITEM_MM_MAGIC_UPGRADE2:
        refillMagic(2);
        break;
    case ITEM_MM_RECOVERY_HEART:
        addHealth(1);
        break;
    case ITEM_MM_HEART_PIECE:
    case ITEM_MM_HEART_PIECE2:
    case ITEM_MM_HEART_CONTAINER:
    case ITEM_MM_DEFENSE_UPGRADE:
        addHealth(20);
        break;
    case ITEM_MM_RUPEE_GREEN:
        addRupees(1);
        break;
    case ITEM_MM_RUPEE_BLUE:
        addRupees(5);
        break;
    case ITEM_MM_RUPEE_RED10:
        addRupees(10);
        break;
    case ITEM_MM_RUPEE_RED:
        addRupees(20);
        break;
    case ITEM_MM_RUPEE_PURPLE:
        addRupees(50);
        break;
    case ITEM_MM_RUPEE_SILVER:
        addRupees(100);
        break;
    case ITEM_MM_RUPEE_GOLD:
        addRupees(200);
        break;
    case ITEM_MM_STRAY_FAIRY:
        if (play->sceneId == SCE_MM_LAUNDRY_POOL || play->sceneId == SCE_MM_CLOCK_TOWN_EAST)
            comboAddStrayFairyMm(4);
        else
            comboAddStrayFairyMm(gSaveContext.dungeonId);
        break;
    case ITEM_MM_SMALL_KEY:
        comboAddSmallKeyMm(gSaveContext.dungeonId);
        break;
    case ITEM_MM_BOSS_KEY:
        comboAddBossKeyMm(gSaveContext.dungeonId);
        break;
    case ITEM_MM_MAP:
        gSave.inventory.dungeonItems[gSaveContext.dungeonId].map = 1;
        break;
    case ITEM_MM_COMPASS:
        gSave.inventory.dungeonItems[gSaveContext.dungeonId].compass = 1;
        break;
    case ITEM_MM_POTION_RED:
    case ITEM_MM_POTION_BLUE:
    case ITEM_MM_POTION_GREEN:
    case ITEM_MM_BOTTLED_MILK_HALF:
    case ITEM_MM_SEAHORSE:
    case ITEM_MM_BOTTLED_SEAHORSE:
    case ITEM_MM_MILK:
    case ITEM_MM_CHATEAU_ROMANI:
        reloadIconsC(play);
        break;
    }
}
