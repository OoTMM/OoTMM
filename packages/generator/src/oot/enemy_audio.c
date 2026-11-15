#include <combo.h>
#include <combo/audio.h>
#include <combo/enemy.h>
#include <combo/sfx.h>
#include <combo/sfx_source.h>

typedef struct EnemyAudioScript {
    u8* pc;
    u8* stack[4];
    u8 remLoopIters[4];
    u8 depth;
    s8 value;
} EnemyAudioScript;
typedef struct EnemyAudioAdsr {
    u8 decayIndex;
    u8 sustain;
    u16 padding;
    void* envelope;
} EnemyAudioAdsr;
typedef struct EnemyAudioPortamento {
    u8 mode;
    u8 padding;
    u16 cur;
    u16 speed;
    u16 padding2;
    float extent;
} EnemyAudioPortamento;
typedef struct EnemyAudioPlayer {
    u8 flags;
    u8 state;
    u8 noteAllocPolicy;
    u8 muteBehavior;
    u8 seqId;
    u8 defaultFont;
    u8 padding6[8];
    s16 transposition;
    u8 padding10[8];
    u8* seqData;
    u8 padding1c[0x78];
    u8* shortNoteVelocityTable;
    u8* shortNoteGateTimeTable;
} EnemyAudioPlayer;
typedef struct EnemyAudioChannel EnemyAudioChannel;
typedef struct EnemyAudioLayer {
    u8 flags;
    u8 stereo;
    u8 instOrWave;
    u8 gateTime;
    u8 semitone;
    u8 portamentoTargetNote;
    u8 pan;
    u8 notePan;
    s16 delay;
    s16 gateDelay;
    s16 delay2;
    u16 portamentoTime;
    s16 transposition;
    s16 shortNoteDefaultDelay;
    s16 lastDelay;
    u16 compatFlags;
    EnemyAudioAdsr adsr;
    EnemyAudioPortamento portamento;
    void* note;
    float freqScale;
    float bend;
    float velocitySquare2;
    float velocitySquare;
    float noteVelocity;
    float noteFreqScale;
    void* instrument;
    void* tunedSample;
    EnemyAudioChannel* channel;
    EnemyAudioScript script;
    u8 listItem[0x10];
} EnemyAudioLayer;
struct EnemyAudioChannel {
    u8 padding0[0x26];
    s16 transposition;
    u8 padding28[0x24];
    EnemyAudioPlayer* player;
    EnemyAudioLayer* layers[4];
};

_Static_assert(sizeof(EnemyAudioLayer) == 0x80, "OoT audio layer");
_Static_assert(__builtin_offsetof(EnemyAudioLayer, compatFlags) == 0x16, "OoT layer spare field");
_Static_assert(__builtin_offsetof(EnemyAudioLayer, script) == 0x54, "OoT layer script");
_Static_assert(__builtin_offsetof(EnemyAudioChannel, player) == 0x4c, "OoT channel player");
_Static_assert(__builtin_offsetof(EnemyAudioPlayer, seqData) == 0x18, "OoT sequence data");
_Static_assert(__builtin_offsetof(EnemyAudioPlayer, shortNoteVelocityTable) == 0x94, "OoT short note table");

#define NativeLayerStep2 ((s32 (*)(EnemyAudioLayer*))0x800bf680)
#define NativeSetLayer ((s32 (*)(EnemyAudioChannel*, s32))0x800bec98)
#define NativeReadU8 ((u8 (*)(EnemyAudioScript*))0x800bf308)
#define NativeReadS16 ((s16 (*)(EnemyAudioScript*))0x800bf31c)
#define NativeReadCompressed ((u16 (*)(EnemyAudioScript*))0x800bf354)
#define NativeFlowArgument ((s32 (*)(EnemyAudioScript*, s32))0x800be900)
#define NativeFlow ((s32 (*)(EnemyAudioPlayer*, EnemyAudioScript*, s32, s32))0x800be968)
#define NativeDisable ((void (*)(EnemyAudioLayer*))0x800bedbc)
#define NativeDecay ((void (*)(EnemyAudioLayer*))0x800bd120)
#define NativeInstrument ((u8 (*)(EnemyAudioChannel*, u8, void**, EnemyAudioAdsr*))0x800c04e4)
#define NativeVibratoInit ((void (*)(u8*))0x800be4c0)
#define NativeSampleState ((void (*)(u8*, void*, u8*))0x800bbf00)
#define BendPitch ((float*)0x801007c4)

