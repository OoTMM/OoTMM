#ifndef COMBO_GI_H
#define COMBO_GI_H

#define GI_NONE 0x00
#include <combo/gi_data.h>

#if !defined(__ASSEMBLER__)
# include <combo/types.h>

typedef struct PACKED ALIGNED(2)
{
    u8  itemId;
    u8  flags;
    u8  drawGiId;
    u8  textId;
    u16 objectId;
}
GetItem;

extern const GetItem kExtendedGetItems[];
extern const u8 kGetItemDrawGiParam[];

#endif

#endif /* COMBO_COMMON_GI_H */
