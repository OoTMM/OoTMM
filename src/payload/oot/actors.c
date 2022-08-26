#include <combo.h>

typedef struct
{
    u16 sceneId;
    u16 actorId;
}
RemovedActor;

static RemovedActor kRemovedActors[] = {
    { SCE_CASTLE_COURTYARD, 0x1d3 }, /* Zelda Courtyard - Zelda */
    { SCE_CASTLE_COURTYARD, 0x0a9 }, /* Zelda Courtyard - Impa */
    { SCE_HYRULE_FIELD, AC_EN_OWL }, /* Hyrule Field - Owl */
    { SCE_HYRULE_CASTLE, AC_EN_OWL }, /* Hyrule Castle - Owl */
    { SCE_TOMB_ROYAL, 0x12e }, /* Sun Song */
    { SCE_SACRED_FOREST_MEADOW, AC_EN_SA }, /* Saria in meadow */
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
        /* Blue warp */
        switch (play->sceneId)
        {
        case 0x11:
            comboSpawnSpecial(play, x, y, z, EV_CHK_STONE_EMERALD, GI_OOT_STONE_EMERALD);
            SetEventChk(EV_CHK_EMERALD_TREE_DEAD);
            SetEventChk(EV_CHK_GOHMA);
            SetEventChk(EV_CHK_TREE_DEAD);
            SetEventChk(EV_CHK_MIDO_TREE_DEAD);
            break;
        }
        break;
    case AC_ITEM_B_HEART:
        /* Heart container */
        /* Always use collectible flag 0x1f */
        actorId = AC_ITEM_ETCETERA;
        rz = 0x1000 | 0x1f;
        variable = GI_OOT_HEART_CONTAINER2;
        break;
    }

    return SpawnActor(const_1, play, actorId, x, y, z, rx, ry, rz, variable);
}
