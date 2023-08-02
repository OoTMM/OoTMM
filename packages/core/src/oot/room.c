#include <combo.h>

static void SpawnRoomActors_Wrapper(GameState_Play* play, int id)
{
    /* Spawn the normal room actors */
    SpawnRoomActors(play, id);
}

PATCH_CALL(0x80080c18, SpawnRoomActors_Wrapper);
PATCH_CALL(0x80080cd0, SpawnRoomActors_Wrapper);

void OnRoomChange(void* arg1, void* arg2)
{
    void (*OnRoomChangeOriginal)(void*, void*);

    /* Clear some flags */
    g.silverRupee = 0;
    g.roomEnemyLackSoul = 0;
    g.actorIndex = 0;

    OnRoomChangeOriginal = (void*)0x80081900;
    OnRoomChangeOriginal(arg1, arg2);
}

Actor* SpawnRoomActor(void* unk, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    Actor* a;

    a = comboSpawnActor(unk, play, actorId, x, y, z, rx, ry, rz, variable);
    g.actorIndex++;
    return a;
}

PATCH_CALL(0x8002562c, SpawnRoomActor);
