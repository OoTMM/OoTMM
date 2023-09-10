#ifndef COMBO_NET_H
#define COMBO_NET_H

#include <combo/types.h>

#define NET_MAGIC 0x905AB56A

#define NET_OP_NOP          0x00
#define NET_OP_ITEM_RECV    0x01
#define NET_OP_ITEM_SEND    0x02

typedef struct
{
    u8  playerFrom;
    u8  playerTo;
    u8  game;
    u8  zero;
    u32 key;
    s16 gi;
    s16 flags;
}
NetCmdItem;

ALIGNED(4) typedef struct
{
    u8 op;
    u8 pad[3];
    union
    {
        u8         raw[12];
        NetCmdItem itemRecv;
        NetCmdItem itemSend;
    };
}
NetCmd;

typedef struct
{
    u8*     uuid;
    u32     ledgerBase;
    NetCmd  cmdOut;
    NetCmd  cmdIn;
}
NetContext;

typedef struct
{
    volatile u32            magic;
    volatile NetContext*    ctx;
    volatile u32            mutexSystem;
    volatile u32            mutexScript;
}
NetGlobal;

void        netInit(void);
NetContext* netMutexLock(void);
void        netMutexUnlock(void);
void        netClose(void);
void        netWaitCmdClear(void);
void        netWaitSave(void);

#endif
