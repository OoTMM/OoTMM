#ifndef INCLUDED_SYS_EMU_H
#define INCLUDED_SYS_EMU_H

#include <ultra64.h>
#include <combo/util.h>

void _emuSysCall(u32* args, int bufSize);

int emuSysCount(void);
int emuSysValidIPC(void);
int emuSysOpenIPC(void);
int emuSysCloseIPC(void);
int emuSysSendIPC(const void* data, u32 size);
int emuSysRecvIPC(void* data, u32 bufSize);

#endif
