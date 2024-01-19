#include <combo.h>

u32 gMultiMarkChests;
u32 gMultiMarkCollectibles;

static int mmSceneId(int sceneId)
{
    switch (sceneId)
    {
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        return SCE_MM_TEMPLE_STONE_TOWER;
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        return SCE_MM_SOUTHERN_SWAMP;
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
        return SCE_MM_MOUNTAIN_VILLAGE_WINTER;
    case SCE_MM_GORON_VILLAGE_SPRING:
        return SCE_MM_GORON_VILLAGE_WINTER;
    case SCE_MM_TWIN_ISLANDS_SPRING:
        return SCE_MM_TWIN_ISLANDS_WINTER;
    case SCE_MM_STONE_TOWER_INVERTED:
        return SCE_MM_STONE_TOWER;
    default:
        return sceneId;
    }
}

static int getChestMarkOot(GameState_Play* play, int sceneId, int flagId)
{
#if defined(GAME_OOT)
    if (play && play->sceneId == sceneId)
        return !!(gMultiMarkChests & (1 << flagId));
#endif

    return !!(gOotSave.perm[sceneId].chests & (1 << flagId));
}

static int getChestMarkMm(GameState_Play* play, int sceneId, int flagId)
{
    sceneId = mmSceneId(sceneId);

#if defined(GAME_MM)
    if (play && mmSceneId(play->sceneId) == sceneId)
        return !!(gMultiMarkChests & (1 << flagId));
    return !!(gSaveContext.cycleSceneFlags[sceneId].chest & (1 << flagId));
#endif

    return !!(gMmSave.permanentSceneFlags[sceneId].chest & (1 << flagId));
}

static void setChestMarkOot(GameState_Play* play, int sceneId, int flagId)
{
#if defined(GAME_OOT)
    if (play && play->sceneId == sceneId)
    {
        SetChestFlag(play, flagId);
        gMultiMarkChests |= (1 << flagId);
        return;
    }
#endif

    gOotSave.perm[sceneId].chests |= (1 << flagId);
}

static void setChestMarkMm(GameState_Play* play, int sceneId, int flagId)
{
    sceneId = mmSceneId(sceneId);

#if defined(GAME_MM)
    if (play && mmSceneId(play->sceneId) == sceneId)
    {
        SetChestFlag(play, flagId);
        gMultiMarkChests |= (1 << flagId);
        return;
    }
    gSaveContext.cycleSceneFlags[sceneId].chest |= (1 << flagId);
#endif

    gMmSave.permanentSceneFlags[sceneId].chest |= (1 << flagId);
}

static int getCollectibleMarkOot(GameState_Play* play, int sceneId, int flagId)
{
#if defined(GAME_OOT)
    if (play && play->sceneId == sceneId)
        return !!(gMultiMarkCollectibles & (1 << flagId));
#endif

    return !!(gOotSave.perm[sceneId].collectibles & (1 << flagId));
}

static int getCollectibleMarkMm(GameState_Play* play, int sceneId, int flagId)
{
    sceneId = mmSceneId(sceneId);

#if defined(GAME_MM)
    if (play && mmSceneId(play->sceneId) == sceneId)
        return !!(gMultiMarkCollectibles & (1 << flagId));
    return !!(gSaveContext.cycleSceneFlags[sceneId].collectible & (1 << flagId));
#endif

    return !!(gMmSave.permanentSceneFlags[sceneId].collectible & (1 << flagId));
}

static void setCollectibleMarkOot(GameState_Play* play, int sceneId, int flagId)
{
#if defined(GAME_OOT)
    if (play && play->sceneId == sceneId)
    {
        SetCollectibleFlag(play, flagId);
        gMultiMarkCollectibles |= (1 << flagId);
        return;
    }
#endif

    gOotSave.perm[sceneId].chests |= (1 << flagId);
}

static void setCollectibleMarkMm(GameState_Play* play, int sceneId, int flagId)
{
    sceneId = mmSceneId(sceneId);

#if defined(GAME_MM)
    if (play && mmSceneId(play->sceneId) == sceneId)
    {
        SetCollectibleFlag(play, flagId);
        gMultiMarkCollectibles |= (1 << flagId);
        return;
    }
    gSaveContext.cycleSceneFlags[sceneId].collectible |= (1 << flagId);
#endif

    gMmSave.permanentSceneFlags[sceneId].chest |= (1 << flagId);
}

void multiSetMarkedOot(GameState_Play* play, u8 ovType, u8 sceneId, u8 roomId, u8 id)
{
    if (!comboConfig(CFG_MULTIPLAYER))
        return;

    switch (ovType)
    {
    case OV_CHEST:
        setChestMarkOot(play, sceneId, id);
        break;
    case OV_COLLECTIBLE:
        setCollectibleMarkOot(play, sceneId, id);
        break;
    case OV_NPC:
        BITMAP8_SET(gSharedCustomSave.oot.npc, id);
        break;
    case OV_GS:
        BITMAP32_SET(gOotSave.gsFlags, id - 8);
        break;
    }
}

void multiSetMarkedMm(GameState_Play* play, u8 ovType, u8 sceneId, u8 roomId, u8 id)
{
    if (!comboConfig(CFG_MULTIPLAYER))
        return;

    switch (ovType)
    {
    case OV_CHEST:
        setChestMarkMm(play, sceneId, id);
        break;
    case OV_COLLECTIBLE:
        setCollectibleMarkMm(play, sceneId, id);
        break;
    case OV_NPC:
        BITMAP8_SET(gSharedCustomSave.mm.npc, id);
        break;
    case OV_GS:
        break;
    }
}

int multiIsMarkedOot(GameState_Play* play, u8 ovType, u8 sceneId, u8 roomId, u8 id)
{
    if (!comboConfig(CFG_MULTIPLAYER))
        return 0;

    switch (ovType)
    {
    case OV_CHEST:
        return getChestMarkOot(play, sceneId, id);
    case OV_COLLECTIBLE:
        return getCollectibleMarkOot(play, sceneId, id);
    case OV_NPC:
        return BITMAP8_GET(gSharedCustomSave.oot.npc, id);
    case OV_GS:
        return BITMAP32_GET(gOotSave.gsFlags, id - 8);
    }

    return 0;
}

int multiIsMarkedMm(GameState_Play* play, u8 ovType, u8 sceneId, u8 roomId, u8 id)
{
    if (!comboConfig(CFG_MULTIPLAYER))
        return 0;

    switch (ovType)
    {
    case OV_CHEST:
        return getChestMarkMm(play, sceneId, id);
    case OV_COLLECTIBLE:
        return getCollectibleMarkMm(play, sceneId, id);
    case OV_NPC:
        return BITMAP8_GET(gSharedCustomSave.mm.npc, id);
    case OV_GS:
        break;
    }

    return 0;
}
