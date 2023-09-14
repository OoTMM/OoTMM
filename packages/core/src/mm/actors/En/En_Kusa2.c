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
