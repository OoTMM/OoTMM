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

#define KEY(scene, id) (((scene) << 8) | (id))

static ALIGNED(16) ComboOverride gComboOverrides[OVERRIDE_MAX];

void comboInitOverride(void)
{
    DMARomToRam(OVERRIDE_ADDR | PI_DOM1_ADDR2, &gComboOverrides, sizeof(gComboOverrides));
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

static s16 comboOverrideImpl(u16 sceneId, u16 id, s16 gi)
{
    s32 override;
    u16 absGi;

    override = comboOverrideRaw(KEY(sceneId, id));
    absGi = gi > 0 ? gi : -gi;
    if (override >= 0)
        absGi = override;
    absGi = comboProgressive(absGi);

    return gi > 0 ? absGi : -absGi;
}

s16 comboOverride(int type, u16 sceneId, u16 id, s16 gi)
{
    switch (type)
    {
    case OV_CHEST:
        id &= 0x3f;
        break;
    case OV_COLLECTIBLE:
        id = (id & 0x3f) | 0x40;
        break;
    case OV_SPECIAL:
        sceneId = SCE_SPECIAL;
        break;
    case OV_NPC:
        sceneId = SCE_NPC;
        break;
    }
    return comboOverrideImpl(sceneId, id, gi);
}
