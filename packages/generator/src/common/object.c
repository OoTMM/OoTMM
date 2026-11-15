#include <combo/enemy_soul_preview.h>
#include <combo.h>
#if defined(GAME_OOT)
#include <combo/oot_gameplay_memory.h>
#endif
#include <combo/custom.h>
#include <combo/dma.h>
#include <combo/global.h>
#include <combo/enemy_keep.h>
#include <combo/enemy_room_models.h>
#include <combo/effect.h>

#if defined(GAME_MM)
s32 Scene_LoadHumanLinkObject(ObjectContext*, s16);
s32 Object_SpawnPersistentCustom(ObjectContext*, s16);
void Object_UpdateEntriesCustom(ObjectContext*);
void Object_LoadAllCustom(ObjectContext*);
void* Object_AllocateSlotCustom(ObjectContext*, s32, s16);
void Object_AfterInitContext(void);
s32 EnHorse_GetLinkHorseSlot(ObjectContext*, s16);
s32 EnHorse_SpawnPersistentLinkHorse(ObjectContext*, s16);
void Horse_SpawnOverworldWrapper(PlayState*, Player*);
ALIGNED(16) static const u32 kNativeObjectBudgetMmV1[] = {
    0x4e4f4231,
    1,
    9,
    CUSTOM_OBJECT_TABLE_VROM,
    (u32)Object_AfterInitContext,
    (u32)Scene_LoadHumanLinkObject,
    (u32)Object_SpawnPersistentCustom,
    (u32)Object_UpdateEntriesCustom,
    (u32)Object_LoadAllCustom,
    (u32)Object_AllocateSlotCustom,
    (u32)EnHorse_GetLinkHorseSlot,
    (u32)EnHorse_SpawnPersistentLinkHorse,
    (u32)Horse_SpawnOverworldWrapper,
    CUSTOM_OBJECT_ID_MM_ADULT_LINK,
    CUSTOM_OBJECT_ID_MM_ADULT_EPONA,
    CUSTOM_OBJECT_ID_MM_ADULT_LINK_SPIN_ATTACK_VTX_1,
    CUSTOM_OBJECT_ID_MM_ADULT_LINK_SPIN_ATTACK_VTX_2,
    CUSTOM_OBJECT_ID_MM_ADULT_LINK_SPIN_ATTACK_VTX_3,
    CUSTOM_OBJECT_ID_MM_ADULT_LINK_MASK_MTX,
};
EXPORT_SYMBOL(FOREIGN_ENEMY_NATIVE_OBJECT_BUDGET_MM_V1, kNativeObjectBudgetMmV1);
#endif

#define OBJECT_COUNT    64
#define OBJECT_TTL      6

static u16   sObjectsIds[OBJECT_COUNT];
static void* sObjectsAddr[OBJECT_COUNT];
static u16   sObjectsTTL[OBJECT_COUNT];

ALIGNED(16) const ObjectData kExtraObjectsTable[] = {
#define X(a, b) { Y(a), Y(b) }
#define Y(x)    ((x) | 0x04000000)

#if defined(GAME_OOT)
# include "data/mm/objects.inc"
#endif

#if defined(GAME_MM)
# include "data/oot/objects.inc"
#endif

#undef X
#undef Y
};
#if defined(GAME_OOT)
EXPORT_SYMBOL(FOREIGN_ENEMY_OBJECT_TABLE_OOT_V1, kExtraObjectsTable);
#else
EXPORT_SYMBOL(FOREIGN_ENEMY_OBJECT_TABLE_MM_V1, kExtraObjectsTable);
#endif

typedef struct
{
    u32 oot;
    u32 mm;
}
ObjectTexturePatch;

static const ObjectTexturePatch kObjectTexturePatches[] = {
    { 0x224b0, 0x35fb0 },
    { 0x32530, 0x443b0 },
    { 0x32930, 0x447b0 },
    { 0x32d30, 0x44bb0 },
    { 0x33130, 0x44fb0 },
    { 0x33530, 0x453b0 },
    { 0x39450, 0x54f20 },
    { 0x53390, 0x7df10 },
    { 0x00000, 0x0c830 }, /* gHilite1Tex */
    { 0x01a00, 0x0CC30 }, /* gDekuStickTex */
};

ALIGNED(16) ObjectData kCustomObjectsTable[128];

