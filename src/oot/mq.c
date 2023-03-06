#include <combo.h>
#include <combo/custom.h>

typedef struct
{
    u8  op;
    u8  data1;
    u32 data2;
}
SceneRoomHeader;

typedef struct
{
    u32 size;
    u32 offset;
    u8  dungeonId;
    u8  roomId;
    u16 actorCount;
    u16 actorOffset;
    u16 objectCount;
    u16 objectOffset;
}
MqDataHeader;

#define ROOM_ACTORS_MAX     64
#define ROOM_OBJECTS_MAX    15
#define ROOM_BUFFER_SIZE    (0x10 * ROOM_ACTORS_MAX + 0x10 * ROOM_OBJECTS_MAX)

ALIGNED(16) static char sMqBufferMisc[0x1000];
ALIGNED(16) static char sMqBufferRoom[ROOM_BUFFER_SIZE];

static int findMqOverride(GameState_Play* play, MqDataHeader* dst)
{
    u32             headerCount;
    MqDataHeader*   header;
    u8              dungeonId;

    DMARomToRam(CUSTOM_MQ_ROOMS_ADDR | PI_DOM1_ADDR2, sMqBufferMisc, sizeof(sMqBufferMisc));
    headerCount = *(u32*)sMqBufferMisc;
    dungeonId = 0;

    for (int i = 0; i < headerCount; ++i)
    {
        header = (MqDataHeader*)(sMqBufferMisc + 0x20 + i * 0x20);
        if (header->dungeonId == dungeonId && header->roomId == play->roomCtx.curRoom.num)
        {
            *dst = *header;
            return 1;
        }
    }

    return 0;
}


static void loadMqRoomMaybe(GameState_Play* play)
{
    MqDataHeader mqHeader;
    SceneRoomHeader* roomHeader;

    if (play->sceneId != SCE_OOT_INSIDE_DEKU_TREE)
        return;

    if (!findMqOverride(play, &mqHeader))
        return;

    /* Load the MQ room data */
    DMARomToRam((CUSTOM_MQ_ROOMS_ADDR + mqHeader.offset) | PI_DOM1_ADDR2, sMqBufferRoom, mqHeader.size);

    /* Patch the room */
    roomHeader = (SceneRoomHeader*)(gSegments[3] + 0x80000000);
    for (;;)
    {
        if (roomHeader->op == 0x14)
            break;
        if (roomHeader->op == 0x01)
        {
            roomHeader->data1 = (u8)mqHeader.actorCount;
            roomHeader->data2 = (((u32)sMqBufferRoom) + mqHeader.actorOffset) - 0x80000000;
        }
        roomHeader++;
    }
}

static void Play_LoadRoom_Hook(GameState_Play* play, void* unk)
{
    /* MQ */
    loadMqRoomMaybe(play);

    /* Forward the call */
    void (*callback)(GameState_Play*, void*);
    callback = (void*)0x800817a0;
    callback(play, unk);
}

PATCH_CALL(0x80080c00, Play_LoadRoom_Hook);
