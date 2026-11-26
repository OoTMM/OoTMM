#include <combo.h>
#if defined(GAME_MM)
#include <combo/mm/regs.h>
#else
#include <combo/oot/regs.h>
#endif
#include <combo/matrix.h>
#include <combo/effect.h>
#include <combo/enemy_wiz_reflected_bolt.h>
#include <combo/rand.h>

#define WIZ_LIMBS 20
#define WIZ_IDLE ((AnimationHeader*)0x060066c0)
#define WIZ_WINDUP ((AnimationHeader*)0x060060e8)
#define WIZ_ATTACK ((AnimationHeader*)0x060007b0)
#define WIZ_RUN ((AnimationHeader*)0x060025f0)
#define WIZ_DANCE_ANIM ((AnimationHeader*)0x06002458)
#define WIZ_TRAIL_COUNT 6
#define WIZ_SHARD_COUNT 5
#define WIZ_ICE_SMOKE_COUNT 24
#define WIZ_ICE_ARROW_MASK 0x1000
#define WIZ_DAMAGE ((AnimationHeader*)0x06002218)
#define WIZ_SKELETON ((FlexSkeletonHeader*)0x0600b320)

typedef enum {
    WIZ_WAIT,
    WIZ_APPEAR,
    WIZ_DANCE,
    WIZ_CAST,
    WIZ_RECOVER,
    WIZ_HURT,
    WIZ_VANISH,
    WIZ_DEFEAT,
    WIZ_DASH,
} WizAction;

typedef struct {
    Vec3f pos;
    s16 yaw;
    u8 alpha;
} WizGhost;

typedef struct {
    ColliderCylinder collider;
    Vec3f pos;
    Vec3f velocity;
    WizProjectileTrail trail;
    Vec3f flameScaleOffset;
    f32 scale;
    u16 flameScroll;
    u8 timer;
    u8 smallFlame;
} WizFireShard;

typedef struct {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    f32 scale;
    u8 scroll;
    u8 age;
} WizIceSmoke;

typedef struct {
    Actor actor;
    SkelAnime skel;
    SkelAnime decoySkel;
    Vec3s joint[WIZ_LIMBS];
    Vec3s morph[WIZ_LIMBS];
    Vec3s decoyJoint[WIZ_LIMBS];
    Vec3s decoyMorph[WIZ_LIMBS];
    ColliderCylinder body;
    ColliderCylinder bolt;
    WizFireShard shards[WIZ_SHARD_COUNT];
    WizIceSmoke iceSmoke[WIZ_ICE_SMOKE_COUNT];
    u8 iceSmokeCount;
    Vec3f boltPos;
    Vec3f boltVelocity;
    WizProjectileTrail boltTrail;
    Vec3f warpFrom;
    Vec3f warpTo;
    WizGhost trail[WIZ_TRAIL_COUNT];
    Vec3f decoys[3];
    Vec3f decoyTargets[4];
    s16 decoyYaw[3];
    u8 decoyRoute[3];
    u8 decoyCount;
    u8 decoyRunning;
    u8 hurtVanish;
    u8 windupLoops;
    u8 attackDelayStarted;
    s16 danceSpin;
    s16 bodyAlpha;
    s16 defeatSpin;
    s16 hurtSpin;
    f32 hurtFloor;
    u8 hurtHop;
    u8 damageTimer;
    u8 damageType;
    Vec3f damageParts[12];
    Vec3f staffTip;
    f32 staffFlameScale;
    u8 staffFlameLit;
    u8 staffFlameScroll;
    u8 staffPoseValid;
    u16 damagePartMask;
    u8 defeatAlpha;
    u8 secondPhase;
    u8 phaseDash;
    u8 appeared;
    u8 appearanceFled;
    u8 escapeChecked;
    u8 escapePending;
    Vec3f pool;
    f32 poolScale;
    f32 poolFlameScale;
    f32 poolSmokeScale;
    s16 poolRadius;
    u8 poolAlpha;
    WizAction action;
    u8 timer;
    u8 boltTimer;
    u8 boltTailTimer;
    f32 boltScale;
    u8 ice;
#if defined(GAME_MM)
    u8 iceContact;
#endif
    u8 teleport;
    u8 warpTimer;
    u8 poolTimer;
} StandaloneWiz;

#if defined(GAME_OOT)
s32 FOREIGN_ENEMY_API_COLLISION_AC(PlayState*, CollisionCheckContext*, Collider*);
void FOREIGN_ENEMY_API_SFX_ACTOR(Actor*, u16);
void FOREIGN_ENEMY_API_SFX_WORLD(PlayState*, Vec3f*, u32, u16);
void FOREIGN_ENEMY_API_DAMAGE_EFFECTS(PlayState*, Actor*, Vec3f*, s16, f32, f32, f32, u8);
void FOREIGN_ENEMY_API_ICE_SHATTER(PlayState*, Actor*, Vec3f*, s32, s32, f32, f32);
#define Wiz_DrawDamageEffects FOREIGN_ENEMY_API_DAMAGE_EFFECTS
#define Wiz_ShatterIce FOREIGN_ENEMY_API_ICE_SHATTER
#define Wiz_SetupXlu Gfx_SetupDL25_Xlu
#define Wiz_Sfx FOREIGN_ENEMY_API_SFX_ACTOR
#define Wiz_SfxWorld FOREIGN_ENEMY_API_SFX_WORLD
#define Wiz_SetAC FOREIGN_ENEMY_API_COLLISION_AC
#define Wiz_Floor BgCheck_EntityRaycastDown1
#define Wiz_ToMtx ((Mtx* (*)(Mtx*))0x800ab8d8)
#define Wiz_LineTest ((s32 (*)(CollisionContext*,Vec3f*,Vec3f*,Vec3f*,CollisionPoly**,s32))0x80030ae8)
#else
#define Wiz_SetupXlu Gfx_SetupDL25_Xlu
#define Wiz_Sfx Actor_PlaySfx
#define Wiz_SfxWorld SoundSource_PlaySfxAtFixedWorldPos
#define Wiz_DrawDamageEffects Actor_DrawDamageEffects
#define Wiz_ShatterIce Actor_SpawnIceEffects
#define Wiz_SetAC CollisionCheck_SetAC
#define Wiz_Floor BgCheck_EntityRaycastFloor1
#define Wiz_ToMtx ((Mtx* (*)(Mtx*))0x80181a18)
#define Wiz_LineTest BgCheck_AnyLineTest1
#endif

static DamageTable sFireDamage = { {
    DMG_ENTRY(0, 1),  DMG_ENTRY(1, 15), DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 15), DMG_ENTRY(1, 15),
    DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 15), DMG_ENTRY(1, 15), DMG_ENTRY(3, 15), DMG_ENTRY(1, 2),
    DMG_ENTRY(2, 3),  DMG_ENTRY(2, 4),  DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 15), DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 15),
    DMG_ENTRY(1, 15), DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15),
} };
static DamageTable sIceDamage = { {
    DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 15), DMG_ENTRY(1, 15),
    DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(2, 15), DMG_ENTRY(1, 15), DMG_ENTRY(3, 15), DMG_ENTRY(2, 2),
    DMG_ENTRY(1, 3),  DMG_ENTRY(2, 4),  DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 15), DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 15),
    DMG_ENTRY(1, 15), DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15),
} };

