#ifndef COMBO_OVL_BOSS_01_H
#define COMBO_OVL_BOSS_01_H

#include <combo.h>

struct Actor_Boss01;

#define ODOLWA_GET_TYPE(thisx) ((thisx)->variable)
#define ODOLWA_GET_AFTERIMAGE_DESPAWN_TIMER(thisx) ((thisx)->world.rot.z)

#define ODOLWA_SHADOW_TEX_WIDTH 64
#define ODOLWA_SHADOW_TEX_HEIGHT 64
#define ODOLWA_SHADOW_TEX_SIZE ((s32)sizeof(u8[ODOLWA_SHADOW_TEX_HEIGHT][ODOLWA_SHADOW_TEX_WIDTH]))

typedef void (*Actor_Boss01ActionFunc)(struct Actor_Boss01*, GameState_Play*);

typedef enum OdolwaType
{
    ODOLWA_TYPE_ODOLWA,
    ODOLWA_TYPE_BUG = 10,
    ODOLWA_TYPE_AFTERIMAGE = 35
}
OdolwaType;

typedef enum OdolwaBodyPart
{
    ODOLWA_BODYPART_HEAD,
    ODOLWA_BODYPART_TORSO,
    ODOLWA_BODYPART_PELVIS,
    ODOLWA_BODYPART_LEFT_UPPER_ARM,
    ODOLWA_BODYPART_LEFT_FOREARM,
    ODOLWA_BODYPART_SHIELD,
    ODOLWA_BODYPART_RIGHT_UPPER_ARM,
    ODOLWA_BODYPART_RIGHT_FOREARM,
    ODOLWA_BODYPART_SWORD,
    ODOLWA_BODYPART_RIGHT_THIGH,
    ODOLWA_BODYPART_RIGHT_SHIN,
    ODOLWA_BODYPART_RIGHT_FOOT,
    ODOLWA_BODYPART_LEFT_THIGH,
    ODOLWA_BODYPART_LEFT_SHIN,
    ODOLWA_BODYPART_LEFT_FOOT,
    ODOLWA_BODYPART_MAX
}
OdolwaBodyPart;

typedef enum OdolwaColliderBodyPart
{
    ODOLWA_COLLIDER_BODYPART_HEAD,
    ODOLWA_COLLIDER_BODYPART_TORSO,
    ODOLWA_COLLIDER_BODYPART_PELVIS,
    ODOLWA_COLLIDER_BODYPART_LEFT_UPPER_ARM,
    ODOLWA_COLLIDER_BODYPART_LEFT_FOREARM,
    ODOLWA_COLLIDER_BODYPART_RIGHT_UPPER_ARM,
    ODOLWA_COLLIDER_BODYPART_RIGHT_FOREARM,
    ODOLWA_COLLIDER_BODYPART_LEFT_THIGH,
    ODOLWA_COLLIDER_BODYPART_LEFT_SHIN,
    ODOLWA_COLLIDER_BODYPART_RIGHT_THIGH,
    ODOLWA_COLLIDER_BODYPART_RIGHT_SHIN,
    ODOLWA_COLLIDER_BODYPART_MAX
}
OdolwaColliderBodyPart;

typedef enum OdolwaSwordCollider
{
    ODOLWA_SWORD_COLLIDER_SWORD_BASE,
    ODOLWA_SWORD_COLLIDER_SWORD_TIP,
    ODOLWA_SWORD_COLLIDER_PELVIS,
    ODOLWA_SWORD_COLLIDER_MAX
}
OdolwaSwordCollider;

typedef enum OdolwaShieldCollider
{
    ODOLWA_SHIELD_COLLIDER_SHIELD,
    ODOLWA_SHIELD_COLLIDER_MAX
}
OdolwaShieldCollider;

typedef enum OdolwaKickAndShieldBashCollider
{
    ODOLWA_KICK_AND_SHIELD_BASH_COLLIDER_SHIELD,
    ODOLWA_KICK_AND_SHIELD_BASH_COLLIDER_LEFT_FOOT,
    ODOLWA_KICK_AND_SHIELD_BASH_COLLIDER_MAX
}
OdolwaKickAndShieldBashCollider;