static const ObjectPatch kObjectPatches[] = {
#if defined(GAME_OOT)
    { OBJ_MM_GI_ARROW, { 0x0378, 0x0418, 0x0bc8, 0x0c90, 0x1868, 0x1960 } },
    { OBJ_MM_GI_ARROWCASE, { 0x09b8, 0x0a98, 0x0be0 } },
    { OBJ_MM_GI_BEAN, { 0x05b8, 0x06c0 } },
    { OBJ_MM_GI_BOMB_1, { 0x09d8, 0x0c40 } },
    { OBJ_MM_GI_BOMB_2, { 0x04e8 } },
    { OBJ_MM_GI_BOMBPOUCH, { 0x0bb8, 0x0db0 } },
    { OBJ_MM_GI_BOSSKEY, { 0x0cd8, 0x0f40 } },
    { OBJ_MM_GI_BOTTLE, { 0x06a8, 0x0788 } },
    { OBJ_MM_GI_COMPASS, { 0x0998, 0x0c80 } },
    { OBJ_MM_GI_FISH, { 0x0638, 0x0760, 0x0858 } },
    { OBJ_MM_GI_GHOST, { 0x09c8, 0x0b00, 0x0c18 } },
    { OBJ_MM_GI_GLASSES, { 0x0db8, 0x1048 } },
    { OBJ_MM_GI_HEART, { 0x0110 } },
    { OBJ_MM_GI_HEARTS, { 0x12c8, 0x14a8, 0x15c8, 0x1600, 0x16f0 } },
    { OBJ_MM_GI_INSECT, { 0x0868, 0x0b58 } },
    { OBJ_MM_GI_KEY, { 0x0838 } },
    { OBJ_MM_GI_KI_TAN_MASK, { 0x0af8 } },
    { OBJ_MM_GI_LIQUID, { 0x1460, 0x16c0, 0x1720, 0x17b8 } },
    { OBJ_MM_GI_LONGSWORD, { 0x0638, 0x0848 } },
    { OBJ_MM_GI_M_ARROW, { 0x0b18, 0x0bb8, 0x0d28 } },
    { OBJ_MM_GI_MAGICPOT, { 0x05b8, 0x0f18 } },
    { OBJ_MM_GI_MAP, { 0x0490, 0x0ba8 } },
    { OBJ_MM_GI_MELODY, { 0x0af8 } },
    { OBJ_MM_GI_MILK, { 0x1098, 0x1158, 0x12c0 } },
    { OBJ_MM_GI_NUTS, { 0x10f8 } },
    { OBJ_MM_GI_OCARINA, { 0x08f8 } },
    { OBJ_MM_GI_RABIT_MASK, { 0x0bf8 } },
    { OBJ_MM_GI_RUPY, { 0x0648, 0x0758 } },
    { OBJ_MM_GI_SOUL, { 0x0c08, 0x0df0, 0x0f50 } },
    { OBJ_MM_GI_STICK, { 0x0508 } },
    { OBJ_MM_GI_SUTARU, { 0x0368, 0x0470, 0x04a8, 0x0528, 0x05e8 } },
    { OBJ_MM_GI_TRUTH_MASK, { 0x1408, 0x16e8 } },
#endif

#if defined(GAME_MM)
    { OBJ_OOT_GI_ARROW, { 0x0378, 0x0418, 0x0bc8, 0x0c90, 0x1868, 0x1960 } },
    { OBJ_OOT_GI_ARROWCASE, { 0x09b8, 0x0a98, 0x0be0 } },
    { OBJ_OOT_GI_BEAN, { 0x05b8, 0x06c0 } },
    { OBJ_OOT_GI_BOMB_1, { 0x09d8, 0x0c40 } },
    { OBJ_OOT_GI_BOMB_2, { 0x04e8 } },
    { OBJ_OOT_GI_BOMBPOUCH, { 0x0bb8, 0x0db0 } },
    { OBJ_OOT_GI_BOOMERANG, { 0x0a68, 0x0b88 } },
    { OBJ_OOT_GI_BOOTS_2, { 0x1668, 0x1810 } },
    { OBJ_OOT_GI_BOSSKEY, { 0x0cd8, 0x0f40 } },
    { OBJ_OOT_GI_BOTTLE, { 0x06a8, 0x0788 } },
    { OBJ_OOT_GI_BOTTLE_LETTER, { 0x0918, 0x09d8, 0x0b18 } },
    { OBJ_OOT_GI_BOW, { 0x09c8, 0x0b10, 0x0c98 } },
    { OBJ_OOT_GI_BRACELET, { 0x0998 } },
    { OBJ_OOT_GI_BROKENSWORD, { 0x0718, 0x0920 } },
    { OBJ_OOT_GI_BUTTERFLY, { 0x0868, 0x0aa8 } },
    { OBJ_OOT_GI_COIN, { 0x0608, 0x07a0 } },
    { OBJ_OOT_GI_COMPASS, { 0x0998, 0x0c80 } },
    { OBJ_OOT_GI_DEKUPOUCH, { 0x0b98, 0x0d20, 0x0e58 } },
    { OBJ_OOT_GI_EGG, { 0x1020, 0x11b0 } },
    { OBJ_OOT_GI_EYE_LOTION, { 0x06b8, 0x07a0 } },
    { OBJ_OOT_GI_FIRE, { 0x0c98 } },
    { OBJ_OOT_GI_FISH, { 0x0638, 0x0760, 0x0858 } },
    { OBJ_OOT_GI_FROG, { 0x0d98 } },
    { OBJ_OOT_GI_GERUDO, { 0x10a8, 0x1198, 0x1250 } },
    { OBJ_OOT_GI_GHOST, { 0x09c8, 0x0b00, 0x0c18 } },
    { OBJ_OOT_GI_GLASSES, { 0x0db8, 0x1048 } },
    { OBJ_OOT_GI_GLOVES, { 0x1568, 0x17e8 } },
    { OBJ_OOT_GI_GODDESS, { 0x0958, 0x0a58, 0x0a90 } },
    { OBJ_OOT_GI_GRASS, { 0x0918, 0x0a00 } },
    { OBJ_OOT_GI_HAMMER, { 0x0a08, 0x0c40 } },
    { OBJ_OOT_GI_HEART, { 0x0110 } },
    { OBJ_OOT_GI_HEARTS, { 0x12c8, 0x14a8, 0x15c8, 0x1600, 0x16f0 } },
    { OBJ_OOT_GI_HOOKSHOT, { 0x0788, 0x09e0, 0x1278, 0x14f0 } },
    { OBJ_OOT_GI_HOVERBOOTS, { 0x1888, 0x1a20 } },
    { OBJ_OOT_GI_INSECT, { 0x0868, 0x0b58 } },
    { OBJ_OOT_GI_KEY, { 0x0838 } },
    { OBJ_OOT_GI_KI_TAN_MASK, { 0x0af8 } },
    { OBJ_OOT_GI_LIQUID, { 0x1460, 0x16c0, 0x1720, 0x17b8 } },
    { OBJ_OOT_GI_LONGSWORD, { 0x0638, 0x0848 } },
    { OBJ_OOT_GI_M_ARROW, { 0x0b18, 0x0bb8, 0x0d28 } },
    { OBJ_OOT_GI_MAGICPOT, { 0x05b8, 0x0f18 } },
    { OBJ_OOT_GI_MAP, { 0x0490, 0x0ba8 } },
    { OBJ_OOT_GI_MELODY, { 0x0af8 } },
    { OBJ_OOT_GI_MILK, { 0x1098, 0x1158, 0x12c0 } },
    { OBJ_OOT_GI_MUSHROOM, { 0x0a08, 0x0c18 } },
    { OBJ_OOT_GI_NIWATORI, { 0x1830 } },
    { OBJ_OOT_GI_NUTS, { 0x10f8 } },
    { OBJ_OOT_GI_OCARINA, { 0x08f8 } },
    { OBJ_OOT_GI_OCARINA_0, { 0x0918 } },
    { OBJ_OOT_GI_PACHINKO, { 0x0978, 0x0a58 } },
    { OBJ_OOT_GI_POWDER, { 0x08e8, 0x09e8 } },
    { OBJ_OOT_GI_PRESCRIPTION, { 0x09f8 } },
    { OBJ_OOT_GI_RABIT_MASK, { 0x0bf8 } },
    { OBJ_OOT_GI_REDEAD_MASK, { 0x0818 } },
    { OBJ_OOT_GI_RUPY, { 0x0608, 0x0718 } },
    { OBJ_OOT_GI_SAW, { 0x0818, 0x0aa0 } },
    { OBJ_OOT_GI_SCALE, { 0x0ac8, 0x0b00, 0x0cf0 } },
    { OBJ_OOT_GI_SEED, { 0x0848 } },
    { OBJ_OOT_GI_SHIELD_1, { 0x0a88, 0x0c90 } },
    { OBJ_OOT_GI_SHIELD_2, { 0x0ca8, 0x0fe8 } },
    { OBJ_OOT_GI_SHIELD_3, { 0x0fe8, 0x1110, 0x1148 } },
    { OBJ_OOT_GI_SKJ_MASK, { 0x0a08 } },
    { OBJ_OOT_GI_SOUL, { 0x0c08, 0x0df0, 0x0f50 } },
    { OBJ_OOT_GI_STICK, { 0x0508 } },
    { OBJ_OOT_GI_SUTARU, { 0x0368, 0x0470, 0x04a8, 0x0528, 0x05e8 } },
    { OBJ_OOT_GI_SWORD_1, { 0x0998, 0x0bc0, 0x0c48 } },
    { OBJ_OOT_GI_TICKETSTONE, { 0x0f38 } },
    { OBJ_OOT_GI_TRUTH_MASK, { 0x1408, 0x16e8 } },
    { CUSTOM_OBJECT_ID_GI_STONE_EMERALD, { 0xba0, 0x12a8 } },
    { CUSTOM_OBJECT_ID_GI_STONE_RUBY, { 0xa70, 0xd38 } },
    { CUSTOM_OBJECT_ID_GI_STONE_SAPPHIRE, { 0x1308 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_FOREST, { 0x3a8, 0xe58 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_FIRE,   { 0x1e8, 0xc48 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_WATER,  { 0x368, 0xe18 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_SPIRIT, { 0x3b8, 0xe58 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_SHADOW, { 0x308, 0xd78 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_LIGHT,  { 0x508, 0xf98 } },
    { CUSTOM_OBJECT_ID_BOMBCHU_BAG,         { 0x6a8, 0x8f0 } },
    { CUSTOM_OBJECT_ID_GI_MASTER_SWORD,     { 0x11a8, 0x12a8 } },
    { CUSTOM_OBJECT_ID_EQ_HAMMER,           { 0x0d18, 0x0dc0 } },
#endif
};

void comboInitObjects(void)
{
    /* Load the DMA table */
    comboDmaLoadFile(kCustomObjectsTable, CUSTOM_OBJECT_TABLE_VROM);
}

void comboLoadCustomKeep(void)
{
    u32 customKeepSize;

    customKeepSize = comboDmaLoadFile(NULL, CUSTOM_KEEP_VROM);
    g.customKeep = malloc(customKeepSize);
    comboDmaLoadFile(g.customKeep, CUSTOM_KEEP_VROM);
}

#if defined(GAME_OOT)
static u32 comboGetTextureOverride(u32 texture)
{
    for (int i = 0; i < ARRAY_COUNT(kObjectTexturePatches); ++i)
    {
        if (kObjectTexturePatches[i].mm == texture)
            return kObjectTexturePatches[i].oot;
    }

    return 0x32930;
}
#endif

#if defined(GAME_MM)
static u32 comboGetTextureOverride(u32 texture)
{
    for (int i = 0; i < ARRAY_COUNT(kObjectTexturePatches); ++i)
    {
        if (kObjectTexturePatches[i].oot == texture)
            return kObjectTexturePatches[i].mm;
    }

    return 0x447b0;
}
#endif

#if defined(GAME_MM)
static void enemyLizalfosNativeKeep(void* buffer)
{
    u32* image = (u32*)((u8*)buffer + 0xdeb0);
    if (image[0] == 0xfd900000 && image[1] == 0x04016ef0)
        image[1] = 0x0402cab0;
}
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_LIZALFOS_NATIVE_KEEP_V1, enemyLizalfosNativeKeep);
#endif

static void comboPatchForeignObject(void* buffer, u16 objectId)
{
    const ObjectPatch* patch;
    u16 offset;
    u32* texture;

#if defined(GAME_MM)
    if (objectId == 0x1b)
        enemyLizalfosNativeKeep(buffer);
#endif
    patch = NULL;
    for (u32 i = 0; i < ARRAY_COUNT(kObjectPatches); ++i)
    {
        if (kObjectPatches[i].objectId == objectId)
        {
            patch = &kObjectPatches[i];
            break;
        }
    }

    if (patch == NULL)
        return;

    for (u32 i = 0; i < ARRAY_COUNT(patch->offsets); ++i)
    {
        offset = patch->offsets[i];
        if (!offset)
            break;
        texture = (u32*)((u8*)buffer + offset + 4);
        *texture = 0x04000000 | comboGetTextureOverride((*texture) & 0xffffff);
    }
}

#if defined(GAME_OOT)
# define OBJECT_NATIVE_COUNT 0x192
#else
# define OBJECT_NATIVE_COUNT 0x283
#endif

static const ObjectData* comboCheckedObjectData(u16 objectId)
{
    u32 index = objectId & 0xfff;
    if (!objectId || (objectId & 0xc000))
        return NULL;
    if (objectId & 0x2000)
        return index < ARRAY_COUNT(kCustomObjectsTable) ? &kCustomObjectsTable[index] : NULL;
    if (objectId & MASK_FOREIGN_OBJECT)
        return index < ARRAY_COUNT(kExtraObjectsTable) ? &kExtraObjectsTable[index] : NULL;
    return index < OBJECT_NATIVE_COUNT ? &kObjectsTable[index] : NULL;
}

const ObjectData* comboGetObjectData(u16 objectId)
{
    static const ObjectData empty = { 0, 0 };
    const ObjectData* entry = comboCheckedObjectData(objectId);
    return entry ? entry : &empty;
}

u32 comboLoadObject(void* buffer, u16 objectId)
{
    u32 vromStart;
    u32 vromEnd;
    const ObjectData* table;
    int isPatchable;
    u16 patchObjectId;

    table = comboCheckedObjectData(objectId);
    if (!table || table->vromEnd <= table->vromStart)
        return 0;

    isPatchable = !!(objectId & 0x3000);
    patchObjectId = objectId & ~MASK_FOREIGN_OBJECT;
    vromStart = table->vromStart;
    vromEnd = table->vromEnd;

    if (buffer)
    {
        LoadFile(buffer, vromStart, vromEnd - vromStart);
        if (isPatchable)
            comboPatchForeignObject(buffer, patchObjectId);
    }
    return vromEnd - vromStart;
}

static void* objectGameplayAlloc(u16 objectId, u32 size)
{
#if defined(GAME_OOT)
    if (comboOotGameplayObject(objectId))
        return mallocGameplay(size);
#endif
    return malloc(size);
}

void* comboGetObject(u16 objectId)
{
    int freeSlot = -1;
    u32 size;
    void* addr;

    if (!objectId)
        return NULL;

    for (int i = 0; i < OBJECT_COUNT; ++i)
    {
        /* Already loaded the object */
        if (sObjectsIds[i] == objectId)
        {
            sObjectsTTL[i] = OBJECT_TTL;
            return sObjectsAddr[i];
        }

        /* Free slot */
        if (!sObjectsIds[i] && freeSlot < 0)
            freeSlot = i;
    }

    if (freeSlot < 0)
        return NULL;
    size = comboLoadObject(NULL, objectId);
    if (!size)
        return NULL;
    addr = objectGameplayAlloc(objectId, size);
    if (!addr)
        return NULL;
    comboLoadObject(addr, objectId);
    sObjectsIds[freeSlot] = objectId;
    sObjectsAddr[freeSlot] = addr;
    sObjectsTTL[freeSlot] = OBJECT_TTL;
    return addr;
}

void comboObjectsReset(void)
{
    for (int i = 0; i < OBJECT_COUNT; ++i)
    {
        free(sObjectsAddr[i]);
        sObjectsIds[i] = 0;
        sObjectsAddr[i] = NULL;
        sObjectsTTL[i] = 0;
    }
}

void comboObjectsGC(void)
{
    for (int i = 0; i < OBJECT_COUNT; ++i)
    {
        if (sObjectsAddr[i] == NULL)
            continue;
        if (sObjectsTTL[i] == 0)
        {
            free(sObjectsAddr[i]);
            sObjectsIds[i] = 0;
            sObjectsAddr[i] = NULL;
            sObjectsTTL[i] = 0;
        }
        else
        {
            sObjectsTTL[i]--;
        }
    }
}

#define EX_OBJECT_SLOTS_NORMAL      OBJECT_EXCHANGE_BANK_MAX
#define EX_OBJECT_SLOTS_TOTAL       128
#define EX_OBJECT_SLOTS_EXTENDED    (EX_OBJECT_SLOTS_TOTAL - EX_OBJECT_SLOTS_NORMAL)

#if defined(GAME_MM)
#ifndef EnemyObjectNativeAlloc
# define EnemyObjectNativeAlloc ((void* (*)(u32))0x80102c60)
#endif
#ifndef EnemyObjectNativeFree
# define EnemyObjectNativeFree ((void (*)(void*))0x80102ce0)
#endif
#ifndef EnemyObjectNativeCleanup
# define EnemyObjectNativeCleanup ((void (*)(void))0x80102dec)
#endif
#endif
#if defined(GAME_OOT)
#ifndef EnemyObjectNativeAlloc
# define EnemyObjectNativeAlloc ((void* (*)(u32))0x80066c10)
#endif
#ifndef EnemyObjectNativeFree
# define EnemyObjectNativeFree ((void (*)(void*))0x80066c90)
#endif
#ifndef EnemyObjectNativeCleanup
# define EnemyObjectNativeCleanup ((void (*)(void))0x80066d9c)
#endif
static u8 sNativeObjectLifecycleOotV1 __attribute__((section(".data"))) = 0;
EXPORT_SYMBOL(ENEMY_NATIVE_OBJECT_LIFECYCLE_OOT_V1, sNativeObjectLifecycleOotV1);
static u8 sNativeObjectOwned[EX_OBJECT_SLOTS_EXTENDED];
static u8 sNativeObjectActive[EX_OBJECT_SLOTS_EXTENDED];
static s8 sNativeObjectRoom[EX_OBJECT_SLOTS_EXTENDED];
#endif

static int isRoomEnemyModel(u16 objectId)
{
#if defined(GAME_OOT)
    if (comboOotGameplayObject(objectId))
        return 0;
#endif
    return (objectId & 0x3000) != 0;
}

static int isNativeEnemyModel(u16 objectId)
{
    return objectId > 3 && objectId < 0x1000 && comboCheckedObjectData(objectId);
}

static void enemyFreeExtendedObject(u16 objectId, void* data)
{
    if (isRoomEnemyModel(objectId))
        EnemyObjectNativeFree(data);
    else
        free(data);
}

static u16 sExObjectsIds[EX_OBJECT_SLOTS_EXTENDED];
void* gExObjectsAddr[EX_OBJECT_SLOTS_TOTAL];
static u8 sNativeEnemyModel[EX_OBJECT_SLOTS_EXTENDED];
static u32 sModelRooms[EX_OBJECT_SLOTS_EXTENDED];
static u32 sModelPinUntil[EX_OBJECT_SLOTS_EXTENDED];
static s8 sModelPreloadRoom = -1;
static u32 sModelPreloadOomBits[(0x1000 + ARRAY_COUNT(kExtraObjectsTable) + ARRAY_COUNT(kCustomObjectsTable) + 31) / 32];
static s8 sModelPreloadOomRoom = -1;

static int enemyModelOomIndex(u16 objectId)
{
    if (!comboCheckedObjectData(objectId))
        return -1;
    if (objectId < 0x1000)
        return objectId;
    return 0x1000 + (objectId & 0xfff) + ((objectId & 0x2000) ? ARRAY_COUNT(kExtraObjectsTable) : 0);
}

void comboEnemyObjectPreloadBegin(s8 room)
{
    memset(sModelPreloadOomBits, 0, sizeof(sModelPreloadOomBits));
    sModelPreloadOomRoom = room >= 0 && room < 32 ? room : -1;
}

int comboEnemyObjectPreloadOom(u16 objectId, s8 room)
{
    int index = enemyModelOomIndex(objectId);
    return room >= 0 && room == sModelPreloadOomRoom && index >= 0 &&
           !!(sModelPreloadOomBits[index / 32] & ((u32)1 << (index & 31)));
}

void comboExObjectsReset(void)
{
    comboEnemySoulPreviewReset();
    comboEnemyKeepReset();
    comboEnemyRoomModelsReset();
    for (int i = 0; i < EX_OBJECT_SLOTS_EXTENDED; ++i)
    {
        if (gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i])
        {
#if defined(GAME_OOT)
            if (sNativeObjectOwned[i])
                EnemyObjectNativeFree(gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i]);
            else
#endif
            if (sNativeEnemyModel[i])
                EnemyObjectNativeFree(gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i]);
            else
                enemyFreeExtendedObject(sExObjectsIds[i], gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i]);
            gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i] = NULL;
        }
    }
    memset(sExObjectsIds, 0xff, sizeof(sExObjectsIds));
    memset(sNativeEnemyModel, 0, sizeof(sNativeEnemyModel));
    memset(sModelRooms, 0, sizeof(sModelRooms));
    memset(sModelPinUntil, 0, sizeof(sModelPinUntil));
    sModelPreloadRoom = -1;
    comboEnemyObjectPreloadBegin(-1);
#if defined(GAME_OOT)
    memset(sNativeObjectOwned, 0, sizeof(sNativeObjectOwned));
    memset(sNativeObjectActive, 0, sizeof(sNativeObjectActive));
    memset(sNativeObjectRoom, -1, sizeof(sNativeObjectRoom));
#endif
}

