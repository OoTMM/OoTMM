#include <combo.h>
#include <combo/sfx.h>
#include <combo/player.h>
#include <combo/mask.h>
#include <combo/math.h>
#include <combo/enemy_native.h>
#include <combo/enemy.h>

extern void* comboEnemyResolveKeepPointer(void* pointer);

ActorProfile* comboEnemyNativeProfile(Actor* actor, s16 id, s16 objectId)
{
    ActorOvl* overlay = actor->ovl;
    ActorProfile* profile;

    if (actor->id != id || !overlay || !overlay->data ||
        overlay->vramEnd < overlay->vramStart || (u32)overlay->dataInit < overlay->vramStart ||
        (u32)overlay->dataInit > overlay->vramEnd ||
        overlay->vramEnd - (u32)overlay->dataInit < sizeof(*profile))
        return NULL;
    profile = (ActorProfile*)((u8*)overlay->data + (u32)overlay->dataInit - overlay->vramStart);
    return objectId < 0 || (profile->id == id && profile->objectId == objectId) ? profile : NULL;
}

void comboEnemySetColorFilter(Actor* actor, u16 color, u16 intensity, u16 buffer, u16 duration)
{
    if (!actor)
        return;
#if defined(GAME_OOT)
    if (color == 0x8000 && !(intensity & 0x8000))
        Actor_PlaySfx(actor, NA_SE_EN_LIGHT_ARROW_HIT);
    Actor_SetColorFilter(actor, color, intensity, buffer, duration);
#else
    Actor_SetColorFilter(actor, color, intensity | 0x8000, buffer, duration);
#endif
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_COLOR_FILTER, comboEnemySetColorFilter);

static u8 enemyMatrixMode(s32 mode)
{
#if defined(GAME_MM)
    mode = (u8)mode;
#endif
    return mode == 1;
}

void comboEnemyMatrixScale(float x, float y, float z, s32 mode)
{
    Matrix_Scale(x, y, z, enemyMatrixMode(mode));
}

void comboEnemyMatrixTranslate(float x, float y, float z, s32 mode)
{
    Matrix_Translate(x, y, z, enemyMatrixMode(mode));
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_SCALE, comboEnemyMatrixScale);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_TRANSLATE, comboEnemyMatrixTranslate);

s32 comboEnemyPlayerMask(PlayState* play)
{
    Player* player;
    if (!play || !(player = GET_PLAYER(play)))
        return 0;
#if defined(GAME_OOT)
    switch (player->mask)
    {
    case MASK_KEATON: return 0x05;
    case MASK_BUNNY: return 0x04;
    case MASK_TRUTH: return 0x01;
    case MASK_BLAST: return 0x12;
    case MASK_STONE: return 0x10;
    case MASK_KAMARO: return 0x0e;
    default: return 0;
    }
#else
    switch (player->currentMask)
    {
    case MASK_KEATON: return 0x01;
    case MASK_BUNNY: return 0x04;
    case MASK_TRUTH: return 0x08;
    case MASK_BLAST: return 0x09;
    case MASK_STONE: return 0x0a;
    case MASK_KAMARO: return 0x0b;
    default: return 0;
    }
#endif
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_PLAYER_MASK, comboEnemyPlayerMask);

void comboEnemyVec3sCopy(Vec3s* dest, Vec3s* source)
{
    if (dest && source)
        *dest = *source;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_VEC3S_COPY, comboEnemyVec3sCopy);

#if defined(GAME_OOT)
f32 Math_Vec3f_StepTo(Vec3f* start, Vec3f* target, f32 speed)
{
    Vec3f diff;
    f32 f2;
    f32 f0;

    Math_Vec3f_Diff(target, start, &diff);
    f0 = Math3D_Vec3fMagnitude(&diff);
    if (speed < f0)
    {
        f2 = speed / f0;
        f0 -= speed;
        start->x += f2 * diff.x;
        start->y += f2 * diff.y;
        start->z += f2 * diff.z;
    }
    else
    {
        Math_Vec3f_Copy(start, target);
        f0 = 0.0f;
    }

    return f0;
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_VEC3F_STEP_TO, Math_Vec3f_StepTo);

void comboEnemyVec3fScaleStore(Vec3f* vec, f32 scale, Vec3f* dest)
{
    dest->x = vec->x * scale;
    dest->y = vec->y * scale;
    dest->z = vec->z * scale;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_VEC3F_SCALE_STORE, comboEnemyVec3fScaleStore);
#endif

