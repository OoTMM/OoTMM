#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/rand.h>
#include <combo/sfx.h>
#include <combo/souls.h>
#include <combo/global.h>
#include <combo/enemy_soul_preview.h>

typedef struct {
    Actor actor;
    SkelAnime skeleton;
    ActorFunc action;
    u8 type, clone, bob, targetTimer, fireCount, flags;
    s16 timer, otherTimer;
    Vec3s joints[12], morph[12];
    Color_RGBA8 color;
} NativePoeSister;
#if defined(GAME_OOT)
#define POE_SISTER_ID 0x91
#define POE_SISTER_INIT 0x8093c340
#define NativePoeSisterSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
_Static_assert(__builtin_offsetof(NativePoeSister, color) == 0x21e, "Poe Sister color");
static Actor* sPoeSisterSelected;
static void poeSisterHover(NativePoeSister* sister, PlayState* play)
{
    if (sPoeSisterSelected != &sister->actor || (sister->type != 1 && sister->type != 2) ||
        sister->action == actorAddr(POE_SISTER_ID, 0x8093f320))
    {
        ((void (*)(NativePoeSister*, PlayState*))actorAddr(POE_SISTER_ID, 0x8093d71c))(sister, play);
        return;
    }
    Actor* actor = &sister->actor;
    f32 targetY = GET_PLAYER(play)->actor.world.pos.y + 5.f;
    if (actor->floorPoly)
        targetY = MAX(targetY, actor->floorHeight + 5.f);
    Math_ApproachF(&actor->world.pos.y, targetY, .5f, 3.f);
    if (!sister->bob)
        sister->bob = 32;
    --sister->bob;
    actor->world.pos.y += (2.f + .5f * Rand_ZeroOne()) * Math_SinS(sister->bob * 0x800);
    if (sister->color.a == 255 && sister->action != actorAddr(POE_SISTER_ID, 0x8093dc80) &&
        sister->action != actorAddr(POE_SISTER_ID, 0x8093dbb0))
    {
        u16 cue = sister->action == actorAddr(POE_SISTER_ID, 0x8093e030) ? NA_SE_EN_PO_AWAY : NA_SE_EN_PO_FLY;
        Actor_PlaySfx_Flagged(actor, cue - 0x800);
    }
}
PATCH_CALL(0x8093f71c, poeSisterHover);
EXPORT_SYMBOL(ENEMY_NATIVE_POE_SISTER_HOVER_V1, poeSisterHover);
void comboEnemyNativePoeSisterSwitch(PlayState* play, s32 flag)
{
    if (!sPoeSisterSelected)
        Flags_SetSwitch(play, flag);
}

void comboEnemyNativePoeSisterUnset(PlayState* play, s32 flag)
{
    if (!sPoeSisterSelected)
        ((void (*)(PlayState*, s32))0x80020510)(play, flag);
}

void comboEnemyNativePoeSisterBgm(u16 id)
{
    if (!sPoeSisterSelected)
        ((void (*)(u16))0x800c6820)(id);
}

void comboEnemyNativePoeSisterRestore(void)
{
    if (!sPoeSisterSelected)
        ((void (*)(void))0x800c6894)();
}

void* comboEnemyNativePoeSisterCutscene(PlayState* play, s16 id, s16 timer, Actor* actor, s16 camera)
{
    if (sPoeSisterSelected)
        return NULL;
    return ((void* (*)(PlayState*, s16, s16, Actor*, s16))0x8006b6fc)(play, id, timer, actor, camera);
}

static void poeSisterUpdate(Actor* actor, PlayState* play)
{
    ActorFunc update = actorAddr(POE_SISTER_ID, 0x8093f698);
    Actor* previous = sPoeSisterSelected;
    u8 lighting = play->envCtx.lightSettingOverride;
    sPoeSisterSelected = actor;
    update(actor, play);
    sPoeSisterSelected = previous;
    play->envCtx.lightSettingOverride = lighting;
}

