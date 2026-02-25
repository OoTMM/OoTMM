#ifndef COMBO_IO_H
#define COMBO_IO_H

#include <combo/types.h>

u8   IO_ReadPhysU8(u32 paddr);
s8   IO_ReadPhysI8(u32 paddr);
u16  IO_ReadPhysU16(u32 paddr);
s16  IO_ReadPhysI16(u32 paddr);
u32  IO_ReadPhysU32(u32 paddr);
s32  IO_ReadPhysI32(u32 paddr);

#endif
