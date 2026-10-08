#include <combo.h>
#include <combo/audio.h>
#include <combo/dma.h>
#include <combo/global.h>
#include <combo/custom.h>
#if defined(GAME_MM)
#include <combo/mm/sequence.h>
#endif

#if defined(GAME_OOT)
# define MUSIC_NAMES_OFFSET 0
#else
# define MUSIC_NAMES_OFFSET 256
#endif

#if defined(GAME_OOT)
# define CUSTOM_SEQ_BANKS_NATIVE_VROM       CUSTOM_SEQ_BANKS_OOT_VROM
# define CUSTOM_SEQ_BANKS_FOREIGN_VROM      CUSTOM_SEQ_BANKS_MM_VROM
# define CUSTOM_SEQ_TABLE_NATIVE_VROM       CUSTOM_SEQ_TABLE_OOT_VROM
# define CUSTOM_SEQ_TABLE_FOREIGN_VROM      CUSTOM_SEQ_TABLE_MM_VROM
# define CUSTOM_AUDIO_TABLE_NATIVE_VROM     CUSTOM_AUDIO_TABLE_OOT_VROM
# define CUSTOM_AUDIO_TABLE_FOREIGN_VROM    CUSTOM_AUDIO_TABLE_MM_VROM
# define CUSTOM_BANK_TABLE_NATIVE_VROM      CUSTOM_BANK_TABLE_OOT_VROM
# define CUSTOM_BANK_TABLE_FOREIGN_VROM     CUSTOM_BANK_TABLE_MM_VROM
#endif

#if defined(GAME_MM)
# define CUSTOM_SEQ_BANKS_NATIVE_VROM       CUSTOM_SEQ_BANKS_MM_VROM
# define CUSTOM_SEQ_BANKS_FOREIGN_VROM      CUSTOM_SEQ_BANKS_OOT_VROM
# define CUSTOM_SEQ_TABLE_NATIVE_VROM       CUSTOM_SEQ_TABLE_MM_VROM
# define CUSTOM_SEQ_TABLE_FOREIGN_VROM      CUSTOM_SEQ_TABLE_OOT_VROM
# define CUSTOM_AUDIO_TABLE_NATIVE_VROM     CUSTOM_AUDIO_TABLE_MM_VROM
# define CUSTOM_AUDIO_TABLE_FOREIGN_VROM    CUSTOM_AUDIO_TABLE_OOT_VROM
# define CUSTOM_BANK_TABLE_NATIVE_VROM      CUSTOM_BANK_TABLE_MM_VROM
# define CUSTOM_BANK_TABLE_FOREIGN_VROM     CUSTOM_BANK_TABLE_OOT_VROM
#endif

static u8 sMusicCustom;
EXPORT_SYMBOL(MUSIC_CUSTOM, sMusicCustom);
static u8 sDisplayMusicNames;
EXPORT_SYMBOL(MUSIC_NAMES, sDisplayMusicNames);
ALIGNED(16) static char sAudioNameBuffer[49];
static u8 sAudioNameTTL;
static u16 sAudioNameSeq = 0xffff;

typedef struct
{
    AudioTable* seq;
    AudioTable* bank;
    AudioTable* audio;
    u8*         seqBanks;
}
CustomAudioTables;

u8 gCustomAudioSeqBanks[256 * 2 + 256 * 2 + 16];
CustomAudioTables gCustomAudioTables = { NULL, NULL, NULL, gCustomAudioSeqBanks };

u8 gAudioLoadStatusSeq[0x100];
u8 gAudioLoadStatusFont[0xf0];
extern u8 gAudioLoadStatusFontSample[0x30];