static ColliderCylinderInit sBody = {
    { COL_MATERIAL_HIT0, AT_NONE, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1, COLSHAPE_CYLINDER },
    { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xffffffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_ON },
    { 35, 130, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sBolt = {
    { COL_MATERIAL_NONE, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_NONE, OC2_NONE, COLSHAPE_CYLINDER },
    { ELEM_MATERIAL_UNK0,
      { 0x20000000, 0, 8 },
      { WIZ_ICE_ARROW_MASK, 0, 0 },
      ATELEM_ON | ATELEM_SFX_NORMAL,
      ACELEM_ON,
      OCELEM_NONE },
    { 25, 25, -5, { 0, 0, 0 } },
};

static void Wiz_Init(Actor* actor, PlayState* play) {
    StandaloneWiz* wiz = (StandaloneWiz*)actor;
    wiz->staffFlameScroll = (u8)(Rand_ZeroOne() * 7.f);
    wiz->ice = (actor->params & 0x100) != 0;
    actor->colChkInfo.damageTable = wiz->ice ? &sIceDamage : &sFireDamage;
    actor->colChkInfo.health = wiz->ice ? 6 : 8;
    actor->colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(actor, .015f);
    wiz->bodyAlpha = 255;
    actor->shape.yOffset = 700.f;
    SkelAnime_InitFlex(play, &wiz->skel, WIZ_SKELETON, WIZ_IDLE, wiz->joint, wiz->morph, WIZ_LIMBS);
    Animation_PlayLoop(&wiz->skel, WIZ_IDLE);
    SkelAnime_InitFlex(play, &wiz->decoySkel, WIZ_SKELETON, WIZ_RUN, wiz->decoyJoint, wiz->decoyMorph, WIZ_LIMBS);
    Animation_PlayLoop(&wiz->decoySkel, WIZ_RUN);
    Collider_InitCylinder(play, &wiz->body);
    Collider_SetCylinder(play, &wiz->body, actor, &sBody);
    Collider_InitCylinder(play, &wiz->bolt);
    Collider_SetCylinder(play, &wiz->bolt, actor, &sBolt);

#if defined(GAME_MM)
    wiz->bolt.elem.atDmgInfo.effect = wiz->ice ? 2 : 9;
#else
    wiz->bolt.elem.atDmgInfo.effect = wiz->ice ? 2 : 1;
#endif
    for (int i = 0; i < WIZ_SHARD_COUNT; ++i) {
        ColliderCylinder* col = &wiz->shards[i].collider;
        Collider_InitCylinder(play, col);
        Collider_SetCylinder(play, col, actor, &sBolt);
        col->elem.atDmgInfo.effect = wiz->bolt.elem.atDmgInfo.effect;
    }
    wiz->timer = 25;
}

static void Wiz_Destroy(Actor* actor, PlayState* play) {
    StandaloneWiz* wiz = (StandaloneWiz*)actor;
    Collider_DestroyCylinder(play, &wiz->body);
    Collider_DestroyCylinder(play, &wiz->bolt);
    for (int i = 0; i < WIZ_SHARD_COUNT; ++i)
        Collider_DestroyCylinder(play, &wiz->shards[i].collider);
}

static int Wiz_ClearLine(PlayState* play, Vec3f* from, Vec3f* to) {
    CollisionPoly* poly;
    Vec3f hit;
    return !Wiz_LineTest(&play->colCtx, from, to, &hit, &poly, 0);
}

static void Wiz_BeginAppear(StandaloneWiz* wiz) {
    Actor* actor = &wiz->actor;
    actor->world.pos = actor->prevPos = wiz->warpTo;
    wiz->action = WIZ_APPEAR;
    wiz->escapeChecked = 0;
    wiz->timer = wiz->secondPhase ? 0 : 20;
    wiz->warpTimer = 16;
    Actor_SetScale(actor, 0.f);
    wiz->bodyAlpha = 0;
    Animation_PlayLoop(&wiz->skel, WIZ_IDLE);
}

static int Wiz_TestWaypoint(StandaloneWiz* wiz, PlayState* play, int index, Vec3f* result) {
    Actor* actor = &wiz->actor;
    f32 radius = index < 8 ? 640.f : index < 16 ? 320.f : index < 24 ? 192.f : 105.f;
    s16 yaw = (index & 7) * 0x2000;
    Vec3f pos = actor->home.pos;
    pos.x += Math_SinS(yaw) * radius;
    pos.z += Math_CosS(yaw) * radius;
    pos.y += 384.f;
    CollisionPoly* floor = NULL;
    f32 y = Wiz_Floor(&play->colCtx, &floor, &pos);
    if (!floor || fabsf(y - actor->home.pos.y) > 384.f || floor->normal.y < 0x6000)
        return 0;

    Vec3f from = actor->world.pos;
    from.y += 35.f;
    pos.y = y + 35.f;
    Vec3f top = pos;
    top.y += 40.f;
    if (!Wiz_ClearLine(play, &from, &pos) || !Wiz_ClearLine(play, &pos, &top))
        return 0;
    f32 dx = actor->world.pos.x - pos.x;
    f32 dz = actor->world.pos.z - pos.z;
    if (dx * dx + dz * dz < 100.f * 100.f)
        return 0;
    pos.y = y;
    *result = pos;
    return 1;
}

static int Wiz_FindEscape(StandaloneWiz* wiz, PlayState* play, Vec3f* result) {
    Vec3f* playerPos = &GET_PLAYER(play)->actor.world.pos;
    int first = wiz->teleport + 1;
    for (int i = 0; i < 32; ++i) {
        int index = (i & 24) | ((first + i) & 7);
        Vec3f pos;
        if (!Wiz_TestWaypoint(wiz, play, index, &pos))
            continue;
        f32 dx = playerPos->x - pos.x;
        f32 dz = playerPos->z - pos.z;
        if (dx * dx + dz * dz < 200.f * 200.f)
            continue;
        *result = pos;
        return index;
    }
    return -1;
}

static void Wiz_Reposition(StandaloneWiz* wiz, PlayState* play) {
    Actor* actor = &wiz->actor;
    wiz->decoyCount = wiz->decoyRunning = 0;
    if (wiz->escapePending) {
        wiz->escapePending = 0;
        int escape = Wiz_FindEscape(wiz, play, &wiz->warpTo);
        if (escape >= 0) {
            wiz->teleport = escape & 7;
            wiz->warpFrom = actor->world.pos;
            Wiz_BeginAppear(wiz);
            return;
        }
    }
    int found = 0;
    int first = wiz->teleport + 1;
    for (int i = 0; i < 32; ++i) {
        int index = (i & 24) | ((first + i) & 7);
        Vec3f pos;
        if (!Wiz_TestWaypoint(wiz, play, index, &pos))
            continue;
        if (found) {
            Vec3f from = pos, to = wiz->warpTo;
            from.y += 35.f;
            to.y += 35.f;
            if (!Wiz_ClearLine(play, &from, &to))
                continue;
            int slot = wiz->decoyCount++;
            wiz->decoys[slot] = wiz->decoyTargets[slot] = pos;
            wiz->decoyRoute[slot] = slot;
            wiz->decoyYaw[slot] = wiz->actor.shape.rot.y;
            if (wiz->decoyCount == ARRAY_COUNT(wiz->decoys))
                break;
            continue;
        }
        wiz->teleport = index & 7;
        wiz->warpFrom = actor->world.pos;
        wiz->warpTo = pos;
        wiz->decoyTargets[ARRAY_COUNT(wiz->decoys)] = pos;
        wiz->warpTimer = 40;
        wiz->action = WIZ_DASH;
        actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
        Animation_PlayLoop(&wiz->skel, WIZ_RUN);
        found = 1;
        if (!wiz->secondPhase)
            break;
    }
    if (found) {
        if (wiz->phaseDash)
            wiz->phaseDash = 0;
        else
            Wiz_BeginAppear(wiz);
    }
}

static int Wiz_PlayerFired(PlayState* play, Player* player) {
#if defined(GAME_MM)
    return player->unk_D57 != 0;
#else
    int limit = 32;
    for (Actor* projectile = play->actorCtx.actors[ACTORCAT_ITEMACTION].first; projectile && limit--;
         projectile = projectile->next) {
        if (projectile->id == ACTOR_EN_ARROW && projectile->update && !projectile->parent && projectile->speed > 0.f &&
            Math_Vec3f_DistXYZ(&projectile->world.pos, &player->actor.world.pos) < 80.f)
            return 1;
    }
    return 0;
#endif
}

static int Wiz_AppearanceThreat(StandaloneWiz* wiz, PlayState* play) {
    Actor* actor = &wiz->actor;
    if (actor->xzDistToPlayer < 200.f)
        return 1;
    Player* player = GET_PLAYER(play);
    s16 facing = actor->yawTowardsPlayer - actor->shape.rot.y;
    s16 aimed = actor->yawTowardsPlayer - (player->actor.shape.rot.y + 0x8000);
    return ABS(facing) < 0x7d0 && ABS(aimed) < 0x7d0 && Wiz_PlayerFired(play, player);
}

static void Wiz_BeginVanish(StandaloneWiz* wiz) {
    wiz->staffFlameLit = 0;
    Actor* actor = &wiz->actor;
    wiz->hurtVanish = wiz->action == WIZ_HURT;
    wiz->action = WIZ_VANISH;
    wiz->appeared = 0;
    wiz->danceSpin = 0x2710;
    Wiz_Sfx(actor, 0x3877);
    actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    if (!wiz->hurtVanish)
        Animation_PlayLoop(&wiz->skel, WIZ_IDLE);
}

static int Wiz_HasMirrorShield(PlayState* play) {
#if defined(GAME_MM)
    return GET_PLAYER(play)->currentShield == 2;
#else
    return GET_PLAYER(play)->currentShield == 3;
#endif
}

static int Wiz_IceArrowHit(ColliderCylinder* col) {
    if (!(col->base.acFlags & AC_HIT))
        return 0;
    col->base.acFlags &= ~AC_HIT;
    return col->elem.acHitElem && (col->elem.acHitElem->atDmgInfo.dmgFlags & WIZ_ICE_ARROW_MASK);
}

static void Wiz_SetProjectilePosition(ColliderCylinder* col, const Vec3f* pos) {
    col->dim.pos.x = pos->x;
    col->dim.pos.y = pos->y;
    col->dim.pos.z = pos->z;
}

static void Wiz_SpawnFireShards(StandaloneWiz* wiz) {
    s16 yaw = 0;
    for (int i = 0; i < WIZ_SHARD_COUNT; ++i) {
        WizFireShard* shard = &wiz->shards[i];
        f32 speed = 7.f + Rand_ZeroOne() * 2.f;
        shard->pos = wiz->pool;
        WizProjectile_ResetTrail(&shard->trail, &shard->pos);
        shard->velocity.x = Math_SinS(yaw) * speed;
        shard->velocity.y = 10.f;
        shard->velocity.z = Math_CosS(yaw) * speed;
        shard->scale = 0.f;
        shard->timer = 50;
        shard->smallFlame = 0;
        shard->collider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        shard->collider.base.acFlags &= ~AC_HIT;
        shard->collider.elem.atDmgInfo.damage = 16;
        yaw += 0x10000 / WIZ_SHARD_COUNT + (s16)((Rand_ZeroOne() - .5f) * 0x1000);
    }
}

static void Wiz_UpdateFireShards(StandaloneWiz* wiz, PlayState* play) {
    for (int i = 0; i < WIZ_SHARD_COUNT; ++i) {
        WizFireShard* shard = &wiz->shards[i];
        ColliderCylinder* col = &shard->collider;
        if (!shard->timer)
            continue;
        if (Wiz_IceArrowHit(col)) {
            if (shard->smallFlame)
                shard->timer = 10;
            else
                shard->timer = 0;
            Wiz_SfxWorld(play, &shard->pos, 50, NA_SE_EV_ICE_MELT);
        }
        if (!shard->timer)
            continue;
        --shard->timer;
        if (!shard->smallFlame) {
            Vec3f next = { shard->pos.x + shard->velocity.x, shard->pos.y + shard->velocity.y,
                           shard->pos.z + shard->velocity.z };
            Vec3f hit;
            CollisionPoly* poly;
            s32 bgId;
            if (BgCheck_EntityLineTest1(&play->colCtx, &shard->pos, &next, &hit, &poly, 1, 1, 1, 0, &bgId)) {
                if (!poly || poly->normal.y <= 0x4000 || shard->velocity.y >= 0.f) {
                    shard->timer = 0;
                    continue;
                }
                shard->pos = hit;
                shard->smallFlame = 1;
                shard->timer = 50 + (u8)(Rand_ZeroOne() * 50.f);
                shard->flameScaleOffset.x = (Rand_ZeroOne() - .5f) * .007f;
                shard->flameScaleOffset.y = (Rand_ZeroOne() - .5f) * .005f;
                shard->flameScaleOffset.z = (Rand_ZeroOne() - .5f) * .007f;
                shard->flameScroll = (u16)(Rand_ZeroOne() * 10000.f);
                shard->scale = 0.f;
                col->elem.atDmgInfo.damage = 2;
            } else {
                shard->pos = next;
                WizProjectile_RecordTrail(&shard->trail, &next);
                shard->velocity.y -= 1.f;
            }
        }
        if (shard->smallFlame) {
            ++shard->flameScroll;
            if (shard->timer <= 10) {
                Math_ApproachF(&shard->scale, .04f, .2f, .002f);
                continue;
            }
            Math_ApproachF(&shard->scale, .02f, .3f, .01f);
        } else {
            Math_ApproachF(&shard->scale, .01f, .2f, .01f);
        }
        Wiz_SetProjectilePosition(col, &shard->pos);
        col->dim.radius = shard->smallFlame ? 10 : 25;
        col->dim.height = shard->smallFlame ? MAX(2, (s16)(shard->scale * 5000.f)) : 25;
        col->dim.yShift = shard->smallFlame ? 0 : -5;
        Wiz_SetAC(play, &play->colChkCtx, &col->base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &col->base);
    }
}

static void Wiz_Launch(StandaloneWiz* wiz, PlayState* play) {
    Actor* actor = &wiz->actor;
    Vec3f target = GET_PLAYER(play)->actor.world.pos;
    wiz->boltPos = actor->world.pos;
    wiz->boltPos.x += 40.f * Math_SinS(actor->world.rot.y);
    wiz->boltPos.y += 60.f;
    wiz->boltPos.z += 40.f * Math_CosS(actor->world.rot.y);
    Vec3f delta = { target.x - wiz->boltPos.x, target.y - wiz->boltPos.y, target.z - wiz->boltPos.z };
    float distance = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    if (distance < 15.f)
        return;
    wiz->boltVelocity.x = delta.x * 12.f / distance;
    wiz->boltVelocity.y = delta.y * 12.f / distance;
    wiz->boltVelocity.z = delta.z * 12.f / distance;
    wiz->boltTimer = 100;
    wiz->boltScale = 0.f;
    wiz->boltTailTimer = 0;
    WizProjectile_ResetTrail(&wiz->boltTrail, &wiz->boltPos);
    wiz->bolt.dim.radius = wiz->bolt.dim.height = 25;
    wiz->bolt.dim.yShift = -5;
    wiz->bolt.base.acFlags &= ~AC_HIT;
    wiz->bolt.base.atFlags &= ~(AT_TYPE_PLAYER | AT_HIT | AT_BOUNCED);
    wiz->bolt.base.atFlags |= AT_TYPE_ENEMY;
    wiz->bolt.elem.atDmgInfo.dmgFlags = Wiz_HasMirrorShield(play) ? 0xf7cfffff : 0x20000000;
    wiz->bolt.elem.atDmgInfo.damage = wiz->ice ? 8 : 16;
#if defined(GAME_MM)
    wiz->bolt.elem.atDmgInfo.effect = wiz->ice ? 2 : 9;
#else
    wiz->bolt.elem.atDmgInfo.effect = wiz->ice ? 2 : 1;
#endif
    Wiz_Sfx(actor, 0x38a8);
    Wiz_Sfx(actor, 0x0879);
}

static void Wiz_SpawnIceSmoke(StandaloneWiz* wiz) {
    if (wiz->poolRadius <= 0)
        return;
    for (int i = 0; i < WIZ_ICE_SMOKE_COUNT; ++i) {
        WizIceSmoke* smoke = &wiz->iceSmoke[i];
        if (smoke->age)
            continue;

        f32 spread = MIN(75.f, wiz->poolRadius * .70710678f);
        f32 x = (Rand_ZeroOne() * 2.f - 1.f) * spread;
        f32 z = (Rand_ZeroOne() * 2.f - 1.f) * spread;
        f32 drift = MAX(0.f, wiz->poolRadius - sqrtf(x * x + z * z));
        f32 accel = MIN(.15f, drift * (.70710678f / 276.f));
        smoke->pos = (Vec3f){ wiz->pool.x + x, wiz->pool.y, wiz->pool.z + z };
        smoke->velocity = (Vec3f){ 0 };
        smoke->accel = (Vec3f){ (Rand_ZeroOne() * 2.f - 1.f) * accel, .23f, (Rand_ZeroOne() * 2.f - 1.f) * accel };
        smoke->scale = .02f + Rand_ZeroOne() * .005f;
        smoke->scroll = (u8)(Rand_ZeroOne() * 100.f);
        smoke->age = 1;
        ++wiz->iceSmokeCount;
        return;
    }
}

static void Wiz_UpdateIceSmoke(StandaloneWiz* wiz) {
    if (!wiz->iceSmokeCount)
        return;
    for (int i = 0; i < WIZ_ICE_SMOKE_COUNT; ++i) {
        WizIceSmoke* smoke = &wiz->iceSmoke[i];
        if (!smoke->age)
            continue;
        if (++smoke->age == 24) {
            smoke->age = 0;
            --wiz->iceSmokeCount;
            continue;
        }
        ++smoke->scroll;
        smoke->pos.x += smoke->velocity.x;
        smoke->pos.y += smoke->velocity.y;
        smoke->pos.z += smoke->velocity.z;
        smoke->velocity.x += smoke->accel.x;
        smoke->velocity.y += smoke->accel.y;
        smoke->velocity.z += smoke->accel.z;
    }
}

static int Wiz_HasActiveProjectile(const StandaloneWiz* wiz) {
    return wiz->boltTimer || wiz->boltScale || wiz->boltTailTimer || wiz->poolTimer || (!wiz->ice && wiz->poolAlpha);
}

static void Wiz_UpdateBolt(StandaloneWiz* wiz, PlayState* play) {
    Player* player = GET_PLAYER(play);
#if defined(GAME_MM)
    if (wiz->ice && (wiz->bolt.base.atFlags & AT_HIT))
        wiz->iceContact = 2;
    if (wiz->iceContact) {
        if (Wiz_HasActiveProjectile(wiz) && (player->stateFlags2 & 0x4000)) {
            player->av2.actionVar2 = MAX(player->av2.actionVar2, 90);
        } else {
            --wiz->iceContact;
        }
    }
#endif
    Wiz_UpdateFireShards(wiz, play);
    if (wiz->poolTimer) {
        if (!wiz->ice && Wiz_IceArrowHit(&wiz->bolt)) {
            wiz->poolTimer = 0;
            Wiz_SfxWorld(play, &wiz->pool, 50, NA_SE_EV_ICE_MELT);
            return;
        }
        --wiz->poolTimer;
        Wiz_SetProjectilePosition(&wiz->bolt, &wiz->pool);
        f32 size = wiz->ice ? .022f : .02f;
        Math_ApproachF(&wiz->poolScale, size, .3f, wiz->ice ? .01f : .002f);
        if (wiz->ice) {
            Wiz_SpawnIceSmoke(wiz);
        } else {
            Math_ApproachF(&wiz->poolSmokeScale, .02f, .3f, .002f);
            Math_ApproachF(&wiz->poolFlameScale, .02f, .3f, .2f);
        }
        if (wiz->boltTailTimer)
            --wiz->boltTailTimer;
        f32 radius = wiz->poolScale * (wiz->ice ? 4300.f : 4000.f);
        wiz->bolt.dim.radius = MIN(wiz->poolRadius, (s16)radius);
        wiz->bolt.dim.height = wiz->ice ? 30 : MAX(2, (s16)(wiz->poolFlameScale * 1850.f));
        wiz->bolt.dim.yShift = wiz->ice ? 15 : -15;
        if (!wiz->ice)
            Wiz_SetAC(play, &play->colChkCtx, &wiz->bolt.base);
        wiz->bolt.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        if (wiz->bolt.dim.radius > 0 && !player->invincibilityTimer)
            CollisionCheck_SetAT(play, &play->colChkCtx, &wiz->bolt.base);
        return;
    }
    if (wiz->poolAlpha) {
        if (wiz->ice)
            Math_ApproachF(&wiz->poolScale, 0.f, .046f, .001f);
        else
            Math_ApproachF(&wiz->poolFlameScale, 0.f, .1f, .01f);
        if (wiz->ice || wiz->poolFlameScale < .01f)
            wiz->poolAlpha = MAX(0, wiz->poolAlpha - (wiz->ice ? 5 : 10));
        if (!wiz->poolAlpha)
            wiz->poolScale = 0.f;
    }
    if (!wiz->boltTimer) {
        if (wiz->boltTailTimer)
            --wiz->boltTailTimer;
        if (wiz->boltScale) {
            Math_ApproachF(&wiz->boltScale, 0.f, .2f, .01f);
            if (wiz->boltScale < .001f) {
                wiz->boltScale = 0.f;
                wiz->boltTailTimer = 12;
            }
        }
        return;
    }
    Math_ApproachF(&wiz->boltScale, .02f, .2f, .01f);
    if (!wiz->ice && Wiz_IceArrowHit(&wiz->bolt)) {
        wiz->boltTimer = 0;
        Wiz_SfxWorld(play, &wiz->boltPos, 50, NA_SE_EV_ICE_MELT);
        return;
    }
    if ((wiz->bolt.base.atFlags & AT_BOUNCED) && Wiz_HasMirrorShield(play)) {
        Actor* bolt = Actor_Spawn(&play->actorCtx, play, ACTOR_ENEMY_WIZ_REFLECTED_BOLT, wiz->boltPos.x, wiz->boltPos.y,
                                  wiz->boltPos.z, 0, 0, 0, wiz->ice);
        if (bolt) {
            bolt->velocity.x = -wiz->boltVelocity.x;
            bolt->velocity.y = -.5f * wiz->boltVelocity.y;
            bolt->velocity.z = -wiz->boltVelocity.z;
            Actor_SetScale(bolt, wiz->boltScale);
            ((WizReflectedBolt*)bolt)->trail = wiz->boltTrail;
        }
        wiz->bolt.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        wiz->boltScale = 0.f;
        wiz->boltTimer = 0;
        Wiz_SfxWorld(play, &wiz->boltPos, 50, NA_SE_IT_SHIELD_REFLECT_MG);
        return;
    }
    if (wiz->bolt.base.atFlags & AT_BOUNCED) {
        wiz->bolt.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        wiz->boltTimer = 0;
        return;
    }
    if (wiz->bolt.base.atFlags & AT_HIT) {
        wiz->bolt.base.atFlags &= ~AT_HIT;
        Wiz_SfxWorld(play, &wiz->boltPos, 50, 0x38b0);
        if (player->invincibilityTimer > 0)
            player->invincibilityTimer = MIN(127, player->invincibilityTimer + (wiz->ice ? 90 : 40));
    }
    Vec3f next = { wiz->boltPos.x + wiz->boltVelocity.x, wiz->boltPos.y + wiz->boltVelocity.y,
                   wiz->boltPos.z + wiz->boltVelocity.z };
    Vec3f impact;
    CollisionPoly* poly;
    s32 bgId;
    if (BgCheck_EntityLineTest1(&play->colCtx, &wiz->boltPos, &next, &impact, &poly, 1, 1, 1, 0, &bgId)) {
        wiz->boltPos = impact;
        WizProjectile_RecordTrail(&wiz->boltTrail, &impact);
        wiz->boltTimer = 0;
        if (bgId == BGCHECK_SCENE && poly && poly->normal.y > 0x4000 && wiz->boltVelocity.y < 0.f) {
            wiz->pool = impact;
            wiz->pool.y += 1.f;
            wiz->poolTimer = wiz->ice ? 100 : 70 + (u8)(Rand_ZeroOne() * 30.f);
            wiz->poolScale = wiz->poolFlameScale = wiz->poolSmokeScale = 0.f;
            wiz->poolAlpha = 255;
            wiz->boltTailTimer = 12;
            wiz->poolRadius = wiz->ice ? 94 : 80;
            Vec3f from = wiz->pool;
            from.y += 20.f;
            for (int i = 0; i < 8; ++i) {
                Vec3f to = from;
                to.x += Math_SinS(i * 0x2000) * wiz->poolRadius;
                to.z += Math_CosS(i * 0x2000) * wiz->poolRadius;
                Vec3f wall;
                if (BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &wall, &poly, 1, 0, 0, 0, &bgId)) {
                    f32 room = Math_Vec3f_DistXZ(&from, &wall);
                    wiz->poolRadius = MAX(0, (s16)room - 2);
                }
            }
            wiz->boltScale = 0.f;
            if (!wiz->ice) {
                Wiz_SpawnFireShards(wiz);
                Wiz_SfxWorld(play, &wiz->pool, 35, 0x180e);
                Wiz_SfxWorld(play, &wiz->pool, 50, 0x38ab);
            } else {
                Wiz_SfxWorld(play, &wiz->pool, 50, 0x28b2);
            }
        }
        return;
    }
    wiz->boltPos = next;
    WizProjectile_RecordTrail(&wiz->boltTrail, &next);
    Wiz_SetProjectilePosition(&wiz->bolt, &next);
    --wiz->boltTimer;
    if (!wiz->ice)
        Wiz_SetAC(play, &play->colChkCtx, &wiz->bolt.base);
    if (!player->invincibilityTimer)
        CollisionCheck_SetAT(play, &play->colChkCtx, &wiz->bolt.base);
}

static void Wiz_Dash(StandaloneWiz* wiz, PlayState* play) {
    Actor* actor = &wiz->actor;
    Vec3f* pos = &actor->world.pos;
    actor->shape.rot.y = actor->world.rot.y = Math_Vec3f_Yaw(pos, &wiz->warpTo);
    for (int i = WIZ_TRAIL_COUNT - 1; i > 0; --i)
        wiz->trail[i] = wiz->trail[i - 1];
    WizGhost* ghost = &wiz->trail[0];
    ghost->pos = *pos;
    ghost->yaw = actor->shape.rot.y;
    ghost->alpha = 128;
    actor->prevPos = *pos;
    Vec3f next = *pos;
    Math_ApproachF(&next.x, wiz->warpTo.x, .3f, 30.f);
    Math_ApproachF(&next.y, wiz->warpTo.y, .3f, 30.f);
    Math_ApproachF(&next.z, wiz->warpTo.z, .3f, 30.f);
    Vec3f from = *pos, to = next;
    from.y += 35.f;
    to.y += 35.f;
    if (!Wiz_ClearLine(play, &from, &to) || !wiz->warpTimer) {
        wiz->warpTo = *pos;
        wiz->decoyTargets[ARRAY_COUNT(wiz->decoys)] = *pos;
        Wiz_BeginAppear(wiz);
        return;
    }
    *pos = next;
    if (fabsf(pos->x - wiz->warpTo.x) + fabsf(pos->y - wiz->warpTo.y) + fabsf(pos->z - wiz->warpTo.z) < 4.f) {
        Wiz_BeginAppear(wiz);
    }
}

static void Wiz_AdvanceDecoy(StandaloneWiz* wiz, PlayState* play, int slot) {
    int index = wiz->decoyRoute[slot];
    int platform = ARRAY_COUNT(wiz->decoys);
    Vec3f from = wiz->decoys[slot];
    from.y += 35.f;
    for (int hop = 0; hop <= wiz->decoyCount; ++hop) {
        index = index == platform ? wiz->decoyCount - 1 : index ? index - 1 : platform;
        Vec3f to = wiz->decoyTargets[index];
        to.y += 35.f;
        if (Wiz_ClearLine(play, &from, &to)) {
            wiz->decoyRoute[slot] = index;
            return;
        }
    }
}

static __attribute__((noinline)) void Wiz_UpdateDecoys(StandaloneWiz* wiz, PlayState* play) {
    int route = wiz->action == WIZ_DANCE || wiz->action == WIZ_CAST || wiz->action == WIZ_RECOVER;
    Player* player = GET_PLAYER(play);
    for (int i = 0; i < wiz->decoyCount; ++i) {
        Vec3f* pos = &wiz->decoys[i];
        if (R_TRANS_FADE_FLASH_ALPHA_STEP != 0) {
            static Color_RGBA8 prim = { 250, 250, 250, 255 };
            static Color_RGBA8 env = { 180, 180, 180, 255 };
            Vec3f rise = { 0.f, 1.f, 0.f };
            for (int puff = 0; puff < 9; ++puff) {
                Vec3f smoke = *pos;
                smoke.x += (20.f + Rand_ZeroOne() * 20.f) * (Rand_ZeroOne() < .5f ? -1.f : 1.f);
                smoke.y += 55.f + Rand_ZeroOne() * 30.f;
                smoke.z += (20.f + Rand_ZeroOne() * 20.f) * (Rand_ZeroOne() < .5f ? -1.f : 1.f);
                s16 scale = 350 + Rand_ZeroOne() * 100.f;
                s16 step = 20 + Rand_ZeroOne() * 10.f;
#if defined(GAME_MM)
                func_800B0DE0(play, &smoke, &rise, &rise, &prim, &env, scale, step);
#else
                EffectSsDust_Spawn(play, 0, &smoke, &rise, &rise, &prim, &env, scale, step, 20, 0);
#endif
            }
            Wiz_SfxWorld(play, pos, 50, 0x38a7);
            continue;
        }
        if (!wiz->decoyRunning) {
            if (wiz->action == WIZ_APPEAR || wiz->action == WIZ_WAIT)
                wiz->decoyYaw[i] = Math_Vec3f_Yaw(pos, &player->actor.world.pos);
            continue;
        }
        if (!route)
            continue;
        Vec3f* target = &wiz->decoyTargets[wiz->decoyRoute[i]];
        f32 dx = pos->x - target->x;
        f32 dy = pos->y - target->y;
        f32 dz = pos->z - target->z;
        if (dx * dx + dy * dy + dz * dz < 30.f * 30.f) {
            Wiz_AdvanceDecoy(wiz, play, i);
            target = &wiz->decoyTargets[wiz->decoyRoute[i]];
        }
        Vec3f next = *pos;
        Math_ApproachF(&next.x, target->x, .3f, 30.f);
        Math_ApproachF(&next.y, target->y, .3f, 30.f);
        Math_ApproachF(&next.z, target->z, .3f, 30.f);
        Vec3f from = *pos, to = next;
        from.y += 35.f;
        to.y += 35.f;
        if (Wiz_ClearLine(play, &from, &to))
            *pos = next;
        else
            Wiz_AdvanceDecoy(wiz, play, i);
        wiz->decoyYaw[i] = Math_Vec3f_Yaw(pos, &wiz->decoyTargets[wiz->decoyRoute[i]]);
    }
    if (R_TRANS_FADE_FLASH_ALPHA_STEP != 0)
        wiz->decoyCount = wiz->decoyRunning = 0;
}

static void Wiz_Update(Actor* actor, PlayState* play) {
    StandaloneWiz* wiz = (StandaloneWiz*)actor;
    int finished = SkelAnime_Update(&wiz->skel);
    if (wiz->timer)
        --wiz->timer;
    if (wiz->warpTimer)
        --wiz->warpTimer;
    if (wiz->action != WIZ_DASH) {
        for (int i = 0; i < WIZ_TRAIL_COUNT; ++i)
            wiz->trail[i].alpha = wiz->trail[i].alpha > 16 ? wiz->trail[i].alpha - 16 : 0;
    }
    if (wiz->action == WIZ_DEFEAT) {
        Wiz_UpdateIceSmoke(wiz);
        actor->shape.rot.y += wiz->defeatSpin;
        Math_SmoothStepToS(&wiz->defeatSpin, 0, 10, 0xbb8, 0x14);
        if (wiz->defeatSpin < 0x1e) {
            if (wiz->defeatAlpha == 255) {
                Vec3f still = { 0.f, 0.f, 0.f };
                for (int i = 0; i < 12; ++i) {
                    Vec3f flame = actor->world.pos;
                    flame.x += Rand_ZeroOne() * 36.f - 18.f;
                    flame.y += Rand_ZeroOne() * 60.f;
                    flame.z += Rand_ZeroOne() * 36.f - 18.f;
                    s16 scale = 70 + (s16)(Rand_ZeroOne() * 50.f);
#if defined(GAME_MM)
                    func_800B3030(play, &flame, &still, &still, scale, 10, 1);
#else
                    EffectSsDeadDb_Spawn(play, &flame, &still, &still, scale, 10, 255, 255, 255, 255, 0, 255, 0, 0, 9,
                                         0);
#endif
                }
            }
            wiz->defeatAlpha = MAX(0, wiz->defeatAlpha - 20);
            if (wiz->defeatAlpha < 30) {
                Wiz_SfxWorld(play, &actor->world.pos, 50, NA_SE_EN_EXTINCT);
                Actor_Kill(actor);
            }
        }
        return;
    }

    Math_ApproachF(&wiz->staffFlameScale, wiz->staffFlameLit ? .006f : 0.f, .3f, .002f);
    if (wiz->decoyCount) {
        if (wiz->decoyRunning && wiz->action != WIZ_APPEAR)
            SkelAnime_Update(&wiz->decoySkel);
        Wiz_UpdateDecoys(wiz, play);
    }
    Wiz_UpdateBolt(wiz, play);
    Wiz_UpdateIceSmoke(wiz);
    if (wiz->action != WIZ_APPEAR && wiz->bodyAlpha < 255)
        Math_SmoothStepToS(&wiz->bodyAlpha, 255, 1, 5, 0);
    if (wiz->action == WIZ_DASH) {
        Wiz_Dash(wiz, play);
        return;
    }
    if (wiz->action == WIZ_APPEAR) {
        if (!wiz->appearanceFled) {
            if (!Wiz_AppearanceThreat(wiz, play)) {
                wiz->escapeChecked = 0;
            } else if (!wiz->escapeChecked) {
                Vec3f escape;
                wiz->escapeChecked = 1;
                if (Wiz_FindEscape(wiz, play, &escape) >= 0) {
                    wiz->appearanceFled = wiz->escapePending = 1;
                    wiz->timer = 0;
                    Wiz_BeginVanish(wiz);
                    return;
                }
            }
        }
        Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 10, 0xbb8, 0);
        actor->shape.rot.y = actor->world.rot.y;
        if (wiz->secondPhase)
            Wiz_Sfx(actor, 0x38a6 - SFX_FLAG);
        if (wiz->timer)
            return;
        f32 scale = actor->scale.x;
        Math_ApproachF(&scale, .015f, .05f, .01f);
        Math_SmoothStepToS(&wiz->bodyAlpha, 255, 1, 5, 0);
        Actor_SetScale(actor, scale);
        if (scale < .0138f)
            return;
        actor->flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
        actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        wiz->appeared = 1;
        wiz->action = WIZ_WAIT;
    }
    if (wiz->body.base.acFlags & AC_HIT) {
        wiz->body.base.acFlags &= ~AC_HIT;
        if (actor->colChkInfo.damage) {
            Actor_ApplyDamage(actor);
            Animation_PlayOnce(&wiz->skel, WIZ_DAMAGE);
            Actor_SetColorFilter(actor, 0x4000, 255, 0, 8);
            Actor_SetScale(actor, .015f);
            wiz->bodyAlpha = 255;
            wiz->hurtSpin = 0x4e20;
            wiz->hurtFloor = actor->world.pos.y;
            u8 effect = actor->colChkInfo.damageEffect;
            int frozen = !wiz->ice && effect == 3;
            int fire = wiz->ice && effect == 2;
            int light = effect == 4;
            int defeated = !actor->colChkInfo.health && wiz->secondPhase;
            wiz->damageType = frozen ? 11 : light ? 20 : 0;
            wiz->damageTimer = frozen ? 80 : fire || light ? 40 : 0;
            if (frozen)
                wiz->hurtSpin = 0;
            wiz->hurtHop = defeated && !wiz->damageTimer;
            actor->velocity.y = wiz->hurtHop ? 30.f : 0.f;
            actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
            if (defeated)
                actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            Wiz_Sfx(actor, defeated ? 0x38aa : 0x38a9);
            wiz->action = WIZ_HURT;
            wiz->timer = 20;
        }
    }

    if (wiz->action == WIZ_HURT) {
        if (wiz->damageTimer) {
            --wiz->damageTimer;
            if (wiz->damageTimer >= (wiz->damageType == 11 ? 50 : 30))
                return;
            if (wiz->damageType == 11 && wiz->damagePartMask == 0xfff)
                Wiz_ShatterIce(play, actor, wiz->damageParts, ARRAY_COUNT(wiz->damageParts), 2, 1.f, .7f);
            wiz->damageTimer = 0;
            wiz->hurtSpin = 0x4e20;
            wiz->hurtHop = 1;
            actor->velocity.y = 30.f;
        }
        actor->world.rot.y += wiz->hurtSpin;
        actor->shape.rot.y = actor->world.rot.y;
        Math_SmoothStepToS(&wiz->hurtSpin, 0, 10, 0xbb8, 0x14);
        for (int i = 0; i < wiz->decoyCount; ++i)
            wiz->decoyYaw[i] += wiz->hurtSpin;
        if (wiz->hurtHop) {
            if (actor->velocity.y > 0.f) {
                Vec3f from = actor->world.pos, to = from;
                from.y += 65.f;
                to.y = from.y + actor->velocity.y;
                if (!Wiz_ClearLine(play, &from, &to))
                    actor->velocity.y = 0.f;
            }
            actor->world.pos.y = MAX(wiz->hurtFloor, actor->world.pos.y + actor->velocity.y);
            actor->velocity.y -= 3.f;
            if (actor->world.pos.y > wiz->hurtFloor || actor->velocity.y > 0.f)
                return;
            wiz->hurtHop = 0;
            actor->velocity.y = 0.f;
        }
        if (wiz->timer)
            return;
        if (!actor->colChkInfo.health && !wiz->secondPhase) {
            wiz->secondPhase = wiz->phaseDash = 1;
            actor->colChkInfo.health = wiz->ice ? 6 : 8;
        } else if (!actor->colChkInfo.health) {
            wiz->action = WIZ_DEFEAT;
            wiz->decoyCount = wiz->decoyRunning = 0;
            wiz->defeatSpin = 0x2710;
            wiz->defeatAlpha = 255;
            wiz->boltTimer = wiz->poolTimer = wiz->boltTailTimer = 0;
            wiz->boltScale = wiz->poolScale = 0.f;
            wiz->poolAlpha = 0;
            for (int i = 0; i < WIZ_SHARD_COUNT; ++i)
                wiz->shards[i].timer = 0;
            return;
        }
    }

    if (wiz->action == WIZ_VANISH) {
        actor->world.rot.y += wiz->danceSpin;
        Math_SmoothStepToS(&wiz->danceSpin, 0, 10, 0xbb8, 0x14);
        f32 scale = actor->scale.x;
        if (!wiz->hurtVanish) {
            for (int i = 0; i < wiz->decoyCount; ++i)
                wiz->decoyYaw[i] += wiz->danceSpin;
        }
        Math_ApproachF(&scale, 0.f, .3f, wiz->secondPhase && !wiz->hurtVanish ? .001f : .01f);
        Actor_SetScale(actor, scale);
        if (scale >= .001f) {
            actor->shape.rot.y = actor->world.rot.y;
            return;
        }
        Actor_SetScale(actor, .015f);
        Wiz_Reposition(wiz, play);
        if (wiz->action == WIZ_DASH || wiz->action == WIZ_APPEAR)
            return;
        actor->flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
        actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        wiz->phaseDash = 0;
        wiz->action = WIZ_WAIT;
        wiz->timer = 20;
        Animation_PlayLoop(&wiz->skel, WIZ_IDLE);
    }
    if (wiz->action == WIZ_DANCE)
        Wiz_Sfx(actor, 0x38a5 - SFX_FLAG);
    if (wiz->action == WIZ_DANCE) {
        actor->world.rot.y += wiz->danceSpin;
        Math_SmoothStepToS(&wiz->danceSpin, 0x1388, 0x64, 0x3e8, 0x3e8);
    } else if (wiz->action == WIZ_CAST) {
        Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 0xc8, 0x1f40, 0x1388);
    } else if (wiz->action != WIZ_RECOVER) {
        actor->world.rot.y = actor->yawTowardsPlayer;
    }
    actor->shape.rot.y = actor->world.rot.y;
    actor->focus.pos = actor->world.pos;
    actor->focus.pos.y += 60.f;
    if (wiz->action == WIZ_RECOVER && !wiz->timer) {
        if (Animation_OnFrame(&wiz->skel, 6.f) && !Wiz_HasActiveProjectile(wiz))
            Wiz_Launch(wiz, play);
        if (wiz->skel.curFrame >= 8.f && !wiz->attackDelayStarted) {
            wiz->timer = 3;
            wiz->attackDelayStarted = 1;
        }
    }
    if (!wiz->timer) {
        if (wiz->action == WIZ_WAIT && (wiz->appeared || actor->xzDistToPlayer < 360.f)) {
            wiz->appeared = wiz->appearanceFled = 0;
            Wiz_Sfx(actor, 0x38a4);
            wiz->action = WIZ_DANCE;
            wiz->staffFlameLit = 1;
            wiz->windupLoops = 3;
            wiz->danceSpin = 0;
            wiz->decoyRunning = wiz->decoyCount != 0;
            if (wiz->decoyRunning)
                Animation_PlayLoop(&wiz->decoySkel, WIZ_RUN);
            Animation_PlayOnce(&wiz->skel, WIZ_DANCE_ANIM);
        } else if (wiz->action == WIZ_DANCE) {
            if (finished && wiz->windupLoops) {
                if (--wiz->windupLoops)
                    Animation_PlayOnce(&wiz->skel, WIZ_DANCE_ANIM);
            }
            if (!wiz->windupLoops && !Wiz_HasActiveProjectile(wiz)) {
                wiz->action = WIZ_CAST;
                wiz->windupLoops = 2;
                Animation_PlayOnce(&wiz->skel, WIZ_WINDUP);
            }
        } else if (wiz->action == WIZ_CAST && finished) {
            if (--wiz->windupLoops) {
                Animation_PlayOnce(&wiz->skel, WIZ_WINDUP);
            } else {
                wiz->action = WIZ_RECOVER;
                wiz->attackDelayStarted = 0;
                Animation_PlayOnce(&wiz->skel, WIZ_ATTACK);
            }
        } else if ((wiz->action == WIZ_RECOVER && wiz->skel.curFrame >= wiz->skel.endFrame) ||
                   wiz->action == WIZ_HURT) {
            Wiz_BeginVanish(wiz);
            return;
        }
    }
    Collider_UpdateCylinder(actor, &wiz->body);
    if (wiz->action != WIZ_HURT)
        Wiz_SetAC(play, &play->colChkCtx, &wiz->body.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &wiz->body.base);
}

