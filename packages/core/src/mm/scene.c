#include <combo.h>

static EntranceTableEntry defaultEntrance = {
    0, // Mayor's Residence
    0, // From Door
    TRANS_TYPE_FADE_BLACK
};

EntranceTableEntry* Entrance_GetTableEntryCustom(u16 entrance)
{
    if (entrance >= 0xF000)
    {
        return &defaultEntrance;
    }

    return Entrance_GetTableEntry(entrance);
}

PATCH_CALL(0x8013231c, Entrance_GetTableEntryCustom);
PATCH_CALL(0x80132348, Entrance_GetTableEntryCustom);
PATCH_CALL(0x80132384, Entrance_GetTableEntryCustom);
PATCH_CALL(0x801323b0, Entrance_GetTableEntryCustom);
