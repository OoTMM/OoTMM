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

void comboAddItemEffect(GameState_Play* play, u16 itemId)
{
    u16 dungeonId;

    switch (itemId)
    {
    case ITEM_OOT_RUPEE_GREEN:
        addRupees(1);
        break;
    case ITEM_OOT_RUPEE_BLUE:
        addRupees(5);
        break;
    case ITEM_OOT_RUPEE_RED:
        addRupees(20);
        break;
    case ITEM_OOT_RUPEE_PURPLE:
        addRupees(50);
        break;
    case ITEM_OOT_RUPEE_HUGE:
        addRupees(200);
        break;
    case ITEM_OOT_MAGIC_UPGRADE:
        addMagicUpgrade(1);
        break;
    case ITEM_OOT_MAGIC_UPGRADE2:
        addMagicUpgrade(2);
        break;
    case ITEM_OOT_RECOVERY_HEART:
        addHealth(1);
        break;
    case ITEM_OOT_DEFENSE_UPGRADE:
    case ITEM_OOT_HEART_PIECE:
    case ITEM_OOT_HEART_PIECE2:
    case ITEM_OOT_HEART_CONTAINER:
        addHealth(20);
        break;
    case ITEM_OOT_SMALL_KEY:
        dungeonId = dungeon(play, 0);
        comboAddSmallKeyOot(dungeonId);
        break;
    case ITEM_OOT_MAP:
        dungeonId = dungeon(play, 0);
        comboAddMapOot(dungeonId);
        break;
    case ITEM_OOT_COMPASS:
        dungeonId = dungeon(play, 0);
        comboAddCompassOot(dungeonId);
        break;
    case ITEM_OOT_BOSS_KEY:
        dungeonId = dungeon(play, 1);
        comboAddBossKeyOot(dungeonId);
        break;
    }
}
