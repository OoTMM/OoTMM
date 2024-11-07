
#include <actors/Obj_Mure2/Obj_Mure2.h>

void ObjMure2_AliasGrass(Xflag* xf)
{
    switch (xf->sceneId)
    {
    case SCE_OOT_HYRULE_FIELD:
        switch (xf->setupId)
        {
        case 1: xf->id += 2; break;
        case 2: xf->id += 8; break;
        }
        xf->setupId = 0;
        break;
    case SCE_OOT_GRAVEYARD:
        switch (xf->setupId)
        {
        case 1: xf->id -= 4; break;
        case 2: xf->id -= 13; break;
        case 3: xf->id -= 13; break;
        }
        xf->setupId = 0;
        break;
    case SCE_OOT_ZORA_RIVER:
        if (xf->setupId == 2)
        {
            xf->id += 28;
            xf->setupId = 0;
        }
        break;
    case SCE_OOT_LAKE_HYLIA:
        if (xf->setupId == 2)
        {
            xf->setupId = 0;
            xf->id -= 4;
        }
        break;
    case SCE_OOT_KOKIRI_FOREST:
        if (xf->setupId == 3)
        {
            xf->setupId = 2;
            xf->id -= 7;
        }
        break;
    }
}

void ObjMure2_AliasRocks(Xflag* xf)
{

}
