#ifndef SAVE_H
#define SAVE_H

#include <save/oot.h>
#include <save/mm.h>

typedef struct
{
    OotSave oot;
    MmSave  mm;
}
Save;

extern Save gSave;

#define gOotSave (gSave.oot)
#define gMmSave  (gSave.mm)

#endif
