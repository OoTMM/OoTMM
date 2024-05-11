#include <combo.h>
#include <combo/audio.h>
#include <combo/dma.h>
#include <combo/global.h>
#include <combo/custom.h>

#if defined(GAME_OOT)
# define MUSIC_NAMES_OFFSET 0
#else
# define MUSIC_NAMES_OFFSET 256
#endif

static u8 sDisplayMusicNames;
COSMETIC(MUSIC_NAMES, sDisplayMusicNames);
ALIGNED(16) static char sAudioNameBuffer[49];
static u8 sAudioNameTTL;
static u16 sAudioNameSeq = 0xffff;

static void Audio_UpdateMusicName(void)
{
    u16 currentSeq;

    currentSeq = Audio_GetActiveSeqId(0);
    if (currentSeq == 0xffff)
    {
        sAudioNameSeq = 0xffff;
        return;
    }

    /* Same sequence */
    if (currentSeq == sAudioNameSeq)
    {
        if (sAudioNameTTL)
            --sAudioNameTTL;
        return;
    }

    /* Different sequence, need to load the name */
    sAudioNameSeq = currentSeq;
    if (comboDmaLoadFilePartial(sAudioNameBuffer, COMBO_VROM_MUSIC_NAMES, (currentSeq + MUSIC_NAMES_OFFSET) * 48, 48))
    {
        sAudioNameTTL = 60;
    }
    else
    {
        sAudioNameBuffer[0] = 0;
        sAudioNameTTL = 0;
    }
}

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

static void drawChar(int x, int y, char c)
{
    OPEN_DISPS(gPlay->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x09, (char*)g.customKeep + CUSTOM_KEEP_FONT + ((c - ' ') * 0x30));
    gSPDisplayList(POLY_OPA_DISP++, kDlistLoadIA4_8x12);
    gSPTextureRectangle(
        POLY_OPA_DISP++,
        x * 4 * kScale, y * 4 * kScale,
        (x + 8) * 4 * kScale, (y + 12) * 4 * kScale,
        0,
        0, 0,
        (1 << 10) * kScaleInv, (1 << 10) * kScaleInv
    );
    CLOSE_DISPS();
}

static void drawStr(int x, int y, const char* str)
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
        drawChar(x + i * 7, y, c);
        i++;
    }
}

static void Audio_DrawMusicName(GameState_Play* play)
{
    u8 alpha;
    GfxContext* ctx;
    Gfx* opaOriginal;
    Gfx* opaTarget;
    Gfx* opaNew;

    ctx = gPlay->gs.gfx;
    opaOriginal = ctx->polyOpa.append;
    ctx->polyOpa.append++;
    opaTarget = ctx->polyOpa.append;

    if (sAudioNameTTL >= 10)
        alpha = 0xff;
    else
        alpha = sAudioNameTTL * 0x19;

    OPEN_DISPS(gPlay->gs.gfx);
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetRenderMode(POLY_OPA_DISP++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0xff, 0xff, 0xff, alpha);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0);
    CLOSE_DISPS();
    drawStr(10, 10, sAudioNameBuffer);
    gSPEndDisplayList(ctx->polyOpa.append++);

    opaNew = ctx->polyOpa.append;
    gSPBranchList(opaOriginal, opaNew);
    gSPDisplayList(ctx->overlay.append++, opaTarget);
}

void Audio_DisplayMusicName(GameState_Play* play)
{
    if (!sDisplayMusicNames)
        return;

    /* Update the music name */
    Audio_UpdateMusicName();

    /* Check for no music name */
    if (!sAudioNameTTL || sAudioNameBuffer[0] == 0)
        return;

    /* Draw the music name */
    Audio_DrawMusicName(play);
}
