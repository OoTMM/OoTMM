#include <combo.h>
#include <combo/enemy_scripted.h>
#include <combo/enemy.h>
#include <combo/enemy_placement.h>
#include <combo/enemy_resource.h>
#include <combo/global.h>
#include <combo/item.h>
#include <combo/actor.h>
#include <combo/mm/sequence.h>
#include <combo/data/sequence.h>
#include <combo/sfx_id.h>

int comboEnemyCombatActive(PlayState* play);

enum { ARENA_INIT, ARENA_ACTIVE, ARENA_HINT, ARENA_REWARD, ARENA_DONE };

typedef struct EnemyScriptedArena {
    EnemyScriptedBinding binding;
    struct EnemyScriptedArena* next;
    EnemyFamily family;
    u16 text;
    u8 phase, music, curtains, reward;
} EnemyScriptedArena;

typedef struct {
    Actor actor;
    u16 lightAngle;
    u8 state[0x153 - 0x146];
    u8 closeAction, lighting;
    u8 tail[0x68a - 0x155];
    s16 subCamId;
} CurtainOwner;

static CurtainOwner sCurtainOwner;
static EnemyScriptedArena* sCurtainArena;
static Actor* sCurtains[2];

ASSERT_OFFSET(CurtainOwner, lightAngle, 0x144);
ASSERT_OFFSET(CurtainOwner, closeAction, 0x153);
ASSERT_OFFSET(CurtainOwner, subCamId, 0x68a);
_Static_assert(sizeof(EnemyScriptedBinding) == 0x16c, "Scripted MM binding ABI");
_Static_assert(__builtin_offsetof(EnemyScriptedBinding, placementVrom) == 0x160, "Scripted MM lease ABI");
const u32 gEnemyScriptedLeaseAbiV1[] __attribute__((used)) = {
    0x45534c41, 1, sizeof(EnemyScriptedBinding),
    __builtin_offsetof(EnemyScriptedBinding, placementVrom),
    __builtin_offsetof(EnemyScriptedBinding, placementVisit),
    __builtin_offsetof(EnemyScriptedArena, next), sizeof(EnemyScriptedArena), 0x4c534531,
};
EXPORT_SYMBOL(ENEMY_SCRIPTED_MM_DRIVER_LEASE_ABI_V1, gEnemyScriptedLeaseAbiV1);

static s16 sourceId(const EnemyScriptedArena* arena)
{
    return arena->binding.original.id & 0x1fff;
}

static int supported(s16 id)
{
    return id == 7 || id == 0x65 || id == 0x12c || id == 0x115 || id == 0x182 || id == 0x184 ||
           id == 0x43 || id == 0x15d;
}

static s16 sourceSwitch(const EnemyScriptedArena* arena)
{
    u16 params = arena->binding.original.params;
    return sourceId(arena) == 0x184 ? (params >> 6) & 0x7f :
           sourceId(arena) == 0x15d ? params & 0x7f : 0x7f;
}

static int completed(EnemyScriptedArena* arena, PlayState* play)
{
    s16 flag = sourceSwitch(arena);
    return flag != 0x7f ? Flags_GetSwitch(play, flag) : Flags_GetClear(play, arena->binding.actor.room);
}

static int live(PlayState* play, Actor* wanted)
{
    for (int category = 0; category < ACTORCAT_MAX; ++category)
        for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
            if (actor == wanted)
                return 1;
    return 0;
}

