#ifndef COMBO_GI_H
#define COMBO_GI_H

#include <types.h>

typedef struct PACKED ALIGNED(2)
{
    u8  itemId;
    u8  flags;
    u8  shaderId;
    u8  textId;
    u16 objectId;
}
GetItem;

extern const GetItem kExtendedGetItems[];

#endif /* COMBO_COMMON_GI_H */
