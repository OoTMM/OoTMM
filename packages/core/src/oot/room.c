#include <combo.h>
#include <combo/global.h>
#include <combo/actor.h>

u8 gCurrentSpawnActorNum = 0xff;

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

static void ZeroActor(Actor* this, int size)
{
    memset(this, 0, size);
    this->actorIndex = gCurrentSpawnActorNum;
}

PATCH_CALL(0x800252f8, ZeroActor);

void ParseSceneRoomHeaders_ActorsList(PlayState* play, void* cmd)
{
    /* Clear some flags */
    g.silverRupee = 0;
    g.roomEnemyLackSoul = 0;
    g.xflagOverride = FALSE;

    /* Update the scene setup */
    updateSceneSetup();

    /* Forward */
    _ParseSceneRoomHeaders_ActorsList(play, cmd);
}

void Actor_SpawnEntryFromRoomActorList(ActorContext* actorCtx, ActorEntry* entry, PlayState* play)
{
    gCurrentSpawnActorNum = (u8)(entry - play->actorEntryList);
    Actor_SpawnEntry(actorCtx, entry, play);
    gCurrentSpawnActorNum = 0xff;
}

PATCH_CALL(0x80023de8, Actor_SpawnEntryFromRoomActorList);
