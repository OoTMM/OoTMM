#ifndef Z_EN_ST_H
#define Z_EN_ST_H

struct EnSt;

typedef void (*EnStActionFunc)(struct EnSt* this, struct PlayState* play);

typedef struct EnSt {
    Actor actor;
    SkelAnime skelAnime;
    EnStActionFunc actionFunc;
    ColliderCylinder colliderCylinders[6];
    ColliderJntSph colliderJntSph;
    ColliderJntSphElement colliderJntSphElements[1];
    s16 initialYaw;
    s16 deathYawTarget;
    s16 groundBounces;
    s16 animFrames;
    s16 swayTimer;
    s16 setTargetYawTimer;
    s16 rotAwayTimer;
    s16 rotTowardsTimer;
    s16 takeDamageSpinTimer;
    s16 stunTimer;
    s16 invulnerableTimer;
    s16 sfxTimer;
    s16 gaveDamageSpinTimer;
    s16 finishDeathTimer;
    s16 deathTimer;
    s16 absPrevSwayAngle;
    u8 playSwayFlag;
    u8 teethR;
    u8 teethG;
    u8 teethB;
    char unk_3E0[4];
    Vec3f unusedPos;
    Vec3f ceilingPos;
    char unk_3FC[0x8];
    s32 blureIdx;
    f32 colliderScale;
    f32 floorHeightOffset;
    s16 swayAngle;
    Vec3s jointTable[30];
    Vec3s morphTable[30];
} EnSt;

#endif
