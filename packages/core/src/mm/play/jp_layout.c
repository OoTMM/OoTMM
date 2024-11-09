#include <combo.h>

void ComboPlay_JpLayout(PlayState* play)
{
    if (play->sceneId != SCE_MM_DEKU_PALACE)
        return;

    switch (play->roomCtx.curRoom.num)
    {
    case 0x01:
        /* Right side */
        Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_SYOKUDAI, 352, 160, 655, 0x5, 0x7f, 0x2a, 0x207f);
        Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_SYOKUDAI, 450, 80, 655, 0x5, 0x7f, 0x2a, 0x207f);
        Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_SYOKUDAI, 352, 160, 655, 0x2, 0x7f, 0x55, 0x287f);
        Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_SYOKUDAI, 450, 80, 655, 0x2, 0x7f, 0x55, 0x287f);

        Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_ANA, 354, 160, 712, 0x7, 0xf, 0x7f, 0x305);
        Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_ANA, 451, 80, 726, 0x7, 0x10, 0x7f, 0x306);

        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ITEM00, 406, 80, 876, 0x7, 0x7f, 0x7f, 0x1101);
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ITEM00, 439, 80, 825, 0x7, 0x7f, 0x7f, 0x1201);
        break;
    }
}
