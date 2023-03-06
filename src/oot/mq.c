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
    u16 transitionActorCount;
    u16 transitionActorOffset;
}
MqSceneHeader;

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
MqRoomHeader;

#define ROOM_ACTORS_MAX     64
#define ROOM_OBJECTS_MAX    15
#define ROOM_BUFFER_SIZE    (0x10 * ROOM_ACTORS_MAX + 2 * ROOM_OBJECTS_MAX)

#define SCENE_TRANSITION_ACTORS_MAX     64
#define SCENE_BUFFER_SIZE               (0x10 * SCENE_TRANSITION_ACTORS_MAX)

ALIGNED(16) static char sMqBufferMisc[0x1000];
ALIGNED(16) static char sMqBufferRoom[ROOM_BUFFER_SIZE];
ALIGNED(16) static char sMqBufferRoom2[ROOM_BUFFER_SIZE];
ALIGNED(16) static char sMqBufferScene[SCENE_BUFFER_SIZE];

static char* sMqBufferRoomPtr = sMqBufferRoom;
static char* sMqBufferRoom2Ptr = sMqBufferRoom2;

static void swapMqRooms(void)
{
    char* tmp = sMqBufferRoomPtr;
    sMqBufferRoomPtr = sMqBufferRoom2Ptr;
    sMqBufferRoom2Ptr = tmp;
}

static int findMqOverrideScene(GameState_Play* play, MqSceneHeader* dst)
{
    u32             headerCount;
    MqSceneHeader*  header;
    u8              dungeonId;

    DMARomToRam(CUSTOM_MQ_SCENES_ADDR | PI_DOM1_ADDR2, sMqBufferMisc, sizeof(sMqBufferMisc));
    headerCount = *(u32*)sMqBufferMisc;
    dungeonId = 0;

    for (int i = 0; i < headerCount; ++i)
    {
        header = (MqSceneHeader*)(sMqBufferMisc + 0x20 + i * 0x20);
        if (header->dungeonId == dungeonId)
        {
            *dst = *header;
            return 1;
        }
    }

    return 0;
}

static int findMqOverrideRoom(GameState_Play* play, MqRoomHeader* dst)
{
    u32             headerCount;
    MqRoomHeader*   header;
    u8              dungeonId;

    DMARomToRam(CUSTOM_MQ_ROOMS_ADDR | PI_DOM1_ADDR2, sMqBufferMisc, sizeof(sMqBufferMisc));
    headerCount = *(u32*)sMqBufferMisc;
    dungeonId = 0;

    for (int i = 0; i < headerCount; ++i)
    {
        header = (MqRoomHeader*)(sMqBufferMisc + 0x20 + i * 0x20);
        if (header->dungeonId == dungeonId && header->roomId == play->roomCtx.curRoom.num)
        {
            *dst = *header;
            return 1;
        }
    }

    return 0;
}

static void loadMqSceneMaybe(GameState_Play* play)
{
    MqSceneHeader mqHeader;
    SceneRoomHeader* sceneHeader;
    int parseEnd;

    if (play->sceneId != SCE_OOT_INSIDE_DEKU_TREE)
        return;

    if (!findMqOverrideScene(play, &mqHeader))
        return;

    /* Load the MQ scene data */
    DMARomToRam((CUSTOM_MQ_SCENES_ADDR + mqHeader.offset) | PI_DOM1_ADDR2, sMqBufferScene, mqHeader.size);

    /* Patch the room */
    parseEnd = 0;
    sceneHeader = (SceneRoomHeader*)(gSegments[2] + 0x80000000);
    while (!parseEnd)
    {
        switch (sceneHeader->op)
        {
        case 0x14:
            parseEnd = 1;
            break;
        case 0x0e:
            sceneHeader->data1 = (u8)mqHeader.transitionActorCount;
            sceneHeader->data2 = (((u32)sMqBufferScene) + mqHeader.transitionActorOffset) - 0x80000000;
            break;
        }
        sceneHeader++;
    }
}

static void loadMqRoomMaybe(GameState_Play* play)
{
    MqRoomHeader mqHeader;
    SceneRoomHeader* roomHeader;
    int parseEnd;

    swapMqRooms();

    if (play->sceneId != SCE_OOT_INSIDE_DEKU_TREE)
        return;

    if (!findMqOverrideRoom(play, &mqHeader))
        return;

    /* Load the MQ room data */
    DMARomToRam((CUSTOM_MQ_ROOMS_ADDR + mqHeader.offset) | PI_DOM1_ADDR2, sMqBufferRoomPtr, mqHeader.size);

    /* Patch the room */
    parseEnd = 0;
    roomHeader = (SceneRoomHeader*)(gSegments[3] + 0x80000000);
    while (!parseEnd)
    {
        switch (roomHeader->op)
        {
        case 0x14:
            parseEnd = 1;
            break;
        case 0x01:
            roomHeader->data1 = (u8)mqHeader.actorCount;
            roomHeader->data2 = (((u32)sMqBufferRoomPtr) + mqHeader.actorOffset) - 0x80000000;
            break;
        case 0x0b:
            roomHeader->data1 = (u8)mqHeader.objectCount;
            roomHeader->data2 = (((u32)sMqBufferRoomPtr) + mqHeader.objectOffset) - 0x80000000;
            break;
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

static void Play_LoadScene_Hook(GameState_Play* play, void* unk)
{
    /* MQ */
    loadMqSceneMaybe(play);

    /* Forward the call */
    void (*callback)(GameState_Play*, void*);
    callback = (void*)0x8009cde8;
    callback(play, unk);
}

PATCH_CALL(0x8009cfd8, Play_LoadScene_Hook);
