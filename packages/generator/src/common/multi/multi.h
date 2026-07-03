#ifndef MULTI_H
#define MULTI_H

#include <combo.h>
#include <combo/multi.h>
#include <combo/ipc.h>

#define MULTI_OP_NOP   0x00
#define MULTI_OP_HELLO 0x01

typedef struct PACKED
{
    u16 seqGame;
    u16 seqNet;
    u8 op;
}
MultiPacketHeader;

typedef struct PACKED
{
    MultiPacketHeader header;
    u8 magic[8];
}
MultiPacketHelloOut;

typedef struct
{
    u8  isConnected:1;
    u16 seqGame;
    u16 seqNet;
    u16 ttl;
    u8  buffer[128];
}
MultiState;

extern MultiState gMulti;

#endif
