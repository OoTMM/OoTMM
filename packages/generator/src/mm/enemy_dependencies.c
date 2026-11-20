#include <combo.h>
#include <combo/dma.h>
#include <combo/enemy.h>
#include <combo/enemy_placement.h>
#include <combo/souls.h>

typedef struct
{
    u32 key;
    Vec3s pos;
    u16 params;
    s16 yaw;
    u8 hostSlot;
    u8 type;
} EnemyDependency;

_Static_assert(sizeof(EnemyDependency) == 16, "MM room dependency ABI");

typedef struct
{
    Vec3f lastPos;
    u16 params;
    s16 yaw;
    u8 hostSlot, type;
    u8 room, slot, seen, done;
} EnemyDependencyState;

_Static_assert(sizeof(EnemyDependencyState) == 24, "Compact runtime dependency state");
static EnemyDependencyState sDependencies[64];
static u32 sDependencyVrom __attribute__((section(".data"))) = 0;
EXPORT_SYMBOL(ENEMY_MM_ROOM_DEPENDENCIES_V1, sDependencyVrom);
extern u8 gCurrentSpawnActorNum;
s16 comboEnemyMmStandaloneMinibossSlot(s16, u16);

void comboEnemyKeetaGuardsReset(void);

void comboEnemyDependenciesReset(void)
{
    memset(sDependencies, 0, sizeof(sDependencies));
    comboEnemyKeetaGuardsReset();
}

void comboEnemyDependenciesBeginRoom(PlayState* play, ActorEntry* list, u32 count)
{
    s32 room = play->roomCtx.curRoom.num;
    if (!sDependencyVrom || !list || !count || room < 0 || room >= play->numRooms || !play->roomList)
        return;
    for (u32 i = 0; i < ARRAY_COUNT(sDependencies); ++i)
    {
        EnemyDependencyState* state = &sDependencies[i];
        if (state->room == room || state->room != (u8)play->roomCtx.prefRoom.num)
            state->type = 0;
    }
    u32 segment = (u32)play->roomCtx.curRoom.segment;
    RomFile* files = play->roomList;
    u32 start = (u32)files[room].vromStart, end = (u32)files[room].vromEnd;
    if (!segment || (u32)list < segment || end <= start || count > 255 ||
        count * sizeof(*list) > end - start || (u32)list - segment > end - start - count * sizeof(*list))
        return;
    u32 key = start + (u32)list - segment;
    DmaEntry file;
    u32 header[2];
    if (!comboDmaLookup(&file, sDependencyVrom) || file.vstart != sDependencyVrom || file.pend ||
        file.vend - file.vstart < 8 || comboDmaLoadFilePartial(header, sDependencyVrom, 0, 8) != 8 ||
        header[0] != 0x45445031 || header[1] > (file.vend - file.vstart - 8) / 16)
        return;
    for (u32 i = 0; i < header[1]; ++i)
    {
        EnemyDependency data;
        if (comboDmaLoadFilePartial(&data, sDependencyVrom, 8 + i * 16, 16) != 16)
            return;
        if (data.key < key || data.key >= key + count * sizeof(*list) || (data.key - key) % sizeof(*list))
            continue;
        if (data.type < 1 || data.type > 4 || data.hostSlot >= count)
            continue;
        for (u32 j = 0; j < ARRAY_COUNT(sDependencies); ++j)
        {
            EnemyDependencyState* state = &sDependencies[j];
            if (state->type)
                continue;
            *state = (EnemyDependencyState){
                .lastPos = { data.pos.x, data.pos.y, data.pos.z },
                .params = data.params,
                .yaw = data.yaw,
                .hostSlot = data.hostSlot,
                .type = data.type,
                .room = room,
                .slot = (data.key - key) / sizeof(*list)
            };
            break;
        }
    }
}

void comboEnemyDependenciesSpawnActors(PlayState* play)
{
    u8 saved = gCurrentSpawnActorNum;
    gCurrentSpawnActorNum = 0xff;
    for (u32 i = 0; i < ARRAY_COUNT(sDependencies); ++i)
    {
        EnemyDependencyState* state = &sDependencies[i];
        if (state->type < 2 || state->type > 4 || state->done || state->room != (u8)play->roomCtx.curRoom.num)
            continue;
        int missingSoul = state->type == 3 && !comboHasSoulMm(GI_MM_SOUL_ENEMY_BIO_BABA);
        u16 params = state->type == 3 ? 2 : state->params;
        s16 id = state->type == 4 ? ACTOR_OBJ_ETCETERA : state->type == 3 ? ACTOR_BOSS_05 : ACTOR_OBJ_ICE_POLY;
        Actor* dependency = Actor_Spawn(&play->actorCtx, play, id,
                                        state->lastPos.x, state->lastPos.y, state->lastPos.z, 0, state->yaw, 0, params);
        if (dependency && (dependency->init || dependency->update))
        {
            dependency->room = state->room;
            state->done = 1;
        }
        if (missingSoul)
            state->done = 1;
    }
    gCurrentSpawnActorNum = saved;
}

/* A native scrub may also roll at a protected flower slot (including a
 * chaotic choice). Its death spawn reuses the flower we made at room load. */
Actor* comboEnemyExistingDekuFlower(PlayState* play, f32 x, f32 y, f32 z)
{
    for (u32 i = 0; i < ARRAY_COUNT(sDependencies); ++i)
    {
        EnemyDependencyState* state = &sDependencies[i];
        if (state->type != 4 || state->room != (u8)play->roomCtx.curRoom.num ||
            state->lastPos.x != x || state->lastPos.y != y || state->lastPos.z != z)
            continue;
        for (Actor* actor = play->actorCtx.actors[ACTORCAT_BG].first; actor; actor = actor->next)
            if (actor->id == ACTOR_OBJ_ETCETERA && ((actor->params & 0xff80) >> 7) <= 1 && (actor->init || actor->update) &&
                actor->room == state->room && actor->home.pos.x == x && actor->home.pos.y == y && actor->home.pos.z == z)
                return actor;
    }
    return NULL;
}

int comboEnemyFairyHost(Actor* fairy, PlayState* play, Vec3f* pos)
{
    for (u32 i = 0; i < ARRAY_COUNT(sDependencies); ++i)
    {
        EnemyDependencyState* state = &sDependencies[i];
        if (state->type != 1 || state->room != fairy->room || state->slot != fairy->actorIndex)
            continue;
        *pos = state->lastPos;
        int foreign = comboIsForeignEnemyActor(state->params);
        if (foreign && comboEnemyRoomSlotComplete(state->room, state->hostSlot))
            return 2;
        for (u32 cat = 0; cat < ACTORCAT_MAX; ++cat)
        {
            for (Actor* actor = play->actorCtx.actors[cat].first; actor; actor = actor->next)
            {
                if (actor->room != state->room || actor->actorIndex != state->hostSlot || actor->parent)
                    continue;
                u16 expected = comboEnemyMmStandaloneMinibossSlot(state->params, 0x807f);
                if (actor->id != state->params && actor->id != expected)
                    continue;
                if (actor->init)
                    return 0;
                if (actor->update)
                {
                    state->seen = 1;
                    state->lastPos = actor->world.pos;
                    state->lastPos.y += 30.f;
                    *pos = state->lastPos;
                    return 1;
                }
            }
        }
        return !foreign && (state->seen || Flags_GetClear(play, state->room)) ? 2 : 0;
    }
    return -1;
}
