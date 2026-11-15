#include <combo.h>
#include <combo/enemy.h>

typedef struct {
    s8 segment;
    s16 type;
    void* params;
} EnemyMaterial;
typedef struct {
    s8 x, y;
    u8 width, height;
} EnemyMaterialScroll;
typedef struct {
    u8 r, g, b, a, lod;
} EnemyMaterialPrim;
typedef struct {
    u8 r, g, b, a;
} EnemyMaterialEnv;
typedef struct
{
    u16 length, count;
    EnemyMaterialPrim* prim;
    EnemyMaterialEnv* env;
    u16* frames;
} EnemyMaterialColor;
typedef struct {
    u16 length;
    void** textures;
    u8* indices;
} EnemyMaterialCycle;
_Static_assert(sizeof(EnemyMaterial) == 8, "MM material header wire size");
_Static_assert(sizeof(EnemyMaterialColor) == 16, "MM material color wire size");
_Static_assert(sizeof(EnemyMaterialCycle) == 12, "MM material cycle wire size");

static void* enemyMaterialPointer(void* address)
{
    return address ? SEGMENTED_TO_VIRTUAL(address) : NULL;
}

static int enemyMaterialError(void)
{
    Actor* actor = comboEnemyCurrentActor();
    g.roomEnemyLackSoul = 1;
    if (actor)
        Actor_Kill(actor);
    return 0;
}

static void enemyMaterialBind(PlayState* play, s32 segment, void* list, u32 flags)
{
    OPEN_DISPS(play->state.gfxCtx);
    if (flags & 1)
        gSPSegment(POLY_OPA_DISP++, segment, list);
    if (flags & 2)
        gSPSegment(POLY_XLU_DISP++, segment, list);
    CLOSE_DISPS();
}

static int enemyMaterialFrames(EnemyMaterialColor* color, u16* frames, int linear)
{
    if (!color->length || color->count < (linear ? 2 : 1) || color->count > 50 || !frames || (linear && frames[0] != 0))
        return 0;
    for (u32 i = 1; i < color->count; ++i)
        if (frames[i] <= frames[i - 1])
            return 0;
    return !linear || frames[color->count - 1] >= color->length - 1;
}

static u8 enemyMaterialLinear(u8 first, u8 last, float t)
{
    return first + (s32)((last - first) * t);
}

static void enemyMaterialColorList(PlayState* play, s32 segment, EnemyMaterialPrim* prim,
                                   EnemyMaterialEnv* env, float alpha, u32 flags)
{
    Gfx* list = GRAPH_ALLOC(play->state.gfxCtx, 3 * sizeof(Gfx));
    Gfx* output = list;
    gDPSetPrimColor(output++, 0, prim->lod, prim->r, prim->g, prim->b, (u8)(prim->a * alpha));
    if (env)
        gDPSetEnvColor(output++, env->r, env->g, env->b, env->a);
    gSPEndDisplayList(output++);
    enemyMaterialBind(play, segment, list, flags);
}

