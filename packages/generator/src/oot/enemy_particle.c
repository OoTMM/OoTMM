#include <combo.h>
#include <combo/effect.h>
#include <combo/math.h>
#include "enemy_particle_defs.h"
#include <combo/sfx_source.h>
#include <combo/enemy_native.h>
#include <combo/enemy_room_models.h>

static const EnemyParticleDescriptor sIceBlockParticle = {
    ENEMY_MM_ICE_SHARD_OBJECT,
    0,
    ENEMY_MM_ICE_SHARD_MODEL,
    40,
    128,
    ENEMY_PARTICLE_BILLBOARD | ENEMY_PARTICLE_ROTATE_Z,
    0.1f,
    0.01f,
    65535.0f,
    512.0f,
    0x400,
};

#define PARTICLE_SCALE regs[1]
#define PARTICLE_ROTATION regs[2]
#define PARTICLE_SPIN regs[3]
extern void EffectSs_Reset(EffectSs* effect);

static int enemyParticleObjects(EffectSs* particle, const EnemyParticleDescriptor* descriptor, void** object, void** keep)
{
    *object = comboEnemySceneObject(descriptor->objectId);
    *keep = descriptor->keepObjectId ? comboEnemySceneObject(descriptor->keepObjectId) : NULL;
    if (*object && (!descriptor->keepObjectId || *keep))
        return 1;
    particle->life = -1;
    particle->draw = NULL;
    return 0;
}

static void enemyParticleUpdate(PlayState* play, s32 index, EffectSs* particle)
{
    const EnemyParticleDescriptor* descriptor = particle->gfx;
    void* object;
    void* keep;
    if (!descriptor)
    {
        particle->life = -1;
        particle->draw = NULL;
        return;
    }
    particle->PARTICLE_ROTATION += particle->PARTICLE_SPIN;
    enemyParticleObjects(particle, descriptor, &object, &keep);
}

static void enemyParticleDraw(PlayState* play, s32 index, EffectSs* particle)
{
    const EnemyParticleDescriptor* descriptor = particle->gfx;
    void* object;
    void* keep;
    u32 savedObject;
    u32 savedKeep;
    float scale;
    if (!play || !descriptor || particle->life < 0 || !enemyParticleObjects(particle, descriptor, &object, &keep))
        return;
    savedObject = gSegments[6];
    savedKeep = gSegments[4];
    gSegments[6] = (u32)object & 0x1fffffff;
    if (keep)
        gSegments[4] = (u32)keep & 0x1fffffff;
    scale = particle->PARTICLE_SCALE * descriptor->scaleBase * descriptor->scaleUnits;
    OPEN_DISPS(play->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 6, object);
    if (keep)
        gSPSegment(POLY_XLU_DISP++, 4, keep);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    Matrix_Translate(particle->pos.x, particle->pos.y, particle->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    if (descriptor->flags & ENEMY_PARTICLE_BILLBOARD)
        Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);
    if (descriptor->flags & ENEMY_PARTICLE_ROTATE_Z)
        Matrix_RotateZYX(0, 0, particle->PARTICLE_ROTATION, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, descriptor->model);
    gSPSegment(POLY_XLU_DISP++, 6, savedObject | 0x80000000);
    if (keep)
        gSPSegment(POLY_XLU_DISP++, 4, savedKeep | 0x80000000);
    CLOSE_DISPS();
    gSegments[6] = savedObject;
    gSegments[4] = savedKeep;
}

static void enemyParticleSpawn(PlayState* play, const EnemyParticleDescriptor* descriptor,
                               Vec3f* position, Vec3f* velocity, Vec3f* acceleration, s16 scale)
{
    EffectSs particle;
    void* object;
    void* keep;
    if (!play || !descriptor || !position || !velocity || !acceleration)
        return;
    EffectSs_Reset(&particle);
    particle.pos = *position;
    particle.velocity = *velocity;
    particle.accel = *acceleration;
    particle.life = descriptor->life;
    particle.priority = descriptor->priority;
    particle.type = 0xff;
    particle.gfx = (void*)descriptor;
    particle.update = enemyParticleUpdate;
    particle.draw = enemyParticleDraw;
    particle.PARTICLE_SCALE = scale;
    particle.PARTICLE_ROTATION = (s16)(s32)(Rand_ZeroOne() * descriptor->initialRotationRange);
    particle.PARTICLE_SPIN = (s32)(Rand_ZeroOne() * descriptor->spinRange) + descriptor->spinBase;
    enemyParticleObjects(&particle, descriptor, &object, &keep);
    EffectSs_Insert(play, &particle);
}

