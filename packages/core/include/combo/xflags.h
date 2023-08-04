#ifndef COMBO_XFLAGS_H
#define COMBO_XFLAGS_H

#include <combo/types.h>
#include <combo/xflags_data.h>

typedef struct Xflag
{
    u8 sceneId;
    u8 setupId;
    u8 roomId;
    u8 id;
}
Xflag;

extern const u16 kXflagsTableScenes[];
extern const u16 kXflagsTableSetups[];
extern const u16 kXflagsTableRooms[];

u16  comboXflagsBitPosLookup(const Xflag* xf);
int  comboXflagsGet(const Xflag* xf);
void comboXflagsSet(const Xflag* xf);

#endif /* COMBO_XFLAGS_H */
