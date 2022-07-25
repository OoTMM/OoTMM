#ifndef OOT_H
#define OOT_H

#include <libultra.h>

void PreparePI(u32 devAddr, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8);
void ReadWriteSaveDMA(void* dramAddr, u32 size, u32 write);

#endif /* OOT_H */
