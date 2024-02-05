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

f32 Elegy_BlockSpeed(Actor* actor, GameState_Play* play, int type)
{
    return 40.0;
}

u32 Elegy_LockParams(Actor_Player* player, GameState_Play* play)
{
    union ElegyLockParams result = {
        .frameCount = 0x01,
        .spawnFrame = 0x01,
    };
    return result.value;
}

u16 Elegy_DespawnCounter(GameState_Play* play, Actor_Player* player)
{
    return 0x1;
}

u16 Elegy_FadeSpeed(GameState_Play* play, Actor_Player* player)
{
    return 0x20;
}


int Elegy_UpdateCamera(Actor* actor, GameState_Play* play)
{
    return 0;
}

int Elegy_Darken(Actor* actor, GameState_Play* play)
{
    return 0;
}

void Elegy_Update(Actor* actor, GameState_Play* play)
{
    u8 index = actor->variable & 7;
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

u32 SongState_HandlePlayback(GameState_Play* play, MessageContext* msgCtxt) {
    s8 song = msgCtxt->songInfo->frameInfo[0].storedSong;
    if (song == 3) {
        // Process state for Elegy of Emptiness
        // Disable sfx being "dampened" (normally action 0x17 would do this before advancing to 0x18)
        AudioOcarina_SetInstrument(0);
        // Code for action 0x18 will stop early unless this value is 0x32.
        // msgCtxt->unk202C = 0x32;
        // Skip past song playback & message box, to state 0x18
        union SongStateResults results = {
            .action = 0x18,
            .unk1F0A = 3,
            .frameCount = 2,
        };
        return results.value;
    } else {
        // Vanilla behavior, prepare for song playback.
        union SongStateResults results = {
            .action = 0x12,
            .unk1F0A = 3,
            .frameCount = 10,
        };
        return results.value;
    }
}
