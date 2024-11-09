#include <combo.h>
#include <actors/En_Kanban/En_Kanban.h>

static void ComboPlay_SpawnExtraSign(PlayState* play, float x, float y, float z, s16 ry, s16 params)
{
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_KANBAN,
        x, y, z,
        0, ry, 0,
        params
    );
}

static void ComboPlay_SpawnExtraSignLoc(PlayState* play, float x, float y, float z, s16 ry)
{
    ComboPlay_SpawnExtraSign(play, x, y, z, ry, ENKANBAN_LOCATION_NAME);
}

void ComboPlay_SpawnExtraSigns(PlayState* play)
{
    if (Config_Flag(CFG_ER_GROTTOS))
    {
        switch (play->sceneId)
        {
        case SCE_MM_GROTTOS:
            switch (play->roomCtx.curRoom.num)
            {
            case 0x04:
                /* Generic Grottos */
                ComboPlay_SpawnExtraSignLoc(play, 2330.f, -5.f, -40.f, 0x2000);
                break;
            case 0x0a:
                /* Cow Grottos */
                ComboPlay_SpawnExtraSignLoc(play, 2350.f, 0.f, 1200.f, 0x2000);
                break;
            }
            break;
        }
    }
}
