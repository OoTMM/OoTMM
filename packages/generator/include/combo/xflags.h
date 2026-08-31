#ifndef COMBO_XFLAGS_H
#define COMBO_XFLAGS_H

#include <combo/types.h>
#include <combo/data/xflags.h>

#define XFLAGID_NONE 0xffff
typedef u16 XflagID;

typedef struct ComboItemQuery ComboItemQuery;
typedef struct ComboItemOverride ComboItemOverride;
typedef struct Actor Actor;
typedef struct PlayState PlayState;

typedef struct Xflag
{
    u8 sceneId;
    u8 setupId;
    u8 roomId;
    u8 sliceId;
    u8 id;
}
Xflag;

XflagID Xflag_Lookup(const Xflag* xf);
int Xflag_Get(XflagID id);
void Xflag_Set(XflagID id);
int Xflag_GetIndirect(const Xflag* xf);
void Xflag_SetIndirect(const Xflag* xf);

void comboXflagItemQuery(ComboItemQuery* q, const Xflag* xf, s16 gi);
void comboXflagItemOverride(ComboItemOverride* o, const Xflag* xf, s16 gi);
int  Xflag_Init(Xflag* xf, Actor* actor, PlayState* play);
int  Xflag_IsValid(Xflag* xf);
int  Xflag_IsShuffled(Xflag* xf);
void Xflag_Clear(Xflag* xf);

#endif /* COMBO_XFLAGS_H */
