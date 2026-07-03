#ifndef IPC_H
#define IPC_H

#include <combo.h>
#include <combo/ipc.h>

typedef struct
{
    u8 systemType;
    u8 isConnected:1;
    u8 isInitialized:1;

    void (*cbRefresh)(void);
    int (*cbRead)(void*, u32);
    int (*cbWrite)(const void*, u32);
}
IPCState;

extern IPCState gIPC;

int IPC_Init_Project64(void);

int IPC_Init_Null(void);

#endif
