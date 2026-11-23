#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/enemy_ground.h>
#if defined(GAME_MM)
#include <combo/enemy_flight.h>
#endif
static Actor* sFighterSelected;
#if defined(GAME_OOT)
#define FIGHTER_ID 0x197
#define FIGHTER_PARAMS 0
#define FIGHTER_INIT 0x80b4111c
#define FIGHTER_UPDATE 0x80b452e0
#define FIGHTER_DRAW 0x80b4588c
#define FIGHTER_FINI 0x80b41340
#define FIGHTER_READY 0x80b41b58
#define FIGHTER_RESTORE 0x800c6894
#define NativeFighterSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
typedef struct {
    Actor actor;
    Vec3s parts[10];
    SkelAnime skeleton;
    Vec3s joints[24], morph[24];
    s32 action;
    u8 pad[4];
    ActorFunc actionFunc;
    s16 unkTimer, lookTimer, iceTimer;
    u8 reaction;
    s32 timer;
    f32 approach;
    u8 pad2[4];
    f32 unkFloat;
    s16 sword, spin, keyFlag, color, invisible;
} NativeFighter;
_Static_assert(__builtin_offsetof(NativeFighter, spin) == 0x302, "fighter spin");
_Static_assert(__builtin_offsetof(NativeFighter, color) == 0x306, "fighter color");

static const Color_RGBA8 kFighterColors[] = {
    { 140, 0, 0, 255 },
    { 255, 130, 10, 255 },
    { 185, 130, 210, 255 },
    { 135, 195, 80, 255 },
};

static int fighterOverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* actor)
{
    if (limbIndex == 3 || limbIndex == 6 || limbIndex == 11 || limbIndex == 16)
    {
        OverrideLimbDrawOpa original = actorAddr(FIGHTER_ID, 0x80b454cc);
        return original(play, limbIndex, dList, pos, rot, actor);
    }
    const Color_RGBA8* color = &kFighterColors[((NativeFighter*)actor)->color];
    OPEN_DISPS(play->state.gfxCtx);
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, color->r, color->g, color->b, color->a);
    CLOSE_DISPS();
    return 0;
}

static void fighterDrawSkeleton(PlayState* play, void** skeleton, Vec3s* joints, s32 count, void* overrideLimbDraw, void* postLimbDraw, void* actor)
{
    if (sFighterSelected == actor)
        overrideLimbDraw = fighterOverrideLimbDraw;
    SkelAnime_DrawFlexOpa(play, skeleton, joints, count, overrideLimbDraw, postLimbDraw, actor);
}
PATCH_CALL(0x80b45aac, fighterDrawSkeleton);
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_DRAW_V1, fighterDrawSkeleton);

static void fighterDraw(Actor* actor, PlayState* play)
{
    Actor* previous = sFighterSelected;
    ActorFunc draw = actorAddr(FIGHTER_ID, FIGHTER_DRAW);
    sFighterSelected = actor;
    draw(actor, play);
    sFighterSelected = previous;
}
#else
#define FIGHTER_ID 0x21d
#define FIGHTER_PARAMS 0x1fc0
#define FIGHTER_INIT 0x80b85590
#define FIGHTER_UPDATE 0x80b8a0a0
#define FIGHTER_DRAW 0x80b8a718
#define FIGHTER_FINI 0x80b85800
#define FIGHTER_READY 0x80b872a4
#define FIGHTER_RESTORE 0x801a2ed8
#define NativeFighterSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
typedef struct {
    Actor actor;
    void* validate;
    SkelAnime skeleton;
    Vec3s joints[24], morph[24];
    ActorFunc actionFunc;
    s16 action, combatTimer, lookTimer, iceTimer, dmgTimer, dmgType, switchFlag;
    f32 dmgScale, steamScale;
    s16 textOffset, textType, eye, blink, spin, sword, exitIndex, csId;
    u8 animationsDisabled;
    u8 pad2[0x2ec - 0x2d9];
    s32 color;
    u8 pad4[0x2f8 - 0x2f0];
    Vec3f swordScaleRight, swordScaleLeft;
    u8 pad3[0x420 - 0x310];
    ColliderQuad swordCollider;
} NativeFighter;
_Static_assert(__builtin_offsetof(NativeFighter, actionFunc) == 0x2ac, "fighter action");
_Static_assert(__builtin_offsetof(NativeFighter, animationsDisabled) == 0x2d8, "fighter animation state");
_Static_assert(__builtin_offsetof(NativeFighter, color) == 0x2ec, "fighter color");
static int fighterCanAttack(Actor* actor, f32 range)
{
    if (actor->xzDistToPlayer > range || fabsf(actor->yDistanceFromLink) >= 50.f)
        return 0;
    Vec3f from = actor->world.pos;
    Vec3f to = GET_PLAYER(gPlay)->actor.world.pos;
    from.y += 40.f;
    to.y += 40.f;
    return enemyFlightClear(gPlay, &from, &to);
}

