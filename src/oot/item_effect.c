#include <combo.h>

static u16 dungeon(GameState_Play* play, int isBossKey)
{
    u16 mapIndex;

    /* Desert colossus hands */
    if (play->sceneId == SCE_OOT_DESERT_COLOSSUS)
        return SCE_OOT_TEMPLE_SPIRIT;

    mapIndex = gSaveContext.mapIndex;
    if (mapIndex == SCE_OOT_GANON_TOWER || mapIndex == SCE_OOT_INSIDE_GANON_CASTLE)
        return isBossKey ? SCE_OOT_GANON_TOWER : SCE_OOT_INSIDE_GANON_CASTLE;
    return mapIndex;
}

static void addHealth(u8 count)
{
    gSaveContext.healthDelta += count * 0x10;
}


static void addRupees(u16 count)
{
    gSaveContext.rupeesDelta += count;
}

static void addMagicUpgrade(int level)
{
    gOotSave.magicSize = 0;
    gSaveContext.magicTarget = 0x30 * level;
}

static void reloadIconsC(GameState_Play* play)
{
    for (int i = 1; i < 4; i++)
        Interface_LoadItemIconImpl(play, i);
}

int comboAddItemEffect(GameState_Play* play, s16 gi)
{
    int count;
    u16 dungeonId;

    count = 0;
    switch (gi)
    {
    case GI_OOT_RUPEE_GREEN:
    case GI_OOT_TC_RUPEE_GREEN:
        addRupees(1);
        break;
    case GI_OOT_RUPEE_BLUE:
    case GI_OOT_TC_RUPEE_BLUE:
        addRupees(5);
        break;
    case GI_OOT_RUPEE_RED:
    case GI_OOT_TC_RUPEE_RED:
        addRupees(20);
        break;
    case GI_OOT_RUPEE_PURPLE:
    case GI_OOT_TC_RUPEE_PURPLE:
        addRupees(50);
        break;
    case GI_OOT_RUPEE_HUGE:
        addRupees(200);
        break;
    case GI_OOT_MAGIC_UPGRADE:
        addMagicUpgrade(1);
        break;
    case GI_OOT_MAGIC_UPGRADE2:
        addMagicUpgrade(2);
        break;
    case GI_OOT_RECOVERY_HEART:
        addHealth(1);
        break;
    case GI_OOT_DEFENSE_UPGRADE:
    case GI_OOT_HEART_PIECE:
    case GI_OOT_TC_HEART_PIECE:
    case GI_OOT_HEART_CONTAINER:
    case GI_OOT_HEART_CONTAINER2:
        addHealth(20);
        break;
    case GI_OOT_SMALL_KEY:
    case GI_OOT_TC_SMALL_KEY:
        dungeonId = dungeon(play, 0);
        count = comboAddSmallKeyOot(dungeonId);
        break;
    case GI_OOT_MAP:
        dungeonId = dungeon(play, 0);
        comboAddMapOot(dungeonId);
        break;
    case GI_OOT_COMPASS:
        dungeonId = dungeon(play, 0);
        comboAddCompassOot(dungeonId);
        break;
    case GI_OOT_BOSS_KEY:
        dungeonId = dungeon(play, 1);
        comboAddBossKeyOot(dungeonId);
        break;
    case GI_OOT_HOOKSHOT:
    case GI_OOT_LONGSHOT:
    case GI_OOT_OCARINA_FAIRY:
    case GI_OOT_OCARINA_TIME:
    case GI_OOT_POTION_RED:
    case GI_OOT_POTION_BLUE:
    case GI_OOT_POTION_GREEN:
    case GI_OOT_MILK_BOTTLE:
    case GI_OOT_FISH:
    case GI_OOT_BLUE_FIRE:
    case GI_OOT_BUG:
    case GI_OOT_LON_LON_MILK:
        reloadIconsC(play);
        break;
    }

    return count;
}