static void Wiz_DrawFloorEffects(StandaloneWiz* wiz, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);
    GraphicsContext* gfx = play->state.gfxCtx;
    u32 frame = play->gameplayFrames;
    Matrix_Push();
    if (wiz->warpTimer) {
        gSPSegment(POLY_XLU_DISP++, 9, Gfx_TwoTexScroll(gfx, 0, 0, 0, 16, 32, 1, frame * 2, frame * 2, 32, 32));
        gSPSegment(POLY_XLU_DISP++, 10, Gfx_TexScroll(gfx, frame, 0, 32, 32));
        for (int i = 0; i < 2; ++i) {
            Vec3f* pos = i ? &wiz->warpTo : &wiz->warpFrom;
            Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_NEW);
            Matrix_Scale(.01f, .01f, .01f, MTXMODE_APPLY);
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 128, 255, 255, 255, wiz->warpTimer * 12);
            gDPSetEnvColor(POLY_XLU_DISP++, wiz->ice ? 50 : 255, 0, wiz->ice ? 255 : 100, 255);
            gSPMatrix(POLY_XLU_DISP++, Wiz_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x06001860);
        }
    }
    if (wiz->poolAlpha && wiz->poolRadius > 0) {
        Matrix_Translate(wiz->pool.x, wiz->pool.y, wiz->pool.z, MTXMODE_NEW);
        f32 radiusScale = wiz->poolRadius / (wiz->ice ? 4300.f : 4000.f);
        f32 scale = MIN(wiz->poolScale, radiusScale);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        gDPPipeSync(POLY_XLU_DISP++);
        gSPMatrix(POLY_XLU_DISP++, Wiz_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        if (wiz->ice) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, wiz->poolAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 40, 30, 80);
            gSPDisplayList(POLY_XLU_DISP++, 0x06005190);
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 215, 215, 215, wiz->poolAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, 128);
            gSPSegment(
                POLY_XLU_DISP++, 8,
                Gfx_TwoTexScroll(gfx, 0, -frame & 127, -frame & 127, 32, 64, 1, frame & 255, frame & 255, 16, 16));
            gSPDisplayList(POLY_XLU_DISP++, 0x06005750);
        } else {
            gSPSegment(POLY_XLU_DISP++, 8,
                       Gfx_TwoTexScroll(gfx, 0, -frame & 127, 0, 32, 32, 1, frame * 2 & 127, 0, 32, 32));
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 100, 40, 0, wiz->poolAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 245, 255, 128);
            gSPDisplayList(POLY_XLU_DISP++, 0x06003120);

            Matrix_Translate(wiz->pool.x, wiz->pool.y, wiz->pool.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            f32 smoke = MIN(wiz->poolSmokeScale, radiusScale);
            Matrix_Scale(smoke, smoke, smoke, MTXMODE_APPLY);
            gDPPipeSync(POLY_XLU_DISP++);
            gSPSegment(POLY_XLU_DISP++, 8,
                       Gfx_TwoTexScroll(gfx, 0, frame & 127, (-frame * 6) & 255, 32, 64, 1, frame * 2 & 127,
                                        (-frame * 6) & 255, 32, 64));
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 80, 0, 0, wiz->poolAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, 100);
            gSPMatrix(POLY_XLU_DISP++, Wiz_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x06003640);

            Matrix_Translate(wiz->pool.x, wiz->pool.y, wiz->pool.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(MIN(wiz->poolFlameScale, radiusScale), wiz->poolFlameScale,
                         MIN(wiz->poolFlameScale, radiusScale), MTXMODE_APPLY);
            gDPPipeSync(POLY_XLU_DISP++);
            gSPSegment(POLY_XLU_DISP++, 8,
                       Gfx_TwoTexScroll(gfx, 0, (-frame * 3) & 127, 0, 32, 32, 1, 0, (-frame * 10) & 255, 32, 64));
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 100, 50, 0, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 200, 235, 240, 128);
            gSPMatrix(POLY_XLU_DISP++, Wiz_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x06003fc0);
        }
    }
    Matrix_Pop();
    CLOSE_DISPS();
}

