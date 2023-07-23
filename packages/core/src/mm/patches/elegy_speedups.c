#include <combo.h>
#include <stdbool.h>

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
    return 50.0;
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


bool Elegy_UpdateCamera(Actor* actor, GameState_Play* play)
{
    return false;
}

bool Elegy_Darken(Actor* actor, GameState_Play* play)
{
    return false;
}

void Elegy_Update(Actor* actor, GameState_Play* play)
{
    u8 index = actor->variable & 7;
    Actor* statue = play->actorCtx.elegyStatues[index];
    // if (statue != NULL)
    // {
    actor->initPos = statue->initPos;
    // }
}