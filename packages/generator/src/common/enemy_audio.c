#include <combo.h>
#include <combo/audio.h>
#include <combo/dma.h>
#include <combo/enemy.h>
#include <combo/enemy_placement.h>
#include <combo/sfx.h>
#include <combo/sfx_source.h>

static u32 sForeignEnemyAudioVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
EXPORT_SYMBOL(FOREIGN_ENEMY_AUDIO_V2_OOT, sForeignEnemyAudioVrom);
EXPORT_SYMBOL(FOREIGN_ENEMY_AUDIO_V3_OOT, sForeignEnemyAudioVrom);
#define NATIVE_VOICE_COUNT 128
#define NATIVE_ENEMY_COUNT 499
#define NATIVE_GAME 0
#define NATIVE_SFX_PARAMS ((void**)0x8010433cu)
#else
EXPORT_SYMBOL(FOREIGN_ENEMY_AUDIO_V2_MM, sForeignEnemyAudioVrom);
EXPORT_SYMBOL(FOREIGN_ENEMY_AUDIO_V3_MM, sForeignEnemyAudioVrom);
#define NATIVE_VOICE_COUNT 400
#define NATIVE_ENEMY_COUNT 736
#define NATIVE_GAME 1
#define NATIVE_SFX_PARAMS ((void**)0x801db450u)
#endif

static u32 sForeignProgramStart;
static u32 sForeignProgramEnd;
static const u8* sForeignSfxParameters;
static u32 sForeignSfxParameterCount;
static const u8* sForeignVoiceParameters;

void* AudioCustom_AllocFontDescriptors(size_t size)
{
    if (sForeignSfxParameters)
        NATIVE_SFX_PARAMS[3] = (void*)sForeignSfxParameters;
    if (sForeignVoiceParameters)
        NATIVE_SFX_PARAMS[6] = (void*)sForeignVoiceParameters;
    return malloc(size);
}

int EnemyAudio_IsForeignProgram(const u8* sequence, const u8* pc, u8 seqId)
{
    u32 address = (u32)pc;
    u32 base = (u32)sequence;
    return seqId == 0 && sForeignProgramEnd > sForeignProgramStart && address >= base &&
           address - base >= sForeignProgramStart && address - base < sForeignProgramEnd;
}

const u8* EnemyAudio_GetForeignSfxParameters(u16 id)
{
    u32 index = id & 0x3ff;
    if ((id >> 12) != 3 || !sForeignSfxParameters || index < NATIVE_ENEMY_COUNT || index >= sForeignSfxParameterCount)
        return NULL;
    return sForeignSfxParameters + index * 4;
}

static u32 EnemyAudio_FileSize(u32 vrom)
{
    DmaEntry file;
    if (!vrom || !comboDmaLookup(&file, vrom) || file.vstart != vrom || file.vend <= vrom || file.pend)
        return 0;
    return file.vend - vrom;
}

int EnemyAudio_Init(AudioTableEntry* sequence)
{
    ALIGNED(16) u32 config[10];
    void* parameters;
    u32 size = EnemyAudio_FileSize(sForeignEnemyAudioVrom);
    u32 voiceCount = 0;

    if (size < 9 * sizeof(u32))
        return 0;
    size = size >= sizeof(config) ? sizeof(config) : 9 * sizeof(u32);
    if (comboDmaLoadFilePartial(config, sForeignEnemyAudioVrom, 0, size) != size ||
        config[0] != 0x46585331 || (config[1] != 2 && config[1] != 3) || config[2] != NATIVE_GAME ||
        !config[4] || config[4] > 0x10000 || config[6] < NATIVE_ENEMY_COUNT || config[6] > 1024 ||
        config[7] >= config[8] || config[8] > config[4])
        return 0;
    if (config[1] == 3)
    {
        if (size != sizeof(config) || config[9] <= NATIVE_VOICE_COUNT || config[9] > 1024)
            return 0;
        voiceCount = config[9];
    }
    size = (config[6] + voiceCount) * 4;
    if (EnemyAudio_FileSize(config[3]) < config[4] || EnemyAudio_FileSize(config[5]) < size)
        return 0;
    parameters = malloc(size);
    if (!parameters)
        return 0;
    if (comboDmaLoadFilePartial(parameters, config[5], 0, size) != size)
    {
        free(parameters);
        return 0;
    }
    sForeignSfxParameters = parameters;
    sForeignSfxParameterCount = config[6];
    sForeignVoiceParameters = voiceCount ? (u8*)parameters + config[6] * 4 : NULL;
    sequence->romAddr = config[3];
    sequence->size = config[4];
    sForeignProgramStart = config[7];
    sForeignProgramEnd = config[8];
    return 1;
}

u32 gForeignEnemySfxMapVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
# define EnemyNativeReverb ((void (*)(Vec3f*, u16, s8))0x800c5264)
# define EnemyNativeVolume ((void (*)(Vec3f*, u16, float))0x800c52b8)
# define EnemyNativeStopId ((void (*)(u16))0x800c9884)
# define EnemyNativeStopPosition ((void (*)(Vec3f*, u16))0x800c95c4)
# define EnemyNativeSfx ((void (*)(u16, Vec3f*, u8, float*, float*, s8*))0x800c806c)
EXPORT_SYMBOL(FOREIGN_ENEMY_SFX_MAP_OOT, gForeignEnemySfxMapVrom);
#else
# define EnemyNativeReverb ((void (*)(Vec3f*, u16, s8))0x8019fdc8)
# define EnemyNativeVolume ((void (*)(Vec3f*, u16, float))0x8019fe1c)
# define EnemyNativeStopId ((void (*)(u16))0x801a75e8)
# define EnemyNativeStopPosition ((void (*)(Vec3f*, u16))0x801a7328)
# define EnemyNativeSfx AudioSfx_PlaySfx
EXPORT_SYMBOL(FOREIGN_ENEMY_SFX_MAP_MM, gForeignEnemySfxMapVrom);
#endif

static u16 (*sSfxEntries)[2];
static u32 sSfxCount;
static u32 sSfxLoadedVrom;
static u16 sSfxLastKey, sSfxLastTarget;

static int enemySfxLoad(void)
{
    DmaEntry dma;
    u32 header[4];
    u16 (*entries)[2] = NULL;
    u32 count;
    if (sSfxLoadedVrom == gForeignEnemySfxMapVrom && sSfxLoadedVrom)
        return 1;
    if (!gForeignEnemySfxMapVrom || gForeignEnemySfxMapVrom < 0x08000000 ||
        !comboDmaLookup(&dma, gForeignEnemySfxMapVrom) || dma.vstart != gForeignEnemySfxMapVrom || dma.pend ||
        dma.vend < dma.vstart || dma.vend - dma.vstart < sizeof(header) ||
        comboDmaLoadFilePartial(header, gForeignEnemySfxMapVrom, 0, sizeof(header)) != sizeof(header) ||
        header[0] != 0x45534658 || header[1] != 1 || header[2] != NATIVE_GAME || header[3] > 7168 ||
        dma.vend - dma.vstart != sizeof(header) + header[3] * sizeof(*entries))
        return 0;
    count = header[3];
    if (count)
    {
        entries = malloc(count * sizeof(*entries));
        if (!entries)
            return 0;
        if (comboDmaLoadFilePartial(entries, gForeignEnemySfxMapVrom, sizeof(header), count * sizeof(*entries)) != count * sizeof(*entries))
        {
            free(entries);
            return 0;
        }
        for (u32 i = 0; i < count; ++i)
            if ((entries[i][0] & 0xc00) || (entries[i][1] & 0xc00) || (entries[i][0] >> 12) >= 7 ||
                (entries[i][1] >> 12) >= 7 || (i && entries[i - 1][0] >= entries[i][0]))
            {
                free(entries);
                return 0;
            }
    }
    if (sSfxEntries)
        free(sSfxEntries);
    sSfxEntries = entries;
    sSfxLastKey = 0xffff;
    sSfxCount = count;
    sSfxLoadedVrom = gForeignEnemySfxMapVrom;
    return 1;
}