static int enemyMaterialColor(PlayState* play, s32 segment, s16 type, EnemyMaterialColor* color,
                              float alpha, u32 step, u32 flags)
{
    EnemyMaterialPrim* prim = enemyMaterialPointer(color->prim);
    EnemyMaterialEnv* env = enemyMaterialPointer(color->env);
    u16* frames = enemyMaterialPointer(color->frames);
    EnemyMaterialPrim result;
    EnemyMaterialEnv environment;
    u32 frame;
    if (!color->length || !prim)
        return enemyMaterialError();
    frame = step % color->length;
    if (type == 2)
    {
        enemyMaterialColorList(play, segment, &prim[frame], env ? &env[frame] : NULL, alpha, flags);
        return 1;
    }
    if (!enemyMaterialFrames(color, frames, type == 3))
        return enemyMaterialError();
    if (type == 3)
    {
        u32 next = 1;
        while (next < color->count && frame > frames[next])
            ++next;
        if (next == color->count)
            return enemyMaterialError();
        float t = (float)(frame - frames[next - 1]) / (frames[next] - frames[next - 1]);
        for (u32 channel = 0; channel < sizeof(result); ++channel)
            ((u8*)&result)[channel] = enemyMaterialLinear(((u8*)&prim[next - 1])[channel], ((u8*)&prim[next])[channel], t);
        if (env)
            for (u32 channel = 0; channel < sizeof(environment); ++channel)
                ((u8*)&environment)[channel] = enemyMaterialLinear(((u8*)&env[next - 1])[channel], ((u8*)&env[next])[channel], t);
    }
    else
    {
        float weights[50];
        for (u32 sample = 0; sample < color->count; ++sample)
        {
            float numerator = 1, denominator = 1;
            for (u32 other = 0; other < color->count; ++other)
                if (other != sample)
                {
                    numerator *= (float)frame - frames[other];
                    denominator *= (float)frames[sample] - frames[other];
                }
            weights[sample] = numerator / denominator;
        }
        for (u32 channel = 0; channel < sizeof(result) + (env ? sizeof(environment) : 0); ++channel)
        {
            float sum = 0;
            for (u32 sample = 0; sample < color->count; ++sample)
                sum += weights[sample] * (channel < sizeof(result) ? ((u8*)&prim[sample])[channel] : ((u8*)&env[sample])[channel - sizeof(result)]);
            s32 value = sum;
            u8 bounded = value < 0 ? 0 : value > 255 ? 255
                                                     : value;
            if (channel < sizeof(result))
                ((u8*)&result)[channel] = bounded;
            else
                ((u8*)&environment)[channel - sizeof(result)] = bounded;
        }
    }
    enemyMaterialColorList(play, segment, &result, env ? &environment : NULL, alpha, flags);
    return 1;
}

static int enemyMaterialDraw(PlayState* play, s32 segment, s16 type, void* params, float alpha, u32 step, u32 flags)
{
    if (!params)
        return enemyMaterialError();
    if (type == 0 || type == 1)
    {
        EnemyMaterialScroll* scroll = params;
        Gfx* list;
        if (type == 0)
            list = Gfx_TexScroll(play->state.gfxCtx, scroll->x * step, -(scroll->y * step), scroll->width, scroll->height);
        else
            list = Gfx_TwoTexScroll(play->state.gfxCtx, 0, scroll[0].x * step, -(scroll[0].y * step), scroll[0].width, scroll[0].height,
                                    1, scroll[1].x * step, -(scroll[1].y * step), scroll[1].width, scroll[1].height);
        enemyMaterialBind(play, segment, list, flags);
        return 1;
    }
    if (type >= 2 && type <= 4)
        return enemyMaterialColor(play, segment, type, params, alpha, step, flags);
    if (type == 5)
    {
        EnemyMaterialCycle* cycle = params;
        void** textures = enemyMaterialPointer(cycle->textures);
        u8* indices = enemyMaterialPointer(cycle->indices);
        if (!cycle->length || !textures || !indices)
            return enemyMaterialError();
        void* texture = enemyMaterialPointer(textures[indices[step % cycle->length]]);
        if (!texture)
            return enemyMaterialError();
        enemyMaterialBind(play, segment, texture, flags);
        return 1;
    }
    return enemyMaterialError();
}

void comboEnemyMaterialMain(PlayState* play, EnemyMaterial* material, float alpha, u32 step, u32 flags)
{
    if (!play || !material || !material->segment || !(flags & 3))
        return;
    if (!(alpha >= 0 && alpha <= 1))
    {
        enemyMaterialError();
        return;
    }
    for (u32 index = 0; index < 64; ++index, ++material)
    {
        s32 segment = material->segment;
        s32 absolute = segment < 0 ? -segment : segment;
        if (!absolute || absolute > 8)
        {
            enemyMaterialError();
            return;
        }
        if (!enemyMaterialDraw(play, absolute + 7, material->type, enemyMaterialPointer(material->params), alpha, step, flags))
            return;
        if (segment < 0)
            return;
    }
    enemyMaterialError();
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_MAIN, comboEnemyMaterialMain);