#if defined(GAME_OOT)
s16 comboEnemyCurrentCutscene(void)
{
    return gPlay && Play_InCsMode(gPlay) ? 0x7fff : -1;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_CUTSCENE_CURRENT, comboEnemyCurrentCutscene);
#endif

void* comboEnemySegmentedToVirtual(void* ptr)
{
    void* asset = comboEnemyResolveKeepPointer(ptr);
    if (asset)
        return asset;
    return SEGMENTED_TO_VIRTUAL(ptr);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL, comboEnemySegmentedToVirtual);

void comboEnemyMatrixMultX(float value, Vec3f* dest)
{
    Vec3f source = { value, 0, 0 };
    Matrix_MultVec3f(&source, dest);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_MULT_X, comboEnemyMatrixMultX);
void comboEnemyMatrixMultY(float value, Vec3f* dest)
{
    Vec3f source = { 0, value, 0 };
    Matrix_MultVec3f(&source, dest);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_MULT_Y, comboEnemyMatrixMultY);
void comboEnemyMatrixMultZ(float value, Vec3f* dest)
{
    Vec3f source = { 0, 0, value };
    Matrix_MultVec3f(&source, dest);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_MULT_Z, comboEnemyMatrixMultZ);
void comboEnemyMatrixMultZero(Vec3f* dest)
{
    Vec3f source = { 0, 0, 0 };
    Matrix_MultVec3f(&source, dest);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_MULT_ZERO, comboEnemyMatrixMultZero);

void comboEnemyMatrixMult(MtxF* matrix, s32 mode)
{
    Matrix_Mult(matrix, enemyMatrixMode(mode));
}

void comboEnemyMatrixRotateZYX(s16 x, s16 y, s16 z, s32 mode)
{
    Matrix_RotateZYX(x, y, z, enemyMatrixMode(mode));
}

void comboEnemyMatrixRotateAxis(float angle, Vec3f* axis, s32 mode)
{
#if defined(GAME_OOT)
    Matrix_RotateAxis(angle, axis, enemyMatrixMode(mode));
#else
    Matrix_RotateAxisF(angle, axis, enemyMatrixMode(mode));
#endif
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_MULT, comboEnemyMatrixMult);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_ZYX, comboEnemyMatrixRotateZYX);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_AXIS, comboEnemyMatrixRotateAxis);
void comboEnemyMatrixRotateXS(s16 angle, s32 mode)
{
    Matrix_RotateZYX(angle, 0, 0, enemyMatrixMode(mode));
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_XS, comboEnemyMatrixRotateXS);
void comboEnemyMatrixRotateYS(s16 angle, s32 mode)
{
    Matrix_RotateZYX(0, angle, 0, enemyMatrixMode(mode));
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_YS, comboEnemyMatrixRotateYS);
void comboEnemyMatrixRotateZS(s16 angle, s32 mode)
{
    Matrix_RotateZYX(0, 0, angle, enemyMatrixMode(mode));
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS, comboEnemyMatrixRotateZS);

void comboEnemyMatrixRotateXF(float angle, s32 mode)
{
    Matrix_RotateXF(angle, enemyMatrixMode(mode));
}

void comboEnemyMatrixRotateYF(float angle, s32 mode)
{
    Matrix_RotateYF(angle, enemyMatrixMode(mode));
}

void comboEnemyMatrixRotateZF(float angle, s32 mode)
{
    Matrix_RotateZF(angle, enemyMatrixMode(mode));
}

void comboEnemyMatrixRotateXFApply(float angle)
{
    Matrix_RotateXF(angle, 1);
}

void comboEnemyMatrixRotateXFNew(float angle)
{
    Matrix_RotateXF(angle, 0);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_XF, comboEnemyMatrixRotateXF);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_YF, comboEnemyMatrixRotateYF);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF, comboEnemyMatrixRotateZF);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_XF_APPLY, comboEnemyMatrixRotateXFApply);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATRIX_ROTATE_XF_NEW, comboEnemyMatrixRotateXFNew);

s16 comboEnemyAtan2S(float first, float second)
{
    return Math_Atan2S(second, first);
}

float comboEnemyAtan2F(float first, float second)
{
    return Math_Atan2F(second, first);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_ATAN2S, comboEnemyAtan2S);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_ATAN2F, comboEnemyAtan2F);
#if defined(GAME_OOT)
s16 comboEnemyAtan2SXY(float x, float y)
{
    return Math_Atan2S(x, y);
}

float comboEnemyAtan2FXY(float x, float y)
{
    return Math_Atan2F(x, y);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_ATAN2S_XY, comboEnemyAtan2SXY);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_ATAN2F_XY, comboEnemyAtan2FXY);