#if defined(GAME_MM)
static void enemyObjectArenaCleanup(void)
{
    comboExObjectsReset();
    EnemyObjectNativeCleanup();
}
PATCH_CALL(0x801663a8, enemyObjectArenaCleanup);
void comboObjectNativeFinishRoomChange(PlayState*, RoomContext*);
static void enemyObjectFinishRoomChange(PlayState* play, RoomContext* roomCtx)
{
    comboObjectNativeFinishRoomChange(play, roomCtx);
    comboEnemyObjectReleaseUnloadedRooms(play);
}
PATCH_FUNC(0x8012ebf8, enemyObjectFinishRoomChange);
#else
static void enemyObjectArenaCleanup(void)
{
    comboExObjectsReset();
    EnemyObjectNativeCleanup();
}
PATCH_CALL(0x8009a710, enemyObjectArenaCleanup);

void comboObjectNativeFinishRoomChange(PlayState*, RoomContext*);
static void enemyObjectFinishRoomChange(PlayState* play, RoomContext* roomCtx)
{
    s8 previous = roomCtx->prevRoom.num;
    comboObjectNativeFinishRoomChange(play, roomCtx);
    comboEnemyObjectReleaseUnloadedRooms(play);
    if (!sNativeObjectLifecycleOotV1 || previous < 0 || previous == roomCtx->curRoom.num)
        return;
    for (int i = 0; i < EX_OBJECT_SLOTS_EXTENDED; ++i)
    {
        if (!sNativeObjectOwned[i])
            continue;
        if (gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i] && sNativeObjectActive[i])
        {
            sNativeObjectActive[i] = 0;
        }
        else if (sNativeObjectRoom[i] != roomCtx->curRoom.num)
        {
            EnemyObjectNativeFree(gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i]);
            gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i] = NULL;
            sExObjectsIds[i] = 0xffff;
            sNativeObjectOwned[i] = 0;
            sNativeObjectActive[i] = 0;
            sNativeObjectRoom[i] = -1;
        }
    }
}
PATCH_FUNC(0x80080c98, enemyObjectFinishRoomChange);
#endif

