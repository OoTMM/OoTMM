#include <combo.h>
#include <combo/draw.h>

#define ICON_SIZE 16

static Gfx gDListLoadStoneAgonyIcon[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_RGBA, G_IM_SIZ_32b, 24, 24, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

void Interface_AgonyIconTick(void)
{

}

void Interface_AgonyIconDraw(PlayState* play)
{
    static const float scale = (float)ICON_SIZE / 24.f;
    static const float revScale = 1.0f / scale;

    int x;
    int y;
    void* tex;

    tex = comboCacheGetFilePartial(0x846000, 9 * 0x900, 0x900);
    if (!tex)
        return;
    gDListLoadStoneAgonyIcon[0].words.w1 = (u32)tex;

    OPEN_DISPS(play->state.gfxCtx);
    Draw_Init2D(&OVERLAY_DISP);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0x80, 255, 255, 255, 255);
    gSPDisplayList(OVERLAY_DISP++, gDListLoadStoneAgonyIcon);
    x = 26;
    y = 220;
    gSPTextureRectangle(OVERLAY_DISP++,
        x * 4.f, y * 4.f,
        (x + 24 * scale) * 4.f, (y + 24 * scale) * 4.f,
        G_TX_RENDERTILE, 0, 0,
        (1 << 10) * revScale, (1 << 10) * revScale
    );
    CLOSE_DISPS();
}
