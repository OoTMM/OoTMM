#include <combo.h>
#include <combo/custom.h>
#include <combo/global.h>
#include <combo/draw.h>

#if defined(GAME_OOT)
# include <assets/oot/objects/gameplay_keep.h>
#endif

#if defined(GAME_MM)
# include <assets/mm/objects/gameplay_keep.h>
#endif

#define M_PI            3.14159265358979323846
#define M_SQRT1_2       0.707106781186547524401
#define DLIST_RAW(x)    (((u32)(x)) - 0x80000000)

static float hueToRgb(float p, float q, float t)
{
    if (t < 0.f) t += 1.f;
    if (t > 1.f) t -= 1.f;
    if (t < (1.f/6.f)) return p + (q - p) * 6.f * t;
    if (t < 0.5f) return q;
    if (t < (2.f/3.f)) return p + (q - p) * ((2.f/3.f) - t) * 6.f;
    return p;
}

static u32 hsla(float h, float s, float l, float a)
{
    float r;
    float g;
    float b;
    float q;
    float p;

    /* Normalize */
    if (h < 0)
        h = ((int)((-h) + 1.f)) - h;
    else
        h = h - ((int)h);
    if (s < 0.f)
        s = 0.f;
    if (s > 1.f)
        s = 1.f;
    if (l < 0.f)
        l = 0.f;
    if (l > 1.f)
        l = 1.f;

    if (s == 0.f)
    {
        r = l;
        g = l;
        b = l;
    }
    else
    {
        q = l < 0.5f ? l * (1.f + s) : l + s - l * s;
        p = 2.f * l - q;

        r = hueToRgb(p, q, h + (1.f/3.f));
        g = hueToRgb(p, q, h);
        b = hueToRgb(p, q, h - (1.f/3.f));
    }

    return ((u32)(r * 255.f)) << 24 | ((u32)(g * 255.f)) << 16 | ((u32)(b * 255.f)) << 8 | ((u32)(a * 255.f));
}

static void color4(u8* r, u8* g, u8* b, u8* a, u32 color)
{
    *r = (color >> 24) & 0xff;
    *g = (color >> 16) & 0xff;
    *b = (color >> 8) & 0xff;
    *a = color & 0xff;
}

/* Custom DrawGi */
void DrawGi_Xlu0(PlayState* play, s16 drawGiId)
{
    const DrawGi* drawGi;

    drawGi = &kDrawGi[drawGiId];
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}

void DrawGi_CustomNote(PlayState* play, s16 drawGiId, u8 param)
{
    static const u32 kColors[] = {
        0x8000ffff /* Purple */,
        0x0000ffff /* Blue */,
        0x00ff00ff /* Green */,
        0xffff00ff /* Yellow */,
        0xff8000ff /* Orange */,
        0xff0000ff /* Red */,
    };

    const DrawGi* drawGi;
    float angle;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    drawGi = &kDrawGi[drawGiId];
    angle = M_PI / 16;
    if (param & 0x80)
        angle += M_PI;
    color4(&r, &g, &b, &a, kColors[param & 0xf]);

    Matrix_RotateZ(angle, MTXMODE_APPLY);

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}

void DrawGi_CustomHeartContainer(PlayState* play, s16 drawGiId)
{
    static const u32 colors[] = {
        /* Normal */
        0xa0ffffff,
        0x0064ffff,
        0xff0064ff,
        0x640032ff,

        /* Double Defense */
        0xffff00ff,
        0xcccc00ff,
        0xffffffff,
        0xffffffff,
    };

    const DrawGi* drawGi;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    u32 c;

    drawGi = &kDrawGi[drawGiId];
    c = drawGi->lists[0];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, colors[c * 4 + 0]);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, colors[c * 4 + 1]);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[1]);
    color4(&r, &g, &b, &a, colors[c * 4 + 2]);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, colors[c * 4 + 3]);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[2]);
    CLOSE_DISPS();
}

void Gfx_DrawFlameColor(PlayState* play, u32 color, float scale, float offsetY)
{
#if defined(GAME_OOT)
    static const u32 kFlameDlist = 0x52a10;
#else
    static const u32 kFlameDlist = 0x7d590;
#endif

    float flameScale = 0.0055f * scale;

    u32 primColor;
    u32 envColor;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    u16 tmp;

    primColor = color;

    /* Auto-compute env color */
    envColor = 0;
    tmp = (primColor >> 24) & 0xff;
    tmp += 0x44;
    if (tmp > 0xff)
        tmp = 0xff;
    envColor |= tmp << 24;
    tmp = (primColor >> 16) & 0xff;
    tmp += 0x44;
    if (tmp > 0xff)
        tmp = 0xff;
    envColor |= tmp << 16;
    tmp = (primColor >> 8) & 0xff;
    tmp += 0x44;
    if (tmp > 0xff)
        tmp = 0xff;
    envColor |= tmp << 8;
    envColor |= (primColor & 0xff);

    OPEN_DISPS(play->state.gfxCtx);
    ModelViewUnkTransform(&play->billboardMtxF);
    Matrix_Translate(0.f, -(30.f + offsetY), -15.f, MTXMODE_APPLY);
    Matrix_Scale(flameScale * 1.7f, flameScale, flameScale, MTXMODE_APPLY);
    gSPSegment(POLY_XLU_DISP++, 0x08, DisplaceTexture(play->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0, (-play->state.frameCount & 0x7f) << 2, 0x20, 0x80));
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, primColor);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, envColor);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | kFlameDlist);
    CLOSE_DISPS();
}

