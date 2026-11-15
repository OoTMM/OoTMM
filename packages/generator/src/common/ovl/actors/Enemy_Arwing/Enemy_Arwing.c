#include <combo.h>
#include <combo/custom.h>
#include <combo/effect.h>
#include <combo/matrix.h>
#include <combo/rand.h>

#if defined(GAME_OOT)
#define ARWING_ACTOR 0x073
#define Arwing_ToMtx ((Mtx* (*)(Mtx*))0x800ab8d8)
#define ARWING_HIT_SFX NA_SE_EN_FANTOM_THUNDER_GND
#define ARWING_CRASH_SFX NA_SE_EN_DODO_K_BREATH
#else
#define ARWING_ACTOR 0x056
#define Arwing_ToMtx ((Mtx* (*)(Mtx*))0x80181a18)
#define ARWING_HIT_SFX NA_SE_IT_SWORD_REFLECT_MG
#define ARWING_CRASH_SFX NA_SE_IT_BOMB_EXPLOSION2
#endif

enum { ARWING_FLYING, ARWING_TARGET_LOCKED, ARWING_CRASHING, ARWING_DEBRIS, ARWING_LASER };

typedef struct {
    Vec3f pos;
    Vec3f velocity;
    Vec3s rot;
    f32 scale;
} ArwingDebris;

typedef struct {
    Actor actor;
    ColliderCylinder collider;
    Vec3f target;
    Vec3f impulse;
    ArwingDebris debris[15];
    s16 turnStep;
    s16 bankStep;
    s16 timer;
    s16 lockTimer;
    u8 state;
    u8 hitTimer;
    u8 blocked;
    u8 shortRoute;
} EnemyArwing;

static ColliderCylinderInit sBody = {
    { COL_MATERIAL_HIT3, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_NONE, OC2_NONE, COLSHAPE_CYLINDER },
    { ELEM_MATERIAL_UNK0,
      { 0xffcfffff, 0, 4 },
      { 0xffdfffff, 0, 0 },
      ATELEM_ON | ATELEM_SFX_NORMAL,
      ACELEM_ON,
      OCELEM_NONE },
    { 20, 15, -5, { 0, 0, 0 } },
};

static int Arwing_HitGeometry(PlayState* play, Vec3f* from, Vec3f* to, Vec3f* hit, CollisionPoly** poly)
{
    s32 bgId;
    return BgCheck_EntityLineTest1(&play->colCtx, from, to, hit, poly, 1, 1, 1, 0, &bgId);
}

static Vec3f Arwing_Velocity(s16 yaw, s16 pitch, f32 speed)
{
    f32 horizontal = Math_CosS(pitch) * speed;
    return (Vec3f){ Math_SinS(yaw) * horizontal, -Math_SinS(pitch) * speed, Math_CosS(yaw) * horizontal };
}

static void Arwing_Init(Actor* actor, PlayState* play)
{
    EnemyArwing* wing = (EnemyArwing*)actor;
    Collider_InitCylinder(play, &wing->collider);
    Collider_SetCylinder(play, &wing->collider, actor, &sBody);
    if (actor->params == 1)
    {
        wing->state = ARWING_LASER;
        wing->timer = 70;
        actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
        Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_PROP);
        actor->shape.rot = actor->world.rot;
        actor->scale = (Vec3f){ .4f, .4f, 2.f };
        actor->velocity = Arwing_Velocity(actor->world.rot.y, actor->world.rot.x, 70.f);
        Vec3f from = actor->world.pos;
        Vec3f next = { from.x + actor->velocity.x * .5f, from.y + actor->velocity.y * .5f,
                       from.z + actor->velocity.z * .5f };
        Vec3f hit;
        CollisionPoly* poly;
        if (Arwing_HitGeometry(play, &from, &next, &hit, &poly))
        {
            Actor_Kill(actor);
            return;
        }
        actor->world.pos = next;
        wing->collider.base.acFlags = AC_NONE;
        wing->collider.dim.radius = 23;
        wing->collider.dim.height = 25;
        wing->collider.dim.yShift = -10;
        Actor_PlaySfx(actor, NA_SE_IT_SWORD_REFLECT_MG);
    }
    else
    {
        actor->attentionRangeType = 5;
        actor->colChkInfo.health = 3;
        wing->state = ARWING_FLYING;
        Actor_SetScale(actor, .2f);
        if (!comboEnemySceneObject(CUSTOM_OBJECT_ID_ARWING))
            Actor_Kill(actor);
    }
}

