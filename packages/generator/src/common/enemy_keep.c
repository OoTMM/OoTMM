#include <combo.h>
#include <combo/dma.h>
#include <combo/enemy.h>
#include <combo/enemy_keep.h>
#include <combo/global.h>
#include <combo/enemy_native.h>
#include <combo/enemy_room_models.h>
#include <combo/enemy_resource.h>

#ifndef EnemyKeepAlloc
#if defined(GAME_OOT)
#define EnemyKeepAlloc ((void* (*)(u32))0x80066c10)
#else
#define EnemyKeepAlloc ((void* (*)(u32))0x80102c60)
#endif
#endif
#ifndef EnemyKeepFree
#if defined(GAME_OOT)
#define EnemyKeepFree ((void (*)(void*))0x80066c90)
#else
#define EnemyKeepFree ((void (*)(void*))0x80102ce0)
#endif
#endif

#define KEEP_ENTRY_LIMIT    128
#define KEEP_ASSET_LIMIT    0x10000u
#define KEEP_TOTAL_LIMIT    0x20000u
#define KEEP_METADATA_LIMIT 0x10000u
#define KEEP_DEPTH_LIMIT    32

typedef struct
{
    u32 objectId, offset, size, crc, fixupOffset, fixupCount;
} KeepEntry;

typedef struct
{
    void* data;
    u32 state;
} KeepState;

static u8* sKeepMetadata;
static KeepState* sKeepStates;
static u32 sKeepCount, sKeepConfig;
static u32 sKeepAssetsVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
#define KEEP_NATIVE_GAME 0
EXPORT_SYMBOL(FOREIGN_ENEMY_KEEP_ASSETS_OOT, sKeepAssetsVrom);
#else
#define KEEP_NATIVE_GAME 1
EXPORT_SYMBOL(FOREIGN_ENEMY_KEEP_ASSETS_MM, sKeepAssetsVrom);
#endif

void comboEnemyKeepReset(void)
{
    if (sKeepStates)
        for (u32 i = 0; i < sKeepCount; ++i)
            if (sKeepStates[i].data)
                EnemyKeepFree(sKeepStates[i].data);
    if (sKeepMetadata)
        EnemyKeepFree(sKeepMetadata);
    sKeepMetadata = NULL;
    sKeepStates = NULL;
    sKeepCount = sKeepConfig = 0;
}