static void drawFire(PlayState* play, u32 primColor, u32 envColor, float scale, float offsetY)
{
#if defined(GAME_OOT)
    static const u32 kFlameDlist = 0x52a10;
#else
    static const u32 kFlameDlist = 0x7d590;
#endif

    u8 r;
    u8 g;
    u8 b;
    u8 a;
    float flameScale;

    OPEN_DISPS(play->state.gfxCtx);

    /* Set the correct billboard matrix*/
    Matrix_Push();
    flameScale = 0.0055f * scale;
    Matrix_Translate(0.f, offsetY, 0.f, MTXMODE_APPLY);
    Matrix_Scale(flameScale * 1.7f, flameScale, flameScale, MTXMODE_APPLY);
    ModelViewUnkTransform(&play->billboardMtxF);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Matrix_Pop();

    /* Set the texture and color */
    gSPSegment(POLY_XLU_DISP++, 0x08, DisplaceTexture(play->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0, (-play->state.frameCount & 0x7f) << 2, 0x20, 0x80));
    color4(&r, &g, &b, &a, primColor);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, envColor);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);

    /* Draw */
    gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | kFlameDlist);

    CLOSE_DISPS();
}

static void shaderFlameEffect(PlayState* play, int colorIndex, float scale, float offsetY)
{
#if defined(GAME_OOT)
    static const u32 kFlameDlist = 0x52a10;
#else
    static const u32 kFlameDlist = 0x7d590;
#endif

    float flameScale = 0.0055f * scale;

    static const u32 kPrimColors[] = {
        0x00ffffc0,
        0xff00ffc0,
        0xff0000c0,
        0x00ff00c0,
    };

    static const u32 kEnvColors[] = {
        0x0000ffc0,
        0xff0000c0,
        0xffff00c0,
        0x44ff44c0,
    };

    u8 r;
    u8 g;
    u8 b;
    u8 a;

    OPEN_DISPS(play->state.gfxCtx);
    ModelViewUnkTransform(&play->billboardMtxF);
    Matrix_Translate(0.f, -(30.f + offsetY), -15.f, MTXMODE_APPLY);
    Matrix_Scale(flameScale * 1.7f, flameScale, flameScale, MTXMODE_APPLY);
    gSPSegment(POLY_XLU_DISP++, 0x08, DisplaceTexture(play->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0, (-play->state.frameCount & 0x7f) << 2, 0x20, 0x80));
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, kPrimColors[colorIndex]);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColors[colorIndex]);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | kFlameDlist);
    CLOSE_DISPS();
}

static const u32 kNutStickPrimColors[] = {
    0xa06428ff,
    0xffffffff,
    0xffffbbff,
};
static const u32 kNutStickEnvColors[] = {
    0x280a00ff,
    0x505050ff,
    0xaaaa00ff,
};

void DrawGi_CustomStick(PlayState* play, s16 drawGiId)
{
    const DrawGi* drawGi;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    drawGi = &kDrawGi[drawGiId];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, kNutStickPrimColors[drawGi->lists[1]]);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, a);
    color4(&r, &g, &b, &a, kNutStickEnvColors[drawGi->lists[1]]);
    gDPSetEnvColor(POLY_OPA_DISP++, r, g, b, a);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);

    /* Draw fire */
    if (drawGi->lists[1])
    {
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
        shaderFlameEffect(play, drawGi->lists[1] - 1, 1.f, 0.f);
    }

    CLOSE_DISPS();
}

void DrawGi_CustomNut(PlayState* play, s16 drawGiId)
{
    const DrawGi* drawGi;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    u32 fc;

    drawGi = &kDrawGi[drawGiId];
    fc = play->state.frameCount * 6;

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x09, DisplaceTexture(play->state.gfxCtx, 0, fc, fc, 0x20, 0x20, 1, fc, fc, 0x20, 0x20));
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, kNutStickPrimColors[drawGi->lists[1]]);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, a);
    color4(&r, &g, &b, &a, kNutStickEnvColors[drawGi->lists[1]]);
    gDPSetEnvColor(POLY_OPA_DISP++, r, g, b, a);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);

    /* Draw fire */
    if (drawGi->lists[1])
    {
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
        shaderFlameEffect(play, drawGi->lists[1] - 1, 1.f, 0.f);
    }

    CLOSE_DISPS();
}

static void* pushMatrix(GraphicsContext* gfx, const float* mat)
{
    void* end = gfx->polyOpa.end;
    end = (char*)end - 0x40;
    gfx->polyOpa.end = end;

    ConvertMatrix(mat, end);

    return end;
}