static void Arwing_Destroy(Actor* actor, PlayState* play)
{
    Collider_DestroyCylinder(play, &((EnemyArwing*)actor)->collider);
}

static void Arwing_Retarget(EnemyArwing* wing, PlayState* play, Player* player)
{
    Actor* actor = &wing->actor;
    wing->shortRoute = 0;
    if (!wing->lockTimer)
    {
        wing->state = ARWING_TARGET_LOCKED;
        wing->timer = 300;
    }
    else
    {
        wing->state = ARWING_FLYING;
        wing->timer = 20 + (s16)(Rand_ZeroOne() * 50.f);
        s16 yaw = player->actor.shape.rot.y;
        Vec3f candidate = player->actor.world.pos;
        candidate.x += Math_SinS(yaw) * 400.f + (Rand_ZeroOne() - .5f) * 700.f;
        candidate.y += 150.f + Rand_ZeroOne() * 200.f;
        candidate.z += Math_CosS(yaw) * 400.f + (Rand_ZeroOne() - .5f) * 700.f;
        Vec3f hit;
        CollisionPoly* poly;
        if (!Arwing_HitGeometry(play, &actor->world.pos, &candidate, &hit, &poly))
        {
            wing->target = candidate;
        }
        else
        {
            wing->shortRoute = 1;
            wing->target = actor->world.pos;
            for (int i = 0; i < 12; ++i)
            {
                s16 direction = actor->world.rot.y + 0x3000 * (i + 1);
                f32 radius = i < 8 ? 100.f : 35.f;
                candidate = actor->world.pos;
                candidate.x += radius * Math_SinS(direction);
                candidate.z += radius * Math_CosS(direction);
                candidate.y += i & 1 ? 18.f : 0.f;
                if (!Arwing_HitGeometry(play, &actor->world.pos, &candidate, &hit, &poly))
                {
                    wing->target = candidate;
                    break;
                }
            }
        }
    }
    wing->turnStep = wing->blocked ? 0x800 : 0;
    wing->bankStep = 0;
}

static void Arwing_StartDebris(EnemyArwing* wing, PlayState* play)
{
    Actor* actor = &wing->actor;
    Vec3f zero = { 0 };
    wing->state = ARWING_DEBRIS;
    wing->timer = 70;
    actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
    Actor_PlaySfx(actor, NA_SE_IT_BOMB_EXPLOSION);
    EffectSsBomb2_SpawnLayered(play, &actor->world.pos, &zero, &zero, 160, 22);
    for (int i = 0; i < ARRAY_COUNT(wing->debris); ++i)
    {
        ArwingDebris* part = &wing->debris[i];
        part->pos = actor->world.pos;
        s16 yaw = i * 0x433b;
        part->velocity.x = Math_SinS(yaw) * i * .3f + (Rand_ZeroOne() - .5f) * .5f;
        part->velocity.z = Math_CosS(yaw) * i * .3f + (Rand_ZeroOne() - .5f) * .5f;
        part->velocity.y = 5.f + Rand_ZeroOne() * 6.f;
        part->rot.x = (s16)(u16)(Rand_ZeroOne() * 65535.f);
        part->rot.y = (s16)(u16)(Rand_ZeroOne() * 65535.f);
        part->scale = .075f + Rand_ZeroOne() * .15f;
    }
}

static void Arwing_UpdateDebris(EnemyArwing* wing, PlayState* play)
{
    if (!--wing->timer)
    {
        Actor_Kill(&wing->actor);
        return;
    }
    for (int i = 0; i < ARRAY_COUNT(wing->debris); ++i)
    {
        ArwingDebris* part = &wing->debris[i];
        Vec3f next = { part->pos.x + part->velocity.x, part->pos.y + part->velocity.y, part->pos.z + part->velocity.z };
        Vec3f hit;
        CollisionPoly* poly;
        if (Arwing_HitGeometry(play, &part->pos, &next, &hit, &poly))
        {
            part->velocity.x *= -.45f;
            part->velocity.z *= -.45f;
            part->velocity.y *= -.45f;
        }
        else
        {
            part->pos = next;
            part->velocity.y -= 1.f;
        }
        part->rot.x += 0x700;
        part->rot.y += 0xb00;
    }
}