#if defined(DEBUG)
extern int gDebugEnemySpawn;
#endif

static int objectGetExtendedSlot(u16 objectId)
{
    if (objectId >= 0x1001 && objectId <= 0x1003)
        return gPlay ? _Object_GetSlot(&gPlay->objectCtx, 1) : -1;

    int nativeEnemyModel = sModelPreloadRoom >= 0 && isNativeEnemyModel(objectId);
    int roomEnemyModel = nativeEnemyModel || isRoomEnemyModel(objectId);
    s8 modelRoom = sModelPreloadRoom;
#if defined(DEBUG)
    if (modelRoom < 0 && gDebugEnemySpawn && gPlay &&
        gPlay->roomCtx.curRoom.num >= 0 && gPlay->roomCtx.curRoom.num < 32)
        modelRoom = gPlay->roomCtx.curRoom.num;
#endif
    int slot;
    int freeSlot;
    void* data;
    u32 size;

    slot = -1;
    freeSlot = -1;

    for (int i = 0; i < EX_OBJECT_SLOTS_EXTENDED; ++i)
    {
        if (sExObjectsIds[i] == objectId && (!nativeEnemyModel || sNativeEnemyModel[i]))
        {
            slot = EX_OBJECT_SLOTS_NORMAL + i;
            if (roomEnemyModel && modelRoom >= 0)
                sModelRooms[i] |= (u32)1 << modelRoom;
#if defined(GAME_OOT)
            if (sNativeObjectOwned[i])
            {
                sNativeObjectActive[i] = 1;
                sNativeObjectRoom[i] = gPlay ? gPlay->roomCtx.curRoom.num : -1;
            }
#endif
            break;
        }

        if (sExObjectsIds[i] == 0xffff && freeSlot == -1)
        {
            freeSlot = i;
        }
    }

    /* Load the object */
    if (slot == -1 && freeSlot != -1)
    {
        if (roomEnemyModel && modelRoom < 0)
            return -1;
        size = comboLoadObject(NULL, objectId);
        if (!size)
            return -1;
        u32 sourceSize = size;
#if defined(GAME_MM)
        if (objectId == 0x105f)
        {
            if (size != 0x14f0)
                return -1;
            size += 4 * sizeof(Gfx);
        }
#else
        if (objectId == 0x11c4)
        {
            if (size != 0x7730)
                return -1;
            size += 12 * sizeof(Gfx);
        }
#endif
        if (roomEnemyModel)
            data = comboEnemyArenaAlloc(size);
#if defined(GAME_OOT)
        else if (sNativeObjectLifecycleOotV1 && objectId < 0x1000)
            data = EnemyObjectNativeAlloc(size);
#endif
        else
            data = objectGameplayAlloc(objectId, size);
        if (!data && roomEnemyModel && modelRoom >= 0)
        {
            if (modelRoom == sModelPreloadOomRoom)
            {
                int index = enemyModelOomIndex(objectId);
                if (index >= 0)
                    sModelPreloadOomBits[index / 32] |= (u32)1 << (index & 31);
            }
        }
        if (data)
        {
            u32 loaded = comboLoadObject(data, objectId);
            if (loaded != sourceSize)
            {
                if (roomEnemyModel)
                    EnemyObjectNativeFree(data);
#if defined(GAME_OOT)
                else if (sNativeObjectLifecycleOotV1 && objectId < 0x1000)
                    EnemyObjectNativeFree(data);
#endif
                else
                    free(data);
                return -1;
            }
#if defined(GAME_MM)
            if (objectId == 0x105f)
                memset((u8*)data + 0x14f0, 0, 4 * sizeof(Gfx));
#else
            if (objectId == 0x11c4)
                memset((u8*)data + 0x7730, 0, 12 * sizeof(Gfx));
#endif
#if defined(GAME_OOT)
            if (!nativeEnemyModel && sNativeObjectLifecycleOotV1 && objectId < 0x1000)
            {
                if (loaded != size)
                {
                    EnemyObjectNativeFree(data);
                    return -1;
                }
                sNativeObjectOwned[freeSlot] = 1;
                sNativeObjectActive[freeSlot] = 1;
                sNativeObjectRoom[freeSlot] = gPlay ? gPlay->roomCtx.curRoom.num : -1;
            }
#endif
            sNativeEnemyModel[freeSlot] = nativeEnemyModel;
            sExObjectsIds[freeSlot] = objectId;
            if (roomEnemyModel)
                sModelRooms[freeSlot] = (u32)1 << modelRoom;
            gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + freeSlot] = data;
            slot = EX_OBJECT_SLOTS_NORMAL + freeSlot;
        }
    }

    return slot;
}

