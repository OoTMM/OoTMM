#ifndef COMBO_SAVE_H
#define COMBO_SAVE_H

#include <combo/oot/save.h>
#include <combo/mm/save.h>

/* Combo-specific save data */
typedef struct PACKED ALIGNED(4)
{
    u32     ootOcarinas;
    u32     ootTradeChild;
    u32     ootTradeAdult;
}
ComboSave;

extern ComboSave gComboSave;

#endif
