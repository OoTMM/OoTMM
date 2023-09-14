#include <combo.h>

extern Actor_ObjGrass* gObjGrass;

static void ObjGrassUnit_Alias(Xflag* xflag)
{
    switch (xflag->sceneId)
    {
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        xflag->sceneId = SCE_MM_SOUTHERN_SWAMP;
        if (xflag->roomId == 0x00)
            xflag->id += 2;
        break;
    case SCE_MM_GROTTOS:
        if (xflag->roomId == 0x0a && gLastScene == SCE_MM_GREAT_BAY_COAST)
            xflag->roomId = 0x0f;
        break;
    }
}

void ObjGrassUnit_AfterSpawn(Actor* this)
{
    Actor_ObjGrass* grass;
    u16 grassPackId;
    Xflag* xflag;

    /* Obtain the grass manager */
    grass = *(Actor_ObjGrass**)(actorAddr(AC_OBJ_GRASS_UNIT, 0x809ac418));
    grassPackId = grass->count - 1;

    /* Set the Xflag for the grass pack */
    xflag = &grass->xflags[grassPackId];
    xflag->sceneId = gPlay->sceneId;
    xflag->setupId = g.sceneSetupId;
    xflag->roomId = this->room;
    xflag->sliceId = 0;
    xflag->id = g.actorIndex;

    /* Alias the Xflag */
    ObjGrassUnit_Alias(xflag);

    /* Store a ref to the grass manager */
    gObjGrass = grass;
}
