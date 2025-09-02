#include <combo.h>

static void ComboPlay_SpawnLayoutDependantRupee(PlayState* play, float x, float y, float z, s16 params, u8 actorIndex)
{
    Xflag_Clear(&g.xflag);
    g.xflag.sceneId = play->sceneId;
    g.xflag.roomId = 0x02;
    g.xflag.sliceId = 0;
    g.xflag.setupId = 0;
    g.xflag.id = actorIndex;
    g.xflagOverride = TRUE;
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ITEM00, x, y, z, 0, 0, 0, params);
    g.xflagOverride = FALSE;
}

static void ComboPlay_SpawnLayoutDependantTorch(PlayState* play, float x, float y, float z)
{
    s16 params;

    params = 0x207f;
    if (gSave.isNight)
        params |= 0x800;

    Actor_Spawn(&play->actorCtx, play, ACTOR_OBJ_SYOKUDAI, x, y, z, 0, 0, 0, params);
}

static void ComboPlay_JpLayoutDekuPalace(PlayState* play)
{
    if (!Config_Flag(CFG_MM_JP_LAYOUT_DEKU_PALACE) || play->sceneId != SCE_MM_DEKU_PALACE)
        return;

    switch (play->roomCtx.curRoom.num)
    {
    case 0x01:
        /* Top grotto */
        Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_ANA, 354, 160, 712, 0, 0, 0, 0x305);

        /* Right Climb Grotto */
        Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_ANA, 423, 0, 1340, 0, 0, 0, 0x304);
        ComboPlay_SpawnLayoutDependantTorch(play, 421, 0, 1296);

        /* Rupees */
        ComboPlay_SpawnLayoutDependantRupee(play, 406, 80, 876, 0x1101, 0x0a);
        ComboPlay_SpawnLayoutDependantRupee(play, 439, 80, 825, 0x1201, 0x0b);
        break;
    case 0x02:
        ComboPlay_SpawnLayoutDependantTorch(play, -1040, 0, 656);
        ComboPlay_SpawnLayoutDependantTorch(play, -420, 0, 1295);
        break;
    }
}

void ComboPlay_JpLayout(PlayState* play)
{
    switch (play->sceneId)
    {
    case SCE_MM_DEKU_PALACE:
        if (Config_Flag(CFG_MM_JP_LAYOUT_DEKU_PALACE))
            ComboPlay_JpLayoutDekuPalace(play);
        break;
    }
}