#if defined(GAME_OOT)
void Scene_SetRenderModeXlu(PlayState* play, s32 index, u32 flags);
#endif

static void Wiz_DrawTrail(StandaloneWiz* wiz, PlayState* play) {
    Scene_SetRenderModeXlu(play, 1, 2);
    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Push();
    for (int i = WIZ_TRAIL_COUNT + wiz->decoyCount - 1; i >= 0; --i) {
        WizGhost* ghost;
        Vec3f* pos;
        Vec3s* pose;
        int alpha;
        s16 yaw;
        if (i >= WIZ_TRAIL_COUNT) {
            pos = &wiz->decoys[i - WIZ_TRAIL_COUNT];
            pose = wiz->decoyRunning ? wiz->decoyJoint : wiz->joint;
            alpha = 100;
            yaw = wiz->decoyYaw[i - WIZ_TRAIL_COUNT];
        } else {
            ghost = &wiz->trail[i];
            alpha = ghost->alpha - 10 * i;
            if (alpha <= 0)
                continue;
            pos = &ghost->pos;
            pose = wiz->joint;
            yaw = ghost->yaw;
        }
        Matrix_Translate(pos->x, pos->y + 10.f, pos->z, MTXMODE_NEW);
        f32 scale = i >= WIZ_TRAIL_COUNT ? wiz->actor.scale.x : .015f;
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        Matrix_RotateYS(yaw, MTXMODE_APPLY);
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, alpha);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, wiz->skel.skeleton, pose, wiz->skel.dListCount, NULL, NULL,
                                           &wiz->actor, POLY_XLU_DISP);
    }
    Matrix_Pop();
    CLOSE_DISPS();
}

