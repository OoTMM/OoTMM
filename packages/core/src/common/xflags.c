#include <combo.h>
#include <combo/xflags.h>

u16 comboXflagsBitPosLookup(const Xflag* xf)
{
    return kXflagsTableRooms[kXflagsTableSetups[kXflagsTableScenes[xf->sceneId] + xf->setupId] + xf->roomId] + xf->id;
}

int comboXflagsGet(const Xflag* xf)
{
    u16 bitPos;

    bitPos = comboXflagsBitPosLookup(xf);
    return !!BITMAP8_GET(gCustomSave.xflags, bitPos);
}

void comboXflagsSet(const Xflag* xf)
{
    u16 bitPos;

    bitPos = comboXflagsBitPosLookup(xf);
    BITMAP8_SET(gCustomSave.xflags, bitPos);
}
