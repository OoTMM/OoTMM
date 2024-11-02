#include <combo.h>
#include <combo/csmc.h>
#include <combo/actor.h>

void ObjHana_DrawWrapper(Actor* this, PlayState* play)
{
    ActorFunc ObjHana_Draw;

    if ((this->params & 0x3) == 0x2)
        csmcGrassPreDraw(play, 0, CSMC_GRASS_NORMAL, 0, 0);

    /* Draw */
    ObjHana_Draw = actorAddr(ACTOR_OBJ_HANA, 0x80abc1c8);
    ObjHana_Draw(this, play);
}
