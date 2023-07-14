#include <combo.h>

static void SpawnRoomActors_Wrapper(GameState_Play* play, int id)
{
    /* Reset the silver rupee counter */
    g.silverRupee = 0;

    /* Spawn the normal room actors */
    SpawnRoomActors(play, id);
}

PATCH_CALL(0x80080c18, SpawnRoomActors_Wrapper);
PATCH_CALL(0x80080cd0, SpawnRoomActors_Wrapper);
