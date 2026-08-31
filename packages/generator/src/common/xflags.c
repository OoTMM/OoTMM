#include <combo.h>
#include <combo/custom.h>
#include <combo/dma.h>
#include <combo/global.h>
#include <combo/io.h>
#include <combo/item.h>
#include <combo/play.h>
#include <combo/xflags.h>

#define CACHE_SIZE 32

typedef struct
{
    u32 key;
    u16 id;
    u16 zero;
}
XflagCheckData;

static u32 sXflagDevAddr;
static XflagCheckData sCache[CACHE_SIZE];
static int sCacheIndex;

void Xflag_InitSystem(void)
{
    DmaEntry e;

    comboDmaLookup(&e, COMBO_VROM_XFLAGS);
    sXflagDevAddr = e.pstart | PI_DOM1_ADDR2;
    for (int i = 0; i < CACHE_SIZE; ++i)
    {
        sCache[i].key = 0xffffffff;
        sCache[i].id = 0;
        sCache[i].zero = 0;
    }
    sCacheIndex = 0;
}

static u32 Xflag_GetKey(const Xflag* xf)
{
    u32 key;

    key = 0;
    key |= xf->id;
    key |= (xf->roomId & 0x3f) << 8;
    key |= (xf->setupId & 0x3) << 16;
    key |= (xf->sceneId & 0xff) << 18;

#if defined(GAME_MM)
    key |= 0x80000000;
#endif

    return key;
}

XflagID Xflag_LookupKey(u32 key)
{
    u32 cartKey;
    u16 cartId;
    u32 min;
    u32 max;
    u32 cursor;

    /* Cache lookup */
    for (int i = 0; i < CACHE_SIZE; ++i)
    {
        if (sCache[i].key == key)
            return sCache[i].id;
    }

    /* Cache lookup did not work, binary search */
    min = 0;
    max = XFLAGS_COUNT_IDS;

    for (;;)
    {
        if (min >= max)
        {
            cartId = XFLAGID_NONE;
            break;
        }

        cursor = (min + max) / 2;
        cartKey = IO_ReadPhysU32(sXflagDevAddr + cursor * sizeof(XflagCheckData) + 0x00);
        if (cartKey == key)
        {
            cartId = IO_ReadPhysU16(sXflagDevAddr + cursor * sizeof(XflagCheckData) + 0x04);
            break;
        }

        if (key > cartKey)
            min = cursor + 1;
        else
            max = cursor;
    }

    sCache[sCacheIndex].key = key;
    sCache[sCacheIndex].id = cartId;
    sCacheIndex = (sCacheIndex + 1) % CACHE_SIZE;
    return cartId;
}

XflagID Xflag_LookupSlice(XflagID id, int sliceId)
{
    u32 key;

    if (id == XFLAGID_NONE)
        return XFLAGID_NONE;
    if (sliceId == 0)
        return id;

    key = 0x40000000 | ((sliceId & 0x3f) << 16) | id;
    return Xflag_LookupKey(key);
}

XflagID Xflag_Lookup(const Xflag* xf)
{
    XflagID id;
    u32 key;

    if (xf->sceneId == 0xff)
        return XFLAGID_NONE;
    key = Xflag_GetKey(xf);
    id = Xflag_LookupKey(key);
    id = Xflag_LookupSlice(id, xf->sliceId);
    return id;
}

int Xflag_Get(XflagID id)
{
    if (id == XFLAGID_NONE)
        return 1;
    return BITMAP8_GET(gSharedCustomSave.xflags, id);
}

void Xflag_Set(XflagID id)
{
    if (id == XFLAGID_NONE)
        return;
    BITMAP8_SET(gSharedCustomSave.xflags, id);
}

int Xflag_GetIndirect(const Xflag* xf)
{
    return Xflag_Get(Xflag_Lookup(xf));
}

void Xflag_SetIndirect(const Xflag* xf)
{
    Xflag_Set(Xflag_Lookup(xf));
}

void comboXflagItemQuery(ComboItemQuery* q, const Xflag* xf, s16 gi)
{
    XflagID id;

    bzero(q, sizeof(*q));
    id = Xflag_Lookup(xf);
    if (id == XFLAGID_NONE)
    {
        q->ovType = OV_NONE;
    }
    else
    {
        q->ovType = OV_XFLAG;
        q->roomId = (id >> 8) & 0xff;
        q->id = id & 0xff;
    }
    q->gi = gi;
}

