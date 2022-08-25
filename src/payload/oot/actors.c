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
    { 0x51, AC_EN_OWL }, /* Hyrule Field - Owl */
    { 0x5f, AC_EN_OWL }, /* Hyrule Castle - Owl */
    { 0x41, 0x12e }, /* Sun Song */
};

void comboSpawnSpecial(GameState_Play* play, float x, float y, float z, u8 specialId, u16 gi)
{
    SpawnActor(
        (char*)play + 0x1c24,
        play,
        AC_ITEM_ETCETERA,
        x, y, z,
        0, 0, 0x2000 | specialId,
        gi
    );
}

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

    switch (actorId)
    {
    case AC_DOOR_WARP1:
        switch (play->sceneId)
        {
        case 0x11:
            comboSpawnSpecial(play, x, y, z, EV_CHK_STONE_EMERALD, GI_OOT_SWORD_KOKIRI);
            break;
        }
        break;
    case AC_ITEM_B_HEART:
        /* Always use collectible flag 0x1f */
        actorId = AC_ITEM_ETCETERA;
        rz = 0x1000 | 0x1f;
        variable = GI_OOT_HEART_CONTAINER2;
        break;
    }

    return SpawnActor(const_1, play, actorId, x, y, z, rx, ry, rz, variable);
}
