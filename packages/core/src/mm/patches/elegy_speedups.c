#include <combo.h>

union ElegyLockParams
{
    struct
    {
        u16 frameCount;
        u16 spawnFrame;
    };
    u32 value;
};

f32 Elegy_BlockSpeed(Actor* actor, PlayState* play, int type)
{
    return 40.0;
}

u32 Elegy_LockParams(Player* player, PlayState* play)
{
    union ElegyLockParams result = {
        .frameCount = 0x01,
        .spawnFrame = 0x01,
    };
    return result.value;
}

u16 Elegy_DespawnCounter(PlayState* play, Player* player)
{
    return 0x1;
}

u16 Elegy_FadeSpeed(PlayState* play, Player* player)
{
    return 0x20;
}


int Elegy_UpdateCamera(Actor* actor, PlayState* play)
{
    return 0;
}

int Elegy_Darken(Actor* actor, PlayState* play)
{
    return 0;
}

void Elegy_Update(Actor* actor, PlayState* play)
{
    u8 index = actor->params & 7;
    Actor* statue = play->actorCtx.elegyStatues[index];
    if (statue != NULL)
    {
        actor->home.pos = statue->home.pos;
    }
}

union SongStateResults {
    struct {
        u8 action;
        u8 unk1F0A;
        u8 frameCount;
        u8 unused;
    };
    u32 value;
};

u32 SongState_HandlePlayback(PlayState* play, MessageContext* msgCtxt) {
    s8 song = msgCtxt->songInfo->frameInfo[0].storedSong;
    if (song == OCARINA_SONG_ELEGY) {
        /* Process state for Elegy of Emptiness */
        /* Disable sfx being "dampened" (normally action 0x17 would do this before advancing to 0x18) */
        AudioOcarina_SetInstrument(0);
        /* Code for action 0x18 will stop early unless this value is 0x32. */
        /* msgCtxt->unk202C = 0x32; */
        /* Skip past song playback & message box, to state 0x18 */
        union SongStateResults results = {
            .action = 0x18,
            .unk1F0A = 3,
            .frameCount = 2,
        };
        return results.value;
    } else {
        /* Vanilla behavior, prepare for song playback. */
        union SongStateResults results = {
            .action = 0x12,
            .unk1F0A = 3,
            .frameCount = 10,
        };
        return results.value;
    }
}
