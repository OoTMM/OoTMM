#ifndef Z_EN_FR_H
#define Z_EN_FR_H

#include <combo.h>

struct Actor_EnFr;

typedef void (*Actor_EnFrActionFunc)(struct Actor_EnFr*, struct PlayState*);
typedef void (*Actor_EnFrBlinkFunc)(struct Actor_EnFr*);

typedef enum FrogType {
    FROG_YELLOW,
    FROG_BLUE,
    FROG_RED,
    FROG_PURPLE,
    FROG_WHITE
} FrogType;

typedef enum FrogSongType {
    FROG_ZL,
    FROG_EPONA,
    FROG_SARIA,
    FROG_SUNS,
    FROG_SOT,
    FROG_STORMS,
    FROG_CHOIR_SONG,
    FROG_NO_SONG
} FrogSongType;

typedef struct Actor_EnFr {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[24];
    Vec3s morphTable[24];
    SkelAnime skelAnimeButterfly;
    Vec3s jointTableButterfly[8];
    Vec3s morphTableButterfly[8];
    Actor_EnFrActionFunc actionFunc;
    LightNode* lightNode;
    LightInfo lightInfo;
    s8 requiredObjectSlot;
    s32 jumpCounter;
    s32 blinkTimer;
    Actor_EnFrBlinkFunc blinkFunc;
    u8 eyeTexIndex;
    u8 isJumpingUp;
    u8 isBelowWaterSurfaceCurrent;
    u8 isBelowWaterSurfacePrevious;
    u8 isDeactivating;
    u8 isActive;
    u8 growingScaleIndex;
    u8 isGrowing;
    u8 ocarinaNoteIndex;
    u8 ocarinaNote;
    u8 songIndex;
    u8 isJumpingToFrogSong;
    char unk_384[0x02];
    s16 frogSongTimer;
    s16 reward;
    Vec3f posLogSpot;
    f32 xzDistToLogSpot;
    f32 unusedFloat;
    f32 scale;
    Actor* unusedButterflyActor;
    u8 isButterflyDrawn;
    s16 xyAngleButterfly;
    Vec3f posButterfly;
    Vec3f posButterflyLight;
    int npc;
} Actor_EnFr;

#endif