static int curtains(EnemyScriptedArena* arena, PlayState* play)
{
    if (sourceId(arena) != 0x115)
        return 1;
    if (!arena->curtains) {
        if (sCurtainArena && sCurtainArena != arena)
            return 0;
        sCurtainArena = arena;
        memset(&sCurtainOwner, 0, sizeof(sCurtainOwner));
        sCurtainOwner.actor = arena->binding.actor;
        sCurtainOwner.closeAction = 2;
        play->envCtx.lightSettingOverride = 7;
        MM_SET_EVENT_INF(EVENTINF_INTRO_CS_WATCHED_IGOS_DU_IKANA);
        arena->curtains = 1;
    }
    sCurtainOwner.lightAngle = gSave.time;
    if (sCurtainOwner.lighting) {
        play->envCtx.lightSettingOverride = 0;
        play->envCtx.lightBlendOverride = 2;
        play->envCtx.lightSetting = sCurtainOwner.lighting == 1 ? 8 : 0;
        play->envCtx.prevLightSetting = sCurtainOwner.lighting == 1 ? 12 : 10;
        u16 time = gSave.time;
        if (time > 0x8000)
            time = 0xffff - time;
        play->envCtx.lightBlend = time * (2.f / 0x10000);
    }
    int ready = 1;
    for (int i = 0; i < 2; ++i) {
        if (!sCurtains[i])
            sCurtains[i] = Actor_SpawnAsChild(&play->actorCtx, &sCurtainOwner.actor, play, 0x12e,
                0.f, 0.f, 0.f, 0, 0, 0, i);
        if (!sCurtains[i])
            ready = 0;
    }
    return ready;
}

static void releasePlacement(EnemyScriptedArena* arena, PlayState* play)
{
    arena->binding.pending = 0;
    comboEnemyScriptedCancelPlacement(&arena->binding.actor, play);
}

static void clearRoom(EnemyScriptedArena* arena, PlayState* play)
{
    s16 flag = sourceSwitch(arena);
    if (flag != 0x7f)
        Flags_SetSwitch(play, flag);
    if (g.roomEnemyLackSoul || comboEnemyEncounterPending(arena->binding.actor.room))
        return;
    for (int category = ACTORCAT_ENEMY; category <= ACTORCAT_BOSS; ++category)
        if (category == ACTORCAT_ENEMY || category == ACTORCAT_BOSS)
            for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
                if (actor->room == arena->binding.actor.room && actor->update && actor->id != 0x12e)
                    return;
    Flags_SetClearTemp(play, arena->binding.actor.room);
    Flags_SetClear(play, arena->binding.actor.room);
}

static int frog(EnemyScriptedArena* arena, PlayState* play)
{
    static const u8 masks[] = { 0x40, 0x80, 1, 2 };
    s16 id = CLAMP(arena->binding.original.params, 1, 4);
    u8 index = id - 1;
    if (gSave.info.weekEventReg[index < 2 ? 32 : 33] & masks[index])
        return 1;
    Actor* actor = &arena->binding.actor;
    return Actor_Spawn(&play->actorCtx, play, ACTOR_EN_MINIFROG, actor->world.pos.x, actor->world.pos.y,
        actor->world.pos.z, 0, actor->shape.rot.y, 0, id) != NULL;
}

static void ArenaInit(Actor* actor, PlayState* play)
{
    ((EnemyScriptedArena*)actor)->binding.pending = 1;
}

static void ArenaDestroy(Actor* actor, PlayState* play)
{
    EnemyScriptedArena* arena = (EnemyScriptedArena*)actor;
    if (sCurtainArena == arena) {
        for (int i = 0; i < 2; ++i) {
            if (live(play, sCurtains[i])) {
                if (sCurtains[i]->child && live(play, sCurtains[i]->child))
                    Actor_Kill(sCurtains[i]->child);
                Actor_Kill(sCurtains[i]);
            }
            sCurtains[i] = NULL;
        }
        sCurtainArena = NULL;
    }
    if (arena->music)
        Audio_RestorePrevBgm();
    comboEnemyFamilyClear(&arena->family);
}