static void poeSisterFini(Actor* actor, PlayState* play)
{
    ActorFunc fini = actorAddr(POE_SISTER_ID, 0x8093c58c);
    Actor* previous = sPoeSisterSelected;
    sPoeSisterSelected = actor;
    fini(actor, play);
    sPoeSisterSelected = previous;
}
EXPORT_SYMBOL(ENEMY_NATIVE_POE_SISTER_SWITCH_V1, comboEnemyNativePoeSisterSwitch);
EXPORT_SYMBOL(ENEMY_NATIVE_POE_SISTER_UNSET_V1, comboEnemyNativePoeSisterUnset);
EXPORT_SYMBOL(ENEMY_NATIVE_POE_SISTER_BGM_V1, comboEnemyNativePoeSisterBgm);
EXPORT_SYMBOL(ENEMY_NATIVE_POE_SISTER_RESTORE_V1, comboEnemyNativePoeSisterRestore);
EXPORT_SYMBOL(ENEMY_NATIVE_POE_SISTER_CS_V1, comboEnemyNativePoeSisterCutscene);
#else
#define POE_SISTER_ID 0x1e8
#define POE_SISTER_INIT 0x80b1a3b0
#define NativePoeSisterSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
_Static_assert(__builtin_offsetof(NativePoeSister, color) == 0x226, "Poe Sister color");
#endif
static void poeSisterActivate(Actor* actor, PlayState* play)
{
    if (!actor->update)
        return;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
#if defined(GAME_OOT)
    NativePoeSister* sister = (NativePoeSister*)actor;
    void (*combat)(NativePoeSister*) = actorAddr(POE_SISTER_ID, sister->type ? 0x8093c77c : 0x8093d2d8);
    sister->flags &= ~0x80;
    if (sister->type)
        sister->flags |= 0x18;
    sister->fireCount = 1;
    sister->color.a = 255;
    actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    u32 savedSegment = gSegments[6];
    Actor_SetObjectDependency(play, actor);
    combat(sister);
    gSegments[6] = savedSegment;
    actor->draw = actorAddr(POE_SISTER_ID, 0x8094040c);
    actor->update = poeSisterUpdate;
    actor->fini = poeSisterFini;
#endif
}

static void poeSisterInit(Actor* actor, PlayState* play)
{
    ActorFunc init = actorAddr(POE_SISTER_ID, POE_SISTER_INIT);
    actor->init = NULL;
    init(actor, play);
    poeSisterActivate(actor, play);
}

Actor* comboEnemyNativePoeSisterSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != POE_SISTER_ID ||
        ((u16)entry->params != 0 && (u16)entry->params != 0x100 && (u16)entry->params != 0x200 && (u16)entry->params != 0x300 && (u16)entry->params != 0x4000))
        return NULL;
    if ((u16)entry->params == 0x4000)
    {
        comboEnemySoulPreviewGateBegin();
        int allowed;
#if defined(DEBUG)
        extern int gDebugEnemySpawn;
        if (gDebugEnemySpawn)
            allowed = 1;
        else
#endif
#if defined(GAME_OOT)
            allowed = comboHasSoulOot(GI_OOT_SOUL_ENEMY_POE);
        int game = 0;
#else
            allowed = comboHasSoulMm(GI_MM_SOUL_ENEMY_POE);
        int game = 1;
#endif
        allowed = comboEnemySoulPreviewGate(play, game, POE_SISTER_ID, entry->params,
            entry->pos.x, entry->pos.y, entry->pos.z, entry->rot.y, allowed);
        if (allowed <= 0)
        {
            if (!allowed)
                g.roomEnemyLackSoul = 1;
            return NULL;
        }
        ActorEntry copy = *entry;
        copy.id = ACTOR_ENEMY_POE_ENCOUNTER;
        copy.params = 0;
        return NativePoeSisterSpawnEntry(ctx, &copy, play);
    }
    actor = NativePoeSisterSpawnEntry(ctx, entry, play);
    if (actor && actor->init)
        actor->init = poeSisterInit;
    else if (actor)
        poeSisterActivate(actor, play);
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_POE_SISTER_V1, comboEnemyNativePoeSisterSpawn);