static int keepLoadMetadata(void)
{
    DmaEntry file;
    u32 header[4], size, count, tableEnd, total = 0;
    u8* data;
    KeepEntry* rows;
    if (sKeepConfig)
        return sKeepConfig == sKeepAssetsVrom && sKeepMetadata != NULL;
    if (!sKeepAssetsVrom)
        return 0;
    sKeepConfig = sKeepAssetsVrom;
    if (sKeepAssetsVrom < 0x08000000 || !comboDmaLookup(&file, sKeepAssetsVrom) ||
        file.vstart != sKeepAssetsVrom || file.vend < file.vstart || file.pend)
        return 0;
    size = file.vend - file.vstart;
    if (size < sizeof(header) || size > KEEP_METADATA_LIMIT ||
        comboDmaLoadFilePartial(header, sKeepAssetsVrom, 0, sizeof(header)) != sizeof(header) ||
        header[0] != 0x454b5041 || header[1] != 1 || header[2] != KEEP_NATIVE_GAME ||
        header[3] > KEEP_ENTRY_LIMIT)
        return 0;
    count = header[3];
    tableEnd = sizeof(header) + count * sizeof(KeepEntry);
    if (tableEnd > size)
        return 0;
    data = EnemyKeepAlloc(((size + 15) & ~15) + count * sizeof(KeepState));
    if (!data)
        return 0;
    if (comboDmaLoadFilePartial(data, sKeepAssetsVrom, 0, size) != size || memcmp(data, header, sizeof(header)))
        goto invalid;
    rows = (KeepEntry*)(data + sizeof(header));
    for (u32 i = 0; i < count; ++i)
    {
        const ObjectData* object;
        KeepEntry* row = &rows[i];
        if (row->objectId < 0x1001 || row->objectId > 0x1003 || !row->size || row->size > KEEP_ASSET_LIMIT ||
            row->offset >= 0x1000000 || row->size > 0x1000000 - row->offset ||
            total > KEEP_TOTAL_LIMIT - row->size)
            goto invalid;
        total += row->size;
        object = comboGetObjectData(row->objectId);
        if (!object || !object->vromStart || object->vromEnd < object->vromStart ||
            row->offset > object->vromEnd - object->vromStart || row->size > object->vromEnd - object->vromStart - row->offset)
            goto invalid;
        if (i && (rows[i - 1].objectId > row->objectId ||
                  (rows[i - 1].objectId == row->objectId && rows[i - 1].offset + rows[i - 1].size > row->offset)))
            goto invalid;
        if (row->fixupCount && ((row->fixupOffset & 3) || row->fixupOffset < tableEnd || row->fixupOffset > size ||
                                row->fixupCount > (size - row->fixupOffset) / 12))
            goto invalid;
        for (u32 f = 0; f < row->fixupCount; ++f)
        {
            const u32* fix = (const u32*)(data + row->fixupOffset + f * 12);
            if ((fix[0] & 3) || row->size < 4 || fix[0] > row->size - 4 ||
                fix[1] < 0x1001 || fix[1] > 0x1003 || fix[2] >= 0x1000000 ||
                (f && fix[0] <= fix[-3]))
                goto invalid;
        }
    }
    sKeepMetadata = data;
    sKeepStates = (KeepState*)(data + ((size + 15) & ~15));
    memset(sKeepStates, 0, count * sizeof(*sKeepStates));
    sKeepCount = count;
    return 1;
invalid:
    EnemyKeepFree(data);
    return 0;
}

static int keepFind(u32 object, u32 offset)
{
    KeepEntry* rows = (KeepEntry*)(sKeepMetadata + 16);
    for (u32 i = 0; i < sKeepCount; ++i)
        if (rows[i].objectId == object && offset >= rows[i].offset && offset - rows[i].offset < rows[i].size)
            return i;
    return -1;
}

static void* keepLoadEntry(u32 index, u32 depth)
{
    KeepEntry* row = &((KeepEntry*)(sKeepMetadata + 16))[index];
    KeepState* state = &sKeepStates[index];
    const ObjectData* object;
    DmaEntry file;
    u8* data;
    if (state->state == 2)
        return state->data;
    if (state->state || depth >= KEEP_DEPTH_LIMIT)
        return NULL;
    state->state = 1;
    object = comboGetObjectData(row->objectId);
    if (!object || !object->vromStart || object->vromEnd < object->vromStart ||
        !comboDmaLookup(&file, object->vromStart) || file.vstart != object->vromStart ||
        file.vend != object->vromEnd || file.pend || !file.pstart ||
        file.vend - file.vstart > 0x04000000u ||
        file.pstart > 0x04000000u - (file.vend - file.vstart) ||
        row->offset > file.vend - file.vstart || row->size > file.vend - file.vstart - row->offset)
        goto invalid;
    data = EnemyKeepAlloc(row->size);
    if (!data)
        goto invalid;
    if (comboDmaLoadFilePartial(data, object->vromStart, row->offset, row->size) != row->size ||
        comboEnemyCrc32(data, row->size) != row->crc)
        goto invalidData;
    for (u32 f = 0; f < row->fixupCount; ++f)
    {
        const u32* fix = (const u32*)(sKeepMetadata + row->fixupOffset + f * 12);
        u32* word = (u32*)(data + fix[0]);
        int dependency = keepFind(fix[1], fix[2]);
        u8* target;
        if (*word != ((fix[1] == 0x1001 ? 4u : 5u) << 24 | fix[2]) || dependency < 0)
            goto invalidData;
        target = keepLoadEntry(dependency, depth + 1);
        if (!target)
            goto invalidData;
        target += fix[2] - ((KeepEntry*)(sKeepMetadata + 16))[dependency].offset;
        *word = (u32)target & 0x1fffffff;
    }
    if (row->fixupCount)
        osWritebackDCache(data, row->size);
    state->data = data;
    state->state = 2;
    return data;
invalidData:
    EnemyKeepFree(data);
invalid:
    state->state = 3;
    return NULL;
}