static void ArenaUpdate(Actor* actor, PlayState* play)
{
    EnemyScriptedArena* arena = (EnemyScriptedArena*)actor;
    if (!comboEnemyNativeScriptedCurrent(actor, play) || actor->room != play->roomCtx.curRoom.num ||
        arena->binding.binding != ENEMY_SCRIPTED_MAGIC || !supported(sourceId(arena)))
        return;
    if (!curtains(arena, play))
        return;
    if (arena->phase == ARENA_INIT) {
        if (completed(arena, play)) {
            releasePlacement(arena, play);
            arena->phase = ARENA_REWARD;
            arena->reward = 2;
        } else {
            CollisionPoly* floor = NULL;
            s32 bgId;
            Vec3f pos = actor->world.pos;
            pos.y += 100.f;
            f32 y = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, actor, &pos);
            if (floor && y != BGCHECK_Y_MIN)
                actor->world.pos.y = y + 8.f;
            arena->phase = ARENA_ACTIVE;
        }
    }
    if (arena->phase == ARENA_ACTIVE) {
        if (!arena->family.started) {
            Vec3f* pos = &actor->world.pos;
            if (!comboEnemyScriptedSpawnTarget(actor, play, pos->x, pos->y, pos->z, actor->shape.rot.y, &arena->family))
                return;
            if (sourceId(arena) == 0x43)
                MM_SET_EVENT_INF(EVENTINF_INTRO_CS_WATCHED_GOMESS);
            else if (sourceId(arena) == 0x12c)
                MM_SET_EVENT_INF(EVENTINF_INTRO_CS_WATCHED_WART);
        }
        if (!comboEnemyFamilyDone(&arena->family)) {
            if (!arena->music && sourceId(arena) != 0x184 &&
                CutsceneManager_GetCurrentCsId() == -1 &&
                Message_GetState(&play->msgCtx) == TEXT_STATE_NONE && comboEnemyCombatActive(play)) {
                Audio_PlayBgm_StorePrevBgm(NA_BGM_MINI_BOSS);
                arena->music = 1;
            }
            return;
        }
        if (arena->music) {
            Audio_RestorePrevBgm();
            arena->music = 0;
        }
        if (sourceId(arena) == 0x182 && arena->binding.original.params == 0) {
            if (Message_GetState(&play->msgCtx) != TEXT_STATE_NONE || CutsceneManager_GetCurrentCsId() != -1)
                return;
            arena->text = 0x13ae;
            Message_StartTextbox(play, arena->text, actor);
            arena->phase = ARENA_HINT;
            return;
        }
        releasePlacement(arena, play);
        arena->phase = ARENA_REWARD;
    }
    if (arena->phase == ARENA_HINT) {
        int state = Message_GetState(&play->msgCtx);
        if ((state != TEXT_STATE_EVENT && state != TEXT_STATE_DONE) || !Message_ShouldAdvance(play))
            return;
        if (arena->text < 0x13b1) {
            Message_ContinueTextbox(play, ++arena->text);
            return;
        }
        Message_Close(play);
        releasePlacement(arena, play);
        arena->phase = ARENA_REWARD;
    }
    if (arena->phase == ARENA_REWARD) {
        clearRoom(arena, play);
        if (sourceId(arena) == 0x115) {
            if (Actor_HasParentZ(actor) || gMmExtraFlags.songEmptiness) {
                gMmExtraFlags.songEmptiness = 1;
                actor->parent = NULL;
            } else {
                if (CutsceneManager_GetCurrentCsId() == -1 && Message_GetState(&play->msgCtx) == TEXT_STATE_NONE)
                    comboGiveItemNpc(actor, play, GI_MM_SONG_EMPTINESS, NPC_MM_SONG_EMPTINESS, 9999.f, 9999.f);
                return;
            }
        } else if ((sourceId(arena) == 7 || sourceId(arena) == 0x65) && arena->reward != 1) {
            if (!frog(arena, play))
                return;
            if (!arena->reward)
                Actor_PlaySfx(actor, NA_SE_EN_NPC_APPEAR);
            arena->reward = 1;
        }
        arena->phase = ARENA_DONE;
        if (sourceId(arena) != 0x115)
            Actor_Kill(actor);
    }
}

ActorProfile Enemy_Arena_Profile = {
    ACTOR_CUSTOM_ENEMY_ARENA,
    ACTORCAT_MISC,
    ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_UPDATE_DURING_OCARINA,
    1,
    sizeof(EnemyScriptedArena),
    ArenaInit,
    ArenaDestroy,
    ArenaUpdate,
    NULL,
};
OVL_INFO_ACTOR(ACTOR_CUSTOM_ENEMY_ARENA, Enemy_Arena_Profile);
