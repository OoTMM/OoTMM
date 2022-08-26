#include <combo.h>
#include <combo/custom.h>

void Shader_Opa0_Xlu1(void*, s16);
void Shader_Opa0_Xlu12(void*, s16);
void Shader_Opa0(void*, s16);
void Shader_Opa01(void*, s16);
void Shader_Opa1023(void*, s16);
void Shader_Opa10_Xlu2(void*, s16);
void Shader_Opa10_Xlu234(void*, s16);
void Shader_Opa10_Xlu32(void*, s16);
void Shader_Opa10234567(void*, s16);
void Shader_Xlu01(void*, s16);
void Shader_BlueFire(void*, s16);
void Shader_BombchuMask(void*, s16);
void Shader_Compass(void*, s16);
void Shader_DekuNut(void*, s16);
void Shader_Fairy(void*, s16);
void Shader_Fish(void*, s16);
void Shader_GiantKnife(void*, s16);
void Shader_GS(void*, s16);
void Shader_Heart(void*, s16);
void Shader_Medallion(void*, s16);
void Shader_MirrorShield(void*, s16);
void Shader_Poe(void*, s16);
void Shader_Potion(void*, s16);
void Shader_Rupee(void*, s16);
void Shader_Scale(void*, s16);
void Shader_SoldOut(void*, s16);
void Shader_Spell(void*, s16);
void Shader_MoonTear(void*, s16);

/* Custom Shaders */
void Shader_CustomNote(GameState* gs, u16 shaderId)
{
    const Shader* shader;
    u32 c;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    shader = &kShaders[shaderId];
    c = shader->lists[0];
    r = (c >> 24) & 0xff;
    g = (c >> 16) & 0xff;
    b = (c >> 8) & 0xff;
    a = (c >> 0) & 0xff;

    OPEN_DISPS(gs->gfx);
    InitListPolyXlu(gs->gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, shader->lists[1]);
    CLOSE_DISPS();
}


#if defined(GAME_OOT)

void ConvertMatrix(const float* in, u16* out);

static const float kMatrixRot[] = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, -1.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 1.f,
};

static void* pushMatrix(GfxContext* gfx, const float* mat)
{
    void* end = gfx->polyOpa.end;
    end = (char*)end - 0x40;
    gfx->polyOpa.end = end;

    ConvertMatrix(mat, end);

    return end;
}

static void* dummySegment(GfxContext* gfx)
{
    Gfx* end = gfx->polyOpa.end - 1;
    gfx->polyOpa.end = end;
    gSPEndDisplayList(end);
    return end;
}

void Shader_SpiritualStones(GameState* gs, u16 shaderId)
{
    u8 primRed, primGreen, primBlue;
    u8 envRed, envGreen, envBlue;
    const Shader* shader;

    float fc = 0;

    shader = &kShaders[shaderId];

    switch (shader->lists[0] & 0xffff)
    {
    case 0x1240:
        /* Emerald */
        primRed = 0xff;
        primGreen = 0xff;
        primBlue = 0xa0;
        envRed = 0x00;
        envGreen = 0xff;
        envBlue = 0x00;
        break;
    case 0x20a0:
        /* Ruby */
        primRed = 0xff;
        primGreen = 0xaa;
        primBlue = 0xff;
        envRed = 0xff;
        envGreen = 0x00;
        envBlue = 100;
        break;
    case 0x3530:
        /* Sapphire */
        primRed = 0x32;
        primGreen = 0xff;
        primBlue = 0xff;
        envRed = 0x32;
        envGreen = 0;
        envBlue = 0x96;
        break;
    }

    OPEN_DISPS(gs->gfx);

    /* Matrix setup */
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(POLY_XLU_DISP++, pushMatrix(gs->gfx, kMatrixRot), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(POLY_OPA_DISP++, pushMatrix(gs->gfx, kMatrixRot), G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    /* Segment setup */
    gSPSegment(POLY_XLU_DISP++, 9, dummySegment(gs->gfx));
    gSPSegment(POLY_OPA_DISP++, 8, dummySegment(gs->gfx));

    InitListPolyXlu(gs->gfx);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, primRed, primGreen, primBlue, 0xFF);
    gDPSetEnvColor(POLY_XLU_DISP++, envRed, envGreen, envBlue, 0xFF);
    gSPDisplayList(POLY_XLU_DISP++, shader->lists[0]);

    InitListPolyOpa(gs->gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x80, 0xff, 0xff, 0xaa, 0xff);
    gDPSetEnvColor(POLY_OPA_DISP++, 0x96, 0x78, 0x00, 0xFF);
    gSPDisplayList(POLY_OPA_DISP++, shader->lists[1]);

    CLOSE_DISPS();
}

#else

void Shader_SpiritualStones(GameState* gs, u16 shaderId)
{

}

#endif

const Shader kShaders[256] = {
#if defined(GAME_OOT)
# include "data/oot/shaders.inc"
# include "data/mm/shaders.inc"
#endif

#if defined(GAME_MM)
# include "data/mm/shaders.inc"
# include "data/oot/shaders.inc"
#endif
};
