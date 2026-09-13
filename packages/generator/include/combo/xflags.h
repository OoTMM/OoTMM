#ifndef COMBO_XFLAGS_H
#define COMBO_XFLAGS_H

#include <combo/types.h>

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

#define XFVF_SHUFFLED       0x01
#define XVFV_UNCOLLECTED    0x02

/* New API */
XflagID Xflag_InitEx(Actor* actor, PlayState* play);
XflagID Xflag_LookupSlice(XflagID id, int sliceId);
int Xflag_Get(XflagID id);
void Xflag_Set(XflagID id);
void Xflag_ItemQuery(ComboItemQuery* q, XflagID id, s16 gi);
void Xflag_ItemOverride(ComboItemOverride* o, XflagID id, s16 gi);
int Xflag_IsValidEx(XflagID id);
int Xflag_IsShuffledEx(XflagID id);

/* Legacy API */
XflagID Xflag_Lookup(const Xflag* xf);
void comboXflagItemQuery(ComboItemQuery* q, const Xflag* xf, s16 gi);
void comboXflagItemOverride(ComboItemOverride* o, const Xflag* xf, s16 gi);
int Xflag_GetIndirect(const Xflag* xf);
void Xflag_SetIndirect(const Xflag* xf);
int  Xflag_Init(Xflag* xf, Actor* actor, PlayState* play);
int  Xflag_IsValid(Xflag* xf);
int  Xflag_IsShuffled(Xflag* xf);
void Xflag_Clear(Xflag* xf);

#endif /* COMBO_XFLAGS_H */
