#include <combo.h>
#include <combo/enemy_native.h>

#if defined(GAME_OOT)

typedef struct {
    Actor actor;
    SkelAnime skeleton;
    ActorFunc action;
    u8 lockFrames;
    u8 spawnFlagIndex;
    s16 timer;
    s16 spin;
    s16 flameTimer;
    Vec3s jointTable[10];
    Vec3s morphTable[10];
    Color_RGBA8 lightColor;
    Color_RGBA8 soulColor;
    f32 scaleModifier;
    f32 flameScale;
    f32 hoverHeight;
    f32 hoverPhase;
    f32 hoverYaw;
    LightNode* lightNode;
    LightInfo lightInfo;
    ColliderCylinder body;
} NativeBigPoe;
_Static_assert(__builtin_offsetof(NativeBigPoe, action) == 0x180, "Big Poe action");
_Static_assert(__builtin_offsetof(NativeBigPoe, body) == 0x234, "Big Poe body");

typedef struct {
    s32 count;
    Vec3s position;
    u8 flag;
} NativeBigPoeSpawnState;
#define NativeBigPoeSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
static void bigPoeSave(NativeBigPoeSpawnState* saved)
{
    memset(saved, 0, sizeof(*saved));
    if (!gActorOvl[0x175].data)
        return;
    s32* count = actorAddr(0x175, 0x80afb2d0);
    Vec3s* pos = actorAddr(0x175, 0x80afb740);
    u8* flag = actorAddr(0x175, 0x80afb780);
    saved->count = *count;
    saved->position = *pos;
    saved->flag = *flag;
    *count = 0;
}

static void bigPoeRestore(NativeBigPoeSpawnState* saved)
{
    if (!gActorOvl[0x175].data)
        return;
    *(s32*)actorAddr(0x175, 0x80afb2d0) = saved->count;
    *(Vec3s*)actorAddr(0x175, 0x80afb740) = saved->position;
    *(u8*)actorAddr(0x175, 0x80afb780) = saved->flag;
}

static Actor* sBigPoeSelectedActor;
void comboEnemyNativeBigPoeSwitch(PlayState* play, s32 flag)
{
    if (!sBigPoeSelectedActor)
        Flags_SetSwitch(play, flag);
}

static void bigPoeHover(Actor* actor, PlayState* play);
static void bigPoeSpinUp(Actor* actor, PlayState* play);
static void bigPoeSpinAttack(Actor* actor, PlayState* play);
static void bigPoeSpinDown(Actor* actor, PlayState* play);
static void bigPoeWarpIn(Actor* actor, PlayState* play);
static void bigPoeWarpOut(Actor* actor, PlayState* play);

static void bigPoeMove(Actor* actor, PlayState* play, f32 height)
{
    (void)play;
    actor->world.pos.x += actor->speed * Math_SinS(actor->world.rot.y);
    actor->world.pos.y = height;
    actor->world.pos.z += actor->speed * Math_CosS(actor->world.rot.y);
}

static void bigPoeStartHover(NativeBigPoe* poe, s16 delay)
{
    Actor* actor = &poe->actor;
    Animation_MorphToLoop(&poe->skeleton, (AnimationHeader*)0x06000924, -5.f);
    poe->timer = delay;
    poe->hoverHeight = actor->world.pos.y;
    poe->hoverPhase = 40.f;
    poe->hoverYaw = actor->shape.rot.y;
    poe->lockFrames = 0;
    actor->world.rot.y = actor->shape.rot.y;
    actor->velocity.y = 0.f;
    actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    poe->body.base.acFlags |= AC_ON;
    poe->body.base.ocFlags1 |= OC1_ON;
    poe->action = bigPoeHover;
}

static void bigPoeStartWarp(NativeBigPoe* poe)
{
    Actor* actor = &poe->actor;
    poe->body.base.acFlags &= ~(AC_ON | AC_HIT);
    poe->body.base.atFlags = AT_TYPE_ENEMY;
    poe->body.base.ocFlags1 &= ~OC1_ON;
    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    actor->speed = 0.f;
    poe->spin = 0x2000;
    poe->timer = 32;
    Actor_PlaySfx(actor, NA_SE_EN_PO_DISAPPEAR);
    poe->action = bigPoeWarpOut;
}