#define FOREIGN_LAYER 0x8000u
#define LAYER_VIBRATO 0x1000u
#define LAYER_GAIN 0x0040u

static s32 EnemyAudio_SetLayer(EnemyAudioChannel* channel, s32 index)
{
    s32 result = NativeSetLayer(channel, index);
    if (result == 0)
        channel->layers[index]->compatFlags = 0;
    return result;
}
PATCH_CALL(0x800c1084, EnemyAudio_SetLayer);
PATCH_CALL(0x800c10d0, EnemyAudio_SetLayer);
PATCH_CALL(0x800c1140, EnemyAudio_SetLayer);

static s32 EnemyAudio_LayerStep2(EnemyAudioLayer* layer)
{
    EnemyAudioChannel* channel = layer->channel;
    EnemyAudioPlayer* player = channel->player;
    EnemyAudioScript* state = &layer->script;
    u8 cmd;
    u8 arg8;
    u16 arg16;

    if (!(layer->compatFlags & FOREIGN_LAYER))
    {
        if (!EnemyAudio_IsForeignProgram(player->seqData, state->pc, player->seqId))
            return NativeLayerStep2(layer);
        layer->compatFlags = 0xffff;
    }
    for (;;)
    {
        cmd = NativeReadU8(state);
        if (cmd <= 0xc0)
            return cmd;
        if (cmd >= 0xf2)
        {
            arg16 = NativeFlowArgument(state, cmd);
            if (NativeFlow(player, state, cmd, arg16) == 0)
                continue;
            NativeDisable(layer);
            return -1;
        }
        switch (cmd)
        {
        case 0xc1:
        case 0xca:
            arg8 = NativeReadU8(state);
            if (cmd == 0xc1)
                layer->velocitySquare = (float)(arg8 * arg8) / (127.f * 127.f);
            else
                layer->pan = arg8;
            break;
        case 0xc9:
        case 0xc2:
            arg8 = NativeReadU8(state);
            if (cmd == 0xc9)
                layer->gateTime = arg8;
            else
                layer->transposition = arg8;
            break;
        case 0xc4:
        case 0xc5:
            if (cmd == 0xc4)
                layer->flags |= 0x10;
            else
                layer->flags &= ~0x10;
            layer->flags &= ~2;
            NativeDecay(layer);
            break;
        case 0xc3:
            layer->shortNoteDefaultDelay = NativeReadCompressed(state);
            break;
        case 0xc6:
            arg8 = NativeReadU8(state);
            if (arg8 >= 0x7e)
            {
                if (arg8 == 0x7e)
                    layer->instOrWave = 1;
                else if (arg8 == 0x7f)
                    layer->instOrWave = 0;
                else
                {
                    layer->instOrWave = arg8;
                    layer->instrument = NULL;
                }
                if (arg8 == 0xff)
                    layer->adsr.decayIndex = 0;
            }
            else if (!(layer->instOrWave = NativeInstrument(channel, arg8, &layer->instrument, &layer->adsr)))
                layer->instOrWave = 0xff;
            break;
        case 0xc7:
            layer->portamento.mode = NativeReadU8(state);
            arg8 = NativeReadU8(state) + channel->transposition + layer->transposition + player->transposition;
            layer->portamentoTargetNote = arg8 >= 0x80 ? 0 : arg8;
            layer->portamentoTime = (layer->portamento.mode & 0x80) ? NativeReadU8(state) : NativeReadCompressed(state);
            break;
        case 0xc8:
            layer->portamento.mode = 0;
            break;
        case 0xcb:
            layer->adsr.envelope = player->seqData + (u16)NativeReadS16(state);
            layer->adsr.decayIndex = NativeReadU8(state);
            break;
        case 0xcf:
            layer->adsr.decayIndex = NativeReadU8(state);
            break;
        case 0xcc:
            layer->flags |= 4;
            break;
        case 0xcd:
            layer->stereo = NativeReadU8(state);
            break;
        case 0xce:
            arg8 = NativeReadU8(state);
            layer->bend = BendPitch[(u8)(arg8 + 0x80)];
            break;
        case 0xf0:
            arg16 = NativeReadS16(state);
            if (arg16 & ~(LAYER_VIBRATO | LAYER_GAIN))
            {
                NativeDisable(layer);
                return -1;
            }
            layer->compatFlags &= ~arg16;
            break;
        default:
            if ((cmd & 0xf0) == 0xd0)
            {
                arg8 = player->shortNoteVelocityTable[cmd & 0xf];
                layer->velocitySquare = (float)(arg8 * arg8) / (127.f * 127.f);
            }
            else if ((cmd & 0xf0) == 0xe0)
                layer->gateTime = player->shortNoteGateTimeTable[cmd & 0xf];
            break;
        }
    }
}
PATCH_CALL(0x800bf404, EnemyAudio_LayerStep2);

