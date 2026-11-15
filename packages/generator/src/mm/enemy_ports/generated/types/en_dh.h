#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_DH_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_DH_H

#include <combo.h>

struct EnDh;

typedef void (*EnDhActionFunc)(struct EnDh*, struct PlayState*);

typedef struct EnDh {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[16];
    Vec3s limbRotTable[16];
    u8 actionState;
    u8 retreat;
    u8 drawDirtWave;
    EnDhActionFunc actionFunc;
    u8 unk_258;
    u8 handsPlaced;
    u8 alpha;
    u8 curAction;
    s16 timer;
    s16 dirtWavePhase;
    ColliderCylinder collider1;
    ColliderJntSph colliderJntSph;
    ColliderJntSphElement colliderJntSphElements[1];
    Vec3f headPos;
    f32 dirtWaveSpread;
    f32 dirtWaveHeight;
    f32 dirtWaveAlpha;
} EnDh;

typedef enum EnDhParams {
    ENDH_HANDS_KILLED_4 = -4,
    ENDH_HANDS_KILLED_3,
    ENDH_HANDS_KILLED_2,
    ENDH_HANDS_KILLED_1,
    ENDH_WAIT_UNDERGROUND,
    ENDH_START_ATTACK_GRAB,
    ENDH_START_ATTACK_BOMB = 5,
    ENDH_DEATH = 10
} EnDhParams;

#endif
