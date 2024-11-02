#ifndef Z_EN_PAMETFROG_H
#define Z_EN_PAMETFROG_H

#include <combo.h>

struct Actor_EnPametfrog;

typedef void (*Actor_EnPametfrogActionFunc)(struct Actor_EnPametfrog*, PlayState*);

typedef enum GekkoLimb
{
    GEKKO_LIMB_NONE,
    GEKKO_LIMB_ROOT,
    GEKKO_LIMB_WAIST,
    GEKKO_LIMB_LEFT_THIGH,
    GEKKO_LIMB_LEFT_SHIN,
    GEKKO_LIMB_LEFT_ANKLE,
    GEKKO_LIMB_LEFT_FOOT,
    GEKKO_LIMB_RIGHT_THIGH,
    GEKKO_LIMB_RIGHT_SHIN,
    GEKKO_LIMB_RIGHT_ANKLE,
    GEKKO_LIMB_RIGHT_FOOT,
    GEKKO_LIMB_TORSO,
    GEKKO_LIMB_LEFT_UPPER_ARM,
    GEKKO_LIMB_LEFT_FOREARM,
    GEKKO_LIMB_LEFT_WRIST,
    GEKKO_LIMB_LEFT_HAND,
    GEKKO_LIMB_RIGHT_UPPER_ARM,
    GEKKO_LIMB_RIGHT_FOREARM,
    GEKKO_LIMB_RIGHT_WRIST,
    GEKKO_LIMB_RIGHT_HAND,
    GEKKO_LIMB_HEAD,
    GEKKO_LIMB_JAW,
    GEKKO_LIMB_LEFT_EYE,
    GEKKO_LIMB_RIGHT_EYE,
    GEKKO_LIMB_MAX
}
GekkoLimb;

typedef enum
{
    GEKKO_PRE_SNAPPER,
    GEKKO_GET_SNAPPER,
    GEKKO_INIT_SNAPPER,
    GEKKO_ON_SNAPPER,
    GEKKO_REAR_ON_SNAPPER,
    GEKKO_CUTSCENE,
    GEKKO_FALL_OFF_SNAPPER,
    GEKKO_RETURN_TO_SNAPPER,
    GEKKO_JUMP_ON_SNAPPER,
    GEKKO_DEFEAT
}
EnPametfrogState;

typedef enum GekkoBodyPart
{
    GEKKO_BODYPART_NONE = -1,
    GEKKO_BODYPART_WAIST,
    GEKKO_BODYPART_LEFT_SHIN,
    GEKKO_BODYPART_LEFT_FOOT,
    GEKKO_BODYPART_RIGHT_SHIN,
    GEKKO_BODYPART_RIGHT_FOOT,
    GEKKO_BODYPART_LEFT_UPPER_ARM,
    GEKKO_BODYPART_LEFT_FOREARM,
    GEKKO_BODYPART_LEFT_HAND,
    GEKKO_BODYPART_RIGHT_UPPER_ARM,
    GEKKO_BODYPART_RIGHT_FOREARM,
    GEKKO_BODYPART_RIGHT_HAND,
    GEKKO_BODYPART_JAW,
    GEKKO_BODYPART_MAX
} GekkoBodyPart;

typedef struct Actor_EnPametfrog
{
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[GEKKO_LIMB_MAX];
    Vec3s morphTable[GEKKO_LIMB_MAX];
    Actor_EnPametfrogActionFunc actionFunc;
    u8 drawDmgEffType;
    u8 wallPauseTimer;
    u8 unk_2AE;
    s16 csId;
    s16 params;
    s16 quakeIndex;
    s16 timer;
    s16 spinYaw;
    s16 subCamId;
    s16 freezeTimer;
    f32 wallRotation;
    f32 drawDmgEffAlpha;
    f32 drawDmgEffScale;
    f32 drawDmgEffFrozenSteamScale;
    Vec3f unk_2D0;
    Vec3f unk_2DC;
    Vec3f unk_2E8;
    Vec3f bodyPartsPos[GEKKO_BODYPART_MAX];
    ColliderJntSph collider;
    ColliderJntSphElement colElement[2];
}
Actor_EnPametfrog;

#endif