static void EnemyAudio_VibratoInit(u8* note)
{
    EnemyAudioLayer* layer = *(EnemyAudioLayer**)(note + 0x44);
    u16 flags = layer->compatFlags;
    NativeVibratoInit(note);
    note[0xb8] = (flags & FOREIGN_LAYER) && !(flags & LAYER_GAIN);
    if ((flags & FOREIGN_LAYER) && !(flags & LAYER_VIBRATO))
        note[0xb0] = 0;
}
PATCH_CALL(0x800bc768, EnemyAudio_VibratoInit);
PATCH_CALL(0x800bf640, EnemyAudio_VibratoInit);

static void EnemyAudio_SampleState(u8* note, void* sample, u8* attributes)
{
    EnemyAudioLayer* layer;
    u8 gain = attributes[1];
    if (!note[0x34])
    {
        layer = *(EnemyAudioLayer**)(note + 0x44);
        if ((u32)layer >= 0x80000000u && layer != (void*)0xffffffffu)
            note[0xb8] = (layer->compatFlags & FOREIGN_LAYER) && !(layer->compatFlags & LAYER_GAIN);
        else
            note[0xb8] = 0;
    }
    if (note[0xb8])
        attributes[1] = 0;
    NativeSampleState(note, sample, attributes);
    attributes[1] = gain;
}
PATCH_CALL(0x800bc9b4, EnemyAudio_SampleState);

typedef struct
{
    float* positionX;
    u8 padding4[0x24];
    u16 id;
    u8 padding2a[6];
} EnemyNativeSfxEntry;
typedef struct
{
    u32 priority;
    u8 index;
    u8 padding5[3];
} EnemyNativeActiveSfx;
_Static_assert(sizeof(EnemyNativeSfxEntry) == 0x30, "Native sound-bank stride");
_Static_assert(__builtin_offsetof(EnemyNativeSfxEntry, id) == 0x28, "Native sound ID offset");
_Static_assert(sizeof(EnemyNativeActiveSfx) == 8, "Native active-channel stride");

#define EnemyNativeSfxBanks ((EnemyNativeSfxEntry**)0x80104368)
#define EnemyNativeActiveSfxs ((EnemyNativeActiveSfx (*)[3])0x801246a8)
#define EnemyNativeChannels ((u8 (*)[7])0x80101888)
#define EnemyNativeChannelLayout (*(u8*)0x8010438c)
#define EnemyNativeQueueIo ((void (*)(u32, s8))0x800bb0e0)
#define EnemyNativePlay ((void (*)(u16, Vec3f*, u8, float*, float*, s8*))0x800c806c)

