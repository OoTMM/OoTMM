#ifndef INCLUDED_SYS_EMU_H
#define INCLUDED_SYS_EMU_H

#include <ultra64.h>
#include <combo/util.h>

void _emuSysCall(u32* args, int bufSize);
int emuSysCount(void);
int emuSysSocketOpen(int slot);
int emuSysSocketClose(int slot);
int emuSysSocketSend(int slot, const void* data, u32 size);
int emuSysSocketRecv(int slot, void* data, u32 size);
int emuSysSocketIsValid(int slot);
int emuSysSocketOpenAsync(int slot);

#endif