static void bigPoeWarpOut(Actor* actor, PlayState* play)
{
    NativeBigPoe* poe = (NativeBigPoe*)actor;
    actor->shape.rot.y += poe->spin;
    if (--poe->timer < 16)
        Math_ScaledStepToS(&poe->spin, 0, 0x200);
    poe->lightColor.a = poe->timer * (255.f / 32.f);
    if (poe->timer)
        return;

    Actor* player = &GET_PLAYER(play)->actor;
    f32 distance = MAX(actor->xzDistToPlayer, 200.f);
    s16 yaw = ((s32)Rand_Next() >> 20) + actor->yawTowardsPlayer;
    Vec3f next = { player->world.pos.x + distance * Math_SinS(yaw), actor->world.pos.y,
                   player->world.pos.z + distance * Math_CosS(yaw) };
    actor->world.pos = next;
    Actor_PlaySfx(actor, NA_SE_EN_PO_APPEAR);
    poe->spin = 0x2000;
    poe->action = bigPoeWarpIn;
}

static void bigPoeWarpIn(Actor* actor, PlayState* play)
{
    NativeBigPoe* poe = (NativeBigPoe*)actor;
    actor->shape.rot.y -= poe->spin;
    if (++poe->timer >= 16)
        Math_ScaledStepToS(&poe->spin, 0, 0x200);
    poe->lightColor.a = poe->timer * (255.f / 32.f);
    if (poe->timer == 32)
        bigPoeStartHover(poe, 0);
}

static void bigPoeHover(Actor* actor, PlayState* play)
{
    NativeBigPoe* poe = (NativeBigPoe*)actor;
    SkelAnime_Update(&poe->skeleton);
    if (poe->timer)
        --poe->timer;
    poe->hoverPhase = poe->hoverPhase == 0.f ? 40.f : poe->hoverPhase - 1.f;
    Math_StepToF(&poe->hoverHeight, GET_PLAYER(play)->actor.world.pos.y + 100.f, 1.5f);
    Math_StepToF(&actor->speed, 3.f, .2f);
    Actor_PlaySfx_Flagged(actor, NA_SE_EN_PO_FLY - SFX_FLAG);
    f32 dx = actor->world.pos.x - actor->home.pos.x;
    f32 dz = actor->world.pos.z - actor->home.pos.z;
    s16 yaw = poe->hoverYaw;
    if (dx * dx + dz * dz > 300.f * 300.f)
        yaw = Math_Vec3f_Yaw(&actor->world.pos, &actor->home.pos);
    if (Math_ScaledStepToS(&actor->shape.rot.y, yaw, 0x200) && Rand_ZeroOne() < .075f)
    {
        s16 turn = (Rand_Next() >> 20) + 0x1000;
        yaw += Rand_ZeroOne() < .5f ? -turn : turn;
    }
    poe->hoverYaw = yaw;
    actor->world.rot.y = actor->shape.rot.y;
    bigPoeMove(actor, play, poe->hoverHeight + 10.f * Math_SinS((s16)(poe->hoverPhase * (65536.f / 40.f))));
    if (!poe->timer)
    {
        poe->body.base.colMaterial = COL_MATERIAL_METAL;
        poe->body.base.acFlags |= AC_HARD;
        poe->body.base.atFlags = AT_ON | AT_TYPE_ENEMY;
        poe->spin = 0x800;
        actor->speed = 0.f;
        poe->action = bigPoeSpinUp;
    }
}

static void bigPoeRotate(NativeBigPoe* poe)
{
    s16 yaw = poe->actor.shape.rot.y;
    poe->actor.shape.rot.y += poe->spin;
    if (yaw < 0 && poe->actor.shape.rot.y > 0)
        Actor_PlaySfx(&poe->actor, NA_SE_EN_PO_ROLL);
}

static void bigPoeSpinUp(Actor* actor, PlayState* play)
{
    NativeBigPoe* poe = (NativeBigPoe*)actor;
    SkelAnime_Update(&poe->skeleton);
    poe->spin += 0x200;
    bigPoeRotate(poe);
    if (poe->spin >= 0x3c00)
    {
        actor->world.rot.y = actor->yawTowardsPlayer;
        poe->action = bigPoeSpinAttack;
    }
}