int comboEnemySfxId(u16 source, u16* target)
{
    u16 key = source & 0xf3ff;
    if (!target)
        return 0;
    if (!source)
    {
        *target = 0;
        return 1;
    }
    if (sSfxLoadedVrom && sSfxLoadedVrom == gForeignEnemySfxMapVrom && sSfxLastKey == key)
    {
        *target = sSfxLastTarget | (source & 0xc00);
        return 1;
    }
    if (!enemySfxLoad())
        return 0;
    u32 low = 0;
    u32 high = sSfxCount;
    while (low < high)
    {
        u32 mid = low + (high - low) / 2;
        u16 entryKey = sSfxEntries[mid][0];
        if (entryKey == key)
        {
            sSfxLastKey = key;
            sSfxLastTarget = sSfxEntries[mid][1];
            *target = sSfxLastTarget | (source & 0xc00);
            return 1;
        }
        if (entryKey < key)
            low = mid + 1;
        else
            high = mid;
    }
    return 0;
}

static u16 enemySfxPlaybackId(u16 source)
{
    u16 target;
    return comboEnemySfxId(source, &target) ? target : 0;
}

void comboEnemySfxActor(Actor* actor, u16 source)
{
    if (!actor)
        return;
    u16 target = enemySfxPlaybackId(source);
    if (target)
        Actor_PlaySfx(actor, target);
}

void comboEnemySfxGeneral(u16 source, Vec3f* pos, u8 token, float* frequency, float* volume, s8* reverb)
{
    u16 target = enemySfxPlaybackId(source);
    if (target)
        EnemyNativeSfx(target, pos, token, frequency, volume, reverb);
}

void comboEnemySfxPosition(Vec3f* pos, u16 source)
{
    comboEnemySfxGeneral(source, pos, 4, &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
}

#if defined(GAME_MM)
void comboEnemyOotPlayerSfx(Actor* actor, u16 source)
{
    if (actor)
        comboEnemySfxPosition(&actor->projectedPos, source);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_OOT_PLAYER_SFX, comboEnemyOotPlayerSfx);
#endif

void comboEnemySfxReverb(Vec3f* pos, u16 source, s8 reverb)
{
    u16 target = enemySfxPlaybackId(source);
    if (target)
        EnemyNativeReverb(pos, target, reverb);
}

void comboEnemySfxVolume(Vec3f* pos, u16 source, float volume)
{
    u16 target = enemySfxPlaybackId(source);
    if (target)
        EnemyNativeVolume(pos, target, volume);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_REVERB, comboEnemySfxReverb);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_VOLUME, comboEnemySfxVolume);

void comboEnemySfxWorld(PlayState* play, Vec3f* pos, s32 duration, u16 source)
{
    u16 target = enemySfxPlaybackId(source);
    if (target)
        SfxSource_PlaySfxAtFixedWorldPos(play, pos, duration, target);
}

static void comboEnemyDeadSound(PlayState* play, Vec3f* projected, u16 source,
                                s16 lowerPriority, s16 repeatMode, s32 life)
{
    Actor* owner = comboEnemyCurrentActor();

    if (!owner || projected != &owner->projectedPos || repeatMode != 1 || life <= 0)
        return;
    comboEnemySfxWorld(play, &owner->world.pos, life, source);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DEAD_SOUND, comboEnemyDeadSound);

void comboEnemySfxStopId(u16 source)
{
    u16 target = enemySfxPlaybackId(source);
    if (target)
        EnemyNativeStopId(target);
}

void comboEnemySfxStopPosition(Vec3f* pos, u16 source)
{
    u16 target = enemySfxPlaybackId(source);
    if (target)
        EnemyNativeStopPosition(pos, target);
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_ACTOR, comboEnemySfxActor);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_GENERAL, comboEnemySfxGeneral);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_POSITION, comboEnemySfxPosition);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_WORLD, comboEnemySfxWorld);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_STOP_ID, comboEnemySfxStopId);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_STOP_POSITION, comboEnemySfxStopPosition);

#define ENEMY_OOT_AUDIO_FLAGS ((1u << 19) | (1u << 20) | (1u << 21) | (1u << 28))

