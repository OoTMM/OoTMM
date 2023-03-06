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
    u8  dungeonId;
    u8  roomId;
    u16 actorCount;
    u32 extraSize;
    u32 actorOffset;
}
MqDataHeader;

ALIGNED(16) static char sMqBuffer[0x2000];

static int findMqOverride(GameState_Play* play, MqDataHeader* dst)
{
    u32             headerCount;
    MqDataHeader*   header;
    u8              dungeonId;

    DMARomToRam(CUSTOM_MQ_ADDR | PI_DOM1_ADDR2, sMqBuffer, sizeof(sMqBuffer));
    headerCount = *(u32*)sMqBuffer;
    dungeonId = 0;

    for (int i = 0; i < headerCount; ++i)
    {
        header = (MqDataHeader*)(sMqBuffer + 0x10 + i * 0x10);
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
    DMARomToRam((CUSTOM_MQ_ADDR + mqHeader.actorOffset) | PI_DOM1_ADDR2, sMqBuffer, mqHeader.extraSize);

    /* Patch the room */
    roomHeader = (SceneRoomHeader*)(gSegments[3] + 0x80000000);
    for (;;)
    {
        if (roomHeader->op == 0x14)
            break;
        if (roomHeader->op == 0x01)
        {
            roomHeader->data1 = (u8)mqHeader.actorCount;
            roomHeader->data2 = ((u32)sMqBuffer) - 0x80000000;
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
