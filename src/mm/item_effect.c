#include <combo.h>

static void addHealth(u8 count)
{
    gSaveContext.healthDelta += count * 0x10;
}

static void refillMagic(int level)
{
    gSave.playerData.magicLevel = 0;
    gSave.playerData.magicAmount = level * 0x30;
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

void comboAddItemSharedForeignEffect(GameState_Play* play, s16 gi)
{

}

int comboAddItemEffect(GameState_Play* play, s16 gi)
{
    int count;

    count = 0;
    switch (gi)
    {
    case GI_MM_SWORD_KOKIRI:
    case GI_MM_SWORD_RAZOR:
    case GI_MM_SWORD_GILDED:
        Interface_LoadItemIconImpl(play, 0);
        break;
    case GI_MM_PROGRESSIVE_SHIELD_HERO:
    case GI_MM_SHIELD_HERO:
    case GI_MM_SHIELD_MIRROR:
        UpdateEquipment(play, GET_LINK(play));
        break;
    case GI_MM_MAGIC_UPGRADE:
        refillMagic(1);
        break;
    case GI_MM_MAGIC_UPGRADE2:
        refillMagic(2);
        break;
    case GI_MM_RECOVERY_HEART:
        addHealth(1);
        break;
    case GI_MM_HEART_PIECE:
    case GI_MM_HEART_CONTAINER:
    case GI_MM_DEFENSE_UPGRADE:
        addHealth(20);
        break;
    case GI_MM_RUPEE_GREEN:
        addRupees(1);
        break;
    case GI_MM_RUPEE_BLUE:
        addRupees(5);
        break;
    case GI_MM_RUPEE_RED:
        addRupees(20);
        break;
    case GI_MM_RUPEE_PURPLE:
        addRupees(50);
        break;
    case GI_MM_RUPEE_SILVER:
        addRupees(100);
        break;
    case GI_MM_RUPEE_GOLD:
        addRupees(200);
        break;
    case GI_MM_STRAY_FAIRY:
        if (play->sceneId == SCE_MM_LAUNDRY_POOL || play->sceneId == SCE_MM_CLOCK_TOWN_EAST)
            count = comboAddStrayFairyMm(4);
        else
            count = comboAddStrayFairyMm(gSaveContext.dungeonId);
        break;
    case GI_MM_SMALL_KEY:
        count = comboAddSmallKeyMm(gSaveContext.dungeonId);
        break;
    case GI_MM_BOSS_KEY:
        comboAddBossKeyMm(gSaveContext.dungeonId);
        break;
    case GI_MM_MAP:
        comboAddMapMm(gSaveContext.dungeonId);
        break;
    case GI_MM_COMPASS:
        comboAddCompassMm(gSaveContext.dungeonId);
        break;
    case GI_MM_POTION_RED:
    case GI_MM_POTION_BLUE:
    case GI_MM_POTION_GREEN:
    case GI_MM_SEAHORSE:
    case GI_MM_SEAHORSE2:
    case GI_MM_MILK:
    case GI_MM_CHATEAU_ROMANI:
        reloadIconsC(play);
        break;
    }

    return count;
}
