#include <combo.h>

#if defined(GAME_OOT)
# define ENTRANCES_ADDR 0x03fe8000
#else
# define ENTRANCES_ADDR 0x03fe9000
#endif

s8  gIsEntranceOverride;
s32 gLastEntrance;

typedef struct PACKED ALIGNED(4)
{
    s32 key;
    s32 value;
}
Entrance;

ALIGNED(16) static Entrance gEntrances[0x20];

void comboInitEntrances(void)
{
    DMARomToRam(ENTRANCES_ADDR | PI_DOM1_ADDR2, gEntrances, sizeof(gEntrances));
}

s32 comboEntranceOverride(s16 entranceId)
{
    for (int i = 0; i < ARRAY_SIZE(gEntrances); i++)
    {
        if (gEntrances[i].key == -1)
            break;
        if (gEntrances[i].key == (s32)((u16)entranceId & 0xffff))
            return gEntrances[i].value;
    }
    return -1;
}