int comboEnemyObjectPreload(u16 objectId, s8 room)
{
    s8 saved;
    int slot;
    if (room < 0 || room >= 32 || (!(objectId & 0x3000) && !isNativeEnemyModel(objectId)) || !comboCheckedObjectData(objectId))
        return 0;
    if (isNativeEnemyModel(objectId) && gPlay && _Object_GetSlot(&gPlay->objectCtx, objectId) >= 0)
        return 1;
    saved = sModelPreloadRoom;
    sModelPreloadRoom = room;
    slot = objectGetExtendedSlot(objectId);
    sModelPreloadRoom = saved;
    if (slot >= 0)
    {
        int index = enemyModelOomIndex(objectId);
        if (index >= 0 && room == sModelPreloadOomRoom)
            sModelPreloadOomBits[index / 32] &= ~((u32)1 << (index & 31));
    }
    return slot >= 0;
}
EXPORT_SYMBOL(ENEMY_FOREIGN_MODEL_ROOMS_V1, comboEnemyObjectPreload);

static void comboEnemyPinObjectSlot(int slot, u16 frames)
{
    if (slot < EX_OBJECT_SLOTS_NORMAL || slot >= EX_OBJECT_SLOTS_TOTAL)
        return;
    if (!gPlay || !gExObjectsAddr[slot])
        return;
    int index = slot - EX_OBJECT_SLOTS_NORMAL;
    u32 until = gPlay->gameplayFrames + frames;
    if (!until)
        until = 1;
    if (!sModelPinUntil[index] || (s32)(until - sModelPinUntil[index]) > 0)
        sModelPinUntil[index] = until;
}

