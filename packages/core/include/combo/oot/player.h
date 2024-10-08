#ifndef COMBO_OOT_PLAYER_H
#define COMBO_OOT_PLAYER_H

#include <ultra64.h>
#include <combo/actor.h>

#define PLAYER_LIMB_NONE             0x00
#define PLAYER_LIMB_ROOT             0x01
#define PLAYER_LIMB_WAIST            0x02
#define PLAYER_LIMB_LOWER            0x03
#define PLAYER_LIMB_R_THIGH          0x04
#define PLAYER_LIMB_R_SHIN           0x05
#define PLAYER_LIMB_R_FOOT           0x06
#define PLAYER_LIMB_L_THIGH          0x07
#define PLAYER_LIMB_L_SHIN           0x08
#define PLAYER_LIMB_L_FOOT           0x09
#define PLAYER_LIMB_UPPER            0x0a
#define PLAYER_LIMB_HEAD             0x0b
#define PLAYER_LIMB_HAT              0x0c
#define PLAYER_LIMB_COLLAR           0x0d
#define PLAYER_LIMB_L_SHOULDER       0x0e
#define PLAYER_LIMB_L_FOREARM        0x0f
#define PLAYER_LIMB_L_HAND           0x10
#define PLAYER_LIMB_R_SHOULDER       0x11
#define PLAYER_LIMB_R_FOREARM        0x12
#define PLAYER_LIMB_R_HAND           0x13
#define PLAYER_LIMB_SHEATH           0x14
#define PLAYER_LIMB_TORSO            0x15
#define PLAYER_LIMB_MAX              0x16

#define PLAYER_MODELTYPE_LH_OPEN                0x00
#define PLAYER_MODELTYPE_LH_CLOSED              0x01
#define PLAYER_MODELTYPE_LH_SWORD               0x02
#define PLAYER_MODELTYPE_LH_SWORD_2             0x03
#define PLAYER_MODELTYPE_LH_BGS                 0x04
#define PLAYER_MODELTYPE_LH_HAMMER              0x05
#define PLAYER_MODELTYPE_LH_BOOMERANG           0x06
#define PLAYER_MODELTYPE_LH_BOTTLE              0x07
#define PLAYER_MODELTYPE_RH_OPEN                0x08
#define PLAYER_MODELTYPE_RH_CLOSED              0x09
#define PLAYER_MODELTYPE_RH_SHIELD              0x0a
#define PLAYER_MODELTYPE_RH_BOW_SLINGSHOT       0x0b
#define PLAYER_MODELTYPE_RH_BOW_SLINGSHOT_2     0x0c
#define PLAYER_MODELTYPE_RH_OCARINA             0x0d
#define PLAYER_MODELTYPE_RH_OOT                 0x0e
#define PLAYER_MODELTYPE_RH_HOOKSHOT            0x0f
#define PLAYER_MODELTYPE_SHEATH_16              0x10
#define PLAYER_MODELTYPE_SHEATH_17              0x11
#define PLAYER_MODELTYPE_SHEATH_18              0x12
#define PLAYER_MODELTYPE_SHEATH_19              0x13
#define PLAYER_MODELTYPE_WAIST                  0x14
#define PLAYER_MODELTYPE_MAX                    0x15
#define PLAYER_MODELTYPE_RH_FF                  0xff

#define LIMB_BUF_COUNT(limbCount)   ((ALIGN16((limbCount) * sizeof(Vec3s)) + sizeof(Vec3s) - 1) / sizeof(Vec3s))
#define PLAYER_LIMB_BUF_COUNT       LIMB_BUF_COUNT(PLAYER_LIMB_MAX)

typedef enum PlayerBodyPart
{
    PLAYER_BODYPART_WAIST,
    PLAYER_BODYPART_R_THIGH,
    PLAYER_BODYPART_R_SHIN,
    PLAYER_BODYPART_R_FOOT,
    PLAYER_BODYPART_L_THIGH,
    PLAYER_BODYPART_L_SHIN,
    PLAYER_BODYPART_L_FOOT,
    PLAYER_BODYPART_HEAD,
    PLAYER_BODYPART_HAT,
    PLAYER_BODYPART_COLLAR,
    PLAYER_BODYPART_L_SHOULDER,
    PLAYER_BODYPART_L_FOREARM,
    PLAYER_BODYPART_L_HAND,
    PLAYER_BODYPART_R_SHOULDER,
    PLAYER_BODYPART_R_FOREARM,
    PLAYER_BODYPART_R_HAND,
    PLAYER_BODYPART_SHEATH,
    PLAYER_BODYPART_TORSO,
    PLAYER_BODYPART_MAX,
}
PlayerBodyPart;

