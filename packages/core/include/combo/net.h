#ifndef COMBO_NET_H
#define COMBO_NET_H

#include <combo/types.h>

#define NET_MAGIC 0x905AB56A

#define NET_OP_NOP          0x00
#define NET_OP_ITEM_RECV    0x01
#define NET_OP_ITEM_SEND    0x02

#define NET_MSG_MAX       32
#define NETMSG_PLAYER_POS 0x01

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
    u32 frameCount;
    u16 sceneKey;
    s16 x;
    s16 y;
    s16 z;
}
NetMsgPlayerPos;

ALIGNED(4) typedef struct
{
    u8 op;
    u8 pad[3];
    union
    {
        u8              raw[28];
        NetMsgPlayerPos playerPos;
    };
}
NetMsg;

typedef struct
{
    u8*     uuid;
    u32     ledgerBase;
    NetCmd  cmdOut;
    NetCmd  cmdIn;
    u8      msgInSize[NET_MSG_MAX];
    u8      msgOutSize[NET_MSG_MAX];
    u16     msgClientId[NET_MSG_MAX];
    NetMsg  msgBuffer[NET_MSG_MAX];
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