static int enemySfxChannelIo(Vec3f* position, u16 id, u8 value, int all)
{
    u32 bank = id >> 12;
    u32 layout = EnemyNativeChannelLayout;
    u32 channel = 0;
    if (bank >= 7 || layout >= 4)
        return 0;
    u32 count = EnemyNativeChannels[layout][bank];
    if (count > 3 || (!all && !EnemyNativeSfxBanks[bank]))
        return 0;
    for (u32 previous = 0; previous < bank; ++previous)
        channel += EnemyNativeChannels[layout][previous];
    for (u32 slot = 0; slot < count; ++slot, ++channel)
    {
        u32 index = EnemyNativeActiveSfxs[bank][slot].index;
        if (all || (index != 0xff && EnemyNativeSfxBanks[bank][index].id == id &&
                    EnemyNativeSfxBanks[bank][index].positionX == (float*)position))
            EnemyNativeQueueIo(0x06020006u | (channel << 8), value);
    }
    return 1;
}

void comboEnemySfxChannelIo(Vec3f* position, u16 source, u8 value)
{
    u16 target;
    if (comboEnemySfxId(source, &target))
        enemySfxChannelIo(position, target, value, 0);
}

static void enemySfxPlayChannelIo(Vec3f* position, u16 source, u8 value, int all)
{
    u16 target;
    if (!comboEnemySfxId(source, &target) || !enemySfxChannelIo(position, target, value, all))
        return;
    EnemyNativePlay(target, position, 4, &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
}

void comboEnemySfxPositionIo(Vec3f* position, u16 source, u8 value)
{
    enemySfxPlayChannelIo(position, source, value, 0);
}

void comboEnemySfxAllChannelsIo(Vec3f* position, u16 source, u8 value)
{
    enemySfxPlayChannelIo(position, source, value, 1);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_CHANNEL_IO, comboEnemySfxChannelIo);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_POSITION_IO, comboEnemySfxPositionIo);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_ALL_CHANNELS_IO, comboEnemySfxAllChannelsIo);

#define EnemyNativeSoundProjection ((void (*)(MtxF*, Vec3f*, Vec3f*))0x8008ed08)

static u16 sPendingWorldLoop;

static void enemySoundSourcePlay(u16 sfx, Vec3f* projected, u8 token, float* frequency, float* volume, s8* reverb)
{
    SfxSource* source = (SfxSource*)((u8*)projected - __builtin_offsetof(SfxSource, projectedPos));
    source->foreignLoopSfx = sPendingWorldLoop;
    Audio_PlaySfxGeneral(sfx, projected, token, frequency, volume, reverb);
}
PATCH_CALL(0x800590c8, enemySoundSourcePlay);