void comboXflagItemOverride(ComboItemOverride* o, const Xflag* xf, s16 gi)
{
    ComboItemQuery q;

    comboXflagItemQuery(&q, xf, gi);
    comboItemOverride(o, &q);
}

int Xflag_Init(Xflag* xf, Actor* actor, PlayState* play)
{
    if (g.xflagOverride)
    {
        memcpy(xf, &g.xflag, sizeof(*xf));
        return FALSE;
    }

    /* Check for an actor index of 0xff, meaning NO XFLAG */
    if (actor->actorIndex == 0xff)
    {
        Xflag_Clear(xf);
        return FALSE;
    }

    xf->sceneId = play->sceneId;
    xf->setupId = g.sceneSetupId;
    xf->roomId = actor->room;
    xf->sliceId = 0;
    xf->id = actor->actorIndex;

#if defined(GAME_OOT)
    xf->sceneId = Play_ExpandMQ(play, xf->sceneId);
    switch (xf->sceneId)
    {
    case SCE_OOT_GROTTOS:
        switch (xf->roomId)
        {
        case 0x00:
            /* Generic grottos */
            xf->roomId = 0x20 | (gGrottoData & 0x1f);
            break;
        case 0x09:
            /* Scrubs x2 */
            switch (gLastScene)
            {
            case SCE_OOT_SACRED_FOREST_MEADOW: xf->roomId = 0x21; break;
            case SCE_OOT_ZORA_RIVER: xf->roomId = 0x24; break;
            case SCE_OOT_GERUDO_VALLEY: xf->roomId = 0x25; break;
            case SCE_OOT_DESERT_COLOSSUS: xf->roomId = 0x26; break;
            default: UNREACHABLE(); break;
            }
            break;
        case 0x0c:
            /* Scrubs x3 */
            switch (gLastScene)
            {
            case SCE_OOT_LON_LON_RANCH: xf->roomId = 0x27; break;
            case SCE_OOT_GORON_CITY: xf->roomId = 0x2a; break;
            case SCE_OOT_DEATH_MOUNTAIN_CRATER: xf->roomId = 0x2b; break;
            case SCE_OOT_LAKE_HYLIA: xf->roomId = 0x2d; break;
            default: UNREACHABLE(); break;
            }
            break;
        }
        break;
    case SCE_OOT_FAIRY_FOUNTAIN:
        switch (gLastScene)
        {
        case SCE_OOT_HYRULE_FIELD: xf->roomId = 0x20; break;
        case SCE_OOT_ZORA_RIVER: xf->roomId = 0x21; break;
        case SCE_OOT_SACRED_FOREST_MEADOW: xf->roomId = 0x22; break;
        case SCE_OOT_ZORA_DOMAIN: xf->roomId = 0x23; break;
        case SCE_OOT_GERUDO_FORTRESS: xf->roomId = 0x24; break;
        default: UNREACHABLE(); break;
        }
        break;
    }
#endif

#if defined(GAME_MM)
    if (xf->sceneId == SCE_MM_GROTTOS)
    {
        switch (xf->roomId)
        {
        case 0x04:
            /* Generic Grottos */
            xf->roomId = 0x20 | (gGrottoData & 0x1f);
            break;
        case 0x0a:
            /* Cow grottos */
            if (gLastScene == SCE_MM_GREAT_BAY_COAST)
                xf->roomId = 0x0f;
            break;
        }
    }
#endif

    return TRUE;
}

int Xflag_IsValid(Xflag* xf)
{
    ComboItemOverride o;

    /* Fast check */
    if (xf->sceneId == 0xff)
        return FALSE;

    comboXflagItemOverride(&o, xf, GI_NONE);
    if (o.gi == GI_NONE)
    {
        xf->sceneId = 0xff;
        return FALSE;
    }

    return TRUE;
}

int Xflag_IsShuffled(Xflag* xf)
{
    return Xflag_IsValid(xf) && !Xflag_GetIndirect(xf);
}

void Xflag_Clear(Xflag* xf)
{
    xf->sceneId = 0xff;
    xf->setupId = 0;
    xf->roomId = 0;
    xf->sliceId = 0;
    xf->id = 0;
}