static void bigPoeSpinAttack(Actor* actor, PlayState* play)
{
    NativeBigPoe* poe = (NativeBigPoe*)actor;
    SkelAnime_Update(&poe->skeleton);
    Math_StepToF(&actor->speed, 10.f, 1.f);
    f32 height = actor->world.pos.y;
    Math_SmoothStepToF(&height, GET_PLAYER(play)->actor.world.pos.y, .3f, 7.5f, 1.f);
    bigPoeMove(actor, play, height);
    bigPoeRotate(poe);
    s16 yawDiff = actor->yawTowardsPlayer - actor->world.rot.y;
    if ((poe->body.base.atFlags & AT_HIT) ||
        (ABS(yawDiff) > 0x4000 && actor->xzDistToPlayer > 50.f))
    {
        poe->body.base.atFlags = AT_TYPE_ENEMY;
        poe->action = bigPoeSpinDown;
    }
}

static void bigPoeSpinDown(Actor* actor, PlayState* play)
{
    NativeBigPoe* poe = (NativeBigPoe*)actor;
    SkelAnime_Update(&poe->skeleton);
    f32 height = actor->world.pos.y;
    Math_SmoothStepToF(&height, GET_PLAYER(play)->actor.world.pos.y + 100.f, .3f, 5.f, 1.f);
    Math_StepToF(&actor->speed, 0.f, .2f);
    bigPoeMove(actor, play, height);
    if (Math_ScaledStepToS(&poe->spin, 0, 0x200))
    {
        poe->body.base.colMaterial = COL_MATERIAL_HIT3;
        poe->body.base.acFlags &= ~AC_HARD;
        bigPoeStartHover(poe, 80);
    }
    bigPoeRotate(poe);
}

static void bigPoeUpdate(Actor* actor, PlayState* play)
{
    ActorFunc update = actorAddr(0x175, actor->category == ACTORCAT_MISC ? 0x80afae0c : 0x80afa6ec);
    ActorFunc wait = actorAddr(0x175, 0x80af88dc);
    Vec3f* middle = actorAddr(0x175, 0x80afb2d4);
    Vec3f saved = *middle;
    Actor* previous = sBigPoeSelectedActor;
    *middle = actor->home.pos;
    sBigPoeSelectedActor = actor;
    NativeBigPoe* poe = (NativeBigPoe*)actor;
    ActorFunc appearAction = actorAddr(0x175, 0x80af8b44);
    ActorFunc flee = actorAddr(0x175, 0x80af8f20);
    if (poe->action == appearAction)
        actor->speed = 0.f;
    ActorFunc damage = actorAddr(0x175, 0x80af90c4);
    ActorFunc oldAction = poe->action;
    int hit = (poe->body.base.acFlags & AC_HIT) != 0;
    if (poe->body.base.acFlags & AC_HARD)
        poe->body.base.acFlags &= ~AC_HIT;
    if (actor->colChkInfo.health && actor->category == ACTORCAT_ENEMY &&
        oldAction != appearAction && oldAction != damage &&
        oldAction != bigPoeWarpOut && oldAction != bigPoeWarpIn &&
        !(poe->body.base.acFlags & AC_HARD) && !hit && actor->isTargeted)
    {
        if (++poe->lockFrames == 40)
        {
            Actor_PlaySfx(actor, NA_SE_EN_PO_LAUGH);
            bigPoeStartWarp(poe);
        }
    }
    else
        poe->lockFrames = 0;
    update(actor, play);
    sBigPoeSelectedActor = previous;
    *middle = saved;
    if (!actor->update)
        return;
    if (poe->action == wait)
    {
        Actor_Kill(actor);
        return;
    }
    if (poe->action == flee)
    {
        actor->speed = 0.f;
        if (oldAction == damage && actor->colChkInfo.health)
            bigPoeStartWarp(poe);
        else
            bigPoeStartHover(poe, 0);
    }
    if (poe->action == appearAction)
        actor->speed = 0.f;
    if (actor->category == ACTORCAT_ENEMY && actor->colChkInfo.health)
        actor->shape.shadowAlpha = poe->lightColor.a;
    if (poe->body.base.atFlags & AT_ON)
        CollisionCheck_SetAT(play, &play->colChkCtx, &poe->body.base);
    actor->update = bigPoeUpdate;
}

static u8 sBigPoeDamageTable[32] = {
    0, 1, 1, 1, 1, 2, 1, 0,
    1, 1, 2, 1, 1, 2, 1, 1,
    1, 0, 0, 0, 0, 0, 1, 2,
    1, 2, 4, 2, 0, 0, 2, 0,
};

