#include <combo.h>
#include <combo/xflags.h>
#include <combo/item.h>

u16 comboXflagsBitPosLookup(const Xflag* xf)
{
    u16 setupIndex;
    u16 roomIndex;

    setupIndex = kXflagsTableScenes[xf->sceneId] + xf->setupId;
    roomIndex = kXflagsTableSetups[setupIndex] + (xf->roomId * 12) + xf->sliceId;
    return kXflagsTableRooms[roomIndex] + xf->id;
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

void comboXflagItemQuery(ComboItemQuery* q, const Xflag* xf, s16 gi)
{
    bzero(q, sizeof(*q));
    q->ovType = OV_XFLAG0 + xf->sliceId;
    q->sceneId = xf->sceneId;
    q->roomId = (xf->roomId | ((xf->setupId & 3) << 6));
    q->id = xf->id;
    q->gi = gi;
}

void comboXflagItemOverride(ComboItemOverride* o, const Xflag* xf, s16 gi)
{
    ComboItemQuery q;

    comboXflagItemQuery(&q, xf, gi);
    comboItemOverride(o, &q);
}
