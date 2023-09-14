#include <combo.h>

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

    if (comboConfig(CFG_MM_SHUFFLE_GRASS))
    {
        /* Extract the ID and build the xflag */
        EnKusa2_GetXflag(&xflag, dropIndex);

        /* Check if the reward was already collected */
        if (!comboXflagsGet(&xflag))
        {
            DropCustomItem(gPlay, pos, &xflag);
            return;
        }
    }

    /* Run the default function */
    Item_DropCollectible(play, pos, dropIndex == 8 ? 2 : 0);
}

PATCH_CALL(0x80a5bd68, EnKusa2_SpawnDrop);