void* comboEnemyKeepAsset(u16 objectId, u32 offset)
{
    int index;
    u8* data;
#if defined(GAME_MM)
    if (objectId == 0x1001 && gPlay)
    {
        u32 nativeOffset = 0;
        if ((offset >= 0x529d0 && offset < 0x52ad0) || (offset >= 0x52b90 && offset < 0x53b90))
            nativeOffset = offset + 0x2ab80;
        else if (offset >= 0x572e0 && offset < 0x592e0)
            nativeOffset = offset + 0x36900;
        if (nativeOffset)
        {
            ObjectContext* ctx = &gPlay->objectCtx;
            u32 slot = ctx->mainKeepIndex;
            if (slot < ctx->num && slot < ARRAY_COUNT(ctx->slots) &&
                ctx->slots[slot].id == OBJECT_GAMEPLAY_KEEP && ctx->slots[slot].segment)
                return (u8*)ctx->slots[slot].segment + nativeOffset;
        }
    }
#endif
    if (objectId < 0x1001 || objectId > 0x1003 || !keepLoadMetadata())
        return NULL;
    index = keepFind(objectId, offset);
    if (index < 0)
        return NULL;
    data = keepLoadEntry(index, 0);
    return data ? data + offset - ((KeepEntry*)(sKeepMetadata + 16))[index].offset : NULL;
}

#if defined(GAME_OOT)
#define NativeSlots(ctx) ((ctx)->status)
#else
#define NativeSlots(ctx) ((ctx)->slots)
#endif

static void* enemyNativeKeep(ObjectContext* ctx, u32 index, int main)
{
    ObjectStatus* slot;
    if (index >= ctx->num || index >= ARRAY_COUNT(NativeSlots(ctx)))
        return NULL;
    slot = &NativeSlots(ctx)[index];
    if ((main && slot->id != 1) || (!main && slot->id != 2 && slot->id != 3) || !slot->segment)
        return NULL;
    if (Object_GetSlot(ctx, slot->id) != index || !Object_IsLoaded(ctx, index))
        return NULL;
    return slot->segment;
}

int comboEnemyNativeEnter(PlayState* play, EnemyNativeSegments* scope, u32 required)
{
    void* main;
    void* sub;
    if (!play || !scope || scope->active || (required & ~(ENEMY_NATIVE_KEEP_MAIN | ENEMY_NATIVE_KEEP_SUB)))
    {
        g.roomEnemyLackSoul = 1;
        return 0;
    }
    main = enemyNativeKeep(&play->objectCtx, play->objectCtx.mainKeepIndex, 1);
    sub = enemyNativeKeep(&play->objectCtx, play->objectCtx.subKeepIndex, 0);
    if (((required & ENEMY_NATIVE_KEEP_MAIN) && !main) || ((required & ENEMY_NATIVE_KEEP_SUB) && !sub))
    {
        g.roomEnemyLackSoul = 1;
        return 0;
    }
    scope->saved[0] = gSegments[4];
    scope->saved[1] = gSegments[5];
    scope->active = 1;
    gSegments[4] = (u32)main & 0x1fffffff;
    gSegments[5] = (u32)sub & 0x1fffffff;
    return 1;
}

void comboEnemyNativeLeave(EnemyNativeSegments* scope)
{
    if (!scope || !scope->active)
        return;
    gSegments[4] = scope->saved[0];
    gSegments[5] = scope->saved[1];
    scope->active = 0;
}