void comboEnemyObjectPinFrames(u16 objectId, u16 frames)
{
    if (!gPlay || !frames)
        return;
    for (int i = 0; i < EX_OBJECT_SLOTS_EXTENDED; ++i)
    {
        if (sExObjectsIds[i] != objectId || !gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i])
            continue;
        comboEnemyPinObjectSlot(EX_OBJECT_SLOTS_NORMAL + i, frames);
        return;
    }
}

static int enemyModelEffectLive(int index)
{
    const ObjectData* object = comboCheckedObjectData(sExObjectsIds[index]);
    if (!object || object->vromEnd <= object->vromStart)
        return 1;
    u32 start = (u32)gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + index] & 0x1fffffff;
    u32 size = object->vromEnd - object->vromStart;
#if defined(GAME_MM)
    if (sExObjectsIds[index] == 0x105f)
        size += 4 * sizeof(Gfx);
#else
    if (sExObjectsIds[index] == 0x11c4)
        size += 12 * sizeof(Gfx);
#endif
    if (gEffectSsTable.size > 0x400)
        return 1;
    if (!gEffectSsTable.data)
        return gEffectSsTable.size != 0;
    for (u32 i = 0; i < gEffectSsTable.size; ++i)
    {
        EffectSs* effect = &gEffectSsTable.data[i];
        if (effect->life < 0)
            continue;
        int slot = -1;
        switch (effect->type)
        {
        case 0x06:
#if defined(GAME_MM)
            slot = effect->regs[9];
#else
            slot = effect->regs[10];
#endif
            break;
        case 0x0f: slot = effect->regs[5]; break;
        case 0x19: slot = effect->regs[11]; break;
#if defined(GAME_MM)
        case 0x25: slot = effect->regs[0]; break;
#endif
        }
        u32 gfx = (u32)effect->gfx & 0x1fffffff;
        if (slot == EX_OBJECT_SLOTS_NORMAL + index || (gfx >= start && gfx - start < size))
        {
            comboEnemyPinObjectSlot(EX_OBJECT_SLOTS_NORMAL + index, 3);
            return 1;
        }
    }
    return 0;
}