typedef enum OdolwaLimb
{
    ODOLWA_LIMB_NONE,
    ODOLWA_LIMB_ROOT,
    ODOLWA_LIMB_PELVIS,
    ODOLWA_LIMB_RIGHT_LEG_ROOT,
    ODOLWA_LIMB_RIGHT_THIGH,
    ODOLWA_LIMB_RIGHT_LOWER_LEG_ROOT,
    ODOLWA_LIMB_RIGHT_SHIN,
    ODOLWA_LIMB_RIGHT_FOOT,
    ODOLWA_LIMB_LEFT_LEG_ROOT,
    ODOLWA_LIMB_LEFT_THIGH,
    ODOLWA_LIMB_LEFT_LOWER_LEG_ROOT,
    ODOLWA_LIMB_LEFT_SHIN,
    ODOLWA_LIMB_LEFT_FOOT,
    ODOLWA_LIMB_UPPER_BODY_ROOT,
    ODOLWA_LIMB_UPPER_BODY_WRAPPER,
    ODOLWA_LIMB_TORSO,
    ODOLWA_LIMB_LEFT_ARM_ROOT,
    ODOLWA_LIMB_LEFT_UPPER_ARM,
    ODOLWA_LIMB_LEFT_LOWER_ARM_ROOT,
    ODOLWA_LIMB_LEFT_FOREARM,
    ODOLWA_LIMB_LEFT_BANGLE,
    ODOLWA_LIMB_LEFT_HAND_ROOT,
    ODOLWA_LIMB_LEFT_HAND,
    ODOLWA_LIMB_SHIELD,
    ODOLWA_LIMB_RIGHT_ARM_ROOT,
    ODOLWA_LIMB_RIGHT_UPPER_ARM,
    ODOLWA_LIMB_RIGHT_LOWER_ARM_ROOT,
    ODOLWA_LIMB_RIGHT_FOREARM,
    ODOLWA_LIMB_RIGHT_BANGLE,
    ODOLWA_LIMB_RIGHT_HAND_ROOT,
    ODOLWA_LIMB_RIGHT_HAND,
    ODOLWA_LIMB_SWORD,
    ODOLWA_LIMB_HEAD,
    ODOLWA_LIMB_RIGHT_EARRING_ROOT,
    ODOLWA_LIMB_RIGHT_EARRING,
    ODOLWA_LIMB_LEFT_EARRING_ROOT,
    ODOLWA_LIMB_LEFT_EARRING,
    ODOLWA_LIMB_RIGHT_PLUME_ROOT,
    ODOLWA_LIMB_RIGHT_PLUME_BASE,
    ODOLWA_LIMB_RIGHT_LOWER_PLUME_ROOT,
    ODOLWA_LIMB_RIGHT_PLUME_MIDDLE,
    ODOLWA_LIMB_RIGHT_PLUME_TIP,
    ODOLWA_LIMB_LEFT_PLUME_ROOT,
    ODOLWA_LIMB_LEFT_PLUME_BASE,
    ODOLWA_LIMB_LEFT_LOWER_PLUME_ROOT,
    ODOLWA_LIMB_LEFT_PLUME_MIDDLE,
    ODOLWA_LIMB_LEFT_PLUME_TIP,
    ODOLWA_LIMB_CENTER_PLUME_ROOT,
    ODOLWA_LIMB_CENTER_PLUME_BASE,
    ODOLWA_LIMB_CENTER_LOWER_PLUME_ROOT,
    ODOLWA_LIMB_CENTER_PLUME_MIDDLE,
    ODOLWA_LIMB_CENTER_PLUME_TIP,
    ODOLWA_LIMB_MAX
}
OdolwaLimb;

typedef enum OdolwaBugLimb
{
    ODOLWA_BUG_LIMB_NONE,
    ODOLWA_BUG_LIMB_ROOT,
    ODOLWA_BUG_LIMB_BODY,
    ODOLWA_BUG_LIMB_FRONT_RIGHT_LEG_ROOT,
    ODOLWA_BUG_LIMB_FRONT_RIGHT_UPPER_LEG,
    ODOLWA_BUG_LIMB_FRONT_RIGHT_LOWER_LEG,
    ODOLWA_BUG_LIMB_MIDDLE_LEFT_LEG_ROOT,
    ODOLWA_BUG_LIMB_MIDDLE_LEFT_UPPER_LEG,
    ODOLWA_BUG_LIMB_MIDDLE_LEFT_LOWER_LEG,
    ODOLWA_BUG_LIMB_MIDDLE_RIGHT_LEG_ROOT,
    ODOLWA_BUG_LIMB_MIDDLE_RIGHT_UPPER_LEG,
    ODOLWA_BUG_LIMB_MIDDLE_RIGHT_LOWER_LEG,
    ODOLWA_BUG_LIMB_BACK_LEFT_LEG_ROOT,
    ODOLWA_BUG_LIMB_BACK_LEFT_UPPER_LEG,
    ODOLWA_BUG_LIMB_BACK_LEFT_LOWER_LEG,
    ODOLWA_BUG_LIMB_BACK_RIGHT_LEG_ROOT,
    ODOLWA_BUG_LIMB_BACK_RIGHT_UPPER_LEG,
    ODOLWA_BUG_LIMB_BACK_RIGHT_LOWER_LEG,
    ODOLWA_BUG_LIMB_FRONT_LEFT_LEG_ROOT,
    ODOLWA_BUG_LIMB_FRONT_LEFT_UPPER_LEG,
    ODOLWA_BUG_LIMB_FRONT_LEFT_LOWER_LEG,
    ODOLWA_BUG_LIMB_MAX
}
OdolwaBugLimb;

