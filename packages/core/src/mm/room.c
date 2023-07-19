#include <combo.h>

static void SpawnRoomActors_Wrapper(GameState_Play* play, int id)
{
    /* Spawn the normal room actors */
    SpawnRoomActors(play, id);
}

PATCH_CALL(0x8012eb18, SpawnRoomActors_Wrapper);
PATCH_CALL(0x8012ec30, SpawnRoomActors_Wrapper);

void OnRoomChange(void* arg1, void* arg2)
{
    void (*OnRoomChangeOriginal)(void*, void*);

    /* Clear some flags */
    g.silverRupee = 0;
    g.roomEnemyLackSoul = 0;

    OnRoomChangeOriginal = (void*)0x8012f90c;
    OnRoomChangeOriginal(arg1, arg2);
}
