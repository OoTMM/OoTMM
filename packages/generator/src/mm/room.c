#include <combo.h>
#include <combo/global.h>
#include <combo/enemy.h>
#include <combo/actor.h>

u8 gCurrentSpawnActorNum = 0xff;

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

void updateSceneSetup(PlayState* play)
{
    for (int i = 0; i < ARRAY_COUNT(kExtraSceneSetups); ++i)
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

void ParseSceneRoomHeaders_ActorsList(PlayState* play, void* cmd)
{
    /* Clear some flags */
    g.silverRupee = 0;
    g.roomEnemyLackSoul = 0;
    g.xflagOverride = FALSE;

    /* Update the scene setup */
    updateSceneSetup(play);

    /* Forward */
    _ParseSceneRoomHeaders_ActorsList(play, cmd);
    comboEnemyEncounterBeginRoom(play, cmd, play->setupActorList, play->numSetupActors);
}

static void ZeroActor(Actor* this, int size)
{
    memset(this, 0, size);
    this->actorIndex = gCurrentSpawnActorNum;
}

PATCH_CALL(0x800baf54, ZeroActor);

PATCH_CALL(0x800b9430, comboEnemySpawnRoomEntry);

static void SpawnSetupActorsNativeFirst(PlayState* play, ActorContext* ctx)
{
    int batch = play->numSetupActors > 0;
    if (batch)
        comboEnemyBeginSpawnBatch();
    ((void (*)(PlayState*, ActorContext*))0x800b9334)(play, ctx);
    if (batch)
        comboEnemyEndSpawnBatch(ctx, play);
}
PATCH_CALL(0x800b97e8, SpawnSetupActorsNativeFirst);
