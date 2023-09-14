#include <combo.h>

extern Actor_ObjGrass* gObjGrass;

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

    /* Store a ref to the grass manager */
    gObjGrass = grass;
}
