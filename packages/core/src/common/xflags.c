#include <combo.h>
#include <combo/xflags.h>

#if defined(GAME_OOT)
u16 comboXflagsBitPosLookup(u16 sceneId, u16 setupId, u16 roomId, u16 id)
{
    return kXflagsTableRooms[kXflagsTableSetups[kXflagsTableScenes[sceneId] + setupId] + roomId] + id;
}

int comboXflagsGet(u16 sceneId, u16 setupId, u16 roomId, u16 id)
{
    u16 bitPos;

    bitPos = comboXflagsBitPosLookup(sceneId, setupId, roomId, id);
    return !!BITMAP8_GET(gCustomSave.xflags, bitPos);
}

void comboXflagsSet(u16 sceneId, u16 setupId, u16 roomId, u16 id)
{
    u16 bitPos;

    bitPos = comboXflagsBitPosLookup(sceneId, setupId, roomId, id);
    BITMAP8_SET(gCustomSave.xflags, bitPos);
}

#endif