#endif

#if defined(GAME_OOT)
#define EnemyNativeToSpherical ((VecSphGeo (*)(Vec3f*))0x80067894)
#define EnemyNativeToCartesian ((Vec3f (*)(VecSphGeo*))0x800677a4)
#define EnemyNativeInCsMode ((s32 (*)(PlayState*))0x8009cb08)
#else
#define EnemyNativeToSpherical ((VecSphGeo (*)(Vec3f*))0x8010c574)
#define EnemyNativeToCartesian ((Vec3f (*)(VecSphGeo*))0x8010c484)
#define EnemyNativeInCsMode ((u8 (*)(PlayState*))0x801690cc)
#endif

void comboEnemyVec3fAdd(PosRot* origin, Vec3f* offset, Vec3f* dest, s16 mode)
{
    if (!offset || !dest)
        return;
    if (mode == 1)
    {
        VecSphGeo spherical;
        Vec3f rotated;
        if (!origin)
            return;
        spherical = EnemyNativeToSpherical(offset);
        spherical.yaw += origin->rot.y;
        rotated = EnemyNativeToCartesian(&spherical);
        Math_Vec3f_Sum(&origin->pos, &rotated, dest);
    }
    else if (mode == 2)
    {
        if (origin)
            Math_Vec3f_Sum(&origin->pos, offset, dest);
    }
    else
        *dest = *offset;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_VEC3F_ADD, comboEnemyVec3fAdd);

s32 comboEnemyPlayInCsMode(PlayState* play)
{
    return play && EnemyNativeInCsMode(play) != 0;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_PLAY_IN_CS_MODE, comboEnemyPlayInCsMode);

#if defined(GAME_OOT)
#define EnemyNativeKnockbackLarge ((void (*)(PlayState*, Actor*, float, s16, float, u32))0x80022e7c)
#define EnemyNativeKnockbackSmall ((void (*)(PlayState*, Actor*, float, s16, float))0x80022f48)
#else
#define EnemyNativeKnockbackLarge ((void (*)(PlayState*, Actor*, float, s16, float, u32))0x800b8d50)
#define EnemyNativeKnockbackRequest ((void (*)(PlayState*, Actor*, float, s16, float, s32, u32))0x800b8d10)
#endif

void comboEnemyKnockbackLarge(PlayState* play, Actor* actor, float speed, s16 yaw, float yVelocity, u32 damage)
{
#if defined(GAME_OOT)
    if (actor && actor->id == 0xd3a)
        speed *= 2.f;
#endif
    EnemyNativeKnockbackLarge(play, actor, speed, yaw, yVelocity, damage);
}

void comboEnemyKnockbackLargeNoDamage(PlayState* play, Actor* actor, float speed, s16 yaw, float yVelocity)
{
    comboEnemyKnockbackLarge(play, actor, speed, yaw, yVelocity, 0);
}

void comboEnemyKnockbackSmallNoDamage(PlayState* play, Actor* actor, float speed, s16 yaw, float yVelocity)
{
#if defined(GAME_OOT)
    EnemyNativeKnockbackSmall(play, actor, speed, yaw, yVelocity);
#else
    EnemyNativeKnockbackRequest(play, actor, speed, yaw, yVelocity, 1, 0);
#endif
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_KNOCKBACK_LARGE, comboEnemyKnockbackLarge);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_KNOCKBACK_LARGE_NO_DAMAGE, comboEnemyKnockbackLargeNoDamage);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_KNOCKBACK_SMALL_NO_DAMAGE, comboEnemyKnockbackSmallNoDamage);

#if defined(GAME_OOT)
static void enemyMmSmallKnockback(PlayState* play, Actor* actor, float speed, s16 yaw, float lift, s32 type, u32 damage)
{
    if (type == 1)
        ((void (*)(PlayState*, Actor*, float, s16, float, u32))0x80022f00)(play, actor, speed, yaw, lift, damage);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MM_KNOCKBACK_SMALL, enemyMmSmallKnockback);
#endif

#if defined(GAME_OOT)
static void enemyHiploopEffectDestroy(PlayState* play, s32 index)
{
    Actor* actor = comboEnemyCurrentActor();
    if (actor && actor->id == 0xde9 && comboEnemyCurrentCallback() == 1 &&
        (actor->params == 8 || actor->params == 10 || actor->params == 13 ||
         actor->params == 17 || actor->params == 23 || actor->params == 26))
        return;
    ((void (*)(PlayState*, s32))0x8001ae04)(play, index);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_HIPLOOP_EFFECT_DESTROY, enemyHiploopEffectDestroy);

