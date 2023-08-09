#include <combo.h>
#include <combo/csmc.h>
#include <combo/item.h>

static void ObjHana_KokiriGrassXflag(Xflag* xflag)
{
    xflag->sceneId = SCE_OOT_KOKIRI_FOREST;
    xflag->setupId = 0;
    xflag->roomId = 0;
    xflag->sliceId = 0;
    xflag->id = 0x3b;
}

void ObjHana_DrawWrapper(Actor* this, GameState_Play* play)
{
    ActorFunc ObjHana_Draw;
    ComboItemOverride o;
    Xflag xflag;

    /* Fake grass */
    if ((this->variable & 0x3) == 0x2)
    {
        ObjHana_KokiriGrassXflag(&xflag);
        if (comboConfig(CFG_OOT_SHUFFLE_GRASS) && !comboXflagsGet(&xflag))
            comboXflagItemOverride(&o, &xflag, 0);
        else
            o.gi = 0;
        csmcGrassPreDraw(play, o.gi, CSMC_GRASS_NORMAL, 0);
    }

    /* Draw */
    ObjHana_Draw = actorAddr(AC_OBJ_HANA, 0x80abc1c8);
    ObjHana_Draw(this, play);
}

static void ObjHana_KokiriCut(Actor* this)
{
    Xflag xflag;

    /* Fake grass */
    ObjHana_KokiriGrassXflag(&xflag);
    if (comboConfig(CFG_OOT_SHUFFLE_GRASS) && !comboXflagsGet(&xflag))
    {
        DropCustomItem(gPlay, &this->position, &xflag);
    }
    ActorDestroy(this);
}

PATCH_CALL(0x80abc0f0, ObjHana_KokiriCut);