void DrawGi_BossRemains(PlayState* play, s16 drawGiId)
{
    static const float scale = 0.03f;
    static const float kMatrixScale[] = {
        scale, 0.f, 0.f, 0.f,
        0.f, scale, 0.f, 0.f,
        0.f, 0.f, scale, 0.f,
        0.f, 0.f, 0.f,   1.f,
    };
    const DrawGi* drawGi;

    drawGi = &kDrawGi[drawGiId];
    OPEN_DISPS(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(POLY_OPA_DISP++, pushMatrix(play->state.gfxCtx, kMatrixScale), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}

void DrawGi_SpiritualStones(PlayState* play, s16 drawGiId)
{
    static const u32 kPrimColors[] = {
        0xffffa0ff,
        0xffaaffff,
        0x32ffffff,
    };

    static const u32 kEnvColors[] = {
        0x00ff00ff,
        0xff0064ff,
        0x320096ff,
    };

    static const float kMatrixRot[] = {
        1.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, -1.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 1.f,
    };

    u8 r, g, b, a;
    const DrawGi* drawGi;
    int colorIndex;

    drawGi = &kDrawGi[drawGiId];
    colorIndex = drawGi->lists[0];

    OPEN_DISPS(play->state.gfxCtx);

    /* Matrix setup */
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(POLY_XLU_DISP++, pushMatrix(play->state.gfxCtx, kMatrixRot), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(POLY_OPA_DISP++, pushMatrix(play->state.gfxCtx, kMatrixRot), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    /* Segment setup */
    gSPSegment(POLY_XLU_DISP++, 9, kDListEmpty);
    gSPSegment(POLY_OPA_DISP++, 8, kDListEmpty);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    color4(&r, &g, &b, &a, kPrimColors[colorIndex]);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColors[colorIndex]);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[1]);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x80, 0xff, 0xff, 0xaa, 0xff);
    gDPSetEnvColor(POLY_OPA_DISP++, 0x96, 0x78, 0x00, 0xFF);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[2]);

    CLOSE_DISPS();
}

void DrawGi_MasterSword(PlayState* play, s16 drawGiId)
{
    static const float scale = 0.07f;
    const DrawGi* drawGi;

    drawGi = &kDrawGi[drawGiId];

    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_RotateZ(3.f * M_PI * 0.25f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 8, Gfx_TexScroll(play->state.gfxCtx, 0, -(play->gameplayFrames & 0x7f), 32, 32));
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}

void DrawGi_CustomSpin(PlayState* play, s16 drawGiId, u8 param)
{
    const DrawGi* drawGi;

    drawGi = &kDrawGi[drawGiId];
    float rot;

    rot = (play->state.frameCount * 0.01f);
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_RotateX(rot * 3, MTXMODE_APPLY);
    Matrix_RotateY(rot * 5, MTXMODE_APPLY);
    Matrix_RotateZ(rot * 7, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (param == 0)
    {
        /* MM */
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
        gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[1]);
    }
    else
    {
        /* OOT */
        gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[2]);
    }

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    shaderFlameEffect(play, 2, 1.f, 0.f);
    CLOSE_DISPS();
}

void DrawGi_CustomOwl(PlayState* play, s16 drawGiId)
{
    const DrawGi* drawGi;
    static const float scale = 0.01f;

    drawGi = &kDrawGi[drawGiId];
    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Translate(0.f, -25.f, 0.f, MTXMODE_APPLY);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}

static SkelAnime sDrawStrayFairySkelAnime;
static Vec3s sDrawStrayFairyJointTable[10];
static u32 sDrawStrayFairyLimit;

#if defined(GAME_OOT)
static void* sDrawStrayFairyObject;
# define STRAY_FAIRY_SKEL CUSTOM_OBJECT_STRAY_FAIRY_1
# define STRAY_FAIRY_ANIM CUSTOM_OBJECT_STRAY_FAIRY_0
#endif

#if defined(GAME_MM)
# define STRAY_FAIRY_SKEL 0x0402ca98
# define STRAY_FAIRY_ANIM 0x0402b494
#endif

static const Gfx kStrayFairyDlistColorWoodfall[] = {
    gsDPSetPrimColor(0, 0, 0xd2, 0xb8, 0xc8, 255),
    gsDPSetEnvColor(0xba, 0x50, 0x84, 255),
    gsSPEndDisplayList(),
};

static const Gfx kStrayFairyDlistColorWoodfallBg[] = {
    gsDPSetPrimColor(0, 0, 0xba, 0x50, 0x84, 255),
    gsDPSetEnvColor(0xba, 0x50, 0x84, 255),
    gsSPEndDisplayList(),
};

static const Gfx kStrayFairyDlistColorSnowhead[] = {
    gsDPSetPrimColor(0, 0, 0xf0, 0xf6, 0xc2, 255),
    gsDPSetEnvColor(0x45, 0x85, 0x2b, 255),
    gsSPEndDisplayList(),
};

static const Gfx kStrayFairyDlistColorSnowheadBg[] = {
    gsDPSetPrimColor(0, 0, 0x45, 0x85, 0x2b, 255),
    gsDPSetEnvColor(0x45, 0x85, 0x2b, 255),
    gsSPEndDisplayList(),
};

static const Gfx kStrayFairyDlistColorGreatBay[] = {
    gsDPSetPrimColor(0, 0, 0xe1, 0xeb, 0xfd, 255),
    gsDPSetEnvColor(0x7f, 0x65, 0xcc, 255),
    gsSPEndDisplayList(),
};

static const Gfx kStrayFairyDlistColorGreatBayBg[] = {
    gsDPSetPrimColor(0, 0, 0x7f, 0x65, 0xcc, 255),
    gsDPSetEnvColor(0x7f, 0x65, 0xcc, 255),
    gsSPEndDisplayList(),
};

static const Gfx kStrayFairyDlistColorStoneTower[] = {
    gsDPSetPrimColor(0, 0, 0xfe, 0xfe, 0xe7, 255),
    gsDPSetEnvColor(0xc2, 0xc1, 0x64, 255),
    gsSPEndDisplayList(),
};

static const Gfx kStrayFairyDlistColorStoneTowerBg[] = {
    gsDPSetPrimColor(0, 0, 0xc2, 0xc1, 0x64, 255),
    gsDPSetEnvColor(0xc2, 0xc1, 0x64, 255),
    gsSPEndDisplayList(),
};