#define ENEMY_MATERIAL_WRAPPERS(suffix, flags) \
void comboEnemyMaterialDraw##suffix(PlayState* play, EnemyMaterial* material) \
{ if (play) comboEnemyMaterialMain(play, material, 1, play->gameplayFrames, flags); } \
void comboEnemyMaterialAlpha##suffix(PlayState* play, EnemyMaterial* material, float alpha) \
{ if (play) comboEnemyMaterialMain(play, material, alpha, play->gameplayFrames, flags); } \
void comboEnemyMaterialStep##suffix(PlayState* play, EnemyMaterial* material, u32 step) \
{ comboEnemyMaterialMain(play, material, 1, step, flags); } \
void comboEnemyMaterialAlphaStep##suffix(PlayState* play, EnemyMaterial* material, float alpha, u32 step) \
{ comboEnemyMaterialMain(play, material, alpha, step, flags); }
ENEMY_MATERIAL_WRAPPERS(, 3)
ENEMY_MATERIAL_WRAPPERS(Opa, 1)
ENEMY_MATERIAL_WRAPPERS(Xlu, 2)
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_DRAW, comboEnemyMaterialDraw);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_OPA, comboEnemyMaterialDrawOpa);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_XLU, comboEnemyMaterialDrawXlu);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_ALPHA, comboEnemyMaterialAlpha);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_ALPHA_OPA, comboEnemyMaterialAlphaOpa);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_ALPHA_XLU, comboEnemyMaterialAlphaXlu);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_STEP, comboEnemyMaterialStep);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_STEP_OPA, comboEnemyMaterialStepOpa);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_STEP_XLU, comboEnemyMaterialStepXlu);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_ALPHA_STEP, comboEnemyMaterialAlphaStep);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_ALPHA_STEP_OPA, comboEnemyMaterialAlphaStepOpa);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_MATERIAL_ALPHA_STEP_XLU, comboEnemyMaterialAlphaStepXlu);

void comboEnemyRenderModeXlu(PlayState* play, s32 mode, u32 flags)
{
    static const Gfx modes[3][4] = {
        { gsSPEndDisplayList(), gsSPEndDisplayList(), gsSPEndDisplayList(), gsSPEndDisplayList() },
        {
            gsDPSetRenderMode(AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1), G_RM_AA_ZB_XLU_SURF2),
            gsSPEndDisplayList(),
            gsDPSetRenderMode(AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL | GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA), G_RM_AA_ZB_XLU_SURF2),
            gsSPEndDisplayList(),
        },
        {
            gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL | GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1), AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
            gsSPEndDisplayList(),
            gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL | GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA), AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL | GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
            gsSPEndDisplayList(),
        },
    };
    if (!play)
        return;
    if (mode < 0 || mode > 2)
    {
        enemyMaterialError();
        return;
    }
    enemyMaterialBind(play, 12, (void*)modes[mode], flags);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_RENDER_XLU, comboEnemyRenderModeXlu);

static void enemyPolyShadowMatrix(CollisionPoly* poly, f32 x, f32 y, f32 z, MtxF* m)
{
    if (!poly)
        return;
    f32 nx = COLPOLY_GET_NORMAL(poly->normal.x);
    f32 ny = COLPOLY_GET_NORMAL(poly->normal.y);
    f32 nz = COLPOLY_GET_NORMAL(poly->normal.z);
    f32 length = sqrtf(ny * ny + nz * nz), cy = 1.f, sz = 0.f;
    if (!IS_ZERO(length))
    {
        f32 inv = 1.f / length;
        cy = ny * inv;
        sz = nz * inv;
    }
    m->xx = length;
    m->yx = -nx * cy;
    m->zx = -nx * sz;
    m->wx = 0.f;
    m->xy = nx;
    m->yy = ny;
    m->zy = nz;
    m->wy = 0.f;
    m->xz = 0.f;
    m->yz = -sz;
    m->zz = cy;
    m->wz = 0.f;
    m->xw = x;
    m->yw = y;
    m->zw = z;
    m->ww = 1.f;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_POLY_SHADOW_MATRIX, enemyPolyShadowMatrix);