void comboEnemyDeathFlame(PlayState* play, Vec3f* position, Vec3f* velocity, Vec3f* acceleration,
                          Color_RGBA8* prim, Color_RGBA8* env, s16 scale, s16 scaleStep, s32 life)
{
    EnemyNativeSegments scope = { 0 };
    if (!prim || !env || !comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    EffectSsDeadDb_Spawn(play, position, velocity, acceleration, scale, scaleStep,
                         prim->r, prim->g, prim->b, prim->a, env->r, env->g, env->b, 0, life, 0);
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DEATH_FLAME, comboEnemyDeathFlame);

static Color_RGBA8* sEnemyDeathFlamePalette __attribute__((section(".data"))) = 0;
EXPORT_SYMBOL(FOREIGN_ENEMY_DATA_REF_DEATH_FLAME_PALETTE, sEnemyDeathFlamePalette);
void comboEnemyDeathFlamePreset(PlayState* play, Vec3f* position, Vec3f* velocity, Vec3f* acceleration,
                                s16 scale, s16 scaleStep, s32 colorIndex)
{
    if (!sEnemyDeathFlamePalette || colorIndex < 0 || colorIndex >= 4)
        return;
    comboEnemyDeathFlame(play, position, velocity, acceleration, sEnemyDeathFlamePalette,
                         &sEnemyDeathFlamePalette[1 + colorIndex], scale, scaleStep, 9);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DEATH_FLAME_PRESET, comboEnemyDeathFlamePreset);
#endif

extern void comboEnemyNativeEffectSpawn(PlayState*, s32, s32, void*);

void comboEnemyEffectSpawn(PlayState* play, s32 type, s32 priority, void* data)
{
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyCurrentActor())
    {
        comboEnemyNativeEffectSpawn(play, type, priority, data);
        return;
    }
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    comboEnemyNativeEffectSpawn(play, type, priority, data);
    comboEnemyNativeLeave(&scope);
}
#if defined(GAME_OOT)
PATCH_FUNC(0x8001b468, comboEnemyEffectSpawn);
#else
PATCH_FUNC(0x800b04d4, comboEnemyEffectSpawn);
#endif
EXPORT_SYMBOL(FOREIGN_ENEMY_NATIVE_EFFECT_INIT_V1, comboEnemyEffectSpawn);
EXPORT_SYMBOL(FOREIGN_ENEMY_NATIVE_EFFECT_TRAMPOLINE_V1, comboEnemyNativeEffectSpawn);

extern void EnemyNativeFixedHitMark(PlayState*, s32, Vec3f*);
void enemyFixedHitMark(PlayState* play, s32 type, Vec3f* pos)
{
    EnemyNativeSegments scope = { 0 };
    if ((u32)type >= 4 || !pos)
    {
        g.roomEnemyLackSoul = 1;
        return;
    }
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    EnemyNativeFixedHitMark(play, type, pos);
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_FIXED_HIT_MARK, enemyFixedHitMark);

static void enemySparkles(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel,
                          Color_RGBA8* prim, Color_RGBA8* env, s16 scale, s32 life)
{
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    EffectSsKiraKira_SpawnDispersed(play, pos, vel, accel, prim, env, scale, life);
    comboEnemyNativeLeave(&scope);
}

static void enemySmallSparkles(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel,
                               Color_RGBA8* prim, Color_RGBA8* env)
{
    enemySparkles(play, pos, vel, accel, prim, env, 1000, 16);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPARKLE_SMALL, enemySmallSparkles);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPARKLE_DISPERSED, enemySparkles);

void enemyFootSparkle(PlayState* play, Vec3f* pos)
{
    static Vec3f velocity = { 0, -1.5f, 0 };
    static Vec3f acceleration = { 0, -0.2f, 0 };
    static Color_RGBA8 prim = { 200, 160, 120, 255 };
    static Color_RGBA8 env = { 130, 90, 50, 255 };
    enemySmallSparkles(play, pos, &velocity, &acceleration, &prim, &env);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_FOOT_SPARKLE, enemyFootSparkle);

#if defined(GAME_OOT)
static u8 enemyFreezardDamage(Actor* actor)
{
    if (actor->colChkInfo.damageEffect == 2 && actor->colChkInfo.damage)
        actor->colChkInfo.damage = actor->colChkInfo.health;
    return Actor_ApplyDamage(actor);
}

PATCH_CALL(0x80a7a82c, enemyFreezardDamage);
PATCH_CALL(0x80a7a910, enemyFreezardDamage);
#endif
