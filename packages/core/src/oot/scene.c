#include <combo.h>

static EntranceTableEntry defaultEntrance = {
    0, /* Deku Tree */
    0, /* From Entrance */
    TRANS_TYPE_FADE_BLACK
};

EntranceTableEntry* Entrance_GetTableEntryCustom(u16 entrance)
{
    if (entrance >= 0x1000)
    {
        return &defaultEntrance;
    }

    EntranceTableEntry* gEntranceTable = (EntranceTableEntry*)(0x800f9c90);

    return &gEntranceTable[entrance];
}

/**
 * Returns the transition effect flags from a given entrance index.
 */
s32 Entrance_GetTransitionFlags(u16 entrance) {
    return Entrance_GetTableEntryCustom(entrance)->flags;
}