void AudioHeap_ResetLoadStatus(void)
{
    for (int i = 0; i < ARRAY_COUNT(gAudioLoadStatusSeq); ++i)
    {
        if (gAudioLoadStatusSeq[i] != 5)
            gAudioLoadStatusSeq[i] = 0;
    }

    for (int i = 0; i < ARRAY_COUNT(gAudioLoadStatusFont); ++i)
    {
        if (gAudioLoadStatusFont[i] != 5)
            gAudioLoadStatusFont[i] = 0;
    }

    for (int i = 0; i < ARRAY_COUNT(gAudioLoadStatusFontSample); ++i)
    {
        if (gAudioLoadStatusFontSample[i] != 5)
            gAudioLoadStatusFontSample[i] = 0;
    }
}

void AudioLoad_SetFontLoadStatus(int fontId, int status)
{
    if (fontId != 0xff && gAudioLoadStatusFont[fontId] != 5)
        gAudioLoadStatusFont[fontId] = (u8)status;
}

void AudioLoad_SetSeqLoadStatus(int seqId, int status)
{
    if (seqId != 0xff && gAudioLoadStatusSeq[seqId] != 5)
        gAudioLoadStatusSeq[seqId] = (u8)status;
}

static AudioTable* allocTable(int count)
{
    AudioTable* table;

    table = malloc(sizeof(AudioTableHeader) + count * sizeof(AudioTableEntry));
    memset(table, 0, sizeof(AudioTableHeader) + count * sizeof(AudioTableEntry));
    table->header.count = count;

    return table;
}

static u8 foreignSampleTable(u8 sampleTableId)
{
    if (sampleTableId == 0xff)
        return 0xff;
    return sampleTableId + 8;
}

static u8 customSampleTable(u8 sampleTableId)
{
    if (sampleTableId == 0xff)
        return 0xff;

#if defined(GAME_MM)
    sampleTableId ^= 0x08;
#endif

    return sampleTableId;
}

static u8 foreignBank(u8 bankId)
{
    if (bankId < 2)
        return bankId;
    if (bankId >= 0x60)
        return bankId;
    if (bankId >= 0x30)
        return bankId - 0x30;
    else
        return bankId + 0x30;
}