#ifndef EnemyRoomModelsAlloc
#if defined(GAME_OOT)
#define EnemyRoomModelsAlloc ((void* (*)(u32))0x80066c10)
#else
#define EnemyRoomModelsAlloc ((void* (*)(u32))0x80102c60)
#endif
#endif
#ifndef EnemyRoomModelsFree
#if defined(GAME_OOT)
#define EnemyRoomModelsFree ((void (*)(void*))0x80066c90)
#else
#define EnemyRoomModelsFree ((void (*)(void*))0x80102ce0)
#endif
#endif

typedef struct
{
    u32 scene, room, first, count;
} RoomModelRow;

static u32* sRoomModelMetadata;
static u32 sRoomModelConfig, sRoomModelCount, sRoomModelVersion;
static int sRoomModelOnlyOom;
static u32 sRoomModelsVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
#define ROOM_MODELS_GAME 0
EXPORT_SYMBOL(ENEMY_ROOM_MODELS_OOT, sRoomModelsVrom);
EXPORT_SYMBOL(ENEMY_NATIVE_ROOM_MODELS_OOT, comboEnemyRoomHasNativeModel);
#else
#define ROOM_MODELS_GAME 1
EXPORT_SYMBOL(ENEMY_ROOM_MODELS_MM, sRoomModelsVrom);
EXPORT_SYMBOL(ENEMY_NATIVE_ROOM_MODELS_MM, comboEnemyRoomHasNativeModel);
#endif

void comboEnemyRoomModelsReset(void)
{
    if (sRoomModelMetadata)
        EnemyRoomModelsFree(sRoomModelMetadata);
    sRoomModelMetadata = NULL;
    sRoomModelConfig = sRoomModelCount = 0;
    sRoomModelVersion = 0;
    sRoomModelOnlyOom = 0;
}

static int roomModelsLoad(void)
{
    DmaEntry file;
    u32 header[4], size, count, first = 0, modelCount;
    u32* data;
    RoomModelRow* rows;
    u32* models;
    if (sRoomModelConfig)
        return sRoomModelConfig == sRoomModelsVrom && sRoomModelMetadata != NULL;
    sRoomModelConfig = sRoomModelsVrom;
    if (!comboDmaLookup(&file, sRoomModelsVrom) || file.vstart != sRoomModelsVrom || file.pend || file.vend < file.vstart)
        return 0;
    size = file.vend - file.vstart;
    if (size < 16 || size > 0x10000 || (size & 3) ||
        comboDmaLoadFilePartial(header, sRoomModelsVrom, 0, 16) != 16 ||
        header[0] != 0x45524d31 || (header[1] != 1 && header[1] != 2) || header[2] != ROOM_MODELS_GAME || header[3] > 4096 ||
        header[3] > (size - 16) / sizeof(RoomModelRow))
        return 0;
    count = header[3];
    modelCount = (size - 16 - count * sizeof(RoomModelRow)) / 4;
    data = EnemyRoomModelsAlloc(size);
    if (!data)
        return 0;
    if (comboDmaLoadFilePartial(data, sRoomModelsVrom, 0, size) != size || memcmp(data, header, 16))
        goto invalid;
    rows = (RoomModelRow*)(data + 4);
    models = (u32*)(rows + count);
    for (u32 i = 0; i < count; ++i)
    {
        RoomModelRow* row = &rows[i];
        if (row->scene > 112 || (row->room > 31 && (ROOM_MODELS_GAME != 1 || row->scene != 7 || row->room < 0x113 || row->room > 0x11f)) || row->first != first ||
            row->count > 128 - OBJECT_EXCHANGE_BANK_MAX || row->count > modelCount - first ||
            (i && (rows[i - 1].scene > row->scene || (rows[i - 1].scene == row->scene && rows[i - 1].room >= row->room))))
            goto invalid;
        for (u32 j = 0; j < row->count; ++j)
        {
            u32 word = models[first + j];
            u32 id = word & 0xffff;
            u32 days = word >> 16;
            if (id <= 3 || (id >= 0x1000 && id < 0x1004) || id >= 0x2080 ||
                (header[1] == 1 ? days != 0 : (!days || days > 0x3ff)) ||
                (j && (models[first + j - 1] & 0xffff) >= id))
                goto invalid;
        }
        first += row->count;
    }
    if (first != modelCount)
        goto invalid;
    sRoomModelMetadata = data;
    sRoomModelCount = count;
    sRoomModelVersion = header[1];
    return 1;
invalid:
    EnemyRoomModelsFree(data);
    return 0;
}

