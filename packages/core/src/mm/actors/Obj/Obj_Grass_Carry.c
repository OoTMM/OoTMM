#include <combo.h>

void ObjGrass_GetXflag(Xflag* xflag, Actor_ObjGrass_PackBush* bush);

void ObjGrassCarry_InitWrapper(Actor_EnKusa* this, GameState_Play* play)
{
    ActorFunc init;

    /* Set dummy xflags (will be set by Obj_Grass) */
    memset(&this->xflag, 0, sizeof(Xflag));
    this->xflag.sceneId = 0xff;

    /* Forward init */
    init = actorAddr(AC_OBJ_GRASS_CARRY, 0x809ab2f0);
    init(&this->base, play);
}

void ObjGrassCarry_SpawnDropsWrapper(Actor_ObjGrassCarry* this)
{
    Xflag xflag;

    if (comboConfig(CFG_MM_SHUFFLE_GRASS))
    {
        /* Extract the ID and build the xflag */
        ObjGrass_GetXflag(&xflag, this->bush);

        /* Check if the reward was already collected */
        if (!comboXflagsGet(&xflag))
        {
            DropCustomItem(gPlay, &this->base.position, &xflag);
            return;
        }
    }
}