static void Wiz_DrawProjectiles(StandaloneWiz* wiz, PlayState* play) {
    GraphicsContext* gfx = play->state.gfxCtx;
    OPEN_DISPS(gfx);
    Matrix_Push();
    if (wiz->boltScale || wiz->boltTailTimer) {
        int first = wiz->boltScale > 0.f ? 0 : 6 - (wiz->boltTailTimer + 1) / 2;
        POLY_XLU_DISP =
            WizProjectile_DrawTrail(POLY_XLU_DISP, play, &wiz->boltTrail, 0x06002b40, wiz->boltScale, first, wiz->ice);
    }
    for (int i = 0; i < WIZ_SHARD_COUNT; ++i) {
        WizFireShard* shard = &wiz->shards[i];
        if (!shard->timer)
            continue;
        if (!shard->smallFlame) {
            POLY_XLU_DISP = WizProjectile_DrawTrail(POLY_XLU_DISP, play, &shard->trail, 0x06002b40, shard->scale, 0, 0);
            continue;
        }
        if (shard->scale <= 0.f)
            continue;
        int alpha = shard->timer > 10 ? 255 : MAX(0, 255 - (10 - shard->timer) * 35);
        u16 scroll = shard->flameScroll;
        Matrix_Translate(shard->pos.x, shard->pos.y + 20.f, shard->pos.z, MTXMODE_NEW);
        Matrix_Scale(shard->scale + shard->flameScaleOffset.x, shard->scale + shard->flameScaleOffset.y,
                     shard->scale + shard->flameScaleOffset.z, MTXMODE_APPLY);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        gDPPipeSync(POLY_XLU_DISP++);
        gSPSegment(POLY_XLU_DISP++, 8,
                   Gfx_TwoTexScroll(gfx, 0, 0, 0, 32, 32, 1, scroll & 127, (-scroll * 10) & 127, 32, 32));
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 100, 50, 0, alpha);
        gDPSetEnvColor(POLY_XLU_DISP++, 200, 235, 245, 255);
        gSPMatrix(POLY_XLU_DISP++, Wiz_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, 0x06002630);
    }
    Matrix_Pop();
    CLOSE_DISPS();
}