int comboEnemyRoomModelsOnlyOom(void)
{
    return sRoomModelOnlyOom;
}

static const RoomModelRow* roomModelRowFor(PlayState* play, s32 room, const u32** modelsOut)
{
    u32 low = 0, high;
    const RoomModelRow* rows;
    if (!play || room < 0 || room > 31 ||
        sRoomModelsVrom < 0x08000000 || !roomModelsLoad())
        return NULL;
#if defined(GAME_MM)
    if (play->sceneId == SCE_MM_GROTTOS && room == 4 && (gGrottoData & 0x1f) >= 0x13)
        room = 0x100 | (gGrottoData & 0x1f);
#endif
    rows = (RoomModelRow*)(sRoomModelMetadata + 4);
    *modelsOut = (u32*)(rows + sRoomModelCount);
    high = sRoomModelCount;
    while (low < high)
    {
        u32 middle = low + (high - low) / 2;
        const RoomModelRow* row = &rows[middle];
        if (row->scene < (u32)play->sceneId || (row->scene == (u32)play->sceneId && row->room < (u32)room))
            low = middle + 1;
        else
            high = middle;
    }
    if (low == sRoomModelCount || rows[low].scene != (u32)play->sceneId || rows[low].room != (u32)room)
        return NULL;
    return &rows[low];
}

static const RoomModelRow* roomModelRow(PlayState* play, const u32** modelsOut)
{
    return roomModelRowFor(play, play ? play->roomCtx.curRoom.num : -1, modelsOut);
}

static const u32* roomModelFind(const RoomModelRow* row, const u32* models, u16 objectId)
{
    if (row)
    {
        for (u32 i = 0; i < row->count; ++i)
        {
            const u32* model = &models[row->first + i];
            if ((u16)*model == objectId)
                return model;
        }
    }
    return NULL;
}

u16 comboEnemyRoomModelHalfDays(PlayState* play, s32 room, u16 objectId)
{
    const u32* models = NULL;
    const RoomModelRow* row = roomModelRowFor(play, room, &models);
    if (sRoomModelVersion < 2)
        return 0x3ff;
    const u32* model = roomModelFind(row, models, objectId);
    return model ? *model >> 16 : 0x3ff;
}
#if defined(GAME_MM)
EXPORT_SYMBOL(ENEMY_ROOM_MODEL_SCHEDULE_MM_V2, comboEnemyRoomModelHalfDays);
#endif

int comboEnemyRoomHasNativeModel(PlayState* play, u16 objectId)
{
    const u32* models = NULL;
    const RoomModelRow* row = roomModelRow(play, &models);
    return roomModelFind(row, models, objectId) != NULL;
}

