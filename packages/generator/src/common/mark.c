#include <combo.h>
#include <combo/mark.h>
#include <combo/play.h>

/* TODO: This system is a big mess, reorganize this someday */

u32 gMarkChests;
u32 gMarkCollectibles;
u32 gMarkSwitch0;
u32 gMarkSwitch1;

static int Mark_NormalizeSceneIdOot(int sceneId)
{
    return Play_MergeMQ(sceneId);
}

static int Mark_NormalizeSceneIdMm(int sceneId)
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
    }

    return sceneId;
}

static int getChestMarkOot(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdOot(sceneId);

#if defined(GAME_OOT)
    if (play && Mark_NormalizeSceneIdOot(play->sceneId) == sceneId)
        return !!(gMarkChests & (1 << flagId));
#endif

    return !!(gOotSave.info.perm[sceneId].chests & (1 << flagId));
}

static int getChestMarkMm(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdMm(sceneId);

#if defined(GAME_MM)
    if (play && Mark_NormalizeSceneIdMm(play->sceneId) == sceneId)
        return !!(gMarkChests & (1 << flagId));
    if (gSaveContext.cycleSceneFlags[sceneId].chest & (1 << flagId))
        return 1;
#endif

    return !!(gMmSave.info.permanentSceneFlags[sceneId].chest & (1 << flagId));
}

static void setChestMarkOot(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdOot(sceneId);

#if defined(GAME_OOT)
    if (play && Mark_NormalizeSceneIdOot(play->sceneId) == sceneId)
    {
        SetChestFlag(play, flagId);
        gMarkChests |= (1 << flagId);
        return;
    }
#endif

    gOotSave.info.perm[sceneId].chests |= (1 << flagId);
}

static void setChestMarkMm(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdMm(sceneId);

#if defined(GAME_MM)
    if (play && Mark_NormalizeSceneIdMm(play->sceneId) == sceneId)
    {
        SetChestFlag(play, flagId);
        gMarkChests |= (1 << flagId);
        return;
    }
    gSaveContext.cycleSceneFlags[sceneId].chest |= (1 << flagId);
#endif

    gMmSave.info.permanentSceneFlags[sceneId].chest |= (1 << flagId);
}

static int getCollectibleMarkOot(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdOot(sceneId);

#if defined(GAME_OOT)
    if (play && Mark_NormalizeSceneIdOot(play->sceneId) == sceneId)
        return !!(gMarkCollectibles & (1 << flagId));
#endif

    return !!(gOotSave.info.perm[sceneId].collectibles & (1 << flagId));
}

static int getCollectibleMarkMm(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdMm(sceneId);

#if defined(GAME_MM)
    if (play && Mark_NormalizeSceneIdMm(play->sceneId) == sceneId)
        return !!(gMarkCollectibles & (1 << flagId));
    if (gSaveContext.cycleSceneFlags[sceneId].collectible & (1 << flagId))
        return 1;
#endif

    return !!(gMmSave.info.permanentSceneFlags[sceneId].collectible & (1 << flagId));
}

static void setCollectibleMarkOot(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdOot(sceneId);

#if defined(GAME_OOT)
    if (play && Mark_NormalizeSceneIdOot(play->sceneId) == sceneId)
    {
        Flags_SetCollectible(play, flagId);
        gMarkCollectibles |= (1 << flagId);
        return;
    }
#endif

    gOotSave.info.perm[sceneId].collectibles |= (1 << flagId);
}

static void setCollectibleMarkMm(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdMm(sceneId);

#if defined(GAME_MM)
    if (play && Mark_NormalizeSceneIdMm(play->sceneId) == sceneId)
    {
        Flags_SetCollectible(play, flagId);
        gMarkCollectibles |= (1 << flagId);
        return;
    }
    gSaveContext.cycleSceneFlags[sceneId].collectible |= (1 << flagId);
#endif

    gMmSave.info.permanentSceneFlags[sceneId].collectible |= (1 << flagId);
}

static int getSwitch0MarkMm(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdMm(sceneId);

#if defined(GAME_MM)
    if (play && Mark_NormalizeSceneIdMm(play->sceneId) == sceneId)
        return !!(gMarkSwitch0 & (1 << flagId));
    if (gSaveContext.cycleSceneFlags[sceneId].switch0 & (1 << flagId))
        return 1;
#endif

    return !!(gMmSave.info.permanentSceneFlags[sceneId].switch0 & (1 << flagId));
}