static const Gfx kStrayFairyDlistColorTown[] = {
    gsDPSetPrimColor(0, 0, 0xf1, 0xe5, 0xd9, 255),
    gsDPSetEnvColor(0xbc, 0x70, 0x2d, 255),
    gsSPEndDisplayList(),
};

static const Gfx kStrayFairyDlistColorTownBg[] = {
    gsDPSetPrimColor(0, 0, 0xbc, 0x70, 0x2d, 255),
    gsDPSetEnvColor(0xbc, 0x70, 0x2d, 255),
    gsSPEndDisplayList(),
};

static int DrawGi_CustomStrayFairyLimbOverride(PlayState* play, int limbIndex, Gfx** dlist, Vec3f* pos, Vec3s* rot, Actor* this)
{
    if (limbIndex == 9)
        *dlist = NULL;

    return FALSE;
}

void DrawGiSystem_Reset(PlayState* play)
{
#if defined(GAME_OOT)
    u32 tmp;

    if (!sDrawStrayFairyObject)
    {
        tmp = comboLoadObject(NULL, CUSTOM_OBJECT_ID_STRAY_FAIRY);
        sDrawStrayFairyObject = malloc(tmp);

        if (sDrawStrayFairyObject)
        {
            comboLoadObject(sDrawStrayFairyObject, CUSTOM_OBJECT_ID_STRAY_FAIRY);
        }
    }
    gSegments[0x06] = (u32)sDrawStrayFairyObject - 0x80000000;
#endif

    memset(&sDrawStrayFairySkelAnime, 0, sizeof(sDrawStrayFairySkelAnime));
    memset(&sDrawStrayFairyJointTable, 0, sizeof(sDrawStrayFairyJointTable));

    SkelAnime_InitFlex(play, &sDrawStrayFairySkelAnime, (void*)STRAY_FAIRY_SKEL, (void*)STRAY_FAIRY_ANIM, sDrawStrayFairyJointTable, sDrawStrayFairyJointTable, 10);
}

void DrawGiSystem_Update(PlayState* play)
{
    sDrawStrayFairyLimit = 0;
#if defined(GAME_OOT)
    gSegments[0x06] = (u32)sDrawStrayFairyObject - 0x80000000;
#endif
    SkelAnime_Update(&sDrawStrayFairySkelAnime);
}

void DrawGi_CustomStrayFairy(PlayState* play, s16 drawGiId)
{
    static const Gfx* const kColors[] = {
        kStrayFairyDlistColorWoodfall,
        kStrayFairyDlistColorSnowhead,
        kStrayFairyDlistColorGreatBay,
        kStrayFairyDlistColorStoneTower,
        kStrayFairyDlistColorTown,
    };

    static const Gfx* const kColorsBg[] = {
        kStrayFairyDlistColorWoodfallBg,
        kStrayFairyDlistColorSnowheadBg,
        kStrayFairyDlistColorGreatBayBg,
        kStrayFairyDlistColorStoneTowerBg,
        kStrayFairyDlistColorTownBg,
    };

    const DrawGi* drawGi;
    int index;

    sDrawStrayFairyLimit++;
    if (sDrawStrayFairyLimit >= 15)
        return;

    drawGi = &kDrawGi[drawGiId];
    index = drawGi->lists[0];
#if defined(GAME_MM)
    if (index == 0)
    {
        if (play->sceneId == SCE_MM_CLOCK_TOWN_EAST || play->sceneId == SCE_MM_LAUNDRY_POOL)
            index = 5;
        else
            index = gSaveContext.dungeonId + 1;
    }
#endif

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x08, kColors[index - 1]);
    gSPSegment(POLY_XLU_DISP++, 0x09, kColorsBg[index - 1]);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    Matrix_Scale(0.03f, 0.03f, 0.03f, MTXMODE_APPLY);
#if defined(GAME_OOT)
    gSegments[0x06] = (u32)sDrawStrayFairyObject - 0x80000000;
    gSPSegment(POLY_XLU_DISP++, 0x06, sDrawStrayFairyObject);
#endif
    POLY_XLU_DISP = SkelAnime_DrawFlex(play, sDrawStrayFairySkelAnime.skeleton, sDrawStrayFairySkelAnime.jointTable, sDrawStrayFairySkelAnime.dListCount, DrawGi_CustomStrayFairyLimbOverride, NULL, NULL, POLY_XLU_DISP);
    CLOSE_DISPS();
}

typedef struct
{
    u32 skullEnv;
    u32 skullPrimary;
    u32 flameEnv;
    u32 flamePrimary;
}
GsColors;

void DrawGi_CustomBottlePotion(PlayState* play, s16 index, u8 param)
{
    static const u32 kPrimColors[] = {
        0xff0000ff,
        0x00ff00ff,
        0x0000ffff,
    };
    static const u32 kEnvColors[] = {
        0xff8888ff,
        0x88ff88ff,
        0x8888ffff,
    };

    const DrawGi* drawGi;

    u8 r;
    u8 g;
    u8 b;
    u8 a;

    drawGi = &kDrawGi[index];
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, kPrimColors[param]);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColors[param]);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[0]);

    CLOSE_DISPS();
}

