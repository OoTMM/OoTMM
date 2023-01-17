#include <combo.h>

#if defined(GAME_OOT)
# define OVERRIDE_ADDR 0x03fe1000
#else
# define OVERRIDE_ADDR 0x03fe3000
#endif

#define OVERRIDE_MAX 512

typedef struct PACKED
{
    u16 key;
    u16 value;
}
ComboOverride;

static ALIGNED(16) ComboOverride gComboOverrides[OVERRIDE_MAX];

void comboInitOverride(void)
{
    DMARomToRam(OVERRIDE_ADDR | PI_DOM1_ADDR2, &gComboOverrides, sizeof(gComboOverrides));
}

static u16 makeKey(u16 sceneId, u16 id)
{
#if defined(GAME_MM)
    switch (sceneId)
    {
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        sceneId = SCE_MM_SOUTHERN_SWAMP;
        break;
    case SCE_MM_TWIN_ISLANDS_SPRING:
        sceneId = SCE_MM_TWIN_ISLANDS_WINTER;
        break;
    case SCE_MM_GORON_VILLAGE_SPRING:
        sceneId = SCE_MM_GORON_VILLAGE_WINTER;
        break;
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
        sceneId = SCE_MM_MOUNTAIN_VILLAGE_WINTER;
        break;
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        sceneId = SCE_MM_TEMPLE_STONE_TOWER;
        break;
    }
#endif

    return (sceneId << 8) | id;
}

static s32 comboOverrideRaw(u16 key)
{
    for (int i = 0; i < OVERRIDE_MAX; ++i)
    {
        ComboOverride* o = &gComboOverrides[i];
        if (o->key == 0xffff)
            break;
        if (o->key == key)
            return o->value;
    }
    return -1;
}

static s16 comboOverrideImpl(u16 sceneId, u16 id, s16 gi, int flags)
{
    s32 override;
    u16 absGi;

    override = comboOverrideRaw(makeKey(sceneId, id));
#if defined(DEBUG) && defined(DEBUG_OVERRIDE)
    override = DEBUG_OVERRIDE;
#endif
    absGi = gi > 0 ? gi : -gi;
    if (override >= 0)
        absGi = override;

    if (!(flags & OVF_NO_PROGRESSIVE))
        absGi = comboProgressive(absGi);

    return gi > 0 ? absGi : -absGi;
}

s16 comboOverride(int type, u16 sceneId, u16 id, s16 gi)
{
    return comboOverrideEx(type, sceneId, id, gi, 0);
}

s16 comboOverrideEx(int type, u16 sceneId, u16 id, s16 gi, int flags)
{
    switch (type)
    {
    case OV_CHEST:
        id &= 0x3f;
        break;
    case OV_COLLECTIBLE:
        id = (id & 0x3f) | 0x40;
        break;
    case OV_SF:
        id = (id & 0x3f) | 0x80;
        break;
    case OV_NPC:
        sceneId = SCE_NPC;
        break;
    case OV_GS:
        sceneId = SCE_GS;
        break;
    }
    return comboOverrideImpl(sceneId, id, gi, flags);
}