static void fighterSetupAttack(Actor* actor, u32 setup, f32 range)
{
    if (sFighterSelected == actor && !fighterCanAttack(actor, range))
    {
        NativeFighter* fighter = (NativeFighter*)actor;
        if (fighter->action != 1)
            ((void (*)(Actor*))actorAddr(FIGHTER_ID, FIGHTER_READY))(actor);
        fighter->sword = -1;
        fighter->swordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        return;
    }
    ((void (*)(Actor*))actorAddr(FIGHTER_ID, setup))(actor);
}

void comboEnemyNativeFighterSlash(Actor* actor)
{
    fighterSetupAttack(actor, 0x80b87f70, 120.f);
}

void comboEnemyNativeFighterSpin(Actor* actor)
{
    fighterSetupAttack(actor, 0x80b88910, 400.f);
}
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_SLASH_V1, comboEnemyNativeFighterSlash);
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_SPIN_V1, comboEnemyNativeFighterSpin);

static void fighterDeath(Actor* actor, PlayState* play)
{
    if (!((NativeFighter*)actor)->combatTimer)
        Actor_Kill(actor);
}

void comboEnemyNativeFighterDeath(Actor* actor, PlayState* play)
{
    void (*death)(Actor*, PlayState*) = actorAddr(FIGHTER_ID, 0x80b8960c);
    death(actor, play);
    if (sFighterSelected == actor)
    {
        NativeFighter* fighter = (NativeFighter*)actor;
        fighter->actionFunc = fighterDeath;
        fighter->combatTimer = 60;
        fighter->animationsDisabled = 0;
        actor->flags &= ~(1u << 20);
    }
}

s32 comboEnemyNativeFighterNext(s16 id)
{
    return sFighterSelected ? 0 : CutsceneManager_IsNext(id);
}

void comboEnemyNativeFighterQueue(s16 id)
{
    if (!sFighterSelected)
        CutsceneManager_Queue(id);
}
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_NEXT_V1, comboEnemyNativeFighterNext);
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_QUEUE_V1, comboEnemyNativeFighterQueue);
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_DEATH_V1, comboEnemyNativeFighterDeath);
#endif
s32 comboEnemyNativeFighterPlayerCs(PlayState* play, Actor* actor, u8 action)
{
    if (sFighterSelected == actor)
        return 0;
    return Player_SetCsActionWithHaltedActors(play, actor, action);
}

void comboEnemyNativeFighterRestore(void)
{
    if (!sFighterSelected)
        ((void (*)(void))FIGHTER_RESTORE)();
}

static void fighterUpdate(Actor* actor, PlayState* play)
{
    Actor* previous = sFighterSelected;
    ActorFunc update = actorAddr(FIGHTER_ID, FIGHTER_UPDATE);
    EnemyGroundGuard guard;
    comboEnemyGroundGuardBeginNative(actor, &guard);
    sFighterSelected = actor;
#if defined(GAME_MM)
    NativeFighter* fighter = (NativeFighter*)actor;
    if (fighter->action == 1 && (actor->xzDistToPlayer > 600.f || fabsf(actor->yDistanceFromLink) >= 50.f))
        fighter->lookTimer = 2;
#endif
    update(actor, play);
    comboEnemyGroundGuardEnd(play, &guard);
#if defined(GAME_OOT)
    if (actor->update && actor->colChkInfo.health && ((NativeFighter*)actor)->spin >= 2)
    {
        void (*ready)(Actor*) = actorAddr(FIGHTER_ID, FIGHTER_READY);
        ((NativeFighter*)actor)->spin = 0;
        ready(actor);
    }
#else
    if (actor->update && actor->colChkInfo.health && fighter->actionFunc == actorAddr(FIGHTER_ID, 0x80b86804))
    {
        void (*ready)(Actor*) = actorAddr(FIGHTER_ID, FIGHTER_READY);
        fighter->spin = 0;
        fighter->swordCollider.base.atFlags &= ~2;
        actor->flags &= ~(1u << 20);
        ready(actor);
    }
#endif
    sFighterSelected = previous;
}

static void fighterFini(Actor* actor, PlayState* play)
{
    Actor* previous = sFighterSelected;
    ActorFunc fini = actorAddr(FIGHTER_ID, FIGHTER_FINI);
    sFighterSelected = actor;
    fini(actor, play);
    sFighterSelected = previous;
}

