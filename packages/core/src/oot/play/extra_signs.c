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
        case SCE_OOT_GROTTOS:
            switch (play->roomCtx.curRoom.num)
            {
            case 0x00:
                /* Generic Grottos */
                ComboPlay_SpawnExtraSignLoc(play, -50.f, 0.f, 70.f, 0x2000);
                break;
            case 0x06:
                /* Lost Woods Upgrade Scrub */
                ComboPlay_SpawnExtraSignLoc(play, 530.f, 0.f, 1260.f, 0x2000);
                break;
            case 0x09:
                /* Scrubs x2 */
                ComboPlay_SpawnExtraSignLoc(play, 2350.f, 0.f, 1260.f, 0x2000);
                break;
            case 0x0c:
                /* Scrubs x3 */
                ComboPlay_SpawnExtraSignLoc(play, 4700.f, 0.f, 1260.f, 0x2000);
                break;
            }
            break;
        case SCE_OOT_FAIRY_FOUNTAIN:
            ComboPlay_SpawnExtraSignLoc(play, -90.f, 38.f, 250.f, 0x2000);
            break;
        }
    }

    if (Config_Flag(CFG_ER_INDOORS))
    {
        switch (play->sceneId)
        {
        case SCE_OOT_GREAT_FAIRY_FOUNTAIN_SPELLS:
        case SCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES:
            ComboPlay_SpawnExtraSignLoc(play, -90.f, 20.f, -150.f, 0x2000);
            break;
        }
    }
}
