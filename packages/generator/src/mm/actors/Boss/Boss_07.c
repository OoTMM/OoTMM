#include <combo.h>

static Actor* Boss07_SpawnTop_HyperSafe(ActorContext* actorCtx, PlayState* play, s16 actorId, f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s16 params)
{
    int prev = play->frameAdvCtx.enabled;
    play->frameAdvCtx.enabled = false;
    Actor* actor = Actor_Spawn(actorCtx, play, actorId, x, y, z, rx, ry, rz, params);
    play->frameAdvCtx.enabled = prev;
    return actor;
}

PATCH_CALL(0x809f8f64, Boss07_SpawnTop_HyperSafe);