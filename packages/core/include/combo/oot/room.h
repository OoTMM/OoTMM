#ifndef INCLUDED_COMBO_OOT_ROOM_H
#define INCLUDED_COMBO_OOT_ROOM_H

#include <combo/util.h>

typedef struct
{
    s8      num;
    u8      unk_01;
    u8      behaviorType2;
    u8      behaviorType1;
    s8      echo;
    u8      lensMode;
    void*   roomShape;
    void*   segment;
    char    unk_10[0x4];
}
Room; /* size = 0x14 */

_Static_assert(sizeof(Room) == 0x14, "OoT Room size is wrong");

typedef struct
{
    Room            curRoom;
    Room            prevRoom;
    void*           bufPtrs[2];
    u8              unk_30;
    s8              status;
    void*           unk_34;
    char            dmaRequest[0x20];
    OSMesgQueue     loadQueue;
    OSMesg          loadMsg;
    s16             unk_74[2]; /* context-specific data used by the current scene draw config */
}
RoomContext;

_Static_assert(sizeof(RoomContext) == 0x78, "OoT Room size is wrong");

#endif