static int getSwitch1MarkMm(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdMm(sceneId);

#if defined(GAME_MM)
    if (play && Mark_NormalizeSceneIdMm(play->sceneId) == sceneId)
        return !!(gMarkSwitch1 & (1 << flagId));
    if (gSaveContext.cycleSceneFlags[sceneId].switch1 & (1 << flagId))
        return 1;
#endif

    return !!(gMmSave.info.permanentSceneFlags[sceneId].switch1 & (1 << flagId));
}

static int getSwitchMarkMm(PlayState* play, int sceneId, int flagId)
{
    if (flagId >= 0x20)
        return getSwitch1MarkMm(play, sceneId, flagId & 0x1f);
    else
        return getSwitch0MarkMm(play, sceneId, flagId & 0x1f);
}

static void setSwitch0MarkMm(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdMm(sceneId);

#if defined(GAME_MM)
    if (play && Mark_NormalizeSceneIdMm(play->sceneId) == sceneId)
    {
        Flags_SetSwitch(play, flagId);
        gMarkSwitch0 |= (1 << flagId);
        return;
    }
    gSaveContext.cycleSceneFlags[sceneId].switch0 |= (1 << flagId);
#endif

    gMmSave.info.permanentSceneFlags[sceneId].switch0 |= (1 << flagId);
}

static void setSwitch1MarkMm(PlayState* play, int sceneId, int flagId)
{
    sceneId = Mark_NormalizeSceneIdMm(sceneId);

#if defined(GAME_MM)
    if (play && Mark_NormalizeSceneIdMm(play->sceneId) == sceneId)
    {
        Flags_SetSwitch(play, flagId | 0x20);
        gMarkSwitch1 |= (1 << flagId);
        return;
    }
    gSaveContext.cycleSceneFlags[sceneId].switch1 |= (1 << flagId);
#endif

    gMmSave.info.permanentSceneFlags[sceneId].switch1 |= (1 << flagId);
}

static void setSwitchMarkMm(PlayState* play, int sceneId, int flagId)
{
    if (flagId >= 0x20)
        setSwitch1MarkMm(play, sceneId, flagId & 0x1f);
    else
        setSwitch0MarkMm(play, sceneId, flagId & 0x1f);
}

static int getStrayFairyMarkMm(PlayState* play, int sceneId, int id)
{
    if (id >= 0x20)
        return getSwitchMarkMm(play, sceneId, id);
    else
        return getCollectibleMarkMm(play, sceneId, id);
}

static void setStrayFairyMarkMm(PlayState* play, int sceneId, int id)
{
    if (id >= 0x20)
        setSwitchMarkMm(play, sceneId, id);
    else
        setCollectibleMarkMm(play, sceneId, id);
}

static void markXflag(Xflag* xf, int sliceId, int sceneId, int roomId, int id)
{
    bzero(xf, sizeof(*xf));
    xf->sliceId = sliceId;
    xf->setupId = (roomId & 0xc0) >> 6;
    xf->sceneId = sceneId;
    xf->roomId = roomId & 0x3f;
    xf->id = id;
}

static int getXflagsMarkOot(PlayState* play, int sliceId, int sceneId, int roomId, int id)
{
    Xflag xf;

    markXflag(&xf, sliceId, sceneId, roomId, id);
    return comboXflagsGetOot(&xf);
}

static int getXflagsMarkMm(PlayState* play, int sliceId, int sceneId, int roomId, int id)
{
    Xflag xf;

    markXflag(&xf, sliceId, sceneId, roomId, id);
    return comboXflagsGetMm(&xf);
}

static void setXflagsMarkOot(PlayState* play, int sliceId, int sceneId, int roomId, int id)
{
    Xflag xf;

    markXflag(&xf, sliceId, sceneId, roomId, id);
    comboXflagsSetOot(&xf);
}

static void setXflagsMarkMm(PlayState* play, int sliceId, int sceneId, int roomId, int id)
{
    Xflag xf;

    markXflag(&xf, sliceId, sceneId, roomId, id);
    comboXflagsSetMm(&xf);
}