static void Arwing_Update(Actor* actor, PlayState* play)
{
    EnemyArwing* wing = (EnemyArwing*)actor;
    if (wing->state == ARWING_DEBRIS)
    {
        Arwing_UpdateDebris(wing, play);
        return;
    }
    Player* player = GET_PLAYER(play);
    if (wing->timer)
        --wing->timer;
    if (wing->lockTimer)
        --wing->lockTimer;
    if (wing->hitTimer)
        --wing->hitTimer;
    if (wing->state == ARWING_LASER)
    {
        if (!wing->timer || (wing->collider.base.atFlags & (AT_HIT | AT_BOUNCED)))
        {
            Actor_Kill(actor);
            return;
        }
    }
    else if (wing->state == ARWING_CRASHING)
    {
        actor->velocity.y -= .2f;
        actor->shape.rot.x += 0x10;
        actor->shape.rot.z -= 0x1460;
        if (!wing->timer)
        {
            Arwing_StartDebris(wing, play);
            return;
        }
    }
    else
    {
        if (wing->collider.base.acFlags & AC_HIT)
        {
            wing->collider.base.acFlags &= ~AC_HIT;
            wing->hitTimer = 20;
            Actor_SetColorFilter(actor, 0x4000, 255, 0, 5);
            Actor_PlaySfx(actor, ARWING_HIT_SFX);
            wing->impulse.x = (Rand_ZeroOne() - .5f) * 15.f;
            wing->impulse.y = (Rand_ZeroOne() - .5f) * 15.f;
            wing->impulse.z = (Rand_ZeroOne() - .5f) * 15.f;
            if (actor->colChkInfo.health && !--actor->colChkInfo.health)
            {
                wing->state = ARWING_CRASHING;
                wing->timer = 90;
                actor->velocity.y = 0.f;
                Actor_PlaySfx(actor, ARWING_CRASH_SFX);
                return;
            }
        }
        if (!wing->timer)
            Arwing_Retarget(wing, play, player);
        int locked = wing->state == ARWING_TARGET_LOCKED;
        if (locked)
        {
            wing->target = player->actor.world.pos;
            wing->target.y += 40.f;
        }
        f32 distance = Math_Vec3f_DistXYZ(&actor->world.pos, &wing->target);
        if (distance < (locked ? 150.f : wing->shortRoute ? 20.f : 100.f))
        {
            wing->timer = 0;
            if (locked)
                wing->lockTimer = 100 + (s16)(Rand_ZeroOne() * 100.f);
            wing->state = ARWING_FLYING;
        }
        s16 yaw = Math_Vec3f_Yaw(&actor->world.pos, &wing->target);
        f32 horizontal = Math_Vec3f_DistXZ(&actor->world.pos, &wing->target);
        f32 vertical = actor->world.pos.y - wing->target.y;
#if defined(GAME_OOT)
        s16 pitch = Math_Atan2S(horizontal, vertical);
#else
        s16 pitch = Math_Atan2S_XY(horizontal, vertical);
#endif
        s16 yawLeft = Math_SmoothStepToS(&actor->world.rot.y, yaw, locked ? 7 : 10, wing->turnStep, 0);
        Math_ApproachS(&actor->world.rot.x, pitch, locked ? 7 : 10, wing->turnStep);
        wing->turnStep = MIN(locked ? 0x1000 : 0x800, wing->turnStep + 0x100);
        int aimed = ABS(yawLeft) < 0x1000;
        s16 bank = aimed ? 0 : yawLeft > 0 ? -0x2500 : 0x2500;
        Math_ApproachS(&actor->world.rot.z, bank, aimed ? 15 : 10, wing->bankStep);
        wing->bankStep = MIN(aimed ? 0x500 : 0x1000, wing->bankStep + (aimed ? 0x100 : 0x200));
        actor->shape.rot = actor->world.rot;
        actor->velocity = Arwing_Velocity(actor->world.rot.y, actor->world.rot.x, 7.f);
        actor->velocity.x += wing->impulse.x;
        actor->velocity.y += wing->impulse.y;
        actor->velocity.z += wing->impulse.z;
        Math_StepToF(&wing->impulse.x, 0.f, 1.f);
        Math_StepToF(&wing->impulse.y, 0.f, 1.f);
        Math_StepToF(&wing->impulse.z, 0.f, 1.f);
        if (wing->state == ARWING_TARGET_LOCKED && aimed && !(play->gameplayFrames & 3) && Rand_ZeroOne() < .75f)
        {
            Actor_Spawn(&play->actorCtx, play, ARWING_ACTOR, actor->world.pos.x, actor->world.pos.y, actor->world.pos.z,
                        actor->world.rot.x, actor->world.rot.y, actor->world.rot.z, 1);
        }
    }
    Vec3f next = { actor->world.pos.x + actor->velocity.x, actor->world.pos.y + actor->velocity.y,
                   actor->world.pos.z + actor->velocity.z };
    Vec3f hit;
    CollisionPoly* poly;
    if (Arwing_HitGeometry(play, &actor->world.pos, &next, &hit, &poly))
    {
        if (wing->state == ARWING_LASER)
        {
            Actor_Kill(actor);
            return;
        }
        if (wing->state == ARWING_CRASHING)
        {
            Arwing_StartDebris(wing, play);
            return;
        }
        wing->state = ARWING_FLYING;
        wing->blocked = 1;
        wing->lockTimer = MAX(wing->lockTimer, 20);
        wing->timer = 0;
    }
    else
    {
        actor->world.pos = next;
        wing->blocked = 0;
    }
    actor->focus.pos = actor->world.pos;
    Collider_UpdateCylinder(actor, &wing->collider);
    if (wing->state < ARWING_CRASHING)
        CollisionCheck_SetAC(play, &play->colChkCtx, &wing->collider.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &wing->collider.base);
}

