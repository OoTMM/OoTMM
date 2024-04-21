#ifndef COMBO_IO_H
#define COMBO_IO_H

#include <combo/types.h>

u8   comboReadPhysU8(u32 paddr);
s8   comboReadPhysI8(u32 paddr);
u16  comboReadPhysU16(u32 paddr);
s16  comboReadPhysI16(u32 paddr);
u32  comboReadPhysU32(u32 paddr);
s32  comboReadPhysI32(u32 paddr);

#endif
