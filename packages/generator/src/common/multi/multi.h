#ifndef MULTI_H
#define MULTI_H

#include <combo.h>
#include <combo/multi.h>
#include <combo/ipc.h>

#define MULTI_OP_NOP            0x00
#define MULTI_OP_HELLO          0x01
#define MULTI_OP_WAL            0x02
#define MULTI_OP_WAL_QUERY      0x03
#define MULTI_OP_WAL_ACK        0x04
#define MULTI_OP_POSITION       0x05

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
    u32 token;
}
MultiPacketWalAckIn;

typedef struct PACKED
{
    MultiPacketHeader header;
    u32 token;
    u8 type;
}
MultiPacketWalOutHeader;

typedef struct PACKED
{
    MultiPacketHeader header;
    u32 index;
    u8 type;
}
MultiPacketWalInHeader;

typedef struct PACKED
{
    MultiPacketWalOutHeader wal;
    u8 to;
    u8 game;
    s16 gi;
    s16 flags;
    u32 key;
}
MultiPacketWalItemOut;

typedef struct PACKED
{
    MultiPacketWalInHeader wal;
    u8 from;
    u8 to;
    u8 game;
    s16 gi;
    s16 flags;
    u32 key;
    u8 playerName[8];
}
MultiPacketWalItemIn;

typedef struct PACKED
{
    MultiPacketHeader header;
    u32 index;
}
MultiPacketWalQuery;

typedef struct PACKED
{
    MultiPacketHeader header;
    u16 id;
    u16 color;
    u8 name[8];
    u16 key;
    s16 x;
    s16 y;
    s16 z;
}
MultiPacketPositionIn;

typedef struct PACKED
{
    MultiPacketHeader header;
    u16 key;
    s16 x;
    s16 y;
    s16 z;
}
MultiPacketPositionOut;

typedef struct
{
    u16 ttl;
    u16 ttlResend;
}
MultiState;

extern MultiState gMulti;

u32 Multi_CRC32(const void* data, int size);
int Multi_SendPacket(MultiPacketHeader* pkt, u32 size);
void Multi_SendPosition(PlayState* play);
void Multi_UpdateWisps(void);
void Multi_UpdateWisp(PlayState* play, MultiPacketPositionIn* pkt);

#endif
