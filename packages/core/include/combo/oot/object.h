#ifndef INCLUDED_COMBO_OOT_OBJECT_H
#define INCLUDED_COMBO_OOT_OBJECT_H

#include <assert.h>
#include <combo/util.h>

#define OBJECT_EXCHANGE_BANK_MAX 19

typedef struct {
    /* 0x00 */ s16         id;
    /* 0x04 */ void*       segment;
    /* 0x08 */ char        dmaRequest[0x20]; // DmaRequest  dmaRequest;
    /* 0x28 */ OSMesgQueue loadQueue;
    /* 0x40 */ OSMesg      loadMsg;
} ObjectStatus; // size = 0x44

typedef struct ObjectContext
{
    /* 0x0000 */ void*  spaceStart;
    /* 0x0004 */ void*  spaceEnd; // original name: "endSegment"
    /* 0x0008 */ u8     num; // number of objects in bank
    /* 0x0009 */ u8     unk_09;
    /* 0x000A */ u8     mainKeepIndex; // "gameplay_keep" index in bank
    /* 0x000B */ u8     subKeepIndex; // "gameplay_field_keep" or "gameplay_dangeon_keep" index in bank
    /* 0x000C */ ObjectStatus status[OBJECT_EXCHANGE_BANK_MAX];
}
ObjectContext; // size = 0x518

ASSERT_SIZE(ObjectContext, 0x518);

#endif
