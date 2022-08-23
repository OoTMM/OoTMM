#ifndef COMBO_SAVE_H
#define COMBO_SAVE_H

#include <combo/oot/save.h>
#include <combo/mm/save.h>

/* Combo-specific save data */
typedef struct PACKED ALIGNED(4)
{
    u16     ootTradeChild;
    u16     ootTradeAdult;
    u8      ootOcarinas;
    u8      ootHookshots;
    u8      ootShields;
}
ComboSave;

extern ComboSave gComboSave;

#endif