void AudioCustom_Init(void)
{
    AudioTableEntry* e;
    ALIGNED(16) u8 banks[0x80];
    u16* tmp;

    /* Init the tables */
    gCustomAudioTables.seq = allocTable(0x100);
    gCustomAudioTables.bank = allocTable(0xf0);
    gCustomAudioTables.audio = allocTable(0x10);

    /* Load the tables */
    LoadFile(gCustomAudioTables.seq->entries + 0x00, CUSTOM_SEQ_TABLE_NATIVE_VROM, 0x80 * sizeof(AudioTableEntry));
    LoadFile(gCustomAudioTables.seq->entries + 0x80, CUSTOM_SEQ_TABLE_FOREIGN_VROM, 0x80 * sizeof(AudioTableEntry));
    LoadFile(gCustomAudioTables.bank->entries + 0x00, CUSTOM_BANK_TABLE_NATIVE_VROM, 0x30 * sizeof(AudioTableEntry));
    LoadFile(gCustomAudioTables.bank->entries + 0x30, CUSTOM_BANK_TABLE_FOREIGN_VROM, 0x30 * sizeof(AudioTableEntry));
    LoadFile(gCustomAudioTables.bank->entries + 0x60, CUSTOM_BANK_TABLE_CUSTOM_VROM, (0xf0 - 0x60) * sizeof(AudioTableEntry));
    LoadFile(gCustomAudioTables.audio->entries + 0, CUSTOM_AUDIO_TABLE_NATIVE_VROM, 8 * sizeof(AudioTableEntry));
    LoadFile(gCustomAudioTables.audio->entries + 8, CUSTOM_AUDIO_TABLE_FOREIGN_VROM, 8 * sizeof(AudioTableEntry));

    /* Fix foreign audio tables */
    for (int i = 0; i < 8; ++i)
    {
        e = gCustomAudioTables.audio->entries + 8 + i;
        if (e->medium && !e->size)
        {
            /* Alias */
            e->romAddr += 8;
        }
    }

    /* Foreign banks need to point to their correct audiotable */
    for (int i = 0; i < 0x30; ++i)
    {
        e = gCustomAudioTables.bank->entries + 0x30 + i;
        if (e->medium)
        {
            e->data[0] = foreignSampleTable(e->data[0]);
            e->data[1] = foreignSampleTable(e->data[1]);
        }
    }

    for (int i = 0; i < (0xf0 - 0x60); ++i)
    {
        e = gCustomAudioTables.bank->entries + 0x60 + i;
        if (e->medium)
        {
            e->data[0] = customSampleTable(e->data[0]);
            e->data[1] = customSampleTable(e->data[1]);
        }
    }

    /* Resolve virtual sequence addresses (for custom music) */
    for (int i = 0; i < 256; ++i)
    {
        DmaEntry dmaEntry;
        AudioTableEntry* e;

        e = gCustomAudioTables.seq->entries + i;
        if (e->romAddr >= 0x08000000)
        {
            if (comboDmaLookup(&dmaEntry, e->romAddr))
            {
                e->romAddr = dmaEntry.pstart + (e->romAddr - dmaEntry.vstart);
            }
        }
    }

    /* Resolve virtual bank addresses (for custom music) */
    for (int i = 0; i < 0xf0; ++i)
    {
        DmaEntry dmaEntry;
        AudioTableEntry* e;

        e = gCustomAudioTables.bank->entries + i;
        if (e->size && e->romAddr >= 0x08000000)
        {
            if (comboDmaLookup(&dmaEntry, e->romAddr))
            {
                e->romAddr = dmaEntry.pstart + (e->romAddr - dmaEntry.vstart);
            }
        }
    }

    /* Resolve virtual bank addresses (for Voice data) */
    for (int i = 0; i < 0x10; ++i)
    {
        DmaEntry dmaEntry;
        AudioTableEntry* e;
        e = gCustomAudioTables.audio->entries + i;
        if (e->size && e->romAddr >= 0x08000000)
            if (comboDmaLookup(&dmaEntry, e->romAddr))
                e->romAddr = dmaEntry.pstart + (e->romAddr - dmaEntry.vstart);
    }

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
        gCustomAudioSeqBanks[512 + 0x80 * 2 + i * 2 + 1] = foreignBank(banks[i]);
    }

    /* Sequence 0 is special */
    tmp = (u16*)(gCustomAudioSeqBanks + 0x00 * 2);
    *tmp = 1024;
    gCustomAudioSeqBanks[1024] = 2;
    gCustomAudioSeqBanks[1025] = 1;
    gCustomAudioSeqBanks[1026] = 0;
}

