#include <combo.h>

Actor* SpawnActor(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable);

Actor* hookSpawnActor(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    switch (actorId)
    {
    case 0x0a: /* En_Box */
        actorId = 0x10f; /* Item_Etcetera */
        variable = 0x1; /* Debug: Golden scale */
        break;
    case 0x15: /* En_item00 */
        if ((variable & 0xff) <= 0x06) /* Heart Piece */
        {
            actorId = 0x10f; /* Item_Etcetera */
            variable = 0x1; /* Debug: Golden scale */
        }
        break;
    }
    return SpawnActor(const_1, play, actorId, x, y, z, rx, ry, rz, variable);
}