extern s32 BgCheck_SphVsFirstWall(CollisionContext* ctx, Vec3f* center, f32 radius);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPH_FIRST_WALL, BgCheck_SphVsFirstWall);

static void enemyFloorRotation(Actor* actor, s16 angle, Vec3s* out)
{
    CollisionPoly* poly = actor->floorPoly;
    if (!poly)
        return;
    f32 nx = COLPOLY_GET_NORMAL(poly->normal.x), ny = COLPOLY_GET_NORMAL(poly->normal.y), nz = COLPOLY_GET_NORMAL(poly->normal.z);
    out->x = -Math_Atan2S(1.f, (-nx * Math_SinS(angle) - nz * Math_CosS(angle)) * ny);
    angle -= 0x3ff7;
    out->z = -Math_Atan2S(1.f, (-nx * Math_SinS(angle) - nz * Math_CosS(angle)) * ny);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_FLOOR_ROTATION, enemyFloorRotation);

typedef struct {
    u8 coverage[4][12][12];
    Vtx vertices[4];
    Gfx material[4];
    Gfx geometry[6];
} EnemyShadowAssets;

static EnemyShadowAssets sShadow __attribute__((section(".data"), aligned(16))) = { 0 };
_Static_assert(sizeof(EnemyShadowAssets) == 720, "MM shadow asset layout");
EXPORT_SYMBOL(FOREIGN_ENEMY_MM_SHADOW_ASSETS, sShadow);

static void enemyShadowGenerate(Vec3f* limbs, Vec3f* world, u8* tex, f32 weight,
                                u8 count, u8* sizes, s8* parents)
{
    for (int i = 0; i < count; ++i)
    {
        Vec3f pos, projected;
        int parent = parents[i], size = sizes[i];
        if (size >= 4 || parent >= count)
            return;
        Math_Vec3f_Diff(&limbs[i], world, &pos);
        if (parent >= 0)
        {
            pos.x += (limbs[parent].x - limbs[i].x) * weight;
            pos.y += (limbs[parent].y - limbs[i].y) * weight;
            pos.z += (limbs[parent].z - limbs[i].z) * weight;
        }
        Matrix_MultVec3f(&pos, &projected);
        s32 col = (s32)(64.f + projected.x) >> 1;
        s32 row = (s32)(64.f - projected.z) >> 1;
        for (int y = 0; y < 12; ++y)
        {
            int at = (row + y) * 64 + col - 390;
            for (int x = 0; x < 12; ++x)
                if (sShadow.coverage[size][y][x] && (unsigned)(at + x) < 4096)
                    tex[at + x] = 255;
        }
    }
}

static void enemyShadowDraw(Actor* actor, GameState* state, u8* tex)
{
    GraphicsContext* gfx = state->gfxCtx;
    OPEN_DISPS(gfx);
    Gfx_SetupDL25_Opa(gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, 100);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0);
    Matrix_Translate(actor->world.pos.x, 0.f, actor->world.pos.z, MTXMODE_NEW);
    Matrix_Scale(.6f, 1.f, .6f, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfx);
    gSPDisplayList(POLY_OPA_DISP++, sShadow.material);
    gDPLoadTextureBlock(POLY_OPA_DISP++, tex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
    gSPDisplayList(POLY_OPA_DISP++, sShadow.geometry);
    CLOSE_DISPS();
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SHADOW_GENERATE, enemyShadowGenerate);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SHADOW_DRAW, enemyShadowDraw);
