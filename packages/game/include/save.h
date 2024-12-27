#ifndef SAVE_H
#define SAVE_H

#include <save/oot.h>

typedef struct
{
    OotSave oot;
}
Save;

extern Save gSave;

#define gOotSave (gSave.oot)

#endif
