#ifndef Z_EN_BB_H
#define Z_EN_BB_H

struct EnBb;

typedef void (*EnBbActionFunc)(struct EnBb*, struct PlayState*);

typedef struct EnBb {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[16];
    Vec3s morphTable[16];
    s32 action;
    s32 unk_254;
    EnBbActionFunc actionFunc;
    s32 moveMode;
    s32 timer;
    s32 unk_264;
    s16 actionState;
    s16 charge;
    s16 actionVar1;
    s16 actionVar2;
    s16 flameScrollMod;
    f32 bobPhase;
    f32 bobSize;
    f32 maxSpeedXZ;
    f32 flyHeightMod;
    f32 bobSpeedMod;
    f32 flameScaleY;
    f32 flameScaleX;
    Vec3f waypointPos;
    u8 path;
    u8 waypoint;
    u8 flamePrimBlue;
    u8 flamePrimAlpha;
    Color_RGB8 flameEnvColor;
    s32 blureIdx;
    s16 fireIceTimer;
    u8 dmgEffect;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[1];
    BodyBreak bodyBreak;
    Actor* targetActor;
} EnBb;

typedef enum EnBbType {
    ENBB_GREEN_BIG = -5,
    ENBB_GREEN,
    ENBB_WHITE,
    ENBB_RED,
    ENBB_BLUE,
    ENBB_FLAME_TRAIL,
    ENBB_KILL_TRAIL = 11
} EnBbType;

#endif