static int getGsFlagOot(int id)
{
    if (id > 0xb0)
        id -= 0xb0;
    return BITMAP32_GET(gOotSave.info.gsFlags, id);
}

static void setGsFlagOot(int id)
{
    if (id > 0xb0)
        id -= 0xb0;
    BITMAP32_SET(gOotSave.info.gsFlags, id);
}

void Mark_SetOot(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id)
{
    switch (ovType)
    {
    case OV_NONE:
        break;
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
        setGsFlagOot(id);
        break;
    case OV_SF:
        break;
    case OV_COW:
        gSharedCustomSave.cows |= (1 << id);
        break;
    case OV_SHOP:
        BITMAP8_SET(gSharedCustomSave.oot.shops, id);
        break;
    case OV_SCRUB:
        BITMAP8_SET(gSharedCustomSave.oot.scrubs, id);
        break;
    case OV_SR:
        BITMAP8_SET(gSharedCustomSave.oot.sr, id);
        break;
    case OV_FISH:
        BITMAP8_SET(gSharedCustomSave.caughtFishFlags, id);
        break;
    default:
        setXflagsMarkOot(play, ovType - OV_XFLAG0, sceneId, roomId, id);
        break;
    }
}

void Mark_SetMm(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id)
{
    switch (ovType)
    {
    case OV_NONE:
        break;
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
    case OV_SF:
        setStrayFairyMarkMm(play, sceneId, id);
        break;
    case OV_COW:
        gSharedCustomSave.cows |= (1 << id);
        break;
    case OV_SHOP:
        BITMAP8_SET(gSharedCustomSave.mm.shops, id);
        break;
    case OV_SCRUB:
        break;
    case OV_SR:
        break;
    case OV_FISH:
        break;
    default:
        setXflagsMarkMm(play, ovType - OV_XFLAG0, sceneId, roomId, id);
        break;
    }
}

int Mark_GetOot(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id)
{
    switch (ovType)
    {
    case OV_NONE:
        break;
    case OV_CHEST:
        return getChestMarkOot(play, sceneId, id);
    case OV_COLLECTIBLE:
        return getCollectibleMarkOot(play, sceneId, id);
    case OV_NPC:
        return BITMAP8_GET(gSharedCustomSave.oot.npc, id);
    case OV_GS:
        return getGsFlagOot(id);
    case OV_SF:
        break;
    case OV_COW:
        return !!(gSharedCustomSave.cows & (1 << id));
    case OV_SHOP:
        return BITMAP8_GET(gSharedCustomSave.oot.shops, id);
    case OV_SCRUB:
        return BITMAP8_GET(gSharedCustomSave.oot.scrubs, id);
    case OV_SR:
        return BITMAP8_GET(gSharedCustomSave.oot.sr, id);
    case OV_FISH:
        return BITMAP8_GET(gSharedCustomSave.caughtFishFlags, id);
    default:
        return getXflagsMarkOot(play, ovType - OV_XFLAG0, sceneId, roomId, id);
    }

    return 0;
}

int Mark_GetMm(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id)
{
    switch (ovType)
    {
    case OV_NONE:
        break;
    case OV_CHEST:
        return getChestMarkMm(play, sceneId, id);
    case OV_COLLECTIBLE:
        return getCollectibleMarkMm(play, sceneId, id);
    case OV_NPC:
        return BITMAP8_GET(gSharedCustomSave.mm.npc, id);
    case OV_GS:
        break;
    case OV_SF:
        return getStrayFairyMarkMm(play, sceneId, id);
    case OV_COW:
        return !!(gSharedCustomSave.cows & (1 << id));
    case OV_SHOP:
        return BITMAP8_GET(gSharedCustomSave.mm.shops, id);
    case OV_SCRUB:
        break;
    case OV_SR:
        break;
    case OV_FISH:
        break;
    default:
        return getXflagsMarkMm(play, ovType - OV_XFLAG0, sceneId, roomId, id);
    }

    return 0;
}

int Mark_GetQuery(PlayState* play, const ComboItemQuery* q)
{
    return Mark_Get(play, q->ovType, q->sceneId, q->roomId, q->id);
}
