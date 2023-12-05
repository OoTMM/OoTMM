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

typedef struct PACKED ALIGNED(2)
{
    GetItem native[MASK_FOREIGN_GI];
    GetItem foreign[];
}
ExtendedItemTable;

extern const ExtendedItemTable kExtendedGetItemsTable;
#define kExtendedGetItems ((const GetItem*)(kExtendedGetItemsTable.native))

extern const u8 kGetItemDrawGiParamOot[];
extern const u8 kGetItemDrawGiParamMm[];

#endif

#endif /* COMBO_COMMON_GI_H */
