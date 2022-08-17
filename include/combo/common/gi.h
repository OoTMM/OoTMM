#ifndef COMBO_COMMON_GI_H
#define COMBO_COMMON_GI_H

#include <types.h>

typedef struct PACKED ALIGNED(2)
{
    u8  item;
    u8  flag;
    u8  gi;
    u8  textId;
    u16 objectId;
}
GetItem;

extern const GetItem kExtendedGetItems[];

#endif /* COMBO_COMMON_GI_H */
