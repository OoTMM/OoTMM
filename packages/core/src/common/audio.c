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

#if defined(GAME_OOT)
# define CUSTOM_SEQ_BANKS_NATIVE_VROM   CUSTOM_SEQ_BANKS_OOT_VROM
# define CUSTOM_SEQ_BANKS_FOREIGN_VROM  CUSTOM_SEQ_TABLE_MM_VROM
# define CUSTOM_SEQ_TABLE_NATIVE_VROM   CUSTOM_SEQ_TABLE_OOT_VROM
# define CUSTOM_SEQ_TABLE_FOREIGN_VROM  CUSTOM_SEQ_TABLE_MM_VROM
#endif

#if defined(GAME_MM)
# define CUSTOM_SEQ_BANKS_NATIVE_VROM  CUSTOM_SEQ_BANKS_MM_VROM
# define CUSTOM_SEQ_BANKS_FOREIGN_VROM CUSTOM_SEQ_TABLE_OOT_VROM
# define CUSTOM_SEQ_TABLE_NATIVE_VROM  CUSTOM_SEQ_TABLE_MM_VROM
# define CUSTOM_SEQ_TABLE_FOREIGN_VROM CUSTOM_SEQ_TABLE_OOT_VROM
#endif

static u8 sDisplayMusicNames;
COSMETIC(MUSIC_NAMES, sDisplayMusicNames);
ALIGNED(16) static char sAudioNameBuffer[49];
static u8 sAudioNameTTL;
static u16 sAudioNameSeq = 0xffff;

ALIGNED(16) CustomAudioTable gCustomAudioTable = { { 256 } };
u8 gCustomAudioSeqBanks[256 * 2 + 256 * 2 + 16];

#if defined(GAME_OOT)
static u8 bankForeignToNative(u8 bankId)
{
    switch (bankId)
    {
    case 0x03: return 0x03;
    case 0x04: return 0x05;
    case 0x05: return 0x08;
    case 0x06: return 0x09;
    case 0x07: return 0x0D;
    case 0x08: return 0x11;
    case 0x09: return 0x12;
    case 0x0A: return 0x14;
    case 0x0B: return 0x15;
    case 0x0C: return 0x16;
    case 0x0D: return 0x1C;
    case 0x0E: return 0x1D;
    case 0x0F: return 0x23;
    case 0x10: return 0x24;
    }

    return 0x03;
}
#endif

#if defined(GAME_MM)
static u8 bankForeignToNative(u8 bankId)
{
    switch (bankId)
    {
    case 0x03: return 0x03;
    case 0x05: return 0x04;
    case 0x08: return 0x05;
    case 0x09: return 0x06;
    case 0x0D: return 0x07;
    case 0x11: return 0x08;
    case 0x12: return 0x09;
    case 0x14: return 0x0A;
    case 0x15: return 0x0B;
    case 0x16: return 0x0C;
    case 0x1C: return 0x0D;
    case 0x1D: return 0x0E;
    case 0x23: return 0x0F;
    case 0x24: return 0x10;
    }

    return 0x03;
}
#endif

void Audio_InitCustom(void)
{
    ALIGNED(16) u8 banks[0x80];
    u16* tmp;

    /* Build the custom bank table */
    for (int i = 0; i < 256; ++i)
    {
        tmp = (u16*)(gCustomAudioSeqBanks + i * 2);
        *tmp = 512 + i * 2;
    }

    /* Load native banks */
    LoadFile(banks, CUSTOM_SEQ_BANKS_NATIVE_VROM, 0x80);
    for (int i = 0; i < 0x80; ++i)
    {
        gCustomAudioSeqBanks[512 + i * 2 + 0] = 1;
        gCustomAudioSeqBanks[512 + i * 2 + 1] = banks[i];
    }

    /* Load foreign banks */
    LoadFile(banks, CUSTOM_SEQ_BANKS_FOREIGN_VROM, 0x80);
    for (int i = 0; i < 0x80; ++i)
    {
        gCustomAudioSeqBanks[512 + 0x80 * 2 + i * 2 + 0] = 1;
        gCustomAudioSeqBanks[512 + 0x80 * 2 + i * 2 + 1] = bankForeignToNative(banks[i]);
    }

    /* Sequence 0 is special */
    tmp = (u16*)(gCustomAudioSeqBanks + 0x00 * 2);
    *tmp = 1024;
    gCustomAudioSeqBanks[1024] = 2;
    gCustomAudioSeqBanks[1025] = 1;
    gCustomAudioSeqBanks[1026] = 0;

    LoadFile(gCustomAudioTable.entries + 0x00, CUSTOM_SEQ_TABLE_NATIVE_VROM, 0x80 * sizeof(AudioTableEntry));
    LoadFile(gCustomAudioTable.entries + 0x80, CUSTOM_SEQ_TABLE_FOREIGN_VROM, 0x80 * sizeof(AudioTableEntry));

    /* Resolve virtual addresses */
    for (int i = 0; i < 256; ++i)
    {
        DmaEntry dmaEntry;
        AudioTableEntry* e;

        e = gCustomAudioTable.entries + i;
        if (e->romAddr >= 0x08000000)
        {
            if (comboDmaLookup(&dmaEntry, e->romAddr))
            {
                e->romAddr = dmaEntry.pstart + (e->romAddr - dmaEntry.vstart);
            }
        }
    }
}

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
    gDPSetCycleType(POLY_OPA_DISP++, G_CYC_1CYCLE);
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
    static u8 sIsInitialized;
    DmaEntry e;

    if (!sDisplayMusicNames)
        return;

    if (!sIsInitialized)
    {
        sIsInitialized = 1;
        if (!comboDmaLookup(&e, COMBO_VROM_MUSIC_NAMES))
        {
            sDisplayMusicNames = 0;
            return;
        }
    }

    /* Update the music name */
    Audio_UpdateMusicName();

    /* Check for no music name */
    if (!sAudioNameTTL || sAudioNameBuffer[0] == 0)
        return;

    /* Draw the music name */
    Audio_DrawMusicName(play);
}