static void Wiz_DrawIceSmoke(StandaloneWiz* wiz, PlayState* play) {
    if (!wiz->iceSmokeCount)
        return;
    GraphicsContext* gfx = play->state.gfxCtx;
    OPEN_DISPS(gfx);
    Matrix_Push();
    gSPDisplayList(POLY_XLU_DISP++, 0x06000e70);
    gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_AA_ZB_XLU_SURF2);
    gSPClearGeometryMode(POLY_XLU_DISP++, G_CULL_BACK | G_FOG);
    for (int i = 0; i < WIZ_ICE_SMOKE_COUNT; ++i) {
        WizIceSmoke* smoke = &wiz->iceSmoke[i];
        if (!smoke->age)
            continue;
        int alpha = smoke->age <= 11 ? (smoke->age - 1) * 10 : 100 - (smoke->age - 11) * 8;
        int scroll = smoke->scroll;
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 195, 225, 235, alpha);
        gSPSegment(POLY_XLU_DISP++, 8,
                   Gfx_TwoTexScroll(gfx, 0, (scroll * 3) & 127, (scroll * 15) & 255, 32, 64, 1, 0, 0, 32, 32));
        Matrix_Translate(smoke->pos.x, smoke->pos.y, smoke->pos.z, MTXMODE_NEW);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_Scale(smoke->scale, smoke->scale, 1.f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Wiz_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, 0x06000fd8);
    }
    Matrix_Pop();
    CLOSE_DISPS();
}

