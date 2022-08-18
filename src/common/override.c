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