static void Audio_UpdateMusicName(PlayState* play)
{
    u16 currentSeq;

    currentSeq = Audio_GetActiveSeqId(0);
    if (currentSeq == 0xffff)
    {
        /* Keep the last known name while paused: seq lookup often returns invalid then. */
        if (play->pauseCtx.state != 0 && sAudioNameBuffer[0] != 0)
            return;

        sAudioNameSeq = 0xffff;
        sAudioNameBuffer[0] = 0;
        sAudioNameTTL = 0;
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
    OPEN_DISPS(gPlay->state.gfxCtx);
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

static void Audio_DrawMusicName(PlayState* play)
{
    u8 alpha;
    GraphicsContext* ctx;
    Gfx* opaOriginal;
    Gfx* opaTarget;
    Gfx* opaNew;

    ctx = gPlay->state.gfxCtx;
    opaOriginal = ctx->polyOpa.append;
    ctx->polyOpa.append++;
    opaTarget = ctx->polyOpa.append;

    if (sAudioNameTTL >= 10)
        alpha = 0xff;
    else
        alpha = sAudioNameTTL * 0x19;

    OPEN_DISPS(gPlay->state.gfxCtx);
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

void Audio_DisplayMusicName(PlayState* play)
{
    static u8 sIsInitialized;
    DmaEntry e;

    if (!sDisplayMusicNames)
        return;

#if defined(GAME_OOT)
    /* Don't show music name on the Song of Soaring map screen */
    if (play->pauseCtx.state >= PAUSE_STATE_OWLWARP_0 && play->pauseCtx.state <= PAUSE_STATE_OWLWARP_6)
        return;
#elif defined(GAME_MM)
    /* Don't show music name on the Song of Soaring map screen */
    if (play->pauseCtx.state >= 0x13 && play->pauseCtx.state <= 0x19)
        return;
#endif

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
    Audio_UpdateMusicName(play);

    /* Show music name while paused */
    if (play->pauseCtx.state != 0 && sAudioNameSeq != 0xffff && sAudioNameBuffer[0] != 0)
    {
        sAudioNameTTL = 60;
    }

    /* Check for no music name */
    if (!sAudioNameTTL || sAudioNameBuffer[0] == 0)
        return;

    /* Draw the music name */
    Audio_DrawMusicName(play);
}

#if defined(GAME_OOT)
void Audio_UpdateMalonSingingWrapper(f32 dist, u16 seqId)
{
    if (sMusicCustom)
        return;
    Audio_UpdateMalonSinging(dist, seqId);
}

PATCH_CALL(0x809ce6a0, Audio_UpdateMalonSingingWrapper);
PATCH_CALL(0x809f18a0, Audio_UpdateMalonSingingWrapper);
PATCH_CALL(0x80b771b0, Audio_UpdateMalonSingingWrapper);

void Audio_ToggleMalonSingingWrapper(u8 malonSingingDisabled)
{
    if (sMusicCustom)
        return;
    Audio_ToggleMalonSinging(malonSingingDisabled);
}

PATCH_CALL(0x809cddf4, Audio_ToggleMalonSingingWrapper);
PATCH_CALL(0x809cde18, Audio_ToggleMalonSingingWrapper);
PATCH_CALL(0x809f0f44, Audio_ToggleMalonSingingWrapper);
PATCH_CALL(0x809f0f68, Audio_ToggleMalonSingingWrapper);

#endif

static u32 sAudioLoadingSequenceId = 0xffffffffu;

void Audio_SetLoadingSequenceId(u32 seqId)
{
    sAudioLoadingSequenceId = seqId;
}

void Audio_ClearLoadingSequenceId(void)
{
    sAudioLoadingSequenceId = 0xffffffffu;
}

s8 Audio_GetLoadCachePolicy(s8 defaultPolicy, u32 tableType)
{
    AudioTable* seqTable;
    if (tableType != 1 ||
        defaultPolicy == 0 ||
        sAudioLoadingSequenceId == 0xffffffffu)
    {
        return defaultPolicy;
    }

    seqTable = gCustomAudioTables.seq;

    if (seqTable == NULL ||
        sAudioLoadingSequenceId >= seqTable->header.count)
    {
        return defaultPolicy;
    }
    return seqTable->entries[sAudioLoadingSequenceId].cachePolicy;
}

#if defined(GAME_MM)

static u8 sSpatialOverridesFinalHours;

static int Audio_CanSpatialOverrideFinalHours(void)
{
    u16 seqId;

    if (sAudioCutsceneFlag || !Environment_IsFinalHours(NULL) || func_800FE5D0(NULL))
        return 0;

    seqId = AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN);
    return (seqId & 0xff) != NA_BGM_SONG_OF_SOARING &&
           AudioSeq_IsSeqCmdNotQueued(NA_BGM_SONG_OF_SOARING, 0xff0000ffu);
}

static void Audio_BeginSpatialFinalHoursOverride(void)
{
    if (sSpatialOverridesFinalHours || !Audio_CanSpatialOverrideFinalHours())
        return;

    sSpatialOverridesFinalHours = 1;
    AudioSeq_StopSequence(SEQ_PLAYER_BGM_MAIN, 0);
}

static void Audio_RestoreFinalHoursAfterSpatial(void)
{
    if (!sSpatialOverridesFinalHours || sSpatialSeqFadeTimer || sSpatialSubBgmFadeTimer)
        return;

    sSpatialOverridesFinalHours = 0;

    if (AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_SUB) != NA_BGM_DISABLED || sSpatialSeqIsActive[SEQ_PLAYER_BGM_SUB])
        AudioSeq_StopSequence(SEQ_PLAYER_BGM_SUB, 0);

    sSpatialSeqIsActive[SEQ_PLAYER_BGM_SUB] = 0;
    sSpatialSeqFlags = 0;
    sSpatialSeqSeqId = NA_BGM_GENERAL_SFX;

    AudioSeq_SetVolumeScale(SEQ_PLAYER_BGM_MAIN, VOL_SCALE_INDEX_BGM_SUB, 0x7f, 0);
    Audio_SplitBgmChannels(0);

    if (Environment_IsFinalHours(NULL) && !func_800FE5D0(NULL)) {
        Audio_StartSceneSequence(NA_BGM_FINAL_HOURS);
        AudioSeq_QueueSeqCmd(0x70040002u);
        sRequestedSceneSeqId = NA_BGM_FINAL_HOURS;
    }
}

static void Audio_EndSpatialFinalHoursOverride(void)
{
    AudioSeq_StopSequence(SEQ_PLAYER_BGM_SUB, 0);
    sSpatialSeqIsActive[SEQ_PLAYER_BGM_SUB] = 0;
    Audio_RestoreFinalHoursAfterSpatial();
}

void Audio_PlaySceneSequence_Hook(u16 seqId, u8 dayMinusOne)
{
    if (seqId == NA_BGM_FINAL_HOURS && sSpatialOverridesFinalHours) {
        sRequestedSceneSeqId = seqId;
        return;
    }

    if (seqId != NA_BGM_FINAL_HOURS)
        sSpatialOverridesFinalHours = 0;

    if (sRequestedSceneSeqId == seqId)
        return;

    if (seqId == NA_BGM_AMBIENCE) {
        Audio_PlayAmbience(8);
    } else if (seqId != NA_BGM_FINAL_HOURS || sPrevMainBgmSeqId == NA_BGM_DISABLED) {
        if (seqId != NA_BGM_FINAL_HOURS && func_800FE5D0(NULL) &&
            AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN) == NA_BGM_FINAL_HOURS)
            AudioSeq_QueueSeqCmd(0x100000ffu);

        Audio_StartSceneSequence(seqId);
        AudioSeq_QueueSeqCmd(0x70040000u | dayMinusOne);
    }

    sRequestedSceneSeqId = seqId;
}