typedef struct Actor_Player
{
    Actor           actor;
    s8              currentTunic; /* current tunic from `PlayerTunic` */
    s8              currentSwordItemId;
    s8              currentShield; /* current shield from `PlayerShield` */
    s8              currentBoots; /* current boots from `PlayerBoots` */
    u8              heldItemButton;
    u8              itemAction;
    u8              heldItemId;
    u8              boots;
    u8              heldItemAction;
    char            unk_155[0x003];
    u8              modelGroup;
    u8              nextModelGroup;
    s8              itemChangeType;
    u8              modelAnimType;
    u8              leftHandType;
    u8              rightHandType;
    u8              sheathType;
    u8              mask;
    Gfx**           rightHandDLists;
    Gfx**           leftHandDLists;
    Gfx**           sheathDLists;
    Gfx**           waistDLists;
    u8              giObjectLoading;
    DmaRequest      giObjectDmaRequest;
    OSMesgQueue     objMsgQueue;
    OSMesg          objMsg;
    void*           objBuffer;
    char            unk_1a4[0x280];
    s16             gi; /* Extended to s16 */
    char            unk_426[0x246];
    u32             state;
    u32             state2;
    Actor*          unk_674;
    Actor*          boomerangActor;
    Actor*          naviActor;
    s16             naviTextId;
    u8              state3;
    s8              exchangeItemId;
    Actor*          talkActor;
    float           talkActorDistance;
    char            unk_68c[0x004];
    float           unk_690;
    float           closestSecretDistSq;
    Actor*          unk_698;
    char            unk_69c[0x1c];
    SkelAnime       upperSkelAnime;
    Vec3s           upperJointTable[PLAYER_LIMB_BUF_COUNT];
    Vec3s           upperMorphTable[PLAYER_LIMB_BUF_COUNT];
    void*           upperActionFunc;
    f32             upperAnimInterpWeight;
    s16             unk_824;
    s8              unk_826;
    u8              putAwayCooldownTimer;
    f32             speedXZ;
    s16             yaw;
    s16             parallelYaw;
    u16             underwaterTimer;
    s8              meleeWeaponAnimation;
    s8              meleeWeaponState;
    s8              unk_834;
    u8              unk_835;
    u8              controlStickDataIndex;
    s8              controlStickSpinAngles[4];
    s8              controlStickDirections[4];
    union
    {
        s8 actionVar1;
    }
    av1;

    union {
        s16 actionVar2;
        s16 bonked;
    }
    av2;
    float           unk_844;
    float           unk_848;
    float           unk_84C;
    s16             unk_850;
    u8              drawGiId;
    float           unk_854;
    float           unk_858;
    float           unk_85C;
    float           unk_860;
    float           unk_864;
    float           unk_868;
    s16             unk_86C;
    s16             unk_86E;
    float           unk_870;
    float           yDistToLedge;
    float           distToInteractWall;
    u8              ledgeClimbType;
    u8              ledgeClimbDelayTimer;
    u8              unk_87e;
    u8              damageFlickerAnimCounter;
    u8              unk_880;
    u8              bodyShockTimer;
    u8              unk_882;
    u8              hoverBootsTimer;
    s16             fallStartHeight;
    s16             fallDistance;
    s16             floorPitch;
    s16             floorPitchAlt;
    s16             unk_88c;
    u16             floorSfxOffset;
    u8              knockbackDamage;
    u8              knockbackType;
    s16             knockbackRot;
    float           knockbackSpeed;
    float           knockbackYVelocity;
    float           pushedSpeed;
    s16             pushedYaw;
    WeaponInfo      meleeWeaponInfo[3];
    Vec3f           bodyPartsPos[PLAYER_BODYPART_MAX];
    MtxF            mf_9d0;
    MtxF            shieldMf;
    u8              bodyIsBurning;
    u8              bodyFlameTimers[PLAYER_BODYPART_MAX];
    u8              unk_a63;
    void*           afterPutAwayFunc;
    s8              invincibilityTimer;
    u8              floorTypeTimer;
    u8              floorProperty;
    u8              prevFloorType;
    float           prevControlStickMagnitude;
    s16             prevControlStickAngle;
    u16             prevFloorSfxOffset;
    s16             unk_a74;
    s8              unk_a76;
    u8              unk_a77;
    Vec3f           unk_a78;
}
Actor_Player;

_Static_assert(sizeof(Actor_Player) == 0xa84, "Actor_Player size is wrong");

ASSERT_OFFSET(Actor_Player, currentTunic,       0x13c);
ASSERT_OFFSET(Actor_Player, heldItemButton,     0x140);
ASSERT_OFFSET(Actor_Player, itemAction,         0x141);
ASSERT_OFFSET(Actor_Player, heldItemId,         0x142);
ASSERT_OFFSET(Actor_Player, boots,              0x143);
ASSERT_OFFSET(Actor_Player, heldItemAction,     0x144);
ASSERT_OFFSET(Actor_Player, mask,               0x14f);
ASSERT_OFFSET(Actor_Player, rightHandDLists,    0x150);
ASSERT_OFFSET(Actor_Player, leftHandDLists,     0x154);
ASSERT_OFFSET(Actor_Player, sheathDLists,       0x158);
ASSERT_OFFSET(Actor_Player, waistDLists,        0x15c);
ASSERT_OFFSET(Actor_Player, giObjectLoading,    0x160);
ASSERT_OFFSET(Actor_Player, giObjectDmaRequest, 0x164);
ASSERT_OFFSET(Actor_Player, unk_1a4,            0x1a4);
ASSERT_OFFSET(Actor_Player, gi,                 0x424);
ASSERT_OFFSET(Actor_Player, unk_426,            0x426);
ASSERT_OFFSET(Actor_Player, unk_674,            0x674);
ASSERT_OFFSET(Actor_Player, boomerangActor,     0x678);
ASSERT_OFFSET(Actor_Player, naviActor,          0x67c);
ASSERT_OFFSET(Actor_Player, naviTextId,         0x680);
ASSERT_OFFSET(Actor_Player, state3,             0x682);
ASSERT_OFFSET(Actor_Player, exchangeItemId,     0x683);
ASSERT_OFFSET(Actor_Player, drawGiId,           0x852);

#endif
