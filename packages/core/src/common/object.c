#include <combo.h>
#include <combo/custom.h>
#include <combo/dma.h>

void* gCustomKeep;

#define OBJECT_COUNT    16
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

ALIGNED(16) ObjectData kCustomObjectsTable[32];

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
    { CUSTOM_OBJECT_ID_GI_STONE_RUBY, { 0x670, 0x938 } },
    { CUSTOM_OBJECT_ID_GI_STONE_SAPPHIRE, { 0x1308 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_FOREST, { 0x3a8, 0xe58 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_FIRE,   { 0x1e8, 0xc48 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_WATER,  { 0x368, 0xe18 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_SPIRIT, { 0x3b8, 0xe58 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_SHADOW, { 0x308, 0xd78 } },
    { CUSTOM_OBJECT_ID_GI_MEDALLION_LIGHT,  { 0x508, 0xf98 } },
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
    gCustomKeep = malloc(customKeepSize);
    comboDmaLoadFile(gCustomKeep, CUSTOM_KEEP_VROM);
}

#if defined(GAME_OOT)
static u32 comboGetTextureOverride(u32 texture)
{
    switch (texture)
    {
    case 0x447b0:
        return 0x32930;
    case 0x44bb0:
        return 0x32930; /* Inaccurate, find in hearts */
    case 0x44fb0:
        return 0x33130;
    case 0x54F20:
        return 0x39450;
    case 0x7DF10:
        return 0x53390;
    default:
        return 0x32930; /* Fine default */
    }
}
#endif

#if defined(GAME_MM)
static u32 comboGetTextureOverride(u32 texture)
{
    switch (texture)
    {
    case 0x32930:
        return 0x447b0;
    case 0x33130:
        return 0x44FB0;
    case 0x39450:
        return 0x54F20;
    case 0x53390:
        return 0x7DF10;
    default:
        return 0x447b0; /* Fine default */
    }
}
#endif

static void comboPatchForeignObject(void* buffer, u16 objectId)
{
    const ObjectPatch* patch;
    u16 offset;
    u32* texture;

    patch = NULL;
    for (u32 i = 0; i < ARRAY_SIZE(kObjectPatches); ++i)
    {
        if (kObjectPatches[i].objectId == objectId)
        {
            patch = &kObjectPatches[i];
            break;
        }
    }

    if (patch == NULL)
        return;

    for (u32 i = 0; i < ARRAY_SIZE(patch->offsets); ++i)
    {
        offset = patch->offsets[i];
        if (!offset)
            break;
        texture = (u32*)((u8*)buffer + offset + 4);
        *texture = 0x04000000 | comboGetTextureOverride((*texture) & 0xffffff);
    }
}

u32 comboLoadObject(void* buffer, u16 objectId)
{
    u32 vromStart;
    u32 vromEnd;
    const ObjectData* table;
    int isPatchable;
    u16 patchObjectId;

    if (!objectId)
        return 0;

    isPatchable = 0;
    patchObjectId = objectId & ~MASK_FOREIGN_OBJECT;
    if (objectId & 0x2000)
    {
        table = kCustomObjectsTable;
        isPatchable = 1;
    }
    else if (objectId & 0x1000)
    {
        table = kExtraObjectsTable;
        isPatchable = 1;
    }
    else
    {
        table = kObjectsTable;
    }

    objectId &= 0xfff;
    vromStart = table[objectId].vromStart;
    vromEnd = table[objectId].vromEnd;

    if (buffer)
    {
        LoadFile(buffer, vromStart, vromEnd - vromStart);
        if (isPatchable)
            comboPatchForeignObject(buffer, patchObjectId);
    }
    return vromEnd - vromStart;
}

void* comboGetObject(u16 objectId)
{
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
        if (sObjectsIds[i] == 0)
        {
            size = comboLoadObject(NULL, objectId);
#if defined(GAME_OOT)
            addr = malloc(size);
#else
            addr = ActorAlloc(size);
#endif
            comboLoadObject(addr, objectId);
            sObjectsIds[i] = objectId;
            sObjectsAddr[i] = addr;
            sObjectsTTL[i] = OBJECT_TTL;
            return addr;
        }
    }
    return NULL;
}

void comboObjectsReset(void)
{
    for (int i = 0; i < OBJECT_COUNT; ++i)
    {
#if defined(GAME_OOT)
        free(sObjectsAddr[i]);
#endif
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
#if defined(GAME_OOT)
            free(sObjectsAddr[i]);
#else
            ActorFree(sObjectsAddr[i]);
#endif
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
