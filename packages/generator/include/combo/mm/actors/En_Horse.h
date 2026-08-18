#ifndef COMBO_MM_EN_HORSE_H
#define COMBO_MM_EN_HORSE_H

#include <combo/actor.h>

#define HORSE_TYPE_EPONA 0
#define HORSE_TYPE_HNI 1
#define HORSE_TYPE_2 2
#define HORSE_TYPE_BANDIT 3
#define HORSE_TYPE_DONKEY 4
#define HORSE_TYPE_MAX 5

typedef struct EnHorse EnHorse;

typedef struct {
    SkeletonHeader* skeletonHeader;
    MtxF mtx;
    s32 limbCount;
    void* vtxTable;
    SkelAnime skelAnime;
} Skin;

typedef void (*EnHorsePostDrawFunc)(
    EnHorse* this,
    PlayState* play,
    Skin* skin
);

struct EnHorse {
    Actor actor;
    u32 action;
    u8 pad_148[0x08];
    s32 type;
    s8 objectSlot;
    u8 pad_155[0x03];
    Skin skin;
    u32 stateFlags;
    s32 unk_1EC;
    u8 pad_1F0[0x18];
    s32 playerControlled;
    s32 animIndex;
    f32 curFrame;
    u8 pad_214[0x2C];
    EnHorsePostDrawFunc postDrawFunc;
    u8 pad_244[0x350];
};

void EponaAge_OnHorseOverlayLoaded(void* loadedRam);
void EponaAge_Request(PlayState* play, s32 targetAdult);
s32 EponaAge_IsBusy(void);
void EponaAge_ClearReload(void);

void Skin_Setup(Skin* skin);
void Skin_Free(GameState* gameState, Skin* skin);

_Static_assert(sizeof(Skin) == 0x90, "MM Skin size is wrong");
_Static_assert(sizeof(EnHorse) == 0x594, "MM EnHorse size is wrong");

#endif