static void bigPoeActivate(Actor* actor, PlayState* play)
{
    void (*appear)(Actor*) = actorAddr(0x175, 0x80af808c);
    if (!actor->update)
        return;
    actor->params = 1;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    u32 previousSegment = gSegments[6];
    Actor_SetObjectDependency(play, actor);
    if (gSegments[6])
    {
        appear(actor);
        NativeBigPoe* poe = (NativeBigPoe*)actor;
        actor->colChkInfo.health = 10;
        actor->colChkInfo.damageTable = (DamageTable*)sBigPoeDamageTable;
        poe->body.dim.radius = 35;
        poe->body.dim.height = 100;
        poe->body.dim.yShift = 10;
        actor->colChkInfo.mass = 50;
        actor->speed = 0.f;
        poe->lockFrames = 0;
        poe->body.elem.atDmgInfo = (ColliderElementDamageInfoAT){ 0xf7cfffff, 0, 0x10 };
        poe->body.elem.atElemFlags = ATELEM_ON | ATELEM_SFX_NORMAL;
        actor->update = bigPoeUpdate;
    }
    gSegments[6] = previousSegment;
}

static void bigPoeInit(Actor* actor, PlayState* play)
{
    NativeBigPoeSpawnState saved;
    ActorFunc init = actorAddr(0x175, 0x80af7d60);
    actor->init = NULL;
    bigPoeSave(&saved);
    init(actor, play);
    bigPoeRestore(&saved);
    bigPoeActivate(actor, play);
}

Actor* comboEnemyNativeBigPoeSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    NativeBigPoeSpawnState saved;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != 0x175 || entry->params != 1)
        return NULL;
    bigPoeSave(&saved);
    actor = NativeBigPoeSpawnEntry(ctx, entry, play);
    bigPoeRestore(&saved);
    if (actor && actor->init)
        actor->init = bigPoeInit;
    else if (actor)
        bigPoeActivate(actor, play);
    return actor;
}

EXPORT_SYMBOL(ENEMY_NATIVE_BIG_POE_SWITCH_V1, comboEnemyNativeBigPoeSwitch);
#elif defined(GAME_MM)
typedef struct {
    Actor actor;
    SkelAnime skeleton;
    Vec3s joints[10], morph[10];
    ActorFunc action;
    u8 storePrevBgm;
    u8 padding[0x290 - 0x205];
    Color_RGBA8 mainColor;
} NativeBigPoe;
_Static_assert(__builtin_offsetof(NativeBigPoe, action) == 0x200, "Big Poe action");
_Static_assert(__builtin_offsetof(NativeBigPoe, mainColor) == 0x290, "Big Poe color");
#define NativeBigPoeSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
static void bigPoeActivate(Actor* actor, PlayState* play)
{
    NativeBigPoe* poe = (NativeBigPoe*)actor;
    void (*idle)(NativeBigPoe*) = actorAddr(0x208, 0x80b624f4);
    if (!actor->update || !idle)
        return;
    u32 previousSegment = gSegments[6];
    Actor_SetObjectDependency(play, actor);
    if (!gSegments[6])
    {
        gSegments[6] = previousSegment;
        return;
    }
    poe->storePrevBgm = 1;
    poe->mainColor.a = 255;
    actor->colChkInfo.health = 10;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED | 5;
    actor->world.pos.y = actor->home.pos.y + 100.f;
    actor->prevPos.y = actor->world.pos.y;
    actor->draw = actorAddr(0x208, 0x80b64470);
    Actor_SetScale(actor, .014f);
    idle(poe);
    gSegments[6] = previousSegment;
}

static void bigPoeInit(Actor* actor, PlayState* play)
{
    ActorFunc init = actorAddr(0x208, 0x80b615e0);
    actor->init = NULL;
    if (init)
        init(actor, play);
    bigPoeActivate(actor, play);
}

Actor* comboEnemyNativeBigPoeSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != 0x208 || (u16)entry->params != 0xff00)
        return NULL;
    actor = NativeBigPoeSpawnEntry(ctx, entry, play);
    if (actor && actor->init)
        actor->init = bigPoeInit;
    else if (actor)
        bigPoeActivate(actor, play);
    return actor;
}
#endif

EXPORT_SYMBOL(ENEMY_NATIVE_BIG_POE_V1, comboEnemyNativeBigPoeSpawn);
