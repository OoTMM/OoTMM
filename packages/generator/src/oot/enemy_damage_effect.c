#include <combo.h>
#include <combo/enemy.h>
#include <combo/enemy_keep.h>
#include <combo/math.h>

#define ENEMY_MM_ICE_MATERIAL       0x04050648
#define ENEMY_MM_ICE_MODEL          0x040506e0
#define ENEMY_MM_STEAM_MATERIAL     0x04051180
#define ENEMY_MM_STEAM_MODEL        0x04051238
#define ENEMY_MM_FIRE_MODEL         0x0407d590
#define ENEMY_MM_ORB_MATERIAL       0x04023348
#define ENEMY_MM_ORB_MODEL          0x04023428
#define ENEMY_MM_SPARK_MATERIAL     0x04023480
#define ENEMY_MM_SPARK_MODEL        0x040234f0
#define ENEMY_MM_SPARK_TEXTURE_0    0x04091de0
#define ENEMY_MM_SPARK_TEXTURE_1    0x04091fe0
#define ENEMY_MM_SPARK_TEXTURE_2    0x040921e0
#define ENEMY_MM_SPARK_TEXTURE_3    0x040923e0

#define EnemyNativeHilite ((void* (*)(Vec3f*, Vec3f*, Vec3f*, GraphicsContext*))0x80022400)
extern void comboEnemySfxActor(Actor*, u16);

static u8 enemyEffectAlpha(float alpha, s32 part)
{
    alpha = alpha * 255 - (part & 3) * 30;
    return alpha < 0 ? 0 : alpha > 255 ? 255
                                       : (u8)alpha;
}

static void enemyEffectPosition(MtxF* matrix, Vec3f* point)
{
    matrix->xw = point->x;
    matrix->yw = point->y;
    matrix->zw = point->z;
}

static int enemyEffectType(u8 type)
{
    return type <= 1 || type == 10 || type == 11 || type == 20 || type == 21 || (type >= 30 && type <= 32);
}

