#include <combo.h>

static void ComboPlay_SpawnLayoutDependantRupee(PlayState* play, float x, float y, float z, s16 params, u8 actorIndex)
{
    Actor_EnItem00* item;
    Xflag xflag;

    item = (Actor_EnItem00*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ITEM00, x, y, z, 0x7, 0x7f, 0x7f, params);
    if (item)
    {
        memset(&xflag, 0, sizeof(xflag));
        xflag.sceneId = play->sceneId;
        xflag.roomId = 0x02;
        xflag.sliceId = 0;
        xflag.setupId = 0;
        xflag.id = actorIndex;

        EnItem00_XflagInit(item, &xflag);
    }
}

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

        ComboPlay_SpawnLayoutDependantRupee(play, 406, 80, 876, 0x1101, 0x0a);
        ComboPlay_SpawnLayoutDependantRupee(play, 439, 80, 825, 0x1201, 0x0b);
        break;
    }
}