static void enemyObjectReap(u32 releasedRooms)
{
#if defined(GAME_MM)
    u32 checkedRetiringRooms = 0, retiringRooms = 0;
#endif
    for (int i = 0; i < EX_OBJECT_SLOTS_EXTENDED; ++i)
    {
        u32 previousRooms = sModelRooms[i];
        sModelRooms[i] &= ~releasedRooms;
#if defined(GAME_MM)
        if (gPlay && gPlay->actorCtx.halfDaysBit)
        {
            for (u32 room = 0; room < 32; ++room)
            {
                u32 bit = 1u << room;
                if ((sModelRooms[i] & bit) &&
                    !(comboEnemyRoomModelHalfDays(gPlay, room, sExObjectsIds[i]) & gPlay->actorCtx.halfDaysBit))
                {
                    if (!(checkedRetiringRooms & bit))
                    {
                        checkedRetiringRooms |= bit;
                        if (comboEnemyRoomHasRetiringActors(room, gPlay->actorCtx.halfDaysBit))
                            retiringRooms |= bit;
                    }
                    if (!(retiringRooms & bit))
                        sModelRooms[i] &= ~bit;
                }
            }
        }
#endif
        if (previousRooms && !sModelRooms[i])
            comboEnemyPinObjectSlot(EX_OBJECT_SLOTS_NORMAL + i, 3);
        if ((!sNativeEnemyModel[i] && !isRoomEnemyModel(sExObjectsIds[i])) || sExObjectsIds[i] == 0xffff || sModelRooms[i] ||
            !gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i] ||
            (sModelPinUntil[i] && gPlay && (s32)(sModelPinUntil[i] - gPlay->gameplayFrames) > 0))
            continue;
        {
            int live = 0;
            for (int category = 0; gPlay && category < 12 && !live; ++category)
                for (Actor* actor = gPlay->actorCtx.actors[category].first; actor; actor = actor->next)
                    if (actor->objectSlot == EX_OBJECT_SLOTS_NORMAL + i)
                    {
                        live = 1;
                        break;
                    }
            if (live)
            {
                comboEnemyPinObjectSlot(EX_OBJECT_SLOTS_NORMAL + i, 3);
                continue;
            }
        }
        if (enemyModelEffectLive(i))
            continue;
        EnemyObjectNativeFree(gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i]);
        gExObjectsAddr[EX_OBJECT_SLOTS_NORMAL + i] = NULL;
        sExObjectsIds[i] = 0xffff;
        sModelPinUntil[i] = 0;
        sNativeEnemyModel[i] = 0;
    }
}

void comboEnemyObjectReleaseUnloadedRooms(PlayState* play)
{
    if (!play)
        return;
#if defined(GAME_MM)
    s8 previous = play->roomCtx.prefRoom.num;
#else
    s8 previous = play->roomCtx.prevRoom.num;
#endif
    u32 candidates = 0;
    for (int i = 0; i < EX_OBJECT_SLOTS_EXTENDED; ++i)
        candidates |= sModelRooms[i];
    u32 released = 0;
    for (int room = 0; room < 32; ++room)
    {
        u32 bit = (u32)1 << room;
        if ((candidates & bit) && room != play->roomCtx.curRoom.num && room != previous &&
            !comboEnemyRoomModelsLive(room))
            released |= bit;
    }
    enemyObjectReap(released);
}

#if defined(GAME_MM)
EXPORT_SYMBOL(FOREIGN_ENEMY_NATIVE_KEEP_MM, objectGetExtendedSlot);
#endif

#if defined(GAME_MM)
#if defined(DEBUG)
static int enemyLoadNativeRoomObject(ObjectContext* ctx, u16 objectId)
{
    int slot = ctx->num;
    if (slot >= ARRAY_COUNT(ctx->slots) - 1)
        return -1;

    u32 start = (u32)ctx->slots[slot].segment;
    u32 end = (u32)ctx->spaceEnd;
    if (start < (u32)ctx->spaceStart || start >= end)
        return -1;

    u32 size = comboLoadObject(NULL, objectId);
    if (!size || size > end - start || ALIGN16(start + size) > end)
        return -1;
    if (comboLoadObject((void*)start, objectId) != size)
        return -1;

    ctx->slots[slot].id = objectId;
    ctx->slots[slot + 1].segment = (void*)ALIGN16(start + size);
    ++ctx->num;
    return slot;
}

#endif

static u16 enemySplitChildObjectId(u16 actorId)
{
    switch (actorId)
    {
    case 0x0c: return 0x0b;
    case 0xf1: return 0x06;
    case 0x109: return 0x14e;
    case 0x0b: return 0x0a;
    case 0x12: return 0x12;
    case 0xec: return 0x141;
    default: return 0;
    }
}