void comboEnemyDrawDamageEffects(PlayState* play, Actor* actor, Vec3f* points, s16 count,
                                 float scale, float steamScale, float alpha, u8 type)
{
    MtxF* matrix;
    u32 frame;
    u32 addresses[4] = { 0 };
    void* assets[4];
    if (!play || !points || count <= 0 || !(alpha > 0.001f) || !enemyEffectType(type))
        return;
    frame = play->gameplayFrames;
    if (type == 10 || type == 11)
    {
        addresses[0] = ENEMY_MM_ICE_MATERIAL;
        addresses[1] = ENEMY_MM_ICE_MODEL;
        addresses[2] = ENEMY_MM_STEAM_MATERIAL;
        addresses[3] = ENEMY_MM_STEAM_MODEL;
    }
    else if (type <= 1)
    {
        addresses[0] = ENEMY_MM_FIRE_MODEL;
    }
    else if (type <= 21)
    {
        addresses[0] = ENEMY_MM_ORB_MATERIAL;
        addresses[1] = ENEMY_MM_ORB_MODEL;
    }
    else
    {
        static const u32 textures[] = { ENEMY_MM_SPARK_TEXTURE_0, ENEMY_MM_SPARK_TEXTURE_1, ENEMY_MM_SPARK_TEXTURE_2, ENEMY_MM_SPARK_TEXTURE_3 };
        addresses[0] = ENEMY_MM_SPARK_MATERIAL;
        addresses[1] = ENEMY_MM_SPARK_MODEL;
        addresses[2] = textures[frame & 3];
    }
    for (u32 i = 0; i < ARRAY_COUNT(addresses); ++i)
    {
        assets[i] = NULL;
        if (addresses[i])
        {
            assets[i] = comboEnemyKeepAsset(0x1001, addresses[i] & 0xffffff);
            if (!assets[i])
                return;
        }
    }
    matrix = Matrix_GetCurrent();
    if (actor && alpha > 0.05f && play->gameOverCtx.state == GAMEOVER_INACTIVE)
    {
        if (type == 0)
            comboEnemySfxActor(actor, 0x205b);
        else if (type == 1)
            comboEnemySfxActor(actor, 0x321f);
        else if (type == 11)
            comboEnemySfxActor(actor, 0x20b2);
        else if (type == 20 || type == 21)
            comboEnemySfxActor(actor, 0x31f7);
    }
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    if (type == 10 || type == 11)
    {
        Vec3f light;
        light.x = play->envCtx.dirLight1.params.dir.x;
        light.y = play->envCtx.dirLight1.params.dir.y;
        light.z = play->envCtx.dirLight1.params.dir.z;
        EnemyNativeHilite(points, &play->view.eye, &light, play->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 8, Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, frame & 255, 32, 16, 1, 0, (frame * 2) & 255, 64, 32));
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 128, 170, 255, 255, 255);
        gSPDisplayList(POLY_XLU_DISP++, assets[0]);
        for (s32 part = 0; part < count; ++part)
        {
            gDPSetEnvColor(POLY_XLU_DISP++, 200, 200, 255, enemyEffectAlpha(alpha, part));
            Matrix_Translate(points[part].x, points[part].y, points[part].z, MTXMODE_NEW);
            Matrix_Scale(scale * 2.3f, scale * 2.3f, scale * 2.3f, MTXMODE_APPLY);
            if (part & 1)
                Matrix_RotateYF(M_PIf, MTXMODE_APPLY);
            if (part & 2)
                Matrix_RotateZF(M_PIf, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_XLU_DISP++, assets[1]);
        }
        gDPSetColorDither(POLY_XLU_DISP++, G_CD_BAYER);
        gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_PATTERN);
        gSPDisplayList(POLY_XLU_DISP++, assets[2]);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 195, 225, 235, alpha > 1 ? 100 : (u8)(alpha * 100));
        for (s32 part = 0; part < count; ++part)
        {
            s32 scroll = part * 3 + frame;
            gSPSegment(POLY_XLU_DISP++, 8, Gfx_TwoTexScroll(play->state.gfxCtx, 0, scroll * 3, scroll * -12, 32, 64, 1, 0, 0, 32, 32));
            Matrix_Translate(points[part].x, points[part].y, points[part].z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(steamScale * 0.035f, steamScale * 0.035f, 1, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_XLU_DISP++, assets[3]);
        }
    }
    else
    {
        Matrix_Put(&play->billboardMtxF);
        if (type <= 1)
        {
            gDPSetEnvColor(POLY_XLU_DISP++, type ? 0 : 255, type ? 255 : 10, type ? 255 : 0, 0);
            Matrix_Scale(scale * 0.00675f, scale * 0.005f, scale * 0.00675f, MTXMODE_APPLY);
        }
        else if (type <= 21)
        {
            gSPDisplayList(POLY_XLU_DISP++, assets[0]);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, type == 21 ? 255 : 200, enemyEffectAlpha(alpha, 0));
            gDPSetEnvColor(POLY_XLU_DISP++, type == 21 ? 0 : 255, 255, type == 21 ? 255 : 100, 128);
            Matrix_Scale(scale * 4, scale * 4, 1, MTXMODE_APPLY);
        }
        else
        {
            float size = scale * (1 + (type - 30) * 0.5f);
            gSPSegment(POLY_XLU_DISP++, 8, assets[2]);
            gSPDisplayList(POLY_XLU_DISP++, assets[0]);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 150, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 0, 0);
            Matrix_Scale(size, size, size, MTXMODE_APPLY);
        }
        for (s32 part = 0; part < count; ++part)
        {
            if (type <= 1)
            {
                gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 255, 255, type ? 255 : 0, enemyEffectAlpha(alpha, part));
                gSPSegment(POLY_XLU_DISP++, 8, Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0, ((part * 10 + frame) * -20) & 511, 32, 128));
                Matrix_RotateYF(M_PIf, MTXMODE_APPLY);
                enemyEffectPosition(matrix, &points[part]);
                MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
                gSPDisplayList(POLY_XLU_DISP++, assets[0]);
            }
            else if (type <= 21)
            {
                Matrix_RotateZF(Rand_CenteredFloat(2 * M_PIf), MTXMODE_APPLY);
                enemyEffectPosition(matrix, &points[part]);
                MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
                gSPDisplayList(POLY_XLU_DISP++, assets[1]);
            }
            else
                for (s32 spark = 0; spark < 2; ++spark)
                {
                    Matrix_RotateXF(Rand_ZeroFloat(2 * M_PIf), MTXMODE_APPLY);
                    Matrix_RotateZF(Rand_ZeroFloat(2 * M_PIf), MTXMODE_APPLY);
                    matrix->xw = points[part].x + Rand_CenteredFloat(30);
                    matrix->yw = points[part].y + Rand_CenteredFloat(30);
                    matrix->zw = points[part].z + Rand_CenteredFloat(30);
                    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
                    gSPDisplayList(POLY_XLU_DISP++, assets[1]);
                }
        }
    }
    CLOSE_DISPS();
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DAMAGE_EFFECTS, comboEnemyDrawDamageEffects);
