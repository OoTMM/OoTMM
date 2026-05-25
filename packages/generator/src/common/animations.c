#include <combo.h>
#include <combo/patch.h>
#include <combo/custom_animations.h>

#define ANIMTASK_LOAD_PLAYER_FRAME 0

#define COMBO_ANIM_TASK_REQ_OFFSET       0x04
#define COMBO_ANIM_TASK_MSGQUEUE_OFFSET  0x24
#define COMBO_ANIM_TASK_MSG_OFFSET       0x3c

typedef void ImportedAnimTaskQueue;
typedef void ImportedAnimTask;

typedef struct ComboDmaRequest {
    u32 vromAddr;
    void* dramAddr;
    u32 size;
    const char* filename;
    s32 line;
    u32 unk_14;
    OSMesgQueue* notifyQueue;
    OSMesg notifyMsg;
} ComboDmaRequest;

static ComboDmaRequest* Combo_AnimTaskReq(ImportedAnimTask* task) {
    return (ComboDmaRequest*)((u8*)task + COMBO_ANIM_TASK_REQ_OFFSET);
}

static OSMesgQueue* Combo_AnimTaskMsgQueue(ImportedAnimTask* task) {
    return (OSMesgQueue*)((u8*)task + COMBO_ANIM_TASK_MSGQUEUE_OFFSET);
}

static OSMesg* Combo_AnimTaskMsg(ImportedAnimTask* task) {
    return (OSMesg*)((u8*)task + COMBO_ANIM_TASK_MSG_OFFSET);
}

static int Combo_IsKseg0OrKseg1(void* ptr) {
    u32 p;

    p = (u32)ptr;
    return p >= 0x80000000 && p < 0xc0000000;
}

static int Combo_IsLikelySegmented(void* ptr) {
    return ((u32)ptr >> 24) < 0x10;
}

#if defined(GAME_OOT)

#define COMBO_ANIMTASKQUEUE_ADD_LOAD_PLAYER_FRAME_ADDR 0x8008b4c4
#define COMBO_ANIMTASKQUEUE_NEW_TASK_ADDR              0x8008b48c
#define COMBO_DMAMGR_REQUEST_ASYNC_ADDR                0x80000d28

#define COMBO_PLAY_ANIM_TASK_QUEUE_OFFSET 0x10b20
#define COMBO_FOREIGN_LINK_ANIMETION_VROM COMBO_LINK_ANIMETION_MM_VROM

#elif defined(GAME_MM)

#define COMBO_ANIMTASKQUEUE_ADD_LOAD_PLAYER_FRAME_ADDR 0x80135954
#define COMBO_ANIMTASKQUEUE_NEW_TASK_ADDR              0x8013591c
#define COMBO_ANIMATION_GET_LENGTH_ADDR                0x80134724
#define COMBO_ANIMATION_GET_LAST_FRAME_ADDR            0x80134748
#define COMBO_LIB_SEGMENTED_TO_VIRTUAL_ADDR            0x80100504
#define COMBO_DMAMGR_REQUEST_ASYNC_ADDR                0x80080c04

#define COMBO_PLAY_ANIM_TASK_QUEUE_OFFSET 0x17104
#define COMBO_FOREIGN_LINK_ANIMETION_VROM COMBO_LINK_ANIMETION_OOT_VROM

#endif

#if defined(GAME_OOT) || defined(GAME_MM)

#define COMBO_IMPORTED_ANIM_HEADER_ENTRY(symbol, sourceOffset, frameCount, frameSize) \
    const ComboImportedPlayerAnimationHeader symbol = {                              \
        frameCount,                                                                  \
        frameSize,                                                                   \
        (COMBO_IMPORTED_LINK_ANIM_SEGMENT << 24) | ((sourceOffset) & 0x00ffffff),     \
    };

COMBO_IMPORTED_LINK_ANIMATION_LIST(COMBO_IMPORTED_ANIM_HEADER_ENTRY)

#undef COMBO_IMPORTED_ANIM_HEADER_ENTRY

typedef ImportedAnimTask* (*ComboAnimTaskQueueNewTaskFn)(ImportedAnimTaskQueue* queue, s32 type);

#if defined(GAME_MM)
typedef void* (*ComboLibSegmentedToVirtualFn)(void* ptr);

#define Combo_Lib_SegmentedToVirtual \
    ((ComboLibSegmentedToVirtualFn)COMBO_LIB_SEGMENTED_TO_VIRTUAL_ADDR)
#endif

#if defined(GAME_OOT)
typedef s32 (*ComboDmaMgrRequestAsyncFn)(
    ComboDmaRequest* req,
    void* ram,
    u32 vrom,
    u32 size,
    u32 unk,
    OSMesgQueue* queue,
    OSMesg msg,
    const char* file,
    s32 line
);
#else
typedef s32 (*ComboDmaMgrRequestAsyncFn)(
    ComboDmaRequest* req,
    void* ram,
    u32 vrom,
    u32 size,
    u32 unk,
    OSMesgQueue* queue,
    OSMesg msg
);
#endif

