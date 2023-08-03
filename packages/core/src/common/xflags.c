#include <combo.h>
#include <combo/xflags.h>

#if defined(GAME_OOT)
u16 comboXflagsBitPosLookup(u16 sceneId, u16 setupId, u16 roomId)
{
    return kXflagsTableRooms[kXflagsTableSetups[kXflagsTableScenes[sceneId] + setupId] + roomId];
}
#endif