void DrawGi_CustomGS(PlayState* play, s16 index)
{
    static const GsColors kColors[] = {
        { 0x967800ff, 0xffffaaff, 0xffd86eff, 0xffd86eff }, /* GS Token */
        { 0x967800ff, 0xffffaaff, 0x82ff80ff, 0x82ff80ff }, /* Swamp Token */
        { 0x967800ff, 0xffffaaff, 0x7d83ffff, 0x7d83ffff }, /* Ocean Token */
        { 0xaaaaaaff, 0xffffffff, 0xff0000ff, 0xff0000ff }, /* Soul */
        { 0xaaaaaaff, 0xffffffff, 0x8800ffff, 0x8800ffff }, /* Boss Soul */
        { 0xaaaaaaff, 0xffffffff, 0x00ff00ff, 0x00ff00ff }, /* NPC Soul */
        { 0xaaaaaaff, 0xffffffff, 0x888888ff, 0x888888ff }, /* Misc. Soul */
    };

    const DrawGi* drawGi;
    const GsColors* gsc;
    float fc;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    drawGi = &kDrawGi[index];
    gsc = &kColors[drawGi->lists[0]];
    fc = play->state.frameCount;

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, gsc->skullEnv);
    gDPSetEnvColor(POLY_OPA_DISP++, r, g, b, a);
    color4(&r, &g, &b, &a, gsc->skullPrimary);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, r, g, b, a);
    gSPDisplayList(POLY_OPA_DISP++, 0x06000330);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 0x08, DisplaceTexture(play->state.gfxCtx, 0, 0, fc * -5, 0x20, 0x20, 1, 0, 0, 0x20, 0x40));
    color4(&r, &g, &b, &a, gsc->flameEnv);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    color4(&r, &g, &b, &a, gsc->flamePrimary);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, 0x06000438);
    gDPPipeSync(POLY_XLU_DISP++);
    color4(&r, &g, &b, &a, gsc->skullEnv);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    color4(&r, &g, &b, &a, gsc->skullPrimary);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, 0x06000508);
    CLOSE_DISPS();
}

void DrawGi_CustomPotion(PlayState* play, s16 index)
{
    static const u32 kPrimColors1[] = {
        0x326400ff,
        0xdc3232ff,
        0x321edcff,
        0xffffffff,
        0x880088ff,
    };

    static const u32 kEnvColors1[] = {
        0x142800ff,
        0x320000ff,
        0x140a3cff,
        0xddddddff,
        0x440044ff,
    };

    static const u32 kPrimColors2[] = {
        0xffffaaff,
        0xffaaffff,
        0xaaffffff,
        0xaaaaaaff,
        0xaaaaaaff,
    };

    static const u32 kEnvColors2[] = {
        0x006400ff,
        0x96001eff,
        0x003296ff,
        0xffffffff,
        0xffffffff,
    };

    static const u32 kPrimColors3[] = {
        0x64c828ff,
        0xff8278ff,
        0x008278ff,
        0x0000ffff,
        0xffffffff,
    };

    static const u32 kEnvColors3[] = {
        0x3c7814ff,
        0xff4650ff,
        0x0046beff,
        0x0000ffff,
        0xffffffff,
    };

    static const u16 kTextureOffsets[] = {
        0x000,
        0x200,
        0x400,
        0x000,
        0x400,
    };

    const DrawGi* drawGi;
    int colorIndex;
    s32 fc;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    drawGi = &kDrawGi[index];
    colorIndex = drawGi->lists[0];
    fc = play->state.frameCount;
    OPEN_DISPS(play->state.gfxCtx);
    /* Opa */
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, DisplaceTexture(play->state.gfxCtx, 0, -fc, fc, 0x20, 0x20, 1, -fc, fc, 0x20, 0x20));
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, kPrimColors1[colorIndex]);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColors1[colorIndex]);
    gDPSetEnvColor(POLY_OPA_DISP++, r, g, b, a);
    gSPDisplayList(POLY_OPA_DISP++, 0x06001438);

    /* We patched the list to separate the stick */
    if (colorIndex < 3)
        gSPDisplayList(POLY_OPA_DISP++, 0x060016a0);

    color4(&r, &g, &b, &a, kPrimColors2[colorIndex]);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColors2[colorIndex]);
    gDPSetEnvColor(POLY_OPA_DISP++, r, g, b, a);
    gSPDisplayList(POLY_OPA_DISP++, 0x06001790);

    /* Xlu */
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, kPrimColors3[colorIndex]);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColors3[colorIndex]);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gDPLoadTextureBlock(POLY_XLU_DISP++, 0x06000000 | kTextureOffsets[colorIndex], G_IM_FMT_IA, G_IM_SIZ_8b, 16, 32, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD, G_TX_NOLOD);
    gSPDisplayList(POLY_XLU_DISP++, 0x06001848);
    CLOSE_DISPS();
}

#if defined(GAME_OOT)
# define DLIST_RUPEE 0x04045150
#else
# define DLIST_RUPEE 0x040622c0
#endif

#if defined(GAME_OOT)
# define OFF_RUPEE1 0x04044F10
# define OFF_RUPEE2 0x04044F40
# define OFF_RUPEE3 0x04045120
#else
# define OFF_RUPEE1 0x04062080
# define OFF_RUPEE2 0x040620B0
# define OFF_RUPEE3 0x04062290
#endif

