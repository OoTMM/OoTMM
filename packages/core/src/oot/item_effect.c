#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>

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

static void addSilverPouchRupees(int id)
{
    addRupees(gSilverRupeeData[id].count * 5);
}

static void addMagicalRupeeRupees(void)
{
    for (int i = 0; i < ARRAY_SIZE(gSilverRupeeData); i++)
        addSilverPouchRupees(i);
}

static void refillMagic(int level)
{
    gOotSave.playerData.magicSize = 0;
    gSaveContext.magicFillTarget = 0x30 * level;
}

void comboAddItemSharedForeignEffect(GameState_Play* play, s16 gi)
{
    if (comboConfig(CFG_SHARED_HEALTH))
    {
        switch (gi)
        {
        case GI_MM_HEART_CONTAINER:
        case GI_MM_HEART_PIECE:
        case GI_MM_DEFENSE_UPGRADE:
            addHealth(20);
            break;
        }
    }
}

int comboAddItemEffect(GameState_Play* play, s16 gi)
{
    int count;
    u16 dungeonId;

    count = 0;
    switch (gi)
    {
    case GI_OOT_RUPEE_SILVER_DC:
    case GI_OOT_RUPEE_SILVER_BOTW:
    case GI_OOT_RUPEE_SILVER_SPIRIT_CHILD:
    case GI_OOT_RUPEE_SILVER_SPIRIT_SUN:
    case GI_OOT_RUPEE_SILVER_SPIRIT_BOULDERS:
    case GI_OOT_RUPEE_SILVER_SPIRIT_LOBBY:
    case GI_OOT_RUPEE_SILVER_SPIRIT_ADULT:
    case GI_OOT_RUPEE_SILVER_SHADOW_SCYTHE:
    case GI_OOT_RUPEE_SILVER_SHADOW_PIT:
    case GI_OOT_RUPEE_SILVER_SHADOW_SPIKES:
    case GI_OOT_RUPEE_SILVER_SHADOW_BLADES:
    case GI_OOT_RUPEE_SILVER_IC_SCYTHE:
    case GI_OOT_RUPEE_SILVER_IC_BLOCK:
    case GI_OOT_RUPEE_SILVER_GTG_SLOPES:
    case GI_OOT_RUPEE_SILVER_GTG_LAVA:
    case GI_OOT_RUPEE_SILVER_GTG_WATER:
    case GI_OOT_RUPEE_SILVER_GANON_SPIRIT:
    case GI_OOT_RUPEE_SILVER_GANON_LIGHT:
    case GI_OOT_RUPEE_SILVER_GANON_FIRE:
    case GI_OOT_RUPEE_SILVER_GANON_FOREST:
    case GI_OOT_RUPEE_SILVER_GANON_SHADOW:
    case GI_OOT_RUPEE_SILVER_GANON_WATER:
        addRupees(5);
        break;
    case GI_OOT_POUCH_SILVER_DC:
        addSilverPouchRupees(SR_DC);
        break;
    case GI_OOT_POUCH_SILVER_BOTW:
        addSilverPouchRupees(SR_BOTW);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_CHILD:
        addSilverPouchRupees(SR_SPIRIT1);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_SUN:
        addSilverPouchRupees(SR_SPIRIT2);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_BOULDERS:
        addSilverPouchRupees(SR_SPIRIT3);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_LOBBY:
        addSilverPouchRupees(SR_SPIRIT1);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_ADULT:
        addSilverPouchRupees(SR_SPIRIT2);
        break;
    case GI_OOT_POUCH_SILVER_SHADOW_SCYTHE:
        addSilverPouchRupees(SR_SHADOW1);
        break;
    case GI_OOT_POUCH_SILVER_SHADOW_PIT:
        addSilverPouchRupees(SR_SHADOW3);
        break;
    case GI_OOT_POUCH_SILVER_SHADOW_SPIKES:
        addSilverPouchRupees(SR_SHADOW4);
        break;
    case GI_OOT_POUCH_SILVER_SHADOW_BLADES:
        addSilverPouchRupees(SR_SHADOW2);
        break;
    case GI_OOT_POUCH_SILVER_IC_SCYTHE:
        addSilverPouchRupees(SR_IC1);
        break;
    case GI_OOT_POUCH_SILVER_IC_BLOCK:
        addSilverPouchRupees(SR_IC2);
        break;
    case GI_OOT_POUCH_SILVER_GTG_SLOPES:
        addSilverPouchRupees(SR_GTG1);
        break;
    case GI_OOT_POUCH_SILVER_GTG_LAVA:
        addSilverPouchRupees(SR_GTG2);
        break;
    case GI_OOT_POUCH_SILVER_GTG_WATER:
        addSilverPouchRupees(SR_GTG3);
        break;
    case GI_OOT_POUCH_SILVER_GANON_SPIRIT:
        addSilverPouchRupees(SR_GANON1);
        break;
    case GI_OOT_POUCH_SILVER_GANON_LIGHT:
        addSilverPouchRupees(SR_GANON2);
        break;
    case GI_OOT_POUCH_SILVER_GANON_FIRE:
        addSilverPouchRupees(SR_GANON3);
        break;
    case GI_OOT_POUCH_SILVER_GANON_FOREST:
        addSilverPouchRupees(SR_GANON4);
        break;
    case GI_OOT_POUCH_SILVER_GANON_SHADOW:
        addSilverPouchRupees(SR_GANON1);
        break;
    case GI_OOT_POUCH_SILVER_GANON_WATER:
        addSilverPouchRupees(SR_GANON2);
        break;
    case GI_OOT_RUPEE_MAGICAL:
        addMagicalRupeeRupees();
        break;
    case GI_OOT_MAGIC_UPGRADE:
        refillMagic(1);
        break;
    case GI_OOT_MAGIC_UPGRADE2:
        refillMagic(2);
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
    case GI_OOT_KEY_RING:
        dungeonId = dungeon(play, 0);
        comboAddKeyRingOot(dungeonId);
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
    }

    return count;
}