#define Combo_AnimTaskQueue_NewTask \
    ((ComboAnimTaskQueueNewTaskFn)COMBO_ANIMTASKQUEUE_NEW_TASK_ADDR)

#define Combo_DmaMgr_RequestAsync \
    ((ComboDmaMgrRequestAsyncFn)COMBO_DMAMGR_REQUEST_ASYNC_ADDR)

static ComboImportedPlayerAnimationHeader* Combo_LinkAnimHeaderToVirtual(
    ComboImportedPlayerAnimationHeader* animation
) {
    if (Combo_IsKseg0OrKseg1(animation)) {
        return animation;
    }

    if (!Combo_IsLikelySegmented(animation)) {
        return NULL;
    }

#if defined(GAME_OOT)
    return SEGMENTED_TO_VIRTUAL(animation);
#else
    return Combo_Lib_SegmentedToVirtual(animation);
#endif
}

static u32 Combo_LinkAnimFrameSize(
    ComboImportedPlayerAnimationHeader* header,
    s32 limbCount
) {
    if (header->frameSize != 0) {
        return header->frameSize;
    }

    return sizeof(Vec3s) * limbCount + sizeof(s16);
}

static int Combo_LinkAnimFrameDmaInfo(
    ComboImportedPlayerAnimationHeader* animation,
    s32 frame,
    s32 limbCount,
    u32* outVrom,
    u32* outSize
) {
    ComboImportedPlayerAnimationHeader* header;
    u32 segment;
    u32 offset;
    u32 frameSize;
    u32 frameCount;

    header = Combo_LinkAnimHeaderToVirtual(animation);

    if (header == NULL) {
        return 0;
    }

    frameCount = header->frameCount;

    if (frameCount == 0) {
        return 0;
    }

    segment = header->linkAnimSegment >> 24;
    offset = header->linkAnimSegment & 0x00ffffff;
    frameSize = Combo_LinkAnimFrameSize(header, limbCount);
    offset += frameSize * frame;

    if (segment == COMBO_IMPORTED_LINK_ANIM_SEGMENT) {
        *outVrom = (COMBO_FOREIGN_LINK_ANIMETION_VROM + offset) | VROM_FOREIGN_OFFSET;
        *outSize = frameSize;
        return 1;
    }

    if (segment != 0x07) {
        return 0;
    }

    *outVrom = COMBO_LINK_ANIMETION_VROM + offset;
    *outSize = frameSize;

    return 1;
}

static void Combo_AnimTaskQueue_AddLoadPlayerFrame(
    void* play,
    ComboImportedPlayerAnimationHeader* animation,
    s32 frame,
    s32 limbCount,
    Vec3s* frameTable
) {
    ImportedAnimTaskQueue* queue;
    ImportedAnimTask* task;
    u32 size;
    u32 vrom;

    if (!Combo_LinkAnimFrameDmaInfo(animation, frame, limbCount, &vrom, &size)) {
        return;
    }

    queue = (ImportedAnimTaskQueue*)((u8*)play + COMBO_PLAY_ANIM_TASK_QUEUE_OFFSET);
    task = Combo_AnimTaskQueue_NewTask(queue, ANIMTASK_LOAD_PLAYER_FRAME);

    osCreateMesgQueue(
        Combo_AnimTaskMsgQueue(task),
        Combo_AnimTaskMsg(task),
        1
    );

#if defined(GAME_OOT)
    Combo_DmaMgr_RequestAsync(
        Combo_AnimTaskReq(task),
        frameTable,
        vrom,
        size,
        0,
        Combo_AnimTaskMsgQueue(task),
        NULL,
        "ComboLinkAnimationHooks",
        __LINE__
    );
#else
    Combo_DmaMgr_RequestAsync(
        Combo_AnimTaskReq(task),
        frameTable,
        vrom,
        size,
        0,
        Combo_AnimTaskMsgQueue(task),
        NULL
    );
#endif
}

#if defined(GAME_MM)

static s16 Combo_MM_Animation_GetLength(void* animation) {
    ComboImportedPlayerAnimationHeader* header;

    header = Combo_LinkAnimHeaderToVirtual(animation);

    if (header == NULL) {
        return 0;
    }

    return header->frameCount;
}

static s16 Combo_MM_Animation_GetLastFrame(void* animation) {
    ComboImportedPlayerAnimationHeader* header;

    header = Combo_LinkAnimHeaderToVirtual(animation);

    if (header == NULL || header->frameCount == 0) {
        return 0;
    }

    return header->frameCount - 1;
}

PATCH_FUNC(
    COMBO_ANIMATION_GET_LENGTH_ADDR,
    Combo_MM_Animation_GetLength
);

PATCH_FUNC(
    COMBO_ANIMATION_GET_LAST_FRAME_ADDR,
    Combo_MM_Animation_GetLastFrame
);

#endif

PATCH_FUNC(
    COMBO_ANIMTASKQUEUE_ADD_LOAD_PLAYER_FRAME_ADDR,
    Combo_AnimTaskQueue_AddLoadPlayerFrame
);

#endif