static const Gfx kRupeeList[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x7D0, 0x3E8, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(0x08000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 4, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 2, 2, 2, 1),
    gsDPSetCombineLERP(TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, 1, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1), GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM) | AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | ALPHA_CVG_SEL),
    gsSPClearGeometryMode(G_TEXTURE_ENABLE | G_FOG),
    gsSPSetGeometryMode(G_TEXTURE_ENABLE | G_CULL_BACK | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(OFF_RUPEE1, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(OFF_RUPEE2, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 27, 28, 29, 0),
    gsSPVertex(OFF_RUPEE3, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

ALIGNED(16) static u16 kSilverRupeeLUT[] = {
    0x635f, 0xbdf1, 0xffff, 0xe739,
    0x6ba1, 0xbdf1, 0xefbd, 0xbe31,
    0x4257, 0x6b9f, 0xc633, 0x94e9,
    0x0849, 0x3a15, 0x8ca7, 0x73a1,
};

static void drawColoredRupee(PlayState* play, u32 color)
{
    static const float scale = 0.04;
    u8 c[4];

    color4(&c[0], &c[1], &c[2], &c[3], color);
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 0x08, &kSilverRupeeLUT);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, c[0], c[1], c[2], c[3]);
    gSPDisplayList(POLY_OPA_DISP++, (u32)kRupeeList & 0xffffff);
    CLOSE_DISPS();
}

void DrawGi_SilverRupee(PlayState* play, s16 index)
{
    drawColoredRupee(play, 0xffffffff);
}

void DrawGi_MagicalRupee(PlayState* play, s16 index)
{
    float h;
    u32 color;

    h = (play->state.frameCount % 60) * (1.f/60.f);
    color = hsla(h, 1.f, 0.5f, 1.f);
    drawColoredRupee(play, color);
}

void DrawGi_RutosLetter(PlayState* play, s16 index)
{
    const DrawGi* drawGi;
    drawGi = &kDrawGi[index];
    OPEN_DISPS(play->state.gfxCtx);
    Matrix_RotateZ((M_PI / 2.f), 1);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[1]);
    CLOSE_DISPS();
}

void DrawGi_CustomSmallKey(PlayState* play, s16 index)
{
    const DrawGi* drawGi;

    drawGi = &kDrawGi[index];
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    switch (drawGi->lists[1])
    {
    case 0:
        /* Silver */
        gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x80, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, 60, 80, 90, 255);
        break;
    case 1:
        /* Gold */
        gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x80, 255, 255, 0, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, 60, 80, 0, 255);
        break;
    }
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}

void DrawGi_Coin(PlayState* play, s16 index)
{
    const DrawGi* drawGi;
    static const float scale = 2.f;

    drawGi = &kDrawGi[index];
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[1]);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[2]);
    CLOSE_DISPS();
}

static const Gfx kMagicJarColorGreen[] = {
    gsDPSetPrimColor(0x00, 0x00, 40, 100, 40, 255),
    gsDPSetEnvColor(0, 30, 20, 255),
    gsSPEndDisplayList(),
};

static const Gfx kMagicJarColorSilver[] = {
    gsDPSetPrimColor(0x00, 0x00, 255, 255, 255, 255),
    gsDPSetEnvColor(50, 60, 60, 255),
    gsSPEndDisplayList(),
};

static const Gfx kMagicJarColorGold[] = {
    gsDPSetPrimColor(0x00, 0x00, 255, 255, 150, 255),
    gsDPSetEnvColor(100, 70, 0, 255),
    gsSPEndDisplayList(),
};

void DrawGi_MagicJar(PlayState* play, s16 index)
{
    const DrawGi* drawGi;
    int isUpgrade;
    int isLarge;
    const Gfx* colorJar;
    const Gfx* colorDetail;
    const Gfx* tmp;

    drawGi = &kDrawGi[index];
    isUpgrade = drawGi->lists[1];
    isLarge = drawGi->lists[2];

    colorJar = kMagicJarColorGreen;
    colorDetail = isLarge ? kMagicJarColorGold : kMagicJarColorSilver;
    if (isUpgrade)
    {
        tmp = colorJar;
        colorJar = colorDetail;
        colorDetail = tmp;
    }

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, colorJar);
    gSPSegment(POLY_OPA_DISP++, 0x09, colorDetail);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();

    if (isUpgrade)
    {
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
        shaderFlameEffect(play, 3, 1.5f, 20.f);
    }
}

void DrawGi_Triforce(PlayState* play, s16 index, u8 param)
{
    const DrawGi* drawGi;
    u32 color;

    drawGi = &kDrawGi[index];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();

    if (param == 0)
        return;

    switch (param)
    {
    case 1:
        color = 0xff0000c0;
        break;
    case 2:
        color = 0x00ff00c0;
        break;
    case 3:
        color = 0x0000ffc0;
        break;
    default:
        UNREACHABLE();
        break;
    }

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    Gfx_DrawFlameColor(play, color, 1.5f, 20.f);
}

void DrawGi_TriforceFull(PlayState* play, s16 index, u8 param)
{
    static const float scale = 0.8f;
    const DrawGi* drawGi;

    drawGi = &kDrawGi[index];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);

    Matrix_Push();
    Matrix_Translate(0.f, 40.f, 0.f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    Matrix_Pop();

    Matrix_Push();
    Matrix_Translate(-23.f, 0.f, 0.f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    Matrix_Pop();

    Matrix_Push();
    Matrix_Translate(23.f, 0.f, 0.f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    Matrix_Pop();

    CLOSE_DISPS();
}

void DrawGi_MaskMajora(PlayState* play, s16 index, u8 param)
{
    static const float scale = 0.03f;
    const DrawGi* drawGi;

    drawGi = &kDrawGi[index];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_RotateX(M_PI, MTXMODE_APPLY);
    Matrix_Translate(-300.f, 0.f, 0.f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}

#define LIST_PTR(x) ((u32)x & 0xffffff)

static const Gfx kWalletRupeeGreen[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x00, 0, 255, 0, 255),
    gsDPSetEnvColor(0, 50, 0, 255),
    gsSPEndDisplayList(),
};

static const Gfx kWalletRupeeBlue[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x00, 0, 0, 255, 255),
    gsDPSetEnvColor(0, 0, 50, 255),
    gsSPEndDisplayList(),
};

static const Gfx kWalletRupeeRed[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x00, 255, 0, 0, 255),
    gsDPSetEnvColor(50, 0, 0, 255),
    gsSPEndDisplayList(),
};

