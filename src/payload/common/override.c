#include <combo.h>

#if defined(GAME_OOT)
# define CHEST_OVERRIDE_ADDR 0x03fe1000
#else
# define CHEST_OVERRIDE_ADDR 0x03fe2000
#endif

typedef struct PACKED
{
    u16 key;
    u16 value;
}
ComboOverride;

#define CHEST_KEY(scene, id) (((scene) << 8) | (id))

static ALIGNED(16) ComboOverride gComboChestOverrides[256];

void comboInitOverride(void)
{
    comboDma(&gComboChestOverrides, CHEST_OVERRIDE_ADDR, sizeof(gComboChestOverrides));
}

s32 comboGetOverride(u16 key)
{
    /* DEBUG */
    return GI_MM_MIRROR_SHIELD;

    for (int i = 0; i < 256; ++i)
    {
        ComboOverride* o = &gComboChestOverrides[i];
        if (o->key == 0xffff)
            break;
        if (o->key == key)
            return o->value;
    }
    return -1;
}

s32 comboGetChestOverride(u16 scene, u8 id)
{
    return comboGetOverride(CHEST_KEY(scene, id));
}

s32 comboGetCollectibleOverride(u16 scene, u8 id)
{
    return comboGetOverride(CHEST_KEY(scene, (id | 0x20)));
}

s32 comboGetSpecialOverride(u8 id)
{
    return comboGetOverride(CHEST_KEY(0xf0, id));
}

s32 comboGetNpcOverride(u8 id)
{
    return comboGetOverride(CHEST_KEY(0xf1, id));
}
