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
    if (!Config_Flag(CFG_MM_JP_LAYOUT) || play->sceneId != SCE_MM_DEKU_PALACE)
        return;

    switch (play->roomCtx.curRoom.num)
    {
    case 0x01:
        /* Top grotto */
        Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_ANA, 354, 160, 712, 0x7, 0xf, 0x7f, 0x305);

        /* Rupees */
        ComboPlay_SpawnLayoutDependantRupee(play, 406, 80, 876, 0x1101, 0x0a);
        ComboPlay_SpawnLayoutDependantRupee(play, 439, 80, 825, 0x1201, 0x0b);
        break;
    }
}
