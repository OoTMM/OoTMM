#include <combo.h>

typedef struct
{
    u16 sceneId;
    u16 actorId;
}
RemovedActor;

static RemovedActor kRemovedActors[] = {
    { SCE_OOT_CASTLE_COURTYARD, 0x0a9 }, /* Zelda Courtyard - Impa */
    { SCE_OOT_HYRULE_FIELD, AC_EN_OWL },
    { SCE_OOT_HYRULE_CASTLE, AC_EN_OWL },
    { SCE_OOT_ZORA_RIVER, AC_EN_OWL },
    { SCE_OOT_LOST_WOODS, AC_EN_OWL },
};

void comboSpawnCollectible(GameState_Play* play, float x, float y, float z, u8 flag, u16 gi)
{
    SpawnActor(
        (char*)play + 0x1c24,
        play,
        AC_ITEM_CUSTOM,
        x, y, z,
        0, 0, 0x1000 | flag,
        gi
    );
}

Actor* hookSpawnActor(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    for (size_t i = 0; i < ARRAY_SIZE(kRemovedActors); ++i)
    {
        if (kRemovedActors[i].sceneId == play->sceneId && kRemovedActors[i].actorId == actorId)
        {
            return NULL;
        }
    }

    switch (actorId)
    {
    case AC_ITEM00:
#if 0
        if ((variable & 0xff) == 0x11) /* Small Key */
        {
            actorId = AC_ITEM_CUSTOM;
            rz = 0x1000 | ((variable >> 8) & 0x3f);
            variable = GI_OOT_SMALL_KEY;
        }
#endif
        break;
    case AC_BG_TOKI_SWD:
        if (gSave.age == AGE_CHILD)
            comboSpawnCollectible(play, -1.f, 70.f, 80.f, 0x1f, GI_OOT_MASTER_SWORD);
        else
            comboSpawnCollectible(play, -1.f, 10.f, 500.f, 0x1e, GI_OOT_MEDALLION_LIGHT);
        break;
    }

    return SpawnActor(const_1, play, actorId, x, y, z, rx, ry, rz, variable);
}
