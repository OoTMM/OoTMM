#include <combo.h>
#include <combo/csmc.h>

void ObjHana_DrawWrapper(Actor* this, GameState_Play* play)
{
    ActorFunc ObjHana_Draw;

    if ((this->variable & 0x3) == 0x2)
        csmcGrassPreDraw(play, 0, CSMC_GRASS_NORMAL, 0);

    /* Draw */
    ObjHana_Draw = actorAddr(AC_OBJ_HANA, 0x80abc1c8);
    ObjHana_Draw(this, play);
}