#if defined(GAME_MM)
u32 comboEnemySfxEnter(Actor* actor, ForeignEnemyAudioState* state, u32 callback)
{
    u32 nativeFlags = actor->flags & ENEMY_OOT_AUDIO_FLAGS;
    if (callback == 0 || callback == 2 || (callback == 3 && !actor->sfx))
    {
        state->sfx = 0;
    }
    actor->sfx = state->sfx;
    actor->flags = (actor->flags & ~ENEMY_OOT_AUDIO_FLAGS) | state->flags;
    return nativeFlags;
}

void comboEnemySfxLeave(Actor* actor, ForeignEnemyAudioState* state, u32 nativeFlags)
{
    u8 mode;
    state->sfx = actor->sfx;
    state->flags = actor->flags & ENEMY_OOT_AUDIO_FLAGS;
    actor->flags = (actor->flags & ~ENEMY_OOT_AUDIO_FLAGS) | nativeFlags;
    mode = 1;
    if (state->flags & (1u << 19))
        mode = 2;
    else if (state->flags & (1u << 20))
        mode = 4;
    else if (state->flags & (1u << 21))
        mode = 8;
    else if (state->flags & (1u << 28))
        mode = 16;
    actor->audioFlags = (actor->audioFlags & ~0x1f) | mode;
    if (mode == 16)
        actor->sfx = state->sfx;
    else
        actor->sfx = enemySfxPlaybackId(state->sfx);
}

#endif

static void enemySetOotAudioFlags(Actor* actor, u8 mode)
{
    actor->flags &= ~ENEMY_OOT_AUDIO_FLAGS;
    if (mode == 2)
        actor->flags |= 1u << 19;
    else if (mode == 4)
        actor->flags |= 1u << 20;
    else if (mode == 8)
        actor->flags |= 1u << 21;
    else if (mode == 16)
        actor->flags |= 1u << 28;
}

static void enemySfxFlagged(Actor* actor, u16 source, u8 mode)
{
    u16 target = 0;
    if (!actor)
        return;
#if defined(GAME_MM)
    ForeignEnemyAudioState* state = comboEnemySourceAudioState(actor);
    if (state)
    {
        int inactive = comboEnemyCurrentActor() != actor;
        u32 nativeFlags = inactive ? comboEnemySfxEnter(actor, state, 3) : 0;
        actor->sfx = source;
        enemySetOotAudioFlags(actor, mode);
        if (inactive)
            comboEnemySfxLeave(actor, state, nativeFlags);
        return;
    }
#endif
    if (mode == 16)
        target = source;
    else if (!comboEnemySfxId(source, &target))
        return;
    actor->sfx = target;
#if defined(GAME_OOT)
    enemySetOotAudioFlags(actor, mode);
#else
    actor->audioFlags = (actor->audioFlags & ~0x1f) | mode;
#endif
}

void comboEnemySfxFlagged(Actor* actor, u16 source)
{
    enemySfxFlagged(actor, source, 1);
}

void comboEnemySfxFlagged2(Actor* actor, u16 source)
{
    enemySfxFlagged(actor, source, 2);
}

void comboEnemySfxCentered1(Actor* actor, u16 source)
{
    enemySfxFlagged(actor, source, 4);
}

void comboEnemySfxCentered2(Actor* actor, u16 source)
{
    enemySfxFlagged(actor, source, 8);
}

void comboEnemySfxTimer(Actor* actor, s32 timer)
{
    enemySfxFlagged(actor, timer < 40 ? 3 : timer < 100 ? 2
                                                        : 1,
                    16);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_FLAGGED, comboEnemySfxFlagged);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_FLAGGED2, comboEnemySfxFlagged2);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_CENTERED1, comboEnemySfxCentered1);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_CENTERED2, comboEnemySfxCentered2);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_TIMER, comboEnemySfxTimer);

#if defined(GAME_MM)
void enemyStoreOotBgm(u16 source)
{
    (void)source;
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_OOT_BGM_STORE, enemyStoreOotBgm);
#endif

int comboEnemyMusicSuppressed(void)
{
    return comboEnemyCurrentActor() != NULL || comboEnemyNativeMusicSuppressed();
}
