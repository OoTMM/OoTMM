#include <combo.h>

void updateSceneSetup(void)
{
    u32* cmd;
    u8 code;
    u8 seg;
    u32 segOffset;
    u32* altHeader;

    /* If the scene setup ID is zero, we're good */
    if (gSaveContext.sceneSetupId == 0)
    {
        g.sceneSetupId = 0;
        return;
    }

    /* If the scene_setup_index is > 3 then we're probably in a cutscene so just return -1. */
    if (gSaveContext.sceneSetupId > 3)
    {
        g.sceneSetupId = 0;
        return;
    }

    /* Parse the scene headers */
    cmd = (u32*)gPlay->sceneSegment;
    for (;;)
    {
        code = ((*cmd) >> 24);

        /* End of header */
        if (code == 0x14)
            break;

        /* Alternate header */
        if (code == 0x18)
        {
            seg = cmd[1] >> 24;
            segOffset = cmd[1] & 0x00ffffff;
            altHeader = (u32*)(gSegments[seg] + 0x80000000 + segOffset);

            for (int i = gSaveContext.sceneSetupId; i > 0; --i)
            {
                if (altHeader[i - 1] != 0)
                {
                    g.sceneSetupId = i;
                    return;
                }
            }
        }
        cmd += 2;
    }

    g.sceneSetupId = 0;
}

static void SpawnRoomActors_Wrapper(GameState_Play* play, int id)
{
    /* Spawn the normal room actors */
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
    g.actorIndex = 0;

    /* Update the scene setup */
    updateSceneSetup();

    OnRoomChangeOriginal = (void*)0x80081900;
    OnRoomChangeOriginal(arg1, arg2);
}

Actor* SpawnRoomActor(void* unk, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    Actor* a;

    a = comboSpawnActor(unk, play, actorId, x, y, z, rx, ry, rz, variable);
    if (a != NULL && actorId == AC_EN_ITEM00)
        EnItem00_XflagInitFreestanding((Actor_EnItem00*)a, play, g.actorIndex, 0);
    g.actorIndex++;
    return a;
}

PATCH_CALL(0x8002562c, SpawnRoomActor);
