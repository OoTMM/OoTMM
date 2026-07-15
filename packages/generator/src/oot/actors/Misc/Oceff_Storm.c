#include <combo.h>

static Actor* OceffStorm_SpawnManager(ActorContext* actorCtx, PlayState* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    gInterruptSongOfStorms = false;
    return Actor_Spawn(actorCtx, play, actorId, x, y, z, rx, ry, rz, variable);
}

PATCH_CALL(0x80b38f30, OceffStorm_SpawnManager);
