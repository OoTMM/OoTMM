#ifndef INCLUDED_COMBO_MM_OBJECT_H
#define INCLUDED_COMBO_MM_OBJECT_H

#include <assert.h>
#include <combo/util.h>

#define OBJECT_EXCHANGE_BANK_MAX 35

typedef struct
{
    s16         id;
    void*       segment;
    DmaRequest  dmaRequest;
    OSMesgQueue loadQueue;
    OSMesg      loadMsg;
}
ObjectStatus;

ASSERT_OFFSET(ObjectStatus, id, 0x00);
ASSERT_OFFSET(ObjectStatus, segment, 0x04);
ASSERT_OFFSET(ObjectStatus, dmaRequest, 0x08);
ASSERT_OFFSET(ObjectStatus, loadQueue, 0x28);
ASSERT_OFFSET(ObjectStatus, loadMsg, 0x40);

ASSERT_SIZE(ObjectStatus, 0x44);

typedef struct ObjectContext
{
    void*        spaceStart;
    void*        spaceEnd; /* original name: "endSegment" */
    u8           num; /* number of objects in bank */
    u8           numPersistentEntries;
    u8           mainKeepIndex; /* "gameplay_keep" index in bank */
    u8           subKeepIndex; /* "gameplay_field_keep" or "gameplay_dangeon_keep" index in bank */
    ObjectStatus slots[OBJECT_EXCHANGE_BANK_MAX];
}
ObjectContext;

ASSERT_OFFSET(ObjectContext, spaceStart, 0x00);
ASSERT_OFFSET(ObjectContext, spaceEnd, 0x04);
ASSERT_OFFSET(ObjectContext, num, 0x08);
ASSERT_OFFSET(ObjectContext, numPersistentEntries, 0x09);
ASSERT_OFFSET(ObjectContext, mainKeepIndex, 0x0A);
ASSERT_OFFSET(ObjectContext, subKeepIndex, 0x0B);
ASSERT_OFFSET(ObjectContext, slots, 0x0C);

ASSERT_SIZE(ObjectContext, 0x958);

#endif