static const Gfx kWalletRupeePurple[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x00, 150, 0, 255, 255),
    gsDPSetEnvColor(30, 0, 50, 255),
    gsSPEndDisplayList(),
};

static const Gfx kWalletRupeeSilver[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x00, 255, 255, 255, 255),
    gsDPSetEnvColor(50, 50, 50, 255),
    gsSPEndDisplayList(),
};

static const Gfx* const kWalletRupee[] = {
    kWalletRupeeGreen,
    kWalletRupeeBlue,
    kWalletRupeeRed,
    kWalletRupeePurple,
    kWalletRupeeSilver,
};

static const Gfx kWalletBodyLightBrown[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x00, 160, 130, 100, 255),
    gsDPSetEnvColor(70, 60, 50, 255),
    gsSPEndDisplayList(),
};

static const Gfx kWalletBodyDarkBrown[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x00, 150, 80, 20, 255),
    gsDPSetEnvColor(40, 20, 0, 255),
    gsSPEndDisplayList(),
};

static const Gfx kWalletBodyGray[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x00, 50, 50, 50, 255),
    gsDPSetEnvColor(15, 15, 15, 255),
    gsSPEndDisplayList(),
};

static const Gfx* const kWalletBody[] = {
    kWalletBodyLightBrown,
    kWalletBodyDarkBrown,
    kWalletBodyGray,
};

void DrawGi_Wallet(PlayState* play, s16 index)
{
    const DrawGi* drawGi;
    drawGi = &kDrawGi[index];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, LIST_PTR(kWalletBody[drawGi->lists[1]]));
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[2]);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[3]);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[4]);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[5]);
    gSPDisplayList(POLY_OPA_DISP++, LIST_PTR(kWalletRupee[drawGi->lists[6]]));
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[7]);
    CLOSE_DISPS();
}

void DrawGi_Button(PlayState* play, s16 index)
{
    const DrawGi* drawGi;
    drawGi = &kDrawGi[index];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    if (drawGi->lists[1])
        Matrix_RotateZ(M_PI, 1);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    if (drawGi->lists[0])
    {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 0, 255);
    }
    else
    {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 0, 0, 255, 255);
    }
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[2]);
    CLOSE_DISPS();
}

void DrawGi_CustomBottleFairy(PlayState* play, s16 drawGiId, u8 param)
{
    static const u32 kColors[] = {
        0xcd69cdff,
        0x1a082eff,
        0xcdcd00ff,
    };

    const DrawGi* drawGi;
    drawGi = &kDrawGi[drawGiId];
    u8 c[4];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[1]);
    ModelViewUnkTransform(&play->billboardMtxF);
    Matrix_Translate(0.f, -8.f, 0.f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&c[0], &c[1], &c[2], &c[3], kColors[param]);
    gDPSetEnvColor(POLY_XLU_DISP++, c[0], c[1], c[2], c[3]);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[2]);
    CLOSE_DISPS();
}

void DrawGi_BombchuBag(PlayState* play, s16 drawGiId, u8 param)
{
    static const u32 kPrimColor = 0xBA3F3AFF;
    static const u32 kEnvColor = 0xA5231EFF;
    static const u32 kPrimColor2 = 0x1695D2FF;
    static const u32 kEnvColor2 = 0x054C7FFF;

    const DrawGi* drawGi;
    u8 r, g, b, a;

    drawGi = &kDrawGi[drawGiId];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, kPrimColor2);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColor2);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[0]);
    color4(&r, &g, &b, &a, kPrimColor);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColor);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[1]);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[2]);
    CLOSE_DISPS();
}

void DrawGi_BigFairy(PlayState* play, s16 drawGiId)
{
    const DrawGi* drawGi;
    u32 tex;

    drawGi = &kDrawGi[drawGiId];

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_ZB_CLD_SURF2);
    tex = DisplaceTexture(
        play->state.gfxCtx,
        G_TX_RENDERTILE,
        0 * (play->state.frameCount * 0), 0 * (play->state.frameCount * 0),
        32, 32,
        1,
        1 * (play->state.frameCount * 1), 1 * -(play->state.frameCount * 6),
        32, 32
    );
    gSPSegment(POLY_XLU_DISP++, 0x08, tex);
    Matrix_Translate(0.0f, -6.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(2.0f, 2.0f, 2.0f, MTXMODE_APPLY);
    ModelViewUnkTransform(&play->billboardMtxF);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[2]);

    CLOSE_DISPS();
}

void DrawGi_BottleBlueFire(PlayState* play, s16 drawGiId)
{
    const DrawGi* drawGi;

    drawGi = &kDrawGi[drawGiId];

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[1]);

    drawFire(play, 0x00ffffe0, 0x0000ffe0, 0.5f, -30.f);

    CLOSE_DISPS();
}

void DrawGi_Clock(PlayState* play, s16 index)
{
    static const float scale = 0.015f;
    const DrawGi* drawGi;

    drawGi = &kDrawGi[index];

    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_Push();
    Matrix_RotateZ(M_PI, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[1]);
    Matrix_Pop();
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[2]);
    Matrix_Translate(0.f, 1100.f, -50.f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[3]);
    CLOSE_DISPS();
}

