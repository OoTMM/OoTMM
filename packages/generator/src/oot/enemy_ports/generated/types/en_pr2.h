#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_PR2_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_PR2_H

#include <combo.h>

#ifndef OBJECT_PR_1_LIMB_01
#define OBJECT_PR_1_LIMB_01 1
#endif
#ifndef OBJECT_PR_1_LIMB_02
#define OBJECT_PR_1_LIMB_02 2
#endif
#ifndef OBJECT_PR_1_LIMB_03
#define OBJECT_PR_1_LIMB_03 3
#endif
#ifndef OBJECT_PR_1_LIMB_04
#define OBJECT_PR_1_LIMB_04 4
#endif
#ifndef OBJECT_PR_1_LIMB_05
#define OBJECT_PR_1_LIMB_05 5
#endif
#ifndef OBJECT_PR_1_LIMB_06
#define OBJECT_PR_1_LIMB_06 6
#endif
#ifndef OBJECT_PR_1_LIMB_07
#define OBJECT_PR_1_LIMB_07 7
#endif
#ifndef OBJECT_PR_1_LIMB_08
#define OBJECT_PR_1_LIMB_08 8
#endif
#ifndef OBJECT_PR_1_LIMB_09
#define OBJECT_PR_1_LIMB_09 9
#endif
#ifndef OBJECT_PR_1_LIMB_MAX
#define OBJECT_PR_1_LIMB_MAX 10
#endif
#ifndef OBJECT_PR_1_LIMB_NONE
#define OBJECT_PR_1_LIMB_NONE 0
#endif
#ifndef PR_2_LIMB_BODY
#define PR_2_LIMB_BODY 1
#endif
#ifndef PR_2_LIMB_JAW
#define PR_2_LIMB_JAW 3
#endif
#ifndef PR_2_LIMB_MAX
#define PR_2_LIMB_MAX 5
#endif
#ifndef PR_2_LIMB_NONE
#define PR_2_LIMB_NONE 0
#endif
#ifndef PR_2_LIMB_SKULL
#define PR_2_LIMB_SKULL 2
#endif
#ifndef PR_2_LIMB_TAIL
#define PR_2_LIMB_TAIL 4
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_ObjectPr1Limb
#define COMBO_ENEMY_ASSET_ENUM_ObjectPr1Limb
typedef s32 ObjectPr1Limb;
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_Pr2Limb
#define COMBO_ENEMY_ASSET_ENUM_Pr2Limb
typedef s32 Pr2Limb;
#endif
struct EnPr2;

typedef void (*EnPr2ActionFunc)(struct EnPr2*, PlayState*);

#define ENPR2_GET_TYPE(thisx) ((thisx)->params & 0xF)
#define ENPR2_GET_AGRO_DISTANCE(thisx) (((thisx)->params >> 4) & 0xFF)

typedef enum EnPr2Type {
    PR2_TYPE_PASSIVE,
    PR2_TYPE_SPAWNED,
    PR2_TYPE_RESIDENT,
    PR2_TYPE_PATHING,
    PR2_TYPE_BROKEN = 10
} EnPr2Type;

typedef enum EnPr2State {
    PR2_STATE_PATHING,
    PR2_STATE_IDLE,
    PR2_STATE_ATTACKING,
    PR2_STATE_DEAD,
} EnPr2State;

typedef struct EnPr2 {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[PR_2_LIMB_MAX];
    Vec3s morphTable[PR_2_LIMB_MAX];
    EnPr2ActionFunc actionFunc;
    s16 pathIndex;
    Path* path;
    s32 waypoint;
    s16 state;
    s16 bubbleToggle;
    s16 mainTimer;
    s16 idleTimer;
    s16 targetingTimer;
    s16 returnHomeTimer;
    s16 type;
    s8 unk1E2[2];
    s16 targetZRot;
    s16 targetYRot;
    s8 unk1E8[4];
    s16 primColor;
    s16 yawTowardsWaypoint;
    s16 wallCollisionAngleAdjustment;
    s16 wallCollisionCounter;
    s16 alpha;
    f32 animEndFrame;
    f32 slowLimbYaw;
    f32 waterSurfaceHeight;
    f32 scale;
    f32 agroDistance;
    f32 randomTargetHeightOffset;
    s32 animIndex;
    s8 unk214[4];
    s32 dropID;
    Vec3f waypointPos;
    Vec3f newHome;
    Vec3f limbPos[5];
    Vec3f limbJawPos;
    ColliderCylinder collider;
} EnPr2;

#endif