static void Wiz_DamageLimb(PlayState* play, s32 limb, Gfx** dList, Vec3s* rot, void* arg) {
    static const u8 limbs[] = { 1, 2, 3, 4, 6, 7, 10, 11, 12, 13, 15, 18 };
    StandaloneWiz* wiz = arg;
    if (limb == 9) {
        Vec3f tip = { 5300.f, 0.f, 0.f };
        if (wiz->action == WIZ_DANCE) {
            tip.x = 7300.f;
            tip.y = -1500.f;
        }
        Matrix_MultVec3f(&tip, &wiz->staffTip);
        wiz->staffPoseValid = 1;
    }
    for (int i = 0; i < ARRAY_COUNT(limbs); ++i) {
        if (limb != limbs[i])
            continue;
        Vec3f origin = { 0.f, 0.f, 0.f };
        Matrix_MultVec3f(&origin, &wiz->damageParts[i]);
        wiz->damagePartMask |= 1u << i;
        break;
    }
}

static void Wiz_SampleLimbXlu(PlayState* play, s32 limb, Gfx** dList, Vec3s* rot, Actor* actor, Gfx** gfx) {
    Wiz_DamageLimb(play, limb, dList, rot, actor);
}

static void Wiz_DrawStaffFlame(StandaloneWiz* wiz, PlayState* play) {
    if (!wiz->staffPoseValid || wiz->staffFlameScale < .0001f || wiz->action == WIZ_DEFEAT)
        return;
    GraphicsContext* gfx = play->state.gfxCtx;
    Wiz_SetupXlu(gfx);
    OPEN_DISPS(gfx);
    Matrix_Push();
    Matrix_Translate(wiz->staffTip.x, wiz->staffTip.y, wiz->staffTip.z, MTXMODE_NEW);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    Matrix_Scale(wiz->staffFlameScale, wiz->staffFlameScale, wiz->staffFlameScale, MTXMODE_APPLY);
    gDPPipeSync(POLY_XLU_DISP++);
    u32 scroll = (wiz->staffFlameScroll * 10u - play->state.frameCount * 20u) & 511;
    gSPSegment(POLY_XLU_DISP++, 8, Gfx_TwoTexScroll(gfx, 0, 0, 0, 32, 64, 1, 0, scroll, 32, 128));
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, wiz->ice ? 170 : 255, 255, wiz->ice ? 255 : 170, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, wiz->ice ? 0 : 255, 50, wiz->ice ? 255 : 0, 255);
    gSPMatrix(POLY_XLU_DISP++, Wiz_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
#if defined(GAME_OOT)
    gSPDisplayList(POLY_XLU_DISP++, 0x04052a10);
#else
    gSPDisplayList(POLY_XLU_DISP++, 0x0407d590);
#endif
    Matrix_Pop();
    CLOSE_DISPS();
}

