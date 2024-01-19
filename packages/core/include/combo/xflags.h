#ifndef COMBO_XFLAGS_H
#define COMBO_XFLAGS_H

#include <combo/types.h>
#include <combo/xflags_data.h>

#if defined(GAME_OOT)
# define comboXflagsGet     comboXflagsGetOot
# define comboXflagsSet     comboXflagsSetOot
#else
# define comboXflagsGet     comboXflagsGetMm
# define comboXflagsSet     comboXflagsSetMm
#endif

typedef struct ComboItemQuery ComboItemQuery;
typedef struct ComboItemOverride ComboItemOverride;

typedef struct Xflag
{
    u8 sceneId;
    u8 setupId;
    u8 roomId;
    u8 sliceId;
    u8 id;
}
Xflag;

int  comboXflagsGetOot(const Xflag* xf);
int  comboXflagsGetMm(const Xflag* xf);
void comboXflagsSetOot(const Xflag* xf);
void comboXflagsSetMm(const Xflag* xf);
void comboXflagItemQuery(ComboItemQuery* q, const Xflag* xf, s16 gi);
void comboXflagItemOverride(ComboItemOverride* o, const Xflag* xf, s16 gi);

#endif /* COMBO_XFLAGS_H */
