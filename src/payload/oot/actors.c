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
    { SCE_ZORA_RIVER, AC_EN_OWL },
    { SCE_LOST_WOODS, AC_EN_OWL },
    { SCE_TOMB_ROYAL, 0x12e }, /* Sun Song */
    { SCE_SACRED_FOREST_MEADOW, AC_EN_SA }, /* Saria in meadow */
    { SCE_HYRULE_FIELD, AC_ITEM_OCARINA },
};

void comboSpawnSpecial(GameState_Play* play, float x, float y, float z, u8 specialId, u16 gi)
{
    SpawnActor(
        (char*)play + 0x1c24,
        play,
        AC_ITEM_CUSTOM,
        x, y, z,
        0, 0, 0x2000 | specialId,
        gi
    );
}

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
        if ((variable & 0xff) == 0x06) /* Heart Piece */
        {
            actorId = AC_ITEM_CUSTOM;
            rz = 0x1000 | ((variable >> 8) & 0x3f);
            variable = GI_OOT_HEART_PIECE;
        }
        break;
    case AC_DOOR_WARP1:
        /* Blue warp */
        switch (play->sceneId)
        {
        case SCE_LAIR_GOHMA:
            comboSpawnSpecial(play, x, y, z, EV_OOT_CHK_STONE_EMERALD, GI_OOT_STONE_EMERALD);
            SetEventChk(EV_OOT_CHK_EMERALD_TREE_DEAD);
            SetEventChk(EV_OOT_CHK_GOHMA);
            SetEventChk(EV_OOT_CHK_TREE_DEAD);
            SetEventChk(EV_OOT_CHK_MIDO_TREE_DEAD);
            break;
        case SCE_LAIR_KING_DODONGO:
            comboSpawnSpecial(play, x, y, z, EV_OOT_CHK_STONE_RUBY, GI_OOT_STONE_RUBY);
            break;
        case SCE_LAIR_BARINADE:
            comboSpawnSpecial(play, x, y, z, EV_OOT_CHK_STONE_SAPPHIRE, GI_OOT_STONE_SAPPHIRE);
            break;
        }
        break;
    case AC_ITEM_B_HEART:
        /* Heart container */
        /* Always use collectible flag 0x1f */
        actorId = AC_ITEM_CUSTOM;
        rz = 0x1000 | 0x1f;
        variable = GI_OOT_HEART_CONTAINER;
        break;
    case AC_BG_TOKI_SWD:
        comboSpawnCollectible(play, -1.f, 70.f, 80.f, 0x1f, GI_OOT_MASTER_SWORD);
        break;
    case AC_ITEM_ETCETERA:
        /* Special item */
        switch (variable & 0xff)
        {
        case 0x1:
            /* Ruto's letter */
            actorId = AC_ITEM_CUSTOM;
            rz = 0x2000 | EV_OOT_CHK_RUTO_LETTER;
            variable = GI_OOT_RUTO_LETTER;
            break;
        }
        break;
    }

    return SpawnActor(const_1, play, actorId, x, y, z, rx, ry, rz, variable);
}
