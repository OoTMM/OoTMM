#ifndef OOT_SAVE_H
#define OOT_SAVE_H

#include <types.h>

typedef struct PACKED
{
    u32     entrance;
    u32     age;
    u32     cutscene;
    u16     worldTime;
    u16     unk_0e;
    u32     isNight;
    char    unk_14[0x8];
    char    newf[6];
    s16     deathCount;
    char    playerName[8];
    char    data[0x132c];
}
OotSaveContext;

_Static_assert(sizeof(OotSaveContext) == 0x1358, "OotSaveContext size is wrong");

#endif /* OOT_SAVE_H */
