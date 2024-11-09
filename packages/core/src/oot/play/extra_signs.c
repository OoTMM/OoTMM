#include <combo.h>
#include <actors/En_Kanban/En_Kanban.h>

void ComboPlay_SpawnExtraSigns(PlayState* play)
{
    if (Config_Flag(CFG_ER_GROTTOS) && play->sceneId == SCE_OOT_GROTTOS && play->roomCtx.curRoom.num == 0x00)
    {
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_KANBAN,
            -50.f, 0.f, 70.f,
            0x0000, 0x2000, 0x0000,
            ENKANBAN_EXTRA_GROTTOS
        );
    }
}
