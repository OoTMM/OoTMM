#ifndef IMPL_MULTI_H
#define IMPL_MULTI_H

#include <combo.h>
#include <combo/multi.h>

#define MAX_PACKET_SIZE 512

#define OP_NOP              0x00
#define OP_WRITE_WAL_ITEM   0x01
#define OP_ECHANGE_POS      0x02

typedef struct
{
    u32     token;
    u32     id;
    char    data[MAX_PACKET_SIZE - 8];
    u16     size;
}
MultiPacket;

typedef struct
{
    char        sessionId[16];
    u32         sessionSecret;
    char        playerId[16];
    u8          worldId;
    u32         token;
    u32         nextPacketId;
    int         isConnected;
    MultiPacket pkt;
}
MultiContext;
extern MultiContext gMultiCtx;

int Multi_ProtocolSend(void);
int Multi_ProtocolRecv(void);

#endif
