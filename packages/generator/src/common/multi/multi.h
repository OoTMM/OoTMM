#ifndef MULTI_H
#define MULTI_H

#include <combo.h>
#include <combo/multi.h>
#include <combo/ipc.h>

#define MULTI_OP_NOP   0x00
#define MULTI_OP_HELLO 0x01
#define MULTI_OP_WAL   0x02

#define WAL_ITEM 0x01

typedef struct PACKED
{
    u32 seq;
    u8 op;
}
MultiPacketHeader;

typedef struct PACKED
{
    MultiPacketHeader header;

    u8  magic[8];
    u8  sessionId[16];
    u8  sessionSecret[8];
    u8  playerId[16];
    u8  playerName[8];
    u8  worldId;
    u8  multi;
}
MultiPacketHelloOut;

typedef struct PACKED
{
    MultiPacketHeader header;

    u8  magic[8];
    u32 seqGame;
    u32 seqNet;
}
MultiPacketHelloIn;

typedef struct PACKED
{
    MultiPacketHeader header;
    u8 type;
}
MultiPacketWalHeader;

typedef struct PACKED
{
    MultiPacketWalHeader wal;
    u8 to;
    u8 game;
    s16 gi;
    s16 flags;
    u32 key;
}
MultiPacketWalItemOut;

typedef struct
{
    u8  isConnected:1;
    u32 seqGame;
    u32 seqNet;
    u16 ttl;
    u8  buffer[128];
}
MultiState;

extern MultiState gMulti;

#endif
