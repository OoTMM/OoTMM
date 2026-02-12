#include <combo.h>
#include <combo/custom.h>

static const Gfx kDlistLoadIA4_8x12[] = {
    gsDPLoadTextureTile_4b(
        0x09000000,
        G_IM_FMT_IA,
        8, 12,
        0, 0,
        7, 11,
        0,
        G_TX_WRAP, G_TX_WRAP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    ),
    gsDPTileSync(),
    gsSPEndDisplayList(),
};

static const float kScale = 0.75f;
static const float kScaleInv = 1.0f / kScale;

void FontCustom_DrawChar(Gfx** gfxP, int x, int y, char c)
{
    gSPSegment((*gfxP)++, 0x09, (char*)g.customKeep + CUSTOM_KEEP_FONT + ((c - ' ') * 0x30));
    gSPDisplayList((*gfxP)++, kDlistLoadIA4_8x12);
    gSPTextureRectangle(
        (*gfxP)++,
        x * 4 * kScale, y * 4 * kScale,
        (x + 8) * 4 * kScale, (y + 12) * 4 * kScale,
        0,
        0, 0,
        (1 << 10) * kScaleInv, (1 << 10) * kScaleInv
    );
}

void FontCustom_DrawStr(Gfx** gfxP, int x, int y, const char* str)
{
    int i;
    char c;

    i = 0;
    for (;;)
    {
        c = str[i];
        if (c == 0)
            break;
        /* 7 instead of 8 for tighter text */
        FontCustom_DrawChar(gfxP, x + i * 7, y, c);
        i++;
    }
}

static const Gfx kDlistSetup[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetPrimColor(0, 0, 0xff, 0xff, 0xff, 0xff),
    gsDPSetEnvColor(0, 0, 0, 0),
};

void FontCustom_Setup(Gfx** gfxP)
{
    gSPDisplayList((*gfxP)++, kDlistSetup);
}