int comboEnemyRoomModelsPreload(PlayState* play)
{
    const u32* models;
    int okay = 1, onlyOom = 1;
    sRoomModelOnlyOom = 0;
    comboEnemyObjectPreloadBegin(play ? play->roomCtx.curRoom.num : -1);
    if (!play)
        goto invalid;
    comboEnemyObjectReleaseUnloadedRooms(play);
    if (!sRoomModelsVrom)
        return 1;
    if (sRoomModelsVrom < 0x08000000 || !roomModelsLoad())
        goto invalid;
    if (play->roomCtx.curRoom.num < 0)
        return 1;
    if (play->roomCtx.curRoom.num > 31)
        goto invalid;
    const RoomModelRow* row = roomModelRow(play, &models);
    if (!row)
        return 1;
    u16 order[128];
    u32 sizes[128], count = 0;
    if (row->count > ARRAY_COUNT(order))
        goto invalid;
    for (u32 i = 0; i < row->count; ++i)
    {
        u32 model = models[row->first + i];
        u16 objectId = model;
#if defined(GAME_MM)
        u16 days = sRoomModelVersion < 2 ? 0x3ff : model >> 16;
        if (play->actorCtx.halfDaysBit && !(days & play->actorCtx.halfDaysBit))
            continue;
#endif
        u32 size = comboLoadObject(NULL, objectId);
        if (!size)
            goto invalid;
        u32 j = count++;
        while (j && sizes[j - 1] < size)
        {
            order[j] = order[j - 1];
            sizes[j] = sizes[j - 1];
            --j;
        }
        order[j] = objectId;
        sizes[j] = size;
    }
    for (u32 i = 0; i < count; ++i)
    {
        u16 objectId = order[i];
        if (!comboEnemyObjectPreload(objectId, play->roomCtx.curRoom.num))
        {
            okay = 0;
            if (!comboEnemyObjectPreloadOom(objectId, play->roomCtx.curRoom.num))
                onlyOom = 0;
        }
    }
    if (okay)
        return 1;
    sRoomModelOnlyOom = onlyOom;
invalid:
    g.roomEnemyLackSoul = 1;
    return 0;
}

#if defined(GAME_OOT)
#define NativeResourceSpawnEntry ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
#else
#define NativeResourceSpawnEntry ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
#endif

static s16 enemyResourceObject(s16 actorId)
{
#if defined(GAME_OOT)
    return 0x39;
#else
    if (actorId == 0x208)
        return 0x1f1;
    if (actorId == 0x21d)
        return 0x204;
    return actorId == 0x66 || actorId == 0x33 ? 0x31 : 0x40;
#endif
}

int comboEnemyNativeResourceInit(EnemyNativeResource* state, const ActorEntry* original, s32 room)
{
#if defined(GAME_OOT)
    int baba, fighter;
#else
    s16 actorId;
    u16 params;
#endif
    if (!state)
        return 0;
    memset(state, 0, sizeof(*state));
    if (!original || room < 0 || room >= 32)
        return 0;
#if defined(GAME_OOT)
    baba = (original->id == 0xc7 && (original->params == 0 || original->params == 1 || (u16)original->params == 0xffff)) ||
           (original->id == 0x55 && (original->params == 0 || original->params == 1));
    fighter = original->id == 0x197 &&
              ((room == 1 && original->params == 0x0a00 && original->rot.z == 16) ||
               (room == 2 && original->params == 0x0c00 && original->rot.z == 17) ||
               (room == 4 && original->params == 0x0e00 && original->rot.z == 18) ||
               (room == 5 && original->params == 0x0f00 && original->rot.z == 19));
    if (!baba && !fighter)
        return 0;
#else
    actorId = original->id & 0x1fff;
    params = original->params;
    u16 halfDays = ((original->rot.x & 7) << 7) | (original->rot.z & 0x7f);
    int legacy = (actorId == 0x33 && params == 0) || (actorId == 0x66 && (params == 1 || params == 2)) ||
                 (actorId == 0x3b && (params == 0xff00 || params == 0xff02));
    int fight = (actorId == 0x208 && room == 12 && params == 0xff00 && (original->rot.y & 0x7f) == 17) ||
                (actorId == 0x21d && ((room == 0 && params == 0x024b) || (room == 1 && params == 0x02cb) || (room == 2 && params == 0x020b)) && (original->rot.y & 0x7f) == 19);
    if ((halfDays != 0 && halfDays != 0x3ff) || (!legacy && !fight) ||
        (legacy && (original->rot.y & 0x7f) != 0x7f))
        return 0;
#endif
    state->room = room;
    state->state = ENEMY_RESOURCE_WAITING;
    state->original = *original;
    state->actorIndex = gCurrentSpawnActorNum;
    return 1;
}

EXPORT_SYMBOL(ENEMY_NATIVE_FIGHT_RESTORE_V1, comboEnemyNativeResourceInit);

