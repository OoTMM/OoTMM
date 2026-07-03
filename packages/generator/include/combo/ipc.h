#ifndef COMBO_IPC_H
#define COMBO_IPC_H

#define IPC_SYSTEM_NONE         0
#define IPC_SYSTEM_PROJECT64    1

typedef struct
{

}
IPCInterface;

void IPC_Init(void);

int IPC_GetSystem(void);
void IPC_Refresh(void);
int IPC_Read(void* buf, u32 len);
int IPC_Write(const void* buf, u32 len);
int IPC_IsConnected(void);

#endif

