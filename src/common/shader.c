#include <combo.h>

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
