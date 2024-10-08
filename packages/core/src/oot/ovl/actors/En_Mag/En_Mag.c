#include <combo.h>
#include <combo/sfx.h>
#include <combo/audio.h>
#include "En_Mag.h"

#define gDPSetTileCustom(pkt, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt)                    \
    do {                                                                                                               \
        gDPPipeSync(pkt);                                                                                              \
        gDPTileSync(pkt);                                                                                              \
        gDPSetTile(pkt, fmt, siz, (((width)*siz##_TILE_BYTES) + 7) >> 3, 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, \
                   masks, shifts);                                                                                     \
        gDPTileSync(pkt);                                                                                              \
        gDPSetTile(pkt, fmt, siz, (((width)*siz##_TILE_BYTES) + 7) >> 3, 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt,  \
                   cms, masks, shifts);                                                                                \
        gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC,                                \
                       ((height)-1) << G_TEXTURE_IMAGE_FRAC);                                                          \
    } while (0)

#define CHECK_BTN_ALL(buttons, mask) ((buttons & mask) == mask)
#define FLAGS (ACTOR_FLAG_OOT_4 | ACTOR_FLAG_OOT_5)

void EnMag_Init(Actor_EnMag* this, GameState_Play* play);
void EnMag_Destroy(Actor_EnMag* this, GameState_Play* play);
void EnMag_Update(Actor_EnMag* this, GameState_Play* play);
void EnMag_Draw(Actor_EnMag* this, GameState_Play* play);

static s16 sDelayTimer = 0;

static const s16 sMmEffectScrollVelocitySs[] = { -1, 1, 1, -1, 1, 1 };
static const s16 sMmEffectScrollVelocityTs[] = { -2, -2, -2, 2, 2, 2 };

void EnMag_Init(Actor_EnMag* this, GameState_Play* play)
{
    YREG(1) = 63;
    YREG(3) = 80;
    YREG(4) = 255;
    YREG(5) = 30;
    YREG(6) = 30;
    YREG(7) = 119;
    YREG(8) = 7;
    YREG(9) = 5;
    YREG(10) = 3;

    VREG(4) = 1;
    VREG(5) = 6;
    VREG(6) = 2;

    this->copyrightAlphaStep = 6;
    this->fadeOutAlphaStep = 10;

    VREG(19) = 99;
    VREG(21) = 9;
    VREG(23) = 10;
    VREG(24) = 8;

    this->effectScroll = 0;
    this->unk_E30C = 0;

    this->effectPrimColor[0] = 0.0f;
    this->effectPrimColor[1] = 100.0f;
    this->effectPrimColor[2] = 170.0f;
    this->effectEnvColor[0] = 0.0f;
    this->effectEnvColor[1] = 100.0f;
    this->effectEnvColor[2] = 0.0f;

    this->effectFadeInTimer = 40;

    this->effectFadeInState = this->effectPrimLodFrac = this->globalState = this->effectAlpha = this->mainAlpha = this->subAlpha = this->copyrightAlpha = 0.0f;

    if (gSaveContext.forceRisingButtonAlphas)
    {
        this->mainAlpha = 210;
        this->subAlpha = 255;
        this->copyrightAlpha = 255;

        this->effectPrimLodFrac = 128.0f;
        this->effectAlpha = 255.0f;

        this->effectPrimColor[0] = 255.0f;
        this->effectPrimColor[1] = 255.0f;
        this->effectPrimColor[2] = 170;
        this->effectEnvColor[0] = 255.0f;
        this->effectEnvColor[1] = 100;

        gSaveContext.forceRisingButtonAlphas = FALSE;
        this->globalState = MAG_STATE_DISPLAY;
        sDelayTimer = 20;
        gSaveContext.transFadeDuration = 1;
        gSaveContext.transWipeSpeed = 255;
    }

    Font_LoadOrderedFont(&this->font);

    this->unk_E316 = 0;
    this->unk_E318 = 0;
    this->unk_E31C = 0;
    this->unk_E320 = 0;

    for (int i = 0; i < 6; ++i)
    {
        this->mmEffectScrollSs[i] = 0;
        this->mmEffectScrollTs[i] = 0;
    }

    this->mmEffectPrimColor[0] = 255;
    this->mmEffectPrimColor[1] = 155;
    this->mmEffectPrimColor[2] = 255;
    this->mmEffectEnvColor[0] = 0;
    this->mmEffectEnvColor[1] = 255;
    this->mmEffectEnvColor[2] = 155;
}

void EnMag_Destroy(Actor_EnMag* this, GameState_Play* play)
{
}

void EnMag_Update(Actor_EnMag* this, GameState_Play* play)
{
    if (gSaveContext.fileIndex != 0xfedc)
    {
        if (this->globalState < MAG_STATE_DISPLAY)
        {
            if (CHECK_BTN_ALL(play->gs.input[0].pressed.buttons, START_BUTTON) || CHECK_BTN_ALL(play->gs.input[0].pressed.buttons, A_BUTTON) || CHECK_BTN_ALL(play->gs.input[0].pressed.buttons, B_BUTTON))
            {
                PlaySound(NA_SE_SY_PIECE_OF_HEART);

                this->mainAlpha = 210;
                this->subAlpha = 255;
                this->copyrightAlpha = 255;

                this->effectPrimLodFrac = 128.0f;
                this->effectAlpha = 255.0f;

                this->effectPrimColor[0] = 255.0f;
                this->effectPrimColor[1] = 255.0f;
                this->effectPrimColor[2] = 170;
                this->effectEnvColor[0] = 255.0f;
                this->effectEnvColor[1] = 100;

                this->globalState = MAG_STATE_DISPLAY;
                sDelayTimer = 20;
                gSaveContext.transFadeDuration = 1;
                gSaveContext.transWipeSpeed = 255;
            }
        }
        else if (this->globalState >= MAG_STATE_DISPLAY)
        {
            if (sDelayTimer == 0)
            {
                if (CHECK_BTN_ALL(play->gs.input[0].pressed.buttons, START_BUTTON) ||
                    CHECK_BTN_ALL(play->gs.input[0].pressed.buttons, A_BUTTON) ||
                    CHECK_BTN_ALL(play->gs.input[0].pressed.buttons, B_BUTTON))
                {
                    if (play->transitionTrigger != TRANS_TRIGGER_NORMAL)
                    {
                        Audio_SetCutsceneFlag(0);
                        PlaySound(NA_SE_SY_PIECE_OF_HEART);

                        gSaveContext.gameMode = GAMEMODE_FILE_SELECT;
                        play->transitionTrigger = TRANS_TRIGGER_NORMAL;
                        play->transitionType = TRANS_TYPE_FADE_BLACK;
                    }

                    this->copyrightAlphaStep = 15;
                    this->fadeOutAlphaStep = 25;
                    this->globalState = MAG_STATE_FADE_OUT;
                }
            }
            else
            {
                sDelayTimer--;
            }
        }
    }

    if (this->globalState == MAG_STATE_FADE_IN)
    {
        if (this->effectFadeInState == 0)
        {
            this->effectAlpha += 6.375f;
            this->effectPrimLodFrac += 0.8f;

            this->effectPrimColor[0] += 6.375f;
            this->effectPrimColor[1] += 3.875f;
            this->effectPrimColor[2] += 2.125f;
            this->effectEnvColor[0] += 6.375f;
            this->effectEnvColor[1] += 3.875f;

            this->effectFadeInTimer--;

            if (this->effectFadeInTimer == 0)
            {
                this->effectPrimLodFrac = 32.0f;
                this->effectAlpha = 255.0f;

                this->effectPrimColor[0] = 255.0f;
                this->effectPrimColor[1] = 255.0f;
                this->effectPrimColor[2] = 255.0f;
                this->effectEnvColor[0] = 255.0f;
                this->effectEnvColor[1] = 255.0f;

                this->effectFadeInTimer = 40;
                this->effectFadeInState = 1;
            }
        }
        else if (this->effectFadeInState == 1)
        {
            this->effectPrimColor[2] += -2.125f;
            this->effectEnvColor[1] += -3.875f;
            this->effectPrimLodFrac += 2.4f;
            this->effectFadeInTimer--;

            if (this->effectFadeInTimer == 0)
            {
                this->effectPrimLodFrac = 128.0f;

                this->effectPrimColor[2] = 170.0f;
                this->effectEnvColor[1] = 100.0f;

                this->effectFadeInTimer = 32;
                this->effectFadeInState = 2;
            }
        }

        if (this->effectAlpha > 160)
        {
            this->mainAlpha += VREG(5);
            if (this->mainAlpha >= 210.0f)
            {
                this->mainAlpha = 210.0f;
            }

            if (this->mainAlpha >= 210)
            {
                this->subAlpha += VREG(6);
                if (this->subAlpha >= 255.0f)
                {
                    this->subAlpha = 255.0f;
                }

                if (this->subAlpha >= 200)
                {
                    this->copyrightAlpha += this->copyrightAlphaStep;
                    if (this->copyrightAlpha >= 255.0f)
                    {
                        this->copyrightAlpha = 255.0f;
                        this->globalState = MAG_STATE_DISPLAY;
                        sDelayTimer = 20;
                    }
                }
            }
        }
    }
    else if (this->globalState == MAG_STATE_FADE_OUT)
    {
        this->effectAlpha -= this->fadeOutAlphaStep;
        if (this->effectAlpha < 0.0f)
        {
            this->effectAlpha = 0.0f;
        }

        this->mainAlpha -= this->fadeOutAlphaStep;
        if (this->mainAlpha < 0.0f)
        {
            this->mainAlpha = 0.0f;
        }

        this->subAlpha -= this->fadeOutAlphaStep;
        if (this->subAlpha < 0.0f)
        {
            this->subAlpha = 0.0f;
        }

        this->copyrightAlpha -= this->copyrightAlphaStep;
        if (this->copyrightAlpha < 0.0f)
        {
            this->copyrightAlpha = 0.0f;
            this->globalState = MAG_STATE_POST_DISPLAY;
        }
    }

    if (this->globalState == MAG_STATE_INITIAL)
    {
        if (CutsceneFlags_Get(play, 3))
        {
            this->effectFadeInTimer = 40;
            this->globalState = MAG_STATE_FADE_IN;
        }
    }
    else if (this->globalState == MAG_STATE_DISPLAY)
    {
        if (CutsceneFlags_Get(play, 4))
        {
            this->globalState = MAG_STATE_FADE_OUT;
        }
    }
}

static void EnMag_DrawTextureI8(Gfx** gfxP, void* texture, s16 texWidth, s16 texHeight, s16 rectLeft, s16 rectTop, s16 rectWidth, s16 rectHeight, u16 dsdx, u16 dtdy)
{
    Gfx* gfx = *gfxP;

    gDPLoadTextureBlock(gfx++, texture, G_IM_FMT_I, G_IM_SIZ_8b, texWidth, texHeight, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(gfx++, rectLeft << 2, rectTop << 2, (rectLeft + rectWidth) << 2, (rectTop + rectHeight) << 2, G_TX_RENDERTILE, 0, 0, dsdx, dtdy);
    *gfxP = gfx;
}

static void EnMag_DrawTextureIA8(Gfx** gfxP, void* texture, s16 texWidth, s16 texHeight, s16 rectLeft, s16 rectTop, s16 rectWidth, s16 rectHeight, u16 dsdx, u16 dtdy)
{
    Gfx* gfx = *gfxP;

    gDPLoadTextureBlock(gfx++, texture, G_IM_FMT_IA, G_IM_SIZ_8b, texWidth, texHeight, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(gfx++, rectLeft << 2, rectTop << 2, (rectLeft + rectWidth) << 2, (rectTop + rectHeight) << 2, G_TX_RENDERTILE, 0, 0, dsdx, dtdy);
    *gfxP = gfx;
}

static void EnMag_DrawEffectTexturesOot(Gfx** gfxP, void* maskTex, void* effectTex, s16 maskWidth, s16 maskHeight,
                              s16 effectWidth, s16 effectHeight, s16 rectLeft, s16 rectTop, s16 rectWidth,
                              s16 rectHeight, u16 dsdx, u16 dtdy, u16 shifts, u16 shiftt, u16 flag, Actor_EnMag* this)
{
    Gfx* gfx = *gfxP;

    gDPLoadMultiBlock_4b(gfx++, maskTex, 0x0000, G_TX_RENDERTILE, G_IM_FMT_I, maskWidth, maskHeight, 0,
                         G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                         G_TX_NOLOD);

    if (!flag)
    {
        gDPLoadMultiBlock(gfx++, effectTex, 0x0100, 1, G_IM_FMT_I, G_IM_SIZ_8b, effectWidth, effectHeight, 0,
                          G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, 5, shifts, shiftt);

        gDPSetTileSize(gfx++, 1, 0, this->effectScroll & 0x7F, 31 << 2, (31 << 2) + (this->effectScroll & 0x7F));
    }

    gSPTextureRectangle(gfx++, rectLeft << 2, rectTop << 2, (rectLeft + rectWidth) << 2, (rectTop + rectHeight) << 2, G_TX_RENDERTILE, 0, 0, dsdx, dtdy);
    *gfxP = gfx;
}

void EnMag_DrawEffectTexturesMm(Gfx** gfxp, u32 maskTex, u32 effectTex, s16 maskWidth, s16 maskHeight,
                              s16 effectWidth, s16 effectHeight, s16 rectLeft, s16 rectTop, s16 rectWidth,
                              s16 rectHeight, u16 dsdx, u16 dtdy, u16 shifts, u16 shiftt, u16 index, Actor_EnMag* this) {
    Gfx* gfx = *gfxp;

    gDPLoadMultiBlock_4b(gfx++, maskTex, 0x0000, G_TX_RENDERTILE, G_IM_FMT_I, maskWidth, maskHeight, 0,
                         G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                         G_TX_NOLOD);

    gDPLoadMultiBlock(gfx++, effectTex, 0x0100, 1, G_IM_FMT_I, G_IM_SIZ_8b, effectWidth, effectHeight, 0,
                      G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, shifts, shiftt);

    gDPSetTileSize(gfx++, 1, this->mmEffectScrollSs[index] & 0x7F, this->mmEffectScrollTs[index] & 0x7F,
                   (this->mmEffectScrollSs[index] & 0x7F) + 15, (this->mmEffectScrollTs[index] & 0x7F) + 15);

    gSPTextureRectangle(gfx++, rectLeft << 2, rectTop << 2, (rectLeft + rectWidth) << 2, (rectTop + rectHeight) << 2,
                        G_TX_RENDERTILE, 0, 0, dsdx, dtdy);

    *gfxp = gfx;
}

static void EnMag_BlitTextureRGBA32(Gfx** gfxP, u32 addr, s16 w, s16 h, s16 x, s16 y, float scale)
{
    Gfx* gfx;
    float rScale;
    s16 hSlice;

    hSlice = 4;
    rScale = 1.f / scale;

    gfx = *gfxP;
    Gfx_SetupDL_56Ptr(&gfx);
    for (int j = 0; j < h / hSlice; ++j)
    {
        gDPLoadTextureTile(
            gfx++,
            addr + (4 * w * hSlice * j),
            G_IM_FMT_RGBA, G_IM_SIZ_32b,
            w, hSlice,
            0, 0,
            w - 1, hSlice - 1,
            0,
            G_TX_WRAP, G_TX_WRAP,
            G_TX_NOMASK, G_TX_NOMASK,
            G_TX_NOLOD, G_TX_NOLOD
        );
        gDPTileSync(gfx++);
        gSPTextureRectangle(
            gfx++,
            x * 4, y * 4 + (j * hSlice * 4) * scale,
            x * 4 + (w * 4) * scale, y * 4 + ((j + 1) * hSlice * 4) * scale,
            0,
            0, 0,
            ((1 << 10) * rScale), ((1 << 10) * rScale)
        );
    }
    *gfxP = gfx;
}

void EnMag_DrawImageRGBA32(Gfx** gfxP, s16 centerX, s16 centerY, u8* source, u32 width, u32 height)
{
    Gfx* gfx = *gfxP;
    u8* curTexture;
    s32 textureCount;
    u32 rectLeft;
    u32 rectTop;
    u32 textureHeight;
    s32 remainingSize;
    s32 textureSize;
    s32 i;

    Gfx_SetupDL_56Ptr(&gfx);

    curTexture = source;
    rectLeft = centerX - (width / 2);
    rectTop = centerY - (height / 2);
    textureHeight = 4096 / (width << 2);
    remainingSize = (width * height) << 2;
    textureSize = (width * textureHeight) << 2;
    textureCount = remainingSize / textureSize;
    if ((remainingSize % textureSize) != 0)
    {
        textureCount += 1;
    }

    gDPSetTileCustom(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, textureHeight, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                     G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    remainingSize -= textureSize;

    for (i = 0; i < textureCount; i++)
    {
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, curTexture);

        gDPLoadSync(gfx++);
        gDPLoadTile(gfx++, G_TX_LOADTILE, 0, 0, (width - 1) << 2, (textureHeight - 1) << 2);

        gSPTextureRectangle(gfx++, rectLeft << 2, rectTop << 2, (rectLeft + (s32)width) << 2,
                            (rectTop + textureHeight) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

        curTexture += textureSize;
        rectTop += textureHeight;

        if ((remainingSize - textureSize) < 0)
        {
            if (remainingSize > 0)
            {
                textureHeight = remainingSize / (s32)(width << 2);
                remainingSize -= textureSize;

                gDPSetTileCustom(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, textureHeight, 0,
                                 G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                 G_TX_NOLOD, G_TX_NOLOD);
            }
        }
        else
        {
            remainingSize -= textureSize;
        }
    }

    *gfxP = gfx;
}

void EnMag_DrawCharTexture(Gfx** gfxP, u8* texture, s32 rectLeft, s32 rectTop)
{
    Gfx* gfx = *gfxP;

    YREG(0) = 1024.0f / (YREG(1) / 100.0f);
    YREG(2) = 16.0f * (YREG(1) / 100.0f);

    gDPLoadTextureBlock_4b(gfx++, texture, G_IM_FMT_I, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                           G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle(gfx++, rectLeft << 2, rectTop << 2, (rectLeft + YREG(2)) << 2, (rectTop + YREG(2)) << 2,
                        G_TX_RENDERTILE, 0, 0, YREG(0), YREG(0));

    *gfxP = gfx;
}

#define LOGO_SCALE          0.75f
#define LOGO_SCALE_REVERSE  (1.f / LOGO_SCALE)

static void EnMag_DrawText(Actor_EnMag* this, Gfx** gfx, const char* text, int x, int y)
{
    char c;
    int i;

    i = 0;
    for (;;)
    {
        c = text[i++];
        if (!c)
            return;
        if (c == ' ')
        {
            x += 5;
        }
        else
        {
            EnMag_DrawCharTexture(gfx, this->font.fontBuf + (c - 'A' + 0xab) * FONT_CHAR_TEX_SIZE, x, y);
            x += 7;
        }
    }
}

static void EnMag_DrawTextWithShadow(Actor_EnMag* this, Gfx** gfx, const char* text, int x, int y, u32 color, u8 alpha)
{
    u8 r;
    u8 g;
    u8 b;

    r = (color >> 16) & 0xff;
    g = (color >> 8) & 0xff;
    b = (color & 0xff) & 0xff;

    gDPPipeSync((*gfx)++);
    gDPSetCombineLERP((*gfx)++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetPrimColor((*gfx)++, 0, 0, 0, 0, 0, alpha);
    EnMag_DrawText(this, gfx, text, x + 1, y + 1);
    gDPSetPrimColor((*gfx)++, 0, 0, r, g, b, alpha);
    EnMag_DrawText(this, gfx, text, x, y);
}

static void EnMag_DrawLogoOot(Actor_EnMag* this, GameState_Play* play, Gfx** gfxP, float centerX, float centerY)
{
    static void* effectMaskTextures[] = {
        (void*)0x06019B00, (void*)0x0601A300, (void*)0x0601AB00, (void*)0x0601B300,
        (void*)0x0601BB00, (void*)0x0601C300, (void*)0x0601CB00, (void*)0x0601D300,
        (void*)0x0601DB00
    };

    Gfx* gfx;
    float baseX;
    float baseY;

    gfx = *gfxP;
    baseX = centerX - (160.f * LOGO_SCALE) * 0.5f;
    baseY = centerY - (160.f * LOGO_SCALE) * 0.5f;

    gSPSegment(gfx++, 0x06, play->objectCtx.status[this->actor.objectSlot].segment);

    if ((s16)this->effectPrimLodFrac != 0)
    {
        /* Effect */
        Gfx_SetupDL_39Ptr(&gfx);
        gDPSetCycleType(gfx++, G_CYC_2CYCLE);
        gDPSetAlphaCompare(gfx++, G_AC_THRESHOLD);
        gDPSetRenderMode(gfx++, G_RM_PASS, G_RM_CLD_SURF2);
        gDPSetCombineLERP(gfx++, TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, 1, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
            ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0);

        gDPSetPrimColor(gfx++, 0, (s16)this->effectPrimLodFrac, (s16)this->effectPrimColor[0],
            (s16)this->effectPrimColor[1], (s16)this->effectPrimColor[2], (s16)this->effectAlpha);
        gDPSetEnvColor(gfx++, (s16)this->effectEnvColor[0], (s16)this->effectEnvColor[1], (s16)this->effectEnvColor[2], 255);

        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                int k = (j * 3) + i;
                EnMag_DrawEffectTexturesOot(&gfx, effectMaskTextures[k], (void*)0x0601e300, 64, 64, 32, 32, baseX + (((i * 64) - 16) * LOGO_SCALE), baseY + (((j * 64) - 16) * LOGO_SCALE), 64 * LOGO_SCALE, 64 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE, 1, 1, k, this);
            }
        }
    }

    if ((s16)this->mainAlpha != 0)
    {
        /* Logo */
        Gfx_SetupDL_39Ptr(&gfx);
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, (s16)this->mainAlpha);
        EnMag_BlitTextureRGBA32(&gfx, 0x06000300, 160, 160, baseX, baseY, LOGO_SCALE);

        /* Subtitles */
        Gfx_SetupDL_39Ptr(&gfx);
        gDPSetAlphaCompare(gfx++, G_AC_NONE);
        gDPSetCombineLERP(gfx++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                        ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

        if ((s16)this->mainAlpha < 100) {
            gDPSetRenderMode(gfx++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
        } else {
            gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        }

        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, (s16)this->mainAlpha);
        gDPSetEnvColor(gfx++, 100, 0, 100, 255);
        EnMag_DrawTextureI8(&gfx, (void*)0x0601e700, 72, 8, baseX + 75 * LOGO_SCALE, baseY + 53 * LOGO_SCALE, 72 * LOGO_SCALE, 8 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE);
        EnMag_DrawTextureI8(&gfx, (void*)0x0601e940, 96, 8, baseX + 71 * LOGO_SCALE, baseY + 107 * LOGO_SCALE, 96 * LOGO_SCALE, 8 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE);

        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, 200, 200, 150, (s16)this->mainAlpha);
        gDPSetEnvColor(gfx++, 100, 100, 50, 255);

        EnMag_DrawTextureI8(&gfx, (void*)0x0601e700, 72, 8, baseX + 74 * LOGO_SCALE, baseY + 52 * LOGO_SCALE, 72 * LOGO_SCALE, 8 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE);
        EnMag_DrawTextureI8(&gfx, (void*)0x0601e940, 96, 8, baseX + 70 * LOGO_SCALE, baseY + 106 * LOGO_SCALE, 96 * LOGO_SCALE, 8 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE);
    }
    *gfxP = gfx;
}

static void EnMag_DrawLogoMm(Actor_EnMag* this, GameState_Play* play, Gfx** gfxP, float centerX, float centerY)
{
    static const u32 sAppearEffectMaskTextures[] = {
        0x0600CF40, 0x0600D740, 0x0600EF40, 0x0600DF40,
        0x0600E740, 0x0600F740
    };

    static const u32 sDisplayEffectMaskTextures[] = {
        0x06009F40, 0x0600A740, 0x0600BF40, 0x0600AF40,
        0x0600B740, 0x0600C740
    };

    static const u32 sEffectTextures[] = {
        0x0600FF40, 0x06010340, 0x06010340, 0x06010740,
        0x06010B40, 0x06010B40
    };

    Gfx* gfx;
    float baseX;
    float baseY;

    gfx = *gfxP;
    baseX = centerX - (144.f * LOGO_SCALE) * 0.5f;
    baseY = centerY - (112.f * LOGO_SCALE) * 0.5f;

    gSPSegment(gfx++, 0x6, this->mmMagData);

    /* Appear Effect */
    if ((s16)this->effectPrimLodFrac != 0)
    {
        Gfx_SetupDL_39Ptr(&gfx);
        gDPSetCycleType(gfx++, G_CYC_2CYCLE);
        gDPSetAlphaCompare(gfx++, G_AC_THRESHOLD);
        gDPSetRenderMode(gfx++, G_RM_PASS, G_RM_CLD_SURF2);
        gDPSetCombineLERP(gfx++, TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, 1, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
            ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0);
        gDPSetPrimColor(gfx++, 0, (s16)this->effectPrimLodFrac, this->mmEffectPrimColor[0], this->mmEffectPrimColor[1],
            this->mmEffectPrimColor[2], (s16)this->effectAlpha);
        gDPSetEnvColor(gfx++, this->mmEffectEnvColor[0], this->mmEffectEnvColor[1], this->mmEffectEnvColor[2], 255);

        for (int j = 0; j < 2; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                int k = (j * 3) + i;
                this->mmEffectScrollSs[k] += sMmEffectScrollVelocitySs[k];
                this->mmEffectScrollTs[k] += sMmEffectScrollVelocityTs[k];
                EnMag_DrawEffectTexturesMm(&gfx, sAppearEffectMaskTextures[k], sEffectTextures[k], 64, 64, 32, 32, baseX + (((i * 64) - 48) * LOGO_SCALE), baseY + (((j * 64) - 6) * LOGO_SCALE), 64 * LOGO_SCALE, 64 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE, 1, 1, k, this);
            }
        }
    }

    /* Mask */
    if ((s16)this->mainAlpha != 0)
    {
        Gfx_SetupDL_39Ptr(&gfx);
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, (s16)this->mainAlpha);
        EnMag_BlitTextureRGBA32(&gfx, 0x06011e48, 0x80, 0x70, baseX - 42 * LOGO_SCALE, baseY + 2 * LOGO_SCALE, LOGO_SCALE);
    }

    /* Full Effect */
    if ((s16)this->effectPrimLodFrac != 0)
    {
        Gfx_SetupDL_39Ptr(&gfx);
        gDPSetCycleType(gfx++, G_CYC_2CYCLE);
        gDPSetAlphaCompare(gfx++, G_AC_THRESHOLD);
        gDPSetRenderMode(gfx++, G_RM_PASS, G_RM_CLD_SURF2);
        gDPSetCombineLERP(gfx++, TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, 1, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
            ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0);
        gDPSetPrimColor(gfx++, 0, (s16)this->effectPrimLodFrac, this->mmEffectPrimColor[0], this->mmEffectPrimColor[1],
            this->mmEffectPrimColor[2], (s16)this->effectAlpha);
        gDPSetEnvColor(gfx++, this->mmEffectEnvColor[0], this->mmEffectEnvColor[1], this->mmEffectEnvColor[2], 255);

        for (int j = 0; j < 2; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                int k = (j * 3) + i;
                EnMag_DrawEffectTexturesMm(&gfx, sDisplayEffectMaskTextures[k], sEffectTextures[k], 64, 64, 32, 32, baseX + (((i * 64) - 48) * LOGO_SCALE), baseY + (((j * 64) - 6) * LOGO_SCALE), 64 * LOGO_SCALE, 64 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE, 1, 1, k, this);
            }
        }
    }

    if ((s16)this->mainAlpha != 0)
    {
        /* Logo */
        Gfx_SetupDL_39Ptr(&gfx);
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, (s16)this->mainAlpha);
        EnMag_BlitTextureRGBA32(&gfx, 0x06000000, 0x90, 0x40, baseX, baseY + 30 * LOGO_SCALE, LOGO_SCALE);

        /* Subtitles */
        gDPSetAlphaCompare(gfx++, G_AC_NONE);
        gDPSetCombineMode(gfx++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

        if (this->mainAlpha < 100)
        {
            gDPSetRenderMode(gfx++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
        }
        else
        {
            gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        }

        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, this->mainAlpha);
        EnMag_DrawTextureI8(&gfx, (void*)0x06009680, 104, 16, baseX + 46 * LOGO_SCALE, baseY + 80 * LOGO_SCALE, 104 * LOGO_SCALE, 16 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE);

        gDPSetPrimColor(gfx++, 0, 120, 208, 102, 222, this->mainAlpha);
        EnMag_DrawTextureI8(&gfx, (void*)0x06009d00, 72, 8, baseX + 55 * LOGO_SCALE, baseY + 28 * LOGO_SCALE, 72 * LOGO_SCALE, 8 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE);
        EnMag_DrawTextureI8(&gfx, (void*)0x06009000, 104, 16, baseX + 46 * LOGO_SCALE, baseY + 80 * LOGO_SCALE, 104 * LOGO_SCALE, 16 * LOGO_SCALE, 1024 * LOGO_SCALE_REVERSE, 1024 * LOGO_SCALE_REVERSE);
    }
    *gfxP = gfx;
}

void EnMag_DrawInner(Actor_EnMag* this, GameState_Play* play, Gfx** gfxP)
{
    static const u32 kPressStartColor = 0xff1e1e;
    static s16 textAlpha = 0;
    static s16 textFadeDirection = 0;
    static s16 textFadeTimer = 0;

    this->effectScroll -= 2;

    EnMag_DrawLogoOot(this, play, gfxP, 90, 100);
    EnMag_DrawLogoMm(this, play, gfxP, 240, 100);

    textAlpha = textFadeTimer * 10;
    if (textAlpha >= 255)
        textAlpha = 255;

    if (gSaveContext.fileIndex == 0xfedc)
    {
        EnMag_DrawTextWithShadow(this, gfxP, "NO CONTROLLER", YREG(19), YREG(10) + 171, 0x64ffff, textAlpha);
    }
    else if (this->copyrightAlpha >= 200.0f)
    {
        EnMag_DrawTextWithShadow(this, gfxP, "PRESS START", YREG(7), YREG(10) + 171, kPressStartColor, textAlpha);
    }

    if (textFadeDirection != 0) {
        if (--textFadeTimer == 0) {
            textFadeDirection = 0;
        }
    } else {
        if (++textFadeTimer >= 26) {
            textFadeDirection = 1;
        }
    }

    /* Copyright */
    if ((s16)this->copyrightAlpha != 0)
    {
        Gfx_SetupDL_39Ptr(gfxP);
        gDPSetAlphaCompare((*gfxP)++, G_AC_NONE);
        gDPSetCombineMode((*gfxP)++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor((*gfxP)++, 0, 0, (s16)this->copyrightAlpha, (s16)this->copyrightAlpha, (s16)this->copyrightAlpha, (s16)this->copyrightAlpha);

        gSPSegment((*gfxP)++, 0x06, play->objectCtx.status[this->actor.objectSlot].segment);
        EnMag_DrawTextureIA8(gfxP, (void*)0x06019300, 128, 16, 94, 198, 128, 16, 1024, 1024);

        gSPSegment((*gfxP)++, 0x06, this->mmMagData);
        EnMag_DrawTextureIA8(gfxP, (void*)0x06010f40, 128, 16, 94, 210, 128, 16, 1024, 1024);
    }
}

void EnMag_Draw(Actor_EnMag* this, GameState_Play* play)
{
    Gfx* gfx;
    Gfx* gfxRef;
    void* mmMag;

    /* Ensure the MM mag object is loaded too! */
    mmMag = comboGetObject(0x115 | MASK_FOREIGN_OBJECT);
    if (!mmMag)
        return;
    this->mmMagData = mmMag;

    OPEN_DISPS(play->gs.gfx);

    gfxRef = POLY_OPA_DISP;
    gfx = Gfx_Open(gfxRef);
    gSPDisplayList(OVERLAY_DISP++, gfx);

    EnMag_DrawInner(this, play, &gfx);

    gSPEndDisplayList(gfx++);
    Gfx_Close(gfxRef, gfx);
    POLY_OPA_DISP = gfx;

    CLOSE_DISPS();
}

ActorProfile En_Mag_InitVars = {
    AC_EN_MAG,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_MAG,
    sizeof(Actor_EnMag),
    (ActorFunc)EnMag_Init,
    (ActorFunc)EnMag_Destroy,
    (ActorFunc)EnMag_Update,
    (ActorFunc)EnMag_Draw,
};

OVL_ACTOR_INFO(AC_EN_MAG, En_Mag_InitVars);