static void Wiz_Draw(Actor* actor, PlayState* play) {
    StandaloneWiz* wiz = (StandaloneWiz*)actor;
    if (wiz->action == WIZ_DEFEAT && wiz->defeatAlpha != 255) {
        Wiz_SetupXlu(play->state.gfxCtx);
        Scene_SetRenderModeXlu(play, 1, 2);
        OPEN_DISPS(play->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, wiz->defeatAlpha);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, wiz->skel.skeleton, wiz->joint, wiz->skel.dListCount, NULL, NULL,
                                           actor, POLY_XLU_DISP);
        CLOSE_DISPS();
        Wiz_DrawIceSmoke(wiz, play);
        return;
    }
    wiz->damagePartMask = 0;
    wiz->staffPoseValid = 0;
    if (wiz->bodyAlpha == 255) {
        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        Scene_SetRenderModeXlu(play, 0, 1);
        OPEN_DISPS(play->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
        CLOSE_DISPS();
        SkelAnime_DrawFlexOpa(play, wiz->skel.skeleton, wiz->skel.jointTable, wiz->skel.dListCount, NULL,
                              Wiz_DamageLimb, actor);
    } else if (wiz->bodyAlpha) {
        Wiz_SetupXlu(play->state.gfxCtx);
        Scene_SetRenderModeXlu(play, 1, 2);
        OPEN_DISPS(play->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, wiz->bodyAlpha);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, wiz->skel.skeleton, wiz->joint, wiz->skel.dListCount, NULL,
                                           Wiz_SampleLimbXlu, actor, POLY_XLU_DISP);
        CLOSE_DISPS();
    }
    Wiz_DrawStaffFlame(wiz, play);
    if (wiz->damageTimer && wiz->damagePartMask == 0xfff) {
        f32 scale = wiz->damageType == 11 ? MIN(.7f, (80 - wiz->damageTimer) * .04f) : .7f;
        Wiz_DrawDamageEffects(play, actor, wiz->damageParts, ARRAY_COUNT(wiz->damageParts), scale, 1.5f, 1.f,
                              wiz->damageType);
    }
    int hasProjectile = wiz->boltScale > 0.f || wiz->boltTailTimer;
    for (int i = 0; i < WIZ_SHARD_COUNT; ++i)
        hasProjectile |= wiz->shards[i].timer != 0;
    if (!hasProjectile && !wiz->iceSmokeCount && !wiz->poolAlpha && !wiz->warpTimer && !wiz->trail[0].alpha &&
        !wiz->decoyCount)
        return;
    Wiz_SetupXlu(play->state.gfxCtx);
    Wiz_DrawFloorEffects(wiz, play);
    if (wiz->trail[0].alpha || wiz->decoyCount)
        Wiz_DrawTrail(wiz, play);
    if (hasProjectile)
        Wiz_DrawProjectiles(wiz, play);
    Wiz_DrawIceSmoke(wiz, play);
}

#if defined(GAME_OOT)
static ActorProfile sWizProfile = {
    0xd5d,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED |
        ACTOR_FLAG_DRAW_CULLING_DISABLED,
    0x1178,
    sizeof(StandaloneWiz),
    Wiz_Init,
    Wiz_Destroy,
    Wiz_Update,
    Wiz_Draw,
};
OVL_INFO_NATIVE_ENEMY(0x15d, sWizProfile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_WIZ, sWizProfile, Wiz_Init, Wiz_Destroy, Wiz_Update, Wiz_Draw);
static const u32 kWizOrigin[] = { 0x4e505431, 1, 0x15d, 0x178, 0x01232f00 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_WIZ_ORIGIN, kWizOrigin);
#else
static ActorProfile sWizProfile = {
    ACTOR_EN_WIZ,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED |
        ACTOR_FLAG_DRAW_CULLING_DISABLED,
    0x178,
    sizeof(StandaloneWiz),
    Wiz_Init,
    Wiz_Destroy,
    Wiz_Update,
    Wiz_Draw,
};
OVL_INFO_ACTOR(0x127, sWizProfile);
#endif
