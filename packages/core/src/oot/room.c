#include <combo.h>

static void SpawnRoomActors_Wrapper(GameState_Play* play, int id)
{
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

    OnRoomChangeOriginal = (void*)0x80081900;
    OnRoomChangeOriginal(arg1, arg2);
}
