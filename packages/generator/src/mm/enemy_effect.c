#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/sfx.h>
#include <combo/enemy_room_models.h>

typedef struct {
    EffectSs* table;
    s32 cursor;
    s32 count;
} EnemyEffectPool;
#define EnemyNativeEffectPool (*(EnemyEffectPool*)0x801ae3a0)
#define EnemyNativeFindEffect ((s32 (*)(s32, s32*))0x800b0304)

typedef void (*EnemyEffectUpdate)(PlayState*, u32, EffectSs*);
static void enemyFlameUpdate(PlayState* play, u32 index, EffectSs* fx)
{
    ((EnemyEffectUpdate)fx->actor)(play, index, fx);
    if (fx->regs[1] == 1)
    {
        float w;
        SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, &fx->pos, &fx->vec, &w);
        Audio_PlaySfx_AtPos(&fx->vec, NA_SE_EN_EXTINCT);
    }
}

void enemyFlame(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 scale, s16 step,
                s16 r, s16 g, s16 b, s16 a, s16 er, s16 eg, s16 eb, s16 unused, s32 life, s16 playSfx)
{
    Color_RGBA8 prim = { r, g, b, a }, env = { er, eg, eb, 0 };
    EnemyNativeSegments scope = { 0 };
    s32 slot = -1;
    (void)unused;
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    if (!EnemyNativeEffectPool.table || EnemyNativeEffectPool.count <= 0)
    {
        comboEnemyNativeLeave(&scope);
        return;
    }
    if (playSfx && EnemyNativeFindEffect(120, &slot))
        slot = -1;
    EffectSsDeadDb_Spawn(play, pos, vel, accel, &prim, &env, scale, step, life);
    comboEnemyNativeLeave(&scope);
    if (slot >= 0 && slot < EnemyNativeEffectPool.count && EnemyNativeEffectPool.cursor == slot + 1)
    {
        EffectSs* fx = &EnemyNativeEffectPool.table[slot];
        if (fx->type == 0x20 && fx->priority == 120 && fx->life == life && fx->update && !fx->actor)
        {
            fx->actor = (Actor*)fx->update;
            fx->update = enemyFlameUpdate;
        }
    }
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DEATH_FLAME, enemyFlame);

static void enemyTileFragments(PlayState* play, Vec3f* pos, f32 speed, s16 flags, s16 scale,
                               s16 scaleRange, s16 count, s16 object, s16 life, Gfx* fragment)
{
    EnemyNativeSegments scope = { 0 };
    if (!play || !pos || object != 0x5f || (u32)fragment != 0x06000a60 ||
        flags || !comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
    {
        g.roomEnemyLackSoul = 1;
        return;
    }
    u8* model = comboEnemySceneObject(0x105f);
    if (model)
    {
        Gfx* list = (Gfx*)(model + 0x14f0);
        void* keep = play->objectCtx.slots[play->objectCtx.mainKeepIndex].segment;
        if (!list[0].words.w0)
        {
            gSPSegment(&list[0], 6, model);
            gSPDisplayList(&list[1], fragment);
            gSPSegment(&list[2], 6, keep);
            gSPEndDisplayList(&list[3]);
            osWritebackDCache(list, 4 * sizeof(Gfx));
        }
        if (life > 0)
            comboEnemyObjectPinFrames(0x105f, life + 2);
        EffectSsHahen_SpawnBurst(play, pos, speed, 0, scale, scaleRange, count, OBJECT_GAMEPLAY_KEEP, life, list);
    }
    else
    {
        g.roomEnemyLackSoul = 1;
    }
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_TILE_FRAGMENTS, enemyTileFragments);

static void enemyDefaultFragment(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel,
                                 s16 flags, s16 scale, s16 object, s16 life, Gfx* dList)
{
    EnemyNativeSegments scope = { 0 };
    if (flags || object != -1 || dList || !comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    EffectSsHahen_Spawn(play, pos, vel, accel, 0, scale, -1, life, NULL);
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DEFAULT_FRAGMENT, enemyDefaultFragment);

extern void EnemyNativeFlyingIce(PlayState*, Actor*, Vec3f*, Color_RGBA8*, Color_RGBA8*, f32);

void enemyFlyingIce(PlayState* play, Actor* actor, Vec3f* pos, s16 pr, s16 pg, s16 pb, s16 pa,
                    s16 er, s16 eg, s16 eb, f32 scale)
{
    Color_RGBA8 prim = { pr, pg, pb, pa };
    Color_RGBA8 env = { er, eg, eb, 255 };
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    EnemyNativeFlyingIce(play, actor, pos, &prim, &env, scale);
    comboEnemyNativeLeave(&scope);
}

void enemyFlyingIceS(PlayState* play, Actor* actor, Vec3s* pos, s16 pr, s16 pg, s16 pb, s16 pa,
                     s16 er, s16 eg, s16 eb, f32 scale)
{
    Vec3f p = { pos->x, pos->y, pos->z };
    enemyFlyingIce(play, actor, &p, pr, pg, pb, pa, er, eg, eb, scale);
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_FLYING_ICE_F, enemyFlyingIce);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_FLYING_ICE_S, enemyFlyingIceS);

s32 Camera_RequestQuake(Camera* camera, s32 unused, s16 y, s32 duration)
{
    s16 index = Quake_Request(camera, 3);
    if (index == 0)
        return 0;
    Quake_SetSpeed(index, 0x61a8);
    Quake_SetPerturbations(index, y, 0, 0, 0);
    Quake_SetDuration(index, duration);
    return 1;
}

void* comboMmEnemyEffectSegment(PlayState* play, int slot)
{
    return comboObjectSlotSegmentAndPin(&play->objectCtx, slot);
}

static void enemyVariantDust(PlayState* play, Vec3f* pos, f32 range, s32 count,
                             s16 scale, s16 step, u8 lit)
{
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    ((void (*)(PlayState*, Vec3f*, f32, s32, s16, s16, u8))0x800bbfb0)(play, pos, range, count, scale, step, lit);
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DUST_CLOUD, enemyVariantDust);

static void enemyVariantShockwave(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel,
                                  s16 scale, s16 step, s16 life)
{
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    ((void (*)(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16, s16))0x800b1a70)(play, pos, vel, accel, scale, step, life);
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SHOCKWAVE, enemyVariantShockwave);

static void enemyVariantDeadDs(PlayState* play, Vec3f* pos, s16 scale, s16 step, s16 alpha, s32 life)
{
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    ((void (*)(PlayState*, Vec3f*, s16, s16, s16, s32))0x800b31bc)(play, pos, scale, step, alpha, life);
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DEAD_DS_STATIONARY, enemyVariantDeadDs);

static void enemyVariantDeadDd(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel,
                               s16 scale, s16 step, s16 pr, s16 pg, s16 pb, s16 pa, s16 er, s16 eg, s16 eb, s16 alphaStep, s32 life)
{
    Color_RGBA8 prim = { pr, pg, pb, pa }, env = { er, eg, eb, 0 };
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    ((void (*)(PlayState*, Vec3f*, Vec3f*, Vec3f*, Color_RGBA8*, Color_RGBA8*, s16, s16, s16, s32))0x800b3088)(
        play, pos, vel, accel, &prim, &env, scale, step, alphaStep, life);
    comboEnemyNativeLeave(&scope);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DEAD_DD, enemyVariantDeadDd);