void comboEnemyIceBlockParticle(PlayState* play, Vec3f* position, Vec3f* velocity, Vec3f* acceleration, s16 scale)
{
    enemyParticleSpawn(play, &sIceBlockParticle, position, velocity, acceleration, scale);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_PARTICLE_ICE_BLOCK, comboEnemyIceBlockParticle);

void comboEnemyIceShatter(PlayState* play, Actor* actor, Vec3f* points, s32 count,
                          s32 perPoint, float scale, float spread)
{
    static Color_RGBA8 prim = { 170, 255, 255, 255 };
    static Color_RGBA8 env = { 200, 200, 255, 255 };
    static Vec3f acceleration = { 0, -1, 0 };
    if (!play || !actor || (count > 0 && !points))
        return;
    SfxSource_PlaySfxAtFixedWorldPos(play, &actor->world.pos, 30, NA_SE_EV_ICE_BROKEN);
    for (s32 point = 0; point < count; ++point)
    {
        s16 outward = Math_Vec3f_Yaw(&actor->world.pos, &points[point]);
        for (s32 particle = 0; particle < perPoint; ++particle)
        {
            Vec3f velocity;
            s16 angle = outward + ((s32)Rand_Next() >> 19);
            float speed = Rand_ZeroFloat(5);
            velocity.x = Math_SinS(angle) * speed;
            velocity.y = 8 + Rand_ZeroFloat(4);
            velocity.z = Math_CosS(angle) * speed;
            EffectSsEnIce_Spawn(play, &points[point], scale + Rand_ZeroFloat(spread),
                                &velocity, &acceleration, &prim, &env, 30);
        }
    }
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_ICE_SHATTER, comboEnemyIceShatter);

static Gfx* snowFragmentList(PlayState* play, s16 flags, s16 object, s16 life, Gfx* source)
{
    static const u32 lists[] = { 0x06005cb0, 0x06006190, 0x06006620 };
    const u16 snowmanObject = MASK_FOREIGN_OBJECT | 0x1c4;
    if (flags || (object != 0x1c4 && object != snowmanObject) || life != 20)
        return NULL;
    for (unsigned i = 0; i < ARRAY_COUNT(lists); i++)
    {
        if ((u32)source != lists[i])
            continue;
        u8* model = comboEnemySceneObject(snowmanObject);
        if (!model)
            return NULL;
        Gfx* list = (Gfx*)(model + 0x7730) + i * 4;
        if (!list[0].words.w0)
        {
            void* keep = play->objectCtx.status[play->objectCtx.mainKeepIndex].segment;
            gSPSegment(&list[0], 6, model);
            gSPDisplayList(&list[1], source);
            gSPSegment(&list[2], 6, keep);
            gSPEndDisplayList(&list[3]);
            osWritebackDCache(list, 4 * sizeof(Gfx));
        }
        comboEnemyObjectPinFrames(snowmanObject, life + 2);
        return list;
    }
    return NULL;
}

static void enemySnowFragment(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel,
                              s16 flags, s16 scale, s16 object, s16 life, Gfx* source)
{
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
    {
        g.roomEnemyLackSoul = 1;
        return;
    }
    Gfx* list = snowFragmentList(play, flags, object, life, source);
    if (list)
        EffectSsHahen_Spawn(play, pos, velocity, accel, 0, scale, OBJECT_GAMEPLAY_KEEP, life, list);
    else
        g.roomEnemyLackSoul = 1;
    comboEnemyNativeLeave(&scope);
}

static void enemySnowBurst(PlayState* play, Vec3f* pos, f32 speed, s16 flags, s16 scale, s16 scaleRange,
                           s16 count, s16 object, s16 life, Gfx* source)
{
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
    {
        g.roomEnemyLackSoul = 1;
        return;
    }
    Gfx* list = snowFragmentList(play, flags, object, life, source);
    if (list)
        EffectSsHahen_SpawnBurst(play, pos, speed, 0, scale, scaleRange, count, OBJECT_GAMEPLAY_KEEP, life, list);
    else
        g.roomEnemyLackSoul = 1;
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SNOW_FRAGMENT, enemySnowFragment);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SNOW_FRAGMENT_BURST, enemySnowBurst);

static void enemyDefaultFragmentBurst(PlayState* play, Vec3f* pos, f32 speed,
                                      s16 flags, s16 scale, s16 scaleRange, s16 count, s16 object, s16 life, Gfx* source)
{
    EnemyNativeSegments scope = { 0 };
    if (object != -1 || source || !comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    EffectSsHahen_SpawnBurst(play, pos, speed, flags, scale, scaleRange, count, -1, life, NULL);
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DEFAULT_FRAGMENT_BURST, enemyDefaultFragmentBurst);
