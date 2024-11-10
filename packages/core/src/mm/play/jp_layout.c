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

static void ComboPlay_SpawnLayoutDependantTorch(PlayState* play, float x, float y, float z)
{
    if (gSave.isNight)
    {
        Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_SYOKUDAI, x, y, z, 0x2, 0x7f, 0x55, 0x287f);
    }
    else
    {
        Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_SYOKUDAI, x, y, z, 0x5, 0x7f, 0x2a, 0x207f);
    }
}

void ComboPlay_JpLayout(PlayState* play)
{
    if (!Config_Flag(CFG_MM_JP_LAYOUT) || play->sceneId != SCE_MM_DEKU_PALACE)
        return;

    switch (play->roomCtx.curRoom.num)
    {
    case 0x01:
        /* Top grotto */
        Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_ANA, 354, 160, 712, 0x7, 0xf, 0x7f, 0x305);

        /* Right Climb Grotto */
        Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_ANA, 423, 0, 1340, 0x7, 0x11, 0x7f, 0x304);
        ComboPlay_SpawnLayoutDependantTorch(play, 421, 0, 1296);

        /* Rupees */
        ComboPlay_SpawnLayoutDependantRupee(play, 406, 80, 876, 0x1101, 0x0a);
        ComboPlay_SpawnLayoutDependantRupee(play, 439, 80, 825, 0x1201, 0x0b);
        break;
    }
}
