#include <combo.h>
#include <combo/draw.h>

#define ICON_SIZE 16

#if defined(GAME_OOT)
# define VROM_FILE 0x846000
#endif

#if defined(GAME_MM)
# define VROM_FILE 0x846000 | VROM_FOREIGN_OFFSET
#endif

static Gfx gDListLoadStoneAgonyIcon[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_RGBA, G_IM_SIZ_32b, 24, 24, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static s16      sAgonyAlpha;
static float    sAgonyOffset;

void Interface_AgonyIconTick(void)
{
    sAgonyAlpha = 300;
}

void Interface_AgonyIconDraw(PlayState* play)
{
    static const float scale = (float)ICON_SIZE / 24.f;
    static const float revScale = 1.0f / scale;

    int val;
    int alpha;
    float x;
    float y;
    float targetOffset;
    float offset;
    float amplitude;
    void* tex;

    /* Compute alpha */
    val = sAgonyAlpha;
    alpha = val;
    sAgonyAlpha -= 10;
    if (sAgonyAlpha < 0)
        sAgonyAlpha = 0;
    if (alpha > 255)
        alpha = 255;

    if (alpha == 0)
    {
        sAgonyOffset = 0;
        return;
    }

    tex = comboCacheGetFilePartial(VROM_FILE, 9 * 0x900, 0x900);
    if (!tex)
        return;
    gDListLoadStoneAgonyIcon[0].words.w1 = (u32)tex;

    /* Compute position */
    amplitude = val * 0.03f;
    targetOffset = Math_SinS((s16)(play->state.frameCount & 7) << 13) * amplitude;
    offset = sAgonyOffset * 0.5f + targetOffset * 0.5f;
    sAgonyOffset = offset;

    OPEN_DISPS(play->state.gfxCtx);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0x80, 255, 255, 255, alpha);
    gSPDisplayList(OVERLAY_DISP++, gDListLoadStoneAgonyIcon);
    x = 26.f + offset;
    y = 220.f;
    gSPTextureRectangle(OVERLAY_DISP++,
        x * 4.f, y * 4.f,
        (x + 24 * scale) * 4.f, (y + 24 * scale) * 4.f,
        G_TX_RENDERTILE, 0, 0,
        (1 << 10) * revScale, (1 << 10) * revScale
    );
    CLOSE_DISPS();
}
