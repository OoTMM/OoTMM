#ifndef COMBO_GI_H
#define COMBO_GI_H

#include <combo/types.h>
#include <combo/gi_data.h>

#define GI_NONE 0x00

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
