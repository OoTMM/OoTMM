#include <combo.h>

typedef struct PACKED
{
    u16 key;
    u16 value;
}
ComboOverride;

#define CHEST_KEY(scene, id) ((scene) | (id << 8))

static ALIGNED(16) ComboOverride gComboChestOverrides[256] = {
    { CHEST_KEY(0x0028, 0), 0xa },
    { 0xffff, 0 }
};

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