#if defined(GAME_OOT)
static int enemyGerudoFightComplete(PlayState* play, const EnemyNativeResource* state)
{
    u16 key = (u16)state->original.params >> 8;
    u16 rescued;
    switch (key)
    {
    case 0x0a:
        rescued = EV_OOT_CHK_CARPENTER_4;
        break;
    case 0x0c:
        rescued = EV_OOT_CHK_CARPENTER_1;
        break;
    case 0x0e:
        rescued = EV_OOT_CHK_CARPENTER_2;
        break;
    case 0x0f:
        rescued = EV_OOT_CHK_CARPENTER_3;
        break;
    default:
        return 0;
    }
    return Config_Flag(CFG_OOT_CARPENTERS_NONE) ||
           (Config_Flag(CFG_OOT_CARPENTERS_ONE) && key != 0x0c) ||
           GetEventChk(rescued) || Flags_GetCollectible(play, key);
}
#endif

int comboEnemyNativeResourceAlreadyComplete(PlayState* play, const EnemyNativeResource* state)
{
    if (!play || !state)
        return 0;
    if (Flags_GetClear(play, state->room))
        return 1;
#if defined(GAME_OOT)
    return state->original.id == ACTOR_EN_GELDB && enemyGerudoFightComplete(play, state);
#else
    s16 actorId = state->original.id & 0x1fff;
    u16 params = state->original.params;
    if (actorId == 0x21d)
    {
        s32 flag = (params >> 6) & 0x7f;
        return flag != 0x7f && Flags_GetSwitch(play, flag);
    }
    if (actorId == 0x208)
    {
        s32 flag = params >> 8;
        return flag != 0xff && Flags_GetSwitch(play, flag);
    }
    return 0;
#endif
}

void comboEnemyNativeResourceDefeated(EnemyNativeResource* state)
{
    if (state && state->state == ENEMY_RESOURCE_WAITING)
        state->state = ENEMY_RESOURCE_PENDING;
}

void comboEnemyNativeResourceUpdate(PlayState* play, EnemyNativeResource* state)
{
    if (!play || !state || state->state != ENEMY_RESOURCE_PENDING ||
        play->roomCtx.curRoom.num != state->room)
        return;

    ActorEntry original = state->original;
    s16 actorId = original.id;
    Actor* actor;
#if defined(GAME_MM)
    actorId &= 0x1fff;
#endif
#if defined(GAME_MM)
    if (actorId == ACTOR_EN_KAIZOKU)
    {
        s32 flag = ((u16)original.params >> 6) & 0x7f;
        if (flag != 0x7f)
            Flags_SetSwitch(play, flag);
        state->state = ENEMY_RESOURCE_RESTORED;
        return;
    }
#endif
#if defined(GAME_OOT)
    if (actorId == ACTOR_EN_GELDB)
    {
        if (enemyGerudoFightComplete(play, state))
        {
            state->state = ENEMY_RESOURCE_RESTORED;
            return;
        }
        Vec3f pos = { original.pos.x, original.pos.y, original.pos.z };
        actor = Item_DropCollectible(play, &pos, (original.params & 0x3f00) | 0x11);
    }
    else
#endif
    {
        s32 slot = Object_GetSlot(&play->objectCtx, enemyResourceObject(actorId));
        if (slot < 0 || !Object_IsLoaded(&play->objectCtx, slot))
            return;
        u8 previousIndex = gCurrentSpawnActorNum;
        gCurrentSpawnActorNum = state->actorIndex;
        actor = NativeResourceSpawnEntry(&play->actorCtx, &original, play);
        gCurrentSpawnActorNum = previousIndex;
    }
    if (!actor || !actor->update)
        return;
    actor->room = state->room;
    state->state = ENEMY_RESOURCE_RESTORED;
#if defined(GAME_OOT)
    if (actorId == ACTOR_EN_GELDB)
        PlaySound(NA_SE_SY_TRE_BOX_APPEAR);
#endif
}