int comboEnemyLoadSplitChildObject(PlayState* play, u16 actorId)
{
    u16 objectId = enemySplitChildObjectId(actorId);
    ObjectContext* ctx = &play->objectCtx;
    if (ctx->num > EX_OBJECT_SLOTS_NORMAL || !comboCheckedObjectData(objectId))
        return 0;
    int slot = _Object_GetSlot(ctx, objectId);
    if (slot >= 0)
        return _Object_IsLoaded(ctx, slot);
    if (play->roomCtx.status || play->roomCtx.curRoom.num < 0)
        return 0;
    return comboEnemyObjectPreload(objectId, play->roomCtx.curRoom.num);
}

#endif

int Object_GetSlotWrapper(ObjectContext* objectCtx, u16 objectId)
{
    int slot;

    if (!objectCtx || objectCtx->num > EX_OBJECT_SLOTS_NORMAL)
        return -1;

    /* Forward */
    slot = _Object_GetSlot(objectCtx, objectId);
    if (slot >= 0)
        return slot;
    if (!comboCheckedObjectData(objectId))
        return -1;

    if (gPlay && objectCtx == &gPlay->objectCtx && isNativeEnemyModel(objectId))
    {
        int room = gPlay->roomCtx.curRoom.num;
        if (room >= 0 && room < 32)
            for (int i = 0; i < EX_OBJECT_SLOTS_EXTENDED; ++i)
                if (sNativeEnemyModel[i] && sExObjectsIds[i] == objectId &&
                    (sModelRooms[i] & ((u32)1 << room)))
                    return EX_OBJECT_SLOTS_NORMAL + i;
    }

    if (gPlay && objectCtx == &gPlay->objectCtx && isNativeEnemyModel(objectId) &&
        comboEnemyRoomHasNativeModel(gPlay, objectId))
    {
        comboEnemyObjectReleaseUnloadedRooms(gPlay);
        s8 saved = sModelPreloadRoom;
        sModelPreloadRoom = gPlay->roomCtx.curRoom.num;
        slot = objectGetExtendedSlot(objectId);
        sModelPreloadRoom = saved;
        if (slot < 0)
            g.roomEnemyLackSoul = 1;
        return slot;
    }

#if defined(GAME_MM)
    if (!(objectId & 0x3000))
    {
#if defined(DEBUG)
        if (gDebugEnemySpawn && gPlay && objectCtx == &gPlay->objectCtx)
            return enemyLoadNativeRoomObject(objectCtx, objectId);
#endif
        return -1;
    }
#endif

    /* Sanity checks */
    switch (objectId)
    {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x14:
    case 0x15:
        return -1;
    }

    return objectGetExtendedSlot(objectId);
}

int comboEnemyObjectSlot(u16 objectId)
{
    if (!(objectId & 0x3000) || !(objectId & 0x0fff) || !comboCheckedObjectData(objectId))
        return -1;
    return objectGetExtendedSlot(objectId);
}

void* comboEnemySceneObject(u16 objectId)
{
    int slot = comboEnemyObjectSlot(objectId);
    return slot >= EX_OBJECT_SLOTS_NORMAL ? gExObjectsAddr[slot] : NULL;
}

int Object_IsLoadedWrapper(ObjectContext* objectCtx, int slot)
{
    if (!objectCtx || slot < 0 || slot >= EX_OBJECT_SLOTS_TOTAL || objectCtx->num > EX_OBJECT_SLOTS_NORMAL)
        return 0;
    if (slot < EX_OBJECT_SLOTS_NORMAL)
        return slot < objectCtx->num && _Object_IsLoaded(objectCtx, slot);
    return gExObjectsAddr[slot] != NULL;
}

void* comboObjectSlotSegmentAndPin(ObjectContext* ctx, int slot)
{
    if (!Object_IsLoadedWrapper(ctx, slot))
        return NULL;
    if (slot >= EX_OBJECT_SLOTS_NORMAL)
    {
        comboEnemyPinObjectSlot(slot, 3);
        return gExObjectsAddr[slot];
    }
#if defined(GAME_OOT)
    return ctx->status[slot].segment;
#else
    return ctx->slots[slot].segment;
#endif
}

static void comboActorSetObjectSegment(PlayState* play, Actor* actor)
{
    if (!play || !actor)
        return;
    int slot = actor->objectSlot;
    if (slot >= 0 && slot < EX_OBJECT_SLOTS_NORMAL)
    {
#if defined(GAME_OOT)
        gSegments[6] = (u32)play->objectCtx.status[slot].segment & 0x1fffffff;
#else
        gSegments[6] = (u32)play->objectCtx.slots[slot].segment & 0x1fffffff;
#endif
        return;
    }
    void* segment = comboObjectSlotSegmentAndPin(&play->objectCtx, slot);
    if (!segment)
        g.roomEnemyLackSoul = 1;
    gSegments[6] = (u32)segment & 0x1fffffff;
}

#if defined(GAME_OOT)
PATCH_FUNC(0x80020fa4, comboActorSetObjectSegment);
#else
PATCH_FUNC(0x800b67fc, comboActorSetObjectSegment);
#endif

#if defined(GAME_OOT)

static void comboActorSetObjectSegmentWithRSP(PlayState* play, Actor* actor)
{
    comboActorSetObjectSegment(play, actor);

    OPEN_DISPS(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x06, gSegments[6] + 0x80000000);
    gSPSegment(POLY_XLU_DISP++, 0x06, gSegments[6] + 0x80000000);
    CLOSE_DISPS();
}

PATCH_CALL(0x80024394, comboActorSetObjectSegmentWithRSP);

void comboAfterRoomObjectLoad(void);
ALIGNED(16) static const u32 kNativeObjectBudgetOotV1[] = {
    0x4e4f4231,
    1,
    (u32)comboAfterRoomObjectLoad,
};
EXPORT_SYMBOL(FOREIGN_ENEMY_NATIVE_OBJECT_BUDGET_OOT_V1, kNativeObjectBudgetOotV1);

void comboAfterRoomObjectLoad(void)
{
    for (int i = 0; i < EX_OBJECT_SLOTS_NORMAL; ++i)
        gExObjectsAddr[i] = gPlay->objectCtx.status[i].segment;
}

#endif
