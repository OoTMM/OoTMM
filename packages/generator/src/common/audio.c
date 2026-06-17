#include <combo.h>
#include <combo/audio.h>
#include <combo/dma.h>
#include <combo/global.h>
#include <combo/custom.h>
#include <combo/common/scene.h>

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
    table->header.numEntries = count;

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

static MusicState sMusicState = {};

#if defined (GAME_MM)
static const u8* sAudioBaseFilter = (u8*)0x801D66E0;
#else
static const u8* sAudioBaseFilter = (u8*)0x8010194c;
#endif

static u16 CalculateCurrentState() {
    u16 state;
    Player* player = GET_PLAYER(gPlay);
    if (player) {
        s8 form;
#if defined (GAME_MM)
        form = player->transformation;
#else
        form = player->currentTunic;
        if (form == 0) {
            form = gSave.age == AGE_ADULT ? 4 : 3;
            if (player->currentSwordItemId == ITEM_SWORD_BIGGORON) {
                form = 0;
            }
        }
#endif
        state = 1 << form;

        if (gSaveContext.save.isNight) {
            state = sMusicState.sequenceData.cumulativeStates.night ? state | SEQUENCE_PLAY_STATE_NIGHT : SEQUENCE_PLAY_STATE_NIGHT;
        } else {
            state = sMusicState.sequenceData.cumulativeStates.day ? state | SEQUENCE_PLAY_STATE_DAY : SEQUENCE_PLAY_STATE_DAY;
        }
        if (!sMusicState.isIndoors && !sMusicState.isInCave) {
            state = sMusicState.sequenceData.cumulativeStates.outdoors ? state | SEQUENCE_PLAY_STATE_OUTDOORS : SEQUENCE_PLAY_STATE_OUTDOORS;
        }
        if (sMusicState.isIndoors) {
            state = sMusicState.sequenceData.cumulativeStates.indoors ? state | SEQUENCE_PLAY_STATE_INDOORS : SEQUENCE_PLAY_STATE_INDOORS;
        }
        if (sMusicState.isInCave) {
            state = sMusicState.sequenceData.cumulativeStates.cave ? state | SEQUENCE_PLAY_STATE_CAVE : SEQUENCE_PLAY_STATE_CAVE;
        }
        if (player->stateFlags1 & PLAYER_ACTOR_STATE_EPONA) {
            state = sMusicState.sequenceData.cumulativeStates.epona ? state | SEQUENCE_PLAY_STATE_EPONA : SEQUENCE_PLAY_STATE_EPONA;
        }
        if (player->stateFlags1 & PLAYER_ACTOR_STATE_WATER
#if defined (GAME_MM)
            || player->stateFlags3 & PLAYER_STATE3_MM_8000
#endif
            || *sAudioBaseFilter == 0x20) {
            state = sMusicState.sequenceData.cumulativeStates.swimming ? state | SEQUENCE_PLAY_STATE_SWIM : SEQUENCE_PLAY_STATE_SWIM;
        }
#if defined (GAME_MM)
        if (player->stateFlags3 & PLAYER_STATE3_MM_80000) {
            state = sMusicState.sequenceData.cumulativeStates.spikeRolling ? state | SEQUENCE_PLAY_STATE_SPIKE_ROLLING : SEQUENCE_PLAY_STATE_SPIKE_ROLLING;
        }
        if (gPlay->actorCtx.targetContext.nearbyEnemy) {
#else
        if (gPlay->actorCtx.targetCtx.bgmEnemy) {
#endif
            state = sMusicState.sequenceData.cumulativeStates.combat ? state | SEQUENCE_PLAY_STATE_COMBAT : SEQUENCE_PLAY_STATE_COMBAT;
        }
#if defined (GAME_MM)
        if (LifeMeter_IsCritical()) {
#else
        if (Health_IsCritical()) {
#endif
            state = sMusicState.sequenceData.cumulativeStates.criticalHealth ? state | SEQUENCE_PLAY_STATE_CRITICAL_HEALTH : SEQUENCE_PLAY_STATE_CRITICAL_HEALTH;
        }
    } else if (sMusicState.currentState) {
        state = sMusicState.currentState;
    } else {
        state = 1;
    }
    return state;
}

static void ProcessChannel(u8 channelIndex, u16 stateMask) {
    SequenceChannel* sequenceChannel = gAudioCtx.seqPlayers[0].channels[channelIndex];
    if (sequenceChannel->enabled) {
        bool shouldBeMuted = false;
        if (!sequenceChannel->muted) {
            sMusicState.forceMute &= ~(1 << channelIndex);
        } else {
            shouldBeMuted = sMusicState.forceMute & (1 << channelIndex);
        }
        if (!shouldBeMuted) {
            u16 playMask = sMusicState.sequenceData.playMask[channelIndex];
            u16 formMask = playMask & ~sMusicState.sequenceData.cumulativeStates.value;
            u16 miscMask = playMask & sMusicState.sequenceData.cumulativeStates.value;
            u16 formState = stateMask & ~sMusicState.sequenceData.cumulativeStates.value;
            u16 miscState = stateMask & sMusicState.sequenceData.cumulativeStates.value;
            bool shouldPlay = (!miscMask || (miscMask & miscState)) && (formMask & formState);
            shouldBeMuted = !shouldPlay;
        }
        bool isMuted = sequenceChannel->muted;
        if (!isMuted && shouldBeMuted) {
            sequenceChannel->muted = true;
        } else if (isMuted && !shouldBeMuted) {
            sequenceChannel->muted = false;
        }
    }
}

#if defined (GAME_MM)
#define CUSTOM_FORM_MASK_TABLE_VROM CUSTOM_FORM_MASK_TABLE_MM_VROM
#else
#define CUSTOM_FORM_MASK_TABLE_VROM CUSTOM_FORM_MASK_TABLE_OOT_VROM
#endif

static void LoadMuteMask() {
    u8 sequenceId = gAudioCtx.seqPlayers[0].seqId;
    if (sMusicState.loadedSequenceId != sequenceId) {
        sMusicState.loadedSequenceId = sequenceId;

        if (!comboDmaLoadFilePartial(&sMusicState.sequenceData, CUSTOM_FORM_MASK_TABLE_VROM, sequenceId * sizeof(SequenceData), sizeof(SequenceData))) {
            bzero(&sMusicState.sequenceData, sizeof(SequenceData));
        }
    }
}

static void HandleFormChannels(PlayState* play) {
    LoadMuteMask();
    if (sMusicState.sequenceData.hasFormMask) {
        u16 state = CalculateCurrentState();
        if (sMusicState.currentState != state) {
            sMusicState.currentState = state;

            for (u8 i = 0; i < sizeof(gAudioCtx.seqPlayers[0].channels) / sizeof(SequenceChannel*); i++) {
                ProcessChannel(i, state);
            }
        }
    }
}

void Music_Update(PlayState* play) {
    HandleFormChannels(play);
}

void Music_AfterChannelInit(SequencePlayer* sequencePlayer, u8 channelIndex) {
    if (sequencePlayer == &gAudioCtx.seqPlayers[0]) {
        LoadMuteMask();
        if (sMusicState.sequenceData.hasFormMask) {
            u16 state = CalculateCurrentState();
            sMusicState.currentState = state;
            ProcessChannel(channelIndex, state);
        }
    }
}

void Music_HandleChannelMute(SequenceChannel* sequenceChannel, AudioCmd* audioCmd) {
    u8 channelIndex;
#if defined (GAME_MM)
    channelIndex = sequenceChannel->channelIndex;
#else
    channelIndex = 0xff;
    for (u8 i = 0; i < ARRAY_COUNT(sequenceChannel->seqPlayer->channels); i++) {
        if (sequenceChannel == sequenceChannel->seqPlayer->channels[i]) {
            channelIndex = i;
            break;
        }
    }
    if (channelIndex == 0xff) {
        return;
    }
#endif
    u8 shouldBeMuted = audioCmd->asSbyte;
    if (shouldBeMuted) {
        if (sMusicState.sequenceData.hasFormMask && sequenceChannel->seqPlayer == &gAudioCtx.seqPlayers[0] && !sequenceChannel->finished) {
            sMusicState.forceMute |= (1 << channelIndex);
        }
        sequenceChannel->muted = true;
    } else {
        if (sMusicState.sequenceData.hasFormMask && sequenceChannel->seqPlayer == &gAudioCtx.seqPlayers[0]) {
            sMusicState.forceMute &= ~(1 << channelIndex);
            ProcessChannel(channelIndex, sMusicState.currentState);
        } else {
            sequenceChannel->muted = false;
        }
    }
}

#if defined (GAME_MM)
#define SCE_CUTSCENE_MAP SCE_MM_CUTSCENE_MAP
#else
#define SCE_CUTSCENE_MAP SCE_OOT_CUTSCENE_MAP
#endif

u8 gNightBgm;
EXPORT_SYMBOL(NIGHT_BGM, gNightBgm);

void Scene_CommandSoundSettings(PlayState* play, SceneCmd* cmd)
{
    u8 ambienceId;
#if defined (GAME_MM)
    ambienceId = cmd->soundSettings.ambienceId;
#else
    ambienceId = cmd->soundSettings.natureAmbienceId;
#endif
    if (gNightBgm)
    {
        switch (play->sceneId)
        {
#if defined (GAME_MM)
            case SCE_MM_MOON:
            case SCE_MM_MOON_DEKU:
            case SCE_MM_MOON_GORON:
            case SCE_MM_MOON_ZORA:
            case SCE_MM_MOON_LINK:
            case SCE_MM_IKANA_GRAVEYARD:
#else
            case SCE_OOT_TEMPLE_OF_TIME_EXTERIOR_CHILD_NIGHT:
            case SCE_OOT_TEMPLE_OF_TIME_EXTERIOR_ADULT:
            case SCE_OOT_GANON_CASTLE_EXTERIOR:
            case SCE_OOT_MARKET_CHILD_NIGHT:
            case SCE_OOT_MARKET_ADULT:
            case SCE_OOT_GRAVEYARD:
#endif
                break;
            default:
                ambienceId = 0x13;
        }
    }

    sMusicState.isInCave = false;
    sMusicState.isIndoors = false;
    switch (play->sceneId) {
#if defined (GAME_MM)
        case SCE_MM_GROTTOS:
        case SCE_MM_DEKU_PLAYGROUND:
        case SCE_MM_FAIRY_FOUNTAIN:
        case SCE_MM_GORON_GRAVEYARD:
#else
        case SCE_OOT_GROTTOS:
        case SCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES:
        case SCE_OOT_FAIRY_FOUNTAIN:
        case SCE_OOT_GREAT_FAIRY_FOUNTAIN_SPELLS:
        case SCE_OOT_TOMB_REDEAD:
        case SCE_OOT_TOMB_FAIRY:
        case SCE_OOT_TOMB_ROYAL:
#endif
            sMusicState.isInCave = true;
            break;
#if defined (GAME_MM)
        case SCE_MM_POTION_SHOP:
        case SCE_MM_CURIOSITY_SHOP:
        case SCE_MM_RANCH_HOUSE_BARN:
        case SCE_MM_HONEY_DARLING:
        case SCE_MM_MAYOR_HOUSE:

        case SCE_MM_TREASURE_SHOP:
        case SCE_MM_SHOOTING_GALLERY:
        case SCE_MM_SHOOTING_GALLERY_SWAMP:
        case SCE_MM_BLACKSMITH:
        case SCE_MM_POST_OFFICE:
        case SCE_MM_LABORATORY:
        case SCE_MM_TRADING_POST:
        case SCE_MM_LOTTERY:
        case SCE_MM_FISHERMAN_HUT:
        case SCE_MM_GORON_SHOP:
        case SCE_MM_ZORA_HALL_ROOMS:
        case SCE_MM_GHOST_HUT:
        case SCE_MM_SWORDSMAN_SCHOOL:
        case SCE_MM_TOURIST_INFORMATION:
        case SCE_MM_STOCK_POT_INN:
        case SCE_MM_BOMB_SHOP:
#else
        case SCE_OOT_TREASURE_SHOP:
        case SCE_OOT_KOKIRI_KNOW_IT_ALL:
        case SCE_OOT_KOKIRI_TWINS:
        case SCE_OOT_KOKIRI_MIDO:
        case SCE_OOT_KOKIRI_SARIA:
        case SCE_OOT_CARPENTER_BOSS_HOUSE:
        case SCE_OOT_BACK_ALLEY_HOUSE:
        case SCE_OOT_BAZAAR:
        case SCE_OOT_KOKIRI_SHOP:
        case SCE_OOT_GORON_SHOP:
        case SCE_OOT_ZORA_SHOP:
        case SCE_OOT_KAKARIKO_POTION_SHOP:
        case SCE_OOT_MARKET_POTION_SHOP:
        case SCE_OOT_BOMBCHU_SHOP:
        case SCE_OOT_HAPPY_MASK_SHOP:
        case SCE_OOT_LINK_HOUSE:
        case SCE_OOT_BACK_ALLEY_HOUSE2:
        case SCE_OOT_STABLE:
        case SCE_OOT_IMPA_HOUSE:
        case SCE_OOT_LABORATORY:
        case SCE_OOT_CARPENTER_TENT:
        case SCE_OOT_GRAVEKEEPER_HUT:
        case SCE_OOT_SHOOTING_GALLERY:
        case SCE_OOT_FISHING_POND:
        case SCE_OOT_BOMBCHU_BOWLING_ALLEY:
        case SCE_OOT_RANCH_HOUSE_SILO:
        case SCE_OOT_GUARD_HOUSE:
        case SCE_OOT_GRANNY_POTION_SHOP:
        case SCE_OOT_HOUSE_OF_SKULLTULA:
#endif
            sMusicState.isIndoors = true;
            break;
    }

    play->sceneSequences.seqId = cmd->soundSettings.seqId;

#if defined (GAME_MM)
    play->sceneSequences.ambienceId = ambienceId;

    if (gSaveContext.seqId == (u8)NA_BGM_DISABLED ||

        Audio_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN) == NA_BGM_FINAL_HOURS)
    {
        Audio_SetSpec(cmd->soundSettings.specId);
    }
#else
    play->sceneSequences.natureAmbienceId = ambienceId;

    if (gSaveContext.seqId == (u8)NA_BGM_DISABLED)
    {
        SEQCMD_RESET_AUDIO_HEAP(0, cmd->soundSettings.specId);
    }
#endif
}

#if defined (GAME_MM)
PATCH_FUNC(0x801303e0, Scene_CommandSoundSettings);
#else
PATCH_FUNC(0x80082478, Scene_CommandSoundSettings);
#endif

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