void Audio_UpdateSubBgmAtPos_Hook(void)
{
    if (!sSpatialSubBgmFadeTimer)
        return;

    if (Audio_CanSpatialOverrideFinalHours())
        Audio_BeginSpatialFinalHoursOverride();

    if (sSpatialSeqFlags & 2)
        Audio_StartSubBgmAtPos(SEQ_PLAYER_BGM_SUB, &sSpatialSeqNoFilterPos, sSpatialSeqSeqId, 0x20, 100.0f, sSpatialSeqMaxDist, 1.0f);
    else
        Audio_StartSubBgmAtPos(SEQ_PLAYER_BGM_SUB, &sSpatialSeqFilterPos, sSpatialSeqSeqId, 0x28, 100.0f, sSpatialSeqMaxDist, 1.0f);

    sSpatialSubBgmFadeTimer--;

    if (!sSpatialSubBgmFadeTimer) {
        if (sSpatialOverridesFinalHours)
            Audio_EndSpatialFinalHoursOverride();
        else
            Audio_StopSequenceAtPos(SEQ_PLAYER_BGM_SUB, 10);
    }

    sSpatialSeqFlags = 0;
}

void Audio_UpdateSequenceAtPos_Hook(void)
{
    u16 mainBgmSeqId;
    u8 volumeFadeTimer;

    if (!sSpatialSeqFadeTimer || sAudioPauseState != 0)
        return;

    mainBgmSeqId = AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN);

    if (sSpatialSeqSeqId == NA_BGM_GENERAL_SFX || mainBgmSeqId == NA_BGM_SONG_OF_SOARING) {
        volumeFadeTimer = 10;

        if (mainBgmSeqId == NA_BGM_SONG_OF_SOARING) {
            sSpatialSeqFadeTimer = 0;
            volumeFadeTimer = 1;
        } else if (sSpatialSeqFadeTimer < 128) {
            sSpatialSeqFadeTimer--;
        }

        if (!sSpatialSeqFadeTimer) {
            if (sSpatialSeqPlayerIndex == SEQ_PLAYER_BGM_SUB && sSpatialOverridesFinalHours) {
                Audio_EndSpatialFinalHoursOverride();
            } else {
                Audio_StopSequenceAtPos(sSpatialSeqPlayerIndex, volumeFadeTimer);
                sSpatialSeqIsActive[sSpatialSeqPlayerIndex] = 0;
            }
        }
    } else {
        if (sSpatialSeqPlayerIndex == SEQ_PLAYER_BGM_SUB && Audio_CanSpatialOverrideFinalHours()) {
            Audio_BeginSpatialFinalHoursOverride();
            mainBgmSeqId = AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN);
        }

        if (sSpatialSeqPlayerIndex == SEQ_PLAYER_BGM_MAIN && mainBgmSeqId == NA_BGM_FINAL_HOURS) {
            Audio_StopSequenceAtPos(sSpatialSeqPlayerIndex, 10);
            sSpatialSeqIsActive[sSpatialSeqPlayerIndex] = 0;
            return;
        }

        Audio_StartSubBgmAtPos(sSpatialSeqPlayerIndex, &sSpatialSeqFilterPos, sSpatialSeqSeqId, 0x20, 200.0f, sSpatialSeqMaxDist, 1.0f);

        if (!sSpatialSeqIsActive[sSpatialSeqPlayerIndex]) {
            sSpatialSeqFadeTimer = 0;

            if (sSpatialSeqPlayerIndex == SEQ_PLAYER_BGM_SUB && sSpatialOverridesFinalHours)
                Audio_RestoreFinalHoursAfterSpatial();
        }
    }

    if (sSpatialSeqFadeTimer < 128)
        sSpatialSeqSeqId = NA_BGM_GENERAL_SFX;
}

void Audio_PlaySequenceAtPos_Hook(u8 seqPlayerIndex, Vec3f* pos, u16 seqId, f32 maxDist)
{
    u16 mainBgmSeqId = AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN);

    if (sAudioCutsceneFlag || (mainBgmSeqId & 0xff) == NA_BGM_SONG_OF_SOARING || gAudioSpecId == 0xC || pos == NULL)
        return;

    if (seqPlayerIndex == SEQ_PLAYER_BGM_SUB && Audio_CanSpatialOverrideFinalHours()) {
        Audio_BeginSpatialFinalHoursOverride();
        mainBgmSeqId = AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN);
    }

    if (sSpatialSeqPlayerIndex == SEQ_PLAYER_BGM_MAIN && mainBgmSeqId == NA_BGM_FINAL_HOURS)
        return;

    sSpatialSeqFilterPos.x = pos->x;
    sSpatialSeqFilterPos.y = pos->y;
    sSpatialSeqFilterPos.z = pos->z;
    sSpatialSeqMaxDist = maxDist;
    sSpatialSeqFadeTimer = 2;
    sSpatialSeqSeqId = seqId & 0xff;
    sSpatialSeqPlayerIndex = seqPlayerIndex;
}

#endif