static void Arwing_Draw(Actor* actor, PlayState* play)
{
    EnemyArwing* wing = (EnemyArwing*)actor;
    void* model = comboEnemySceneObject(CUSTOM_OBJECT_ID_ARWING);
    if (!model)
        return;
    GraphicsContext* gfx = play->state.gfxCtx;
    Gfx_SetupDL25_Opa(gfx);
    Gfx_SetupDL25_Xlu(gfx);
    OPEN_DISPS(gfx);
    gSPSegment(POLY_OPA_DISP++, 6, model);
    gSPSegment(POLY_XLU_DISP++, 6, model);
    Matrix_Push();
    if (wing->state == ARWING_LASER)
    {
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 255, 0, 255);
        Matrix_Translate(25.f, 0.f, 0.f, MTXMODE_APPLY);
        for (int i = 0; i < 2; ++i)
        {
            gSPMatrix(POLY_XLU_DISP++, Arwing_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, CUSTOM_OBJECT_ARWING_1);
            Matrix_Translate(-50.f, 0.f, 0.f, MTXMODE_APPLY);
        }
    }
    else if (wing->state == ARWING_DEBRIS)
    {
        gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_ARWING_3);
        for (int i = 0; i < ARRAY_COUNT(wing->debris); ++i)
        {
            ArwingDebris* part = &wing->debris[i];
            Matrix_SetTranslateRotateYXZ(part->pos.x, part->pos.y, part->pos.z, &part->rot);
            Matrix_Scale(part->scale, part->scale, part->scale, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Arwing_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_ARWING_4);
        }
    }
    else
    {
        f32 shake = wing->hitTimer * .05f;
        Matrix_RotateX(Math_SinS(play->gameplayFrames * 0x3000) * shake, MTXMODE_APPLY);
        Matrix_RotateY(Math_SinS(play->gameplayFrames * 0x3700) * shake, MTXMODE_APPLY);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
        gSPMatrix(POLY_OPA_DISP++, Arwing_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_ARWING_0);
        Matrix_Translate(0.f, 0.f, -60.f, MTXMODE_APPLY);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        f32 flicker = play->gameplayFrames & 1 ? 1.15f : 1.f;
        Matrix_Scale(2.5f * flicker, 1.3f * flicker, 0.f, MTXMODE_APPLY);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 200, 155);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 50, 0, 0);
        gSPMatrix(POLY_XLU_DISP++, Arwing_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, CUSTOM_OBJECT_ARWING_2);
    }
    Matrix_Pop();
    gSPSegment(POLY_OPA_DISP++, 6, gSegments[6] + K0BASE);
    gSPSegment(POLY_XLU_DISP++, 6, gSegments[6] + K0BASE);
    CLOSE_DISPS();
}

static ActorProfile sArwingProfile = {
    ARWING_ACTOR,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED |
        ACTOR_FLAG_DRAW_CULLING_DISABLED,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnemyArwing),
    Arwing_Init,
    Arwing_Destroy,
    Arwing_Update,
    Arwing_Draw,
};

OVL_INFO_ACTOR(ARWING_ACTOR, sArwingProfile);
