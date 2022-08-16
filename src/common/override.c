#include <combo.h>

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
    comboDma(&gComboChestOverrides, 0x03fe0000, sizeof(gComboChestOverrides));
}

s32 comboGetChestOverride(u16 scene, u8 id)
{
    u16 key;

    key = CHEST_KEY(scene, id);
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
