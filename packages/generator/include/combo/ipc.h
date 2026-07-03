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

#endif

