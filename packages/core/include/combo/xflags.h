#ifndef COMBO_XFLAGS_H
#define COMBO_XFLAGS_H

#include <combo/types.h>
#include <combo/xflags_data.h>

extern const u16 kXflagsTableScenes[];
extern const u16 kXflagsTableSetups[];
extern const u16 kXflagsTableRooms[];

u16 comboXflagsBitPosLookup(u16 sceneId, u16 setupId, u16 roomId);

#endif /* COMBO_XFLAGS_H */