void DrawGi_PondFish(PlayState* play, s16 index, u8 param)
{
    const DrawGi* drawGi;
    float scaleBase;
    float scaleActual;
    float scale;

    drawGi = &kDrawGi[index];
    scaleBase = FISH_WEIGHT_TO_LENGTH(2);
    scaleActual = FISH_WEIGHT_TO_LENGTH(param * 0.2f + 2.f);
    scale = scaleActual / scaleBase;

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}

typedef struct
{
    u32 listColorOpa;
    u32 listColorXlu;
}
RupeeColorDlists;

static const Gfx kDlistRupeeBlackOpa[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x80, 20, 20, 20, 255),
    gsDPSetEnvColor(0, 0, 0, 255),
    gsSPEndDisplayList(),
};

static const Gfx kDlistRupeeBlackXlu[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x80, 20, 20, 20, 255),
    gsDPSetEnvColor(30, 30, 30, 255),
    gsSPEndDisplayList(),
};

/**
 * Draw a (3D) rupee with a custom color.
 * param:
 *   0 - Black (Rupoor)
 *   1 - Green
 *   2 - Blue
 *   3 - Red
 *   4 - Purple
 *   5 - Silver
 *   6 - Gold/Huge
 */
void DrawGi_Rupee(PlayState* play, s16 index, u8 param)
{
    float h;
    u32 color;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    static const float smallRupeeScale = 0.7f;
    static const RupeeColorDlists kColors[] = {
        { DLIST_RAW(kDlistRupeeBlackOpa), DLIST_RAW(kDlistRupeeBlackXlu) },
        { 0x060004a0, 0x06000560 },
        { 0x060004c0, 0x06000580 },
        { 0x060004e0, 0x060005a0 },
        { 0x06000500, 0x060005c0 },
        { 0x06000520, 0x060005e0 },
        { 0x06000540, 0x06000600 },
    };

    if (param < 4)
        Matrix_Scale(smallRupeeScale, smallRupeeScale, smallRupeeScale, MTXMODE_APPLY);

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (param < 7)
    {
        gSPDisplayList(POLY_OPA_DISP++, kColors[param].listColorOpa);
    }
    else
    {
        h = (play->state.frameCount % 30) * (1.f/30.f);
        color = hsla(h, 1.f, 0.5f, 1.f);
        color4(&r, &g, &b, &a, color);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, r, g, b, a);
        r *= 0.8f;
        g *= 0.8f;
        b *= 0.8f;
        gDPSetEnvColor(POLY_OPA_DISP++, r, g, b, a);
    }
    gSPDisplayList(POLY_OPA_DISP++, kDrawGi[index].lists[0]);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (param < 7)
    {
        gSPDisplayList(POLY_XLU_DISP++, kColors[param].listColorXlu);
    }
    else
    {
        gSPDisplayList(POLY_XLU_DISP++, kColors[5].listColorXlu);
    }
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[1]);
    CLOSE_DISPS();
}

void DrawGi_CustomShield(PlayState* play, s16 index, u8 param)
{
    const DrawGi* drawGi;
    u32 dlistOpa;
    u32 dlistXlu;

    drawGi = &kDrawGi[index];
    dlistOpa = drawGi->lists[0];
    dlistXlu = drawGi->lists[1];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, dlistOpa);

    if (dlistXlu || param)
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    if (dlistXlu)
    {
        gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, dlistXlu);
    }

    /* Flame for progressive */
    if (param)
        shaderFlameEffect(play, drawGi->lists[2], 1.5f, 20.f);

    CLOSE_DISPS();
}

void DrawGi_TrapIce(PlayState* play, s16 drawGiId, u8 param)
{
    static const float scale = 0.5f;

    Matrix_Translate(0.f, -25.f, 0.f, MTXMODE_APPLY);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, (0 - play->gameplayFrames) % 128, 32, 32, 1, 0, (play->gameplayFrames * -2) % 128, 32, 32));
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 50, 100, 255);
    gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment3DL);
    CLOSE_DISPS();
}

struct CustomScaleColorDlists
{
    const void* l1;
    const void* l2;
};

static const Gfx kBronzeScaleList1[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x60, 255, 255, 255, 255),
    gsDPSetEnvColor(255, 145, 0, 255),
    gsSPEndDisplayList(),
};

static const Gfx kBronzeScaleList2[] = {
    gsDPPipeSync(),
    gsDPSetPrimColor(0x00, 0x80, 255, 200, 100, 255),
    gsDPSetEnvColor(150, 100, 50, 255),
    gsSPEndDisplayList(),
};


void DrawGi_CustomScale(PlayState* play, s16 index, u8 param)
{
    static const struct CustomScaleColorDlists kColors[] = {
        { kBronzeScaleList1, kBronzeScaleList2 },
        { (void*)0x06000a20, (void*)0x06000a60 },
        { (void*)0x06000a40, (void*)0x06000a80 },
    };

    s32 fc;
    const DrawGi* drawGi;
    const struct CustomScaleColorDlists* l;

    fc = play->state.frameCount;
    drawGi = &kDrawGi[index];
    l = &kColors[param - 1];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 8, DisplaceTexture(play->state.gfxCtx, 0, fc * 2, fc * -2, 0x40, 0x40, 1, fc * 4, fc * -4, 0x20, 0x20));
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, l->l2);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[1]);
    gSPDisplayList(POLY_XLU_DISP++, l->l1);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}
