#ifndef LIBULTRA_H
#define LIBULTRA_H

#include <stddef.h>
#include <stdint.h>

#define OS_READ     0
#define OS_WRITE    1

#define OS_MESG_NOBLOCK   0
#define OS_MESG_BLOCK     1

#define PHYS_TO_K1(x) ((u32)((x) | 0xa0000000))

typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef void* OSMesg;

typedef struct
{
    void*       mtqueue;
    void*       fullqueue;
    s32         validCount;
    s32         first;
    s32         msgCount;
    OSMesg*     msg;
}
OSMesgQueue;

typedef struct
{
    u16             type;
    u8              pri;
    u8              status;
    OSMesgQueue*    retQueue;
}
OSIoMesgHdr;

typedef struct
{
    OSIoMesgHdr     hdr;
    void*           dramAddr;
    u32             devAddr;
    u32             size;
    void*           pihandle;
}
OSIoMesg;

void osCreateMesgQueue(OSMesgQueue* queue, OSMesg* buffer, s32 count);
void osRecvMesg(OSMesgQueue* queue, OSMesg* msg, s32 flag);

void osEPiRawReadIo(void* pihandle, u32 devAddr, u32* data);
void osEPiRawWriteIo(void* pihandle, u32 devAddr, u32 data);
s32  osEPiStartDma(void* pihandle, OSIoMesg* mb, s32 direction);

extern void* __osPiHandle;

#endif /* LIBULTRA_H */
