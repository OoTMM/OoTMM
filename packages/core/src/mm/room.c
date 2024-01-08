#include <combo.h>

u8 gActorNum;

typedef struct
{
    u8 sceneId;
    u8 extraCount;
}
ExtraSceneSetups;

static const ExtraSceneSetups kExtraSceneSetups[] = {
    { SCE_MM_IKANA_CANYON, 3 },
    { SCE_MM_PATH_MOUNTAIN_VILLAGE, 1 },
    { SCE_MM_GORON_SHRINE, 1 },
    { SCE_MM_ZORA_HALL, 1 },
    { SCE_MM_GREAT_BAY_COAST, 1 },
    { SCE_MM_ZORA_CAPE, 1 },
    { SCE_MM_DEKU_KING_CHAMBER, 1 },
    { SCE_MM_WOODFALL, 2 },
    { SCE_MM_PATH_SNOWHEAD, 1 },
    { SCE_MM_SNOWHEAD, 1 },
    { SCE_MM_GORON_RACETRACK, 1 },
    { SCE_MM_CLOCK_TOWN_NORTH, 1 },
};

void updateSceneSetup(GameState_Play* play)
{
    for (int i = 0; i < ARRAY_SIZE(kExtraSceneSetups); ++i)
    {
        if (play->sceneId == kExtraSceneSetups[i].sceneId)
        {
            if (gSaveContext.sceneSetupId > kExtraSceneSetups[i].extraCount)
                g.sceneSetupId = 0;
            else
                g.sceneSetupId = gSaveContext.sceneSetupId;
            return;
        }
    }

    g.sceneSetupId = 0;
}

static void SpawnRoomActors_Wrapper(GameState_Play* play, int id)
{
    /* Spawn the normal room actors */
    SpawnRoomActors(play, id);
}

PATCH_CALL(0x8012eb18, SpawnRoomActors_Wrapper);
PATCH_CALL(0x8012ec30, SpawnRoomActors_Wrapper);

void OnRoomChange(GameState_Play* play, void* arg2)
{
    void (*OnRoomChangeOriginal)(GameState_Play*, void*);

    /* Clear some flags */
    g.silverRupee = 0;
    g.roomEnemyLackSoul = 0;
    g.actorIndex = 0;

    /* Update the scene setup */
    updateSceneSetup(play);

    OnRoomChangeOriginal = (void*)0x8012f90c;
    OnRoomChangeOriginal(play, arg2);
}

Actor* SpawnRoomActorEx(void* unk, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3)
{
    Actor* a;

    g.actorIndex = gActorNum;
    a = comboSpawnActorEx(unk, play, actorId, x, y, z, rx, ry, rz, variable, ex1, ex2, ex3);
    if (a != NULL && actorId == AC_EN_ITEM00)
        EnItem00_XflagInitFreestanding((Actor_EnItem00*)a, play, g.actorIndex, 0);
    return a;
}

PATCH_CALL(0x800bb47c, SpawnRoomActorEx);
