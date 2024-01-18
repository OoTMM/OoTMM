#include <combo.h>
#include <combo/item.h>
#include <combo/csmc.h>

static void EnKusa2_GetXflag(Xflag* xflag, int id)
{
    xflag->sceneId = gPlay->sceneId;
    xflag->setupId = 0;
    xflag->roomId = 0;
    switch (xflag->sceneId)
    {
    case SCE_MM_MILK_ROAD:
        xflag->id = 6;
        break;
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
        xflag->id = 44;
        break;
    case SCE_MM_CLOCK_TOWN_NORTH:
        xflag->id = 16;
        break;
    }

    xflag->sliceId = id;
}

static void EnKusa2_SpawnDrop(GameState_Play* play, Vec3f* pos, u16 dropIndex)
{
    Xflag xflag;

    if ((s8)dropIndex > g.keatonGrassMax)
        g.keatonGrassMax = (s8)dropIndex;

    if (comboConfig(CFG_MM_SHUFFLE_GRASS))
    {
        /* Extract the ID and build the xflag */
        EnKusa2_GetXflag(&xflag, dropIndex);

        /* Check if the reward was already collected */
        if (!comboXflagsGet(&xflag))
        {
            EnItem00_DropCustom(gPlay, pos, &xflag);
            return;
        }
    }

    /* Run the default function */
    Item_DropCollectible(play, pos, dropIndex == 8 ? 2 : 0);
}

PATCH_CALL(0x80a5bd68, EnKusa2_SpawnDrop);

static void EnKusa2_Draw(GameState_Play* play)
{
    static u32 sLastFrameCount;
    static int sDrawIndex;

    Xflag xflag;
    ComboItemOverride o;
    int id;

    o.gi = 0;
    if (comboConfig(CFG_MM_SHUFFLE_GRASS))
    {
        /* Compute a nice ID */
        if (sLastFrameCount != play->gs.frameCount)
        {
            sLastFrameCount = play->gs.frameCount;
            sDrawIndex = 0;
        }

        if (g.keatonGrassMax == -1)
        {
            id = sDrawIndex++;
            id += (play->gs.frameCount / 5) % 9;
            id %= 9;
        }
        else
            id = g.keatonGrassMax + 1;

        /* Get the xflag and check  */
        EnKusa2_GetXflag(&xflag, id);
        if (!comboXflagsGet(&xflag))
            comboXflagItemOverride(&o, &xflag, 0);
    }

    /* Prepare */
    csmcGrassPreDraw(play, o.gi, CSMC_GRASS_NORMAL, 0, 0);

    /* Draw */
    DrawSimpleOpa(play, 0x50078a0);
}

PATCH_CALL(0x80a5e95c, EnKusa2_Draw);
PATCH_CALL(0x80a5ea30, EnKusa2_Draw);
PATCH_CALL(0x80a5ea70, EnKusa2_Draw);