typedef struct Actor_Boss01
{
    Actor                   actor;
    s16                     frameCounter;
    s32                     phaseFrameCounter;
    union {
        u8                  jumpIfPlayerIsClose;
        u8                  shouldPerformFallingSlash;
        u8                  bugDrawDmgEffType;
    };
    s16                     timers[3];
    f32                     animMorphFrames1;
    f32                     animMorphFrames2;
    s16                     damagedTimer;
    s16                     damagedFlashTimer;
    u8                      isPerformingVerticalSlash; // set, but never checked
    u8                      landedFromJump;
    u8                      waitType;
    u8                      lookAtPlayer;
    SkelAnime               skelAnime;
    f32                     animEndFrame;
    f32                     prevJumpVelocityY;
    f32                     runTargetPosAngularVelocityY;
    f32                     runTargetPosRotY;
    u8                      swordState;
    u8                      kickAndShieldBashCollisionEnabled;
    u8                      swordAndShieldCollisionEnabled;
    u8                      canGuardOrEvade;
    u8                      bodyInvincibilityTimer;
    s16                     disableCollisionTimer;
    u8                      afterimageSpawnFrameMask; // used as a bitmask with frameCounter to control how often to spawn afterimages
    u8                      hasPlayedSummonBugCs;
    f32                     additionalVelocityX;
    f32                     additionalVelocityZ;
    s16                     headRotY;
    s16                     headRotX;
    Vec3s                   jointTable[ODOLWA_LIMB_MAX];
    Vec3s                   morphTable[ODOLWA_LIMB_MAX];
    Vec3f                   feetPos[2];
    Vec3f                   pelvisPos;
    Actor_Boss01ActionFunc  actionFunc;
    ColliderJntSph          swordCollider;
    ColliderJntSphElement   swordColliderElements[ODOLWA_SWORD_COLLIDER_MAX];
    ColliderJntSph          shieldCollider;
    ColliderJntSphElement   shieldColliderElements[ODOLWA_SHIELD_COLLIDER_MAX];
    ColliderJntSph          bodyCollider;
    ColliderJntSphElement   bodyColliderElements[ODOLWA_COLLIDER_BODYPART_MAX];
    ColliderJntSph          kickAndShieldBashCollider;
    ColliderJntSphElement   kickAndShieldBashColliderElements[ODOLWA_KICK_AND_SHIELD_BASH_COLLIDER_MAX];
    Vec3f                   bodyPartsPos[ODOLWA_BODYPART_MAX];
    union {
        u32                 cutsceneTimer;
        u32                 waitTimer;
        u32                 summonMothsTimer;
    };
    s16                     cutsceneState;
    s16                     subCamId;
    Vec3f                   subCamEye;
    Vec3f                   subCamAt;
    Vec3f                   subCamUp;
    Vec3f                   subCamEyeNext;
    f32                     deathCsInitialSubCamRot;
    f32                     deathCsSubCamRot;
    f32                     subCamVelocity;
    f32                     deathShrinkSpeed;
    f32                     screenShakeOffsetY;
    f32                     screenShakeMagnitude;
    ColliderCylinder        bugACCollider;
    ColliderCylinder        bugATCollider;
    f32                     bugDrawDmgEffScale;
    f32                     bugDrawDmgEffAlpha;
    f32                     drawDmgEffScale;
    f32                     drawDmgEffFrozenSteamScale;
    f32                     drawDmgEffAlpha;
    u8                      drawDmgEffType;
    u8                      drawDmgEffState;
    s16                     drawDmgEffTimer;
}
Actor_Boss01;

ASSERT_SIZE(Actor_Boss01, 0xadc);

#endif