static void enemySoundSourceProject(MtxF* matrix, Vec3f* world, Vec3f* projected)
{
    SfxSource* source = (SfxSource*)((u8*)projected - __builtin_offsetof(SfxSource, projectedPos));
    EnemyNativeSoundProjection(matrix, world, projected);
    if (source->foreignLoopSfx)
        Audio_PlaySfxGeneral(source->foreignLoopSfx, projected, 4, &gSfxDefaultFreqAndVolScale,
                             &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
}
PATCH_CALL(0x80058fc4, enemySoundSourceProject);

void comboEnemySfxWorldLoop(PlayState* play, Vec3f* world, u32 duration, u16 source)
{
    u16 target;
    u16 previous;
    if (!play || !world || !comboEnemySfxId(source, &target) || !target)
        return;
    previous = sPendingWorldLoop;
    sPendingWorldLoop = target;
    SfxSource_PlaySfxAtFixedWorldPos(play, world, duration, target);
    sPendingWorldLoop = previous;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_WORLD_LOOP, comboEnemySfxWorldLoop);

typedef struct
{
    u8 padding0[0x1c];
    float distance;
    u8 padding20[6];
    u16 params;
    u16 id;
    u8 padding2a[6];
} EnemySpatialSfx;
_Static_assert(sizeof(EnemySpatialSfx) == 0x30, "Native spatial-entry stride");
_Static_assert(__builtin_offsetof(EnemySpatialSfx, params) == 0x26, "Native spatial-parameter offset");

#define EnemyNativeSpatialBanks ((EnemySpatialSfx**)0x80104368)
#define EnemyNativeVolumeAt ((float (*)(u8, u8))0x800c3ebc)
#define EnemyNativeSurroundFilter ((u8 (*)(float, EnemySpatialSfx*, s8))0x800c44ec)
const u8* EnemyAudio_GetForeignSfxParameters(u16 id);

static int enemySpatialRange(EnemySpatialSfx* entry)
{
    const u8* parameters = EnemyAudio_GetForeignSfxParameters(entry->id);
    if (!parameters || !(parameters[1] & 2))
        return -1;
    return 4 | (entry->params & 3);
}

static float enemySfxSpatialVolume(u8 bank, u8 index)
{
    static const float radii[4] = { 1000.0f, 666.6667f, 500.0f, 400.0f };
    if (bank >= 7 || !EnemyNativeSpatialBanks[bank])
        return EnemyNativeVolumeAt(bank, index);
    EnemySpatialSfx* entry = &EnemyNativeSpatialBanks[bank][index];
    int range = enemySpatialRange(entry);
    if (range < 4)
        return EnemyNativeVolumeAt(bank, index);
    if (entry->params & 0x2000)
        return 1.0f;
    float distance = entry->distance;
    if (distance > 10000.0f)
        return 0.0f;
    float outer = radii[range - 4], inner = outer / 5.0f;
    float amplitude;
    if (distance < inner)
        amplitude = 1.0f;
    else if (distance < outer)
        amplitude = ((outer - distance) / (outer - inner)) * 0.65f + 0.35f;
    else
        amplitude = (1.0f - (distance - outer) / (10000.0f - outer)) * 0.35f;
    return amplitude * amplitude;
}

static u8 enemySfxSpatialSurround(float behindScreenZ, EnemySpatialSfx* entry, s8 pan)
{
    int range = enemySpatialRange(entry);
    if (range < 4)
        return EnemyNativeSurroundFilter(behindScreenZ, entry, pan);
    float distance = entry->distance;
    if (distance > 1923.077f)
        distance = 1923.077f;
    u8 highPass = ((18.0f + (range - 4) * 3.0f) * distance) / (2.0f * 1923.077f);
    return (entry->params & 0x200 ? 0xf0 : 0) | highPass;
}
PATCH_CALL(0x800c4820, enemySfxSpatialVolume);
PATCH_CALL(0x800c49ac, enemySfxSpatialSurround);
EXPORT_SYMBOL(FOREIGN_ENEMY_AUDIO_SPATIAL_OOT, enemySfxSpatialVolume);

#define EnemyNativeVolume ((void (*)(Vec3f*, u16, float))0x800c52b8)
#define EnemyNativeVolumeValue (*(float*)0x80121df8)
#define EnemyNativeFanfareUpdate ((void (*)(void))0x800c6a34)
#define EnemyNativeSfxReset ((void (*)(void))0x800c4c1c)

static u16 sVolumeFrames;
static float sVolumeStep;

void comboEnemySfxVolumeTransition(Vec3f* pos, u16 source, u16 duration)
{
    u16 target;
    if (!comboEnemySfxId(source, &target))
        return;
    EnemyNativeVolume(pos, target, 0.0f);
    sVolumeFrames = duration;
    sVolumeStep = duration ? -1.0f / duration : 0.0f;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SFX_VOLUME_TRANSITION, comboEnemySfxVolumeTransition);

static void enemySfxTransitionUpdate(void)
{
    EnemyNativeFanfareUpdate();
    if (sVolumeFrames)
    {
        if (!--sVolumeFrames)
            EnemyNativeVolumeValue = 1.0f;
        else
            EnemyNativeVolumeValue -= sVolumeStep;
    }
}
PATCH_CALL(0x800c3e10, enemySfxTransitionUpdate);

static void enemySfxTransitionReset(void)
{
    EnemyNativeSfxReset();
    sVolumeFrames = 0;
    sVolumeStep = 0.0f;
}
PATCH_CALL(0x800c7e10, enemySfxTransitionReset);
PATCH_CALL(0x800c7ebc, enemySfxTransitionReset);
PATCH_CALL(0x800c7f08, enemySfxTransitionReset);
