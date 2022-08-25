#include <combo.h>

typedef struct
{
    u16 sceneId;
    u16 actorId;
}
RemovedActor;

static RemovedActor kRemovedActors[] = {
    { 0x4a, 0x1d3 }, /* Zelda Courtyard - Zelda */
    { 0x4a, 0x0a9 }, /* Zelda Courtyard - Impa */
};

Actor* hookSpawnActor(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
#if 0
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
#endif

    for (size_t i = 0; i < ARRAY_SIZE(kRemovedActors); ++i)
    {
        if (kRemovedActors[i].sceneId == play->sceneId && kRemovedActors[i].actorId == actorId)
        {
            return NULL;
        }
    }
    return SpawnActor(const_1, play, actorId, x, y, z, rx, ry, rz, variable);
}
