#ifndef LIBULTRA_H
#define LIBULTRA_H

#include <stddef.h>
#include <types.h>

#define OS_READ     0
#define OS_WRITE    1

#define OS_MESG_NOBLOCK   0
#define OS_MESG_BLOCK     1

#define PHYS_TO_K1(x) ((u32)((x) | 0xa0000000))

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
void osSendMesg(OSMesgQueue* queue, OSMesg* msg, s32 flag);

void osEPiRawReadIo(void* pihandle, u32 devAddr, u32* data);
void osEPiRawWriteIo(void* pihandle, u32 devAddr, u32 data);
s32  osEPiStartDma(void* pihandle, OSIoMesg* mb, s32 direction);

extern void* __osPiHandle;

/* GFX */
#define __gfxSetList(x) (__dlist = (u32*)(((u64*)(x))))
#define __gfxWrite(x, a, b)     \
    do {                        \
        __gfxSetList((x));      \
        __dlist[0] = (a);       \
        __dlist[1] = (b);       \
    } while (0)

#define __gfxColor(x, b)    ((((u32)(x)) & 0xff) << b)

#define G_MTX_NOPUSH        0x00
#define G_MTX_PUSH          0x01
#define G_MTX_MUL           0x00
#define G_MTX_LOAD          0x02
#define G_MTX_MODELVIEW     0x00
#define G_MTX_PROJECTION    0x04

#define G_MW_SEGMENT        0x06

#define gMoveWd(p, t, o, v)             __gfxWrite((p), (0xdb000000 | ((t) << 16) | (o)), (v))
#define gSPSegment(p, seg, base)        gMoveWd((p), G_MW_SEGMENT, (seg) * 4, (base))
#define gSPDisplayList(p, dl)           __gfxWrite((p), 0xde000000, (dl))
#define gSPMatrix(p, m, f)              __gfxWrite((p), (0xda380000 | (f)), ((u32)(m)))
#define gDPSetEnvColor(p, r, g, b, a)   __gfxWrite((p), 0xfb000000, __gfxColor(r, 24) | __gfxColor(g, 16) | __gfxColor(b, 8) | __gfxColor(a, 0))

#endif /* LIBULTRA_H */
