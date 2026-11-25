#ifndef COMBO_XFLAGS_H
#define COMBO_XFLAGS_H

#include <combo/types.h>

#define XFLAGID_NONE 0xffff
typedef u16 XflagID;

typedef struct ComboItemQuery ComboItemQuery;
typedef struct ComboItemOverride ComboItemOverride;
typedef struct Actor Actor;
typedef struct PlayState PlayState;

typedef struct
{
    u8 sceneId;
    u8 setupId;
    u8 roomId;
    u8 sliceId;
    u8 id;
}
XflagQuery;

XflagID Xflag_Lookup(const XflagQuery* xf);
XflagID Xflag_LookupSlice(XflagID id, int sliceId);
XflagID Xflag_Init(Actor* actor, PlayState* play);
XflagID Xflag_InitShuffled(Actor* actor, PlayState* play);
int Xflag_Get(XflagID id);
void Xflag_Set(XflagID id);
void Xflag_ItemQuery(ComboItemQuery* q, XflagID id, s16 gi);
void Xflag_ItemOverride(ComboItemOverride* o, XflagID id, s16 gi);
int Xflag_IsValid(XflagID id);
int Xflag_IsShuffled(XflagID id);

#endif /* COMBO_XFLAGS_H */