static void fighterActivate(Actor* actor, PlayState* play)
{
    void (*ready)(Actor*) = actorAddr(FIGHTER_ID, FIGHTER_READY);
    if (!actor->update || !ready)
        return;
    u32 previousSegment = gSegments[6];
    Actor_SetObjectDependency(play, actor);
    if (!gSegments[6])
    {
        gSegments[6] = previousSegment;
        return;
    }
    actor->world.pos = actor->home.pos;
    actor->prevPos = actor->world.pos;
    actor->gravity = -2.f;
    actor->minVelocityY = -20.f;
    actor->flags &= ~((1u << 27) | (1u << 20));
    actor->shape.shadowScale = 90.f;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED | 5;
#if defined(GAME_OOT)
    ((NativeFighter*)actor)->invisible = 0;
    ((NativeFighter*)actor)->color = (s16)(Rand_ZeroOne() * ARRAY_COUNT(kFighterColors));
#else
    NativeFighter* fighter = (NativeFighter*)actor;
    fighter->color = (s32)(Rand_ZeroOne() * 3.f);
    fighter->animationsDisabled = 0;
    fighter->swordScaleRight.x = fighter->swordScaleRight.y = fighter->swordScaleRight.z = 1.f;
    fighter->swordScaleLeft = fighter->swordScaleRight;
#endif
    ready(actor);
    gSegments[6] = previousSegment;
#if defined(GAME_OOT)
    actor->draw = fighterDraw;
#else
    actor->draw = actorAddr(FIGHTER_ID, FIGHTER_DRAW);
#endif
    actor->update = fighterUpdate;
    actor->fini = fighterFini;
}

static void fighterInit(Actor* actor, PlayState* play)
{
    ActorFunc init = actorAddr(FIGHTER_ID, FIGHTER_INIT);
    actor->init = NULL;
    init(actor, play);
    fighterActivate(actor, play);
}

Actor* comboEnemyNativeFighterSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != FIGHTER_ID || (u16)entry->params != FIGHTER_PARAMS)
        return NULL;
    actor = NativeFighterSpawnEntry(ctx, entry, play);
    if (actor && actor->init)
        actor->init = fighterInit;
    else if (actor)
        fighterActivate(actor, play);
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_V1, comboEnemyNativeFighterSpawn);
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_CS_V1, comboEnemyNativeFighterPlayerCs);
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_RESTORE_V1, comboEnemyNativeFighterRestore);

#if defined(GAME_OOT)
void comboEnemyNativeFighterText(PlayState* play, u16 id, Actor* actor)
{
    if (sFighterSelected != actor)
        Message_StartTextbox(play, id, actor);
}
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_TEXT_V1, comboEnemyNativeFighterText);
extern void EnGeldB_Caught_Trampoline(void);
EXPORT_SYMBOL(ENEMY_NATIVE_FIGHTER_CAUGHT_V1, EnGeldB_Caught_Trampoline);
#endif

#if defined(GAME_MM)
PATCH_VROM_U32(0xff032c, 0);
PATCH_VROM_U32(0xff0334, 0);
#endif

#if defined(GAME_MM)
PATCH_CALL(0x80b85dd0, comboEnemyNativeFighterSlash);
PATCH_VROM_U32(0xfeff9c, 0);
PATCH_CALL(0x80b87894, comboEnemyNativeFighterSlash);
PATCH_VROM_U32(0xff00b4, 0);
PATCH_CALL(0x80b87b10, comboEnemyNativeFighterSlash);
PATCH_VROM_U32(0xff00d4, 0);
PATCH_CALL(0x80b87e0c, comboEnemyNativeFighterSlash);
PATCH_VROM_U32(0xff0130, 0);
PATCH_CALL(0x80b882f8, comboEnemyNativeFighterSlash);
PATCH_VROM_U32(0xff01a0, 0);
PATCH_CALL(0x80b885d0, comboEnemyNativeFighterSlash);
PATCH_VROM_U32(0xff01d0, 0);
PATCH_CALL(0x80b89138, comboEnemyNativeFighterSlash);
PATCH_VROM_U32(0xff0274, 0);
PATCH_CALL(0x80b895e0, comboEnemyNativeFighterSlash);
PATCH_VROM_U32(0xff02d0, 0);
PATCH_CALL(0x80b89c30, comboEnemyNativeFighterSlash);
PATCH_VROM_U32(0xff0318, 0);
PATCH_CALL(0x80b8746c, comboEnemyNativeFighterSpin);
PATCH_VROM_U32(0xff0080, 0);
PATCH_CALL(0x80b87f24, comboEnemyNativeFighterSpin);
PATCH_VROM_U32(0xff0148, 0);
PATCH_CALL(0x80b886cc, comboEnemyNativeFighterSpin);
PATCH_VROM_U32(0xff01e4, 0);
#endif
