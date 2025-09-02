#ifndef COMBO_OOT_PLAYER_H
#define COMBO_OOT_PLAYER_H

#include <ultra64.h>
#include <combo/actor.h>

#define PLAYER_STATE1_0 (1 << 0)
#define PLAYER_STATE1_SWINGING_BOTTLE (1 << 1) // Bottle is swung; Bottle is active and can catch things
#define PLAYER_STATE1_2 (1 << 2)
#define PLAYER_STATE1_3 (1 << 3)
#define PLAYER_STATE1_HOSTILE_LOCK_ON (1 << 4) // Currently locked onto a hostile actor. Triggers a "battle" variant of many actions.
#define PLAYER_STATE1_5 (1 << 5)
#define PLAYER_STATE1_TALKING (1 << 6) // Currently talking to an actor. This includes item exchanges.
#define PLAYER_STATE1_DEAD (1 << 7) // Player has died. Note that this gets set when the death cutscene has started, after landing from the air.
#define PLAYER_STATE1_START_CHANGING_HELD_ITEM (1 << 8) // Item change process has begun
#define PLAYER_STATE1_9 (1 << 9)
#define PLAYER_STATE1_10 (1 << 10)
#define PLAYER_STATE1_ACTOR_CARRY (1 << 11) // Currently carrying an actor
#define PLAYER_STATE1_CHARGING_SPIN_ATTACK (1 << 12) // Currently charing a spin attack (by holding down the B button)
#define PLAYER_STATE1_13 (1 << 13)
#define PLAYER_STATE1_14 (1 << 14)
#define PLAYER_STATE1_Z_TARGETING (1 << 15) // Either lock-on or parallel is active. This flag is never checked for and is practically unused.
#define PLAYER_STATE1_FRIENDLY_ACTOR_FOCUS (1 << 16) // Currently focusing on a friendly actor. Includes friendly lock-on, talking, and more. Usually does not include hostile actor lock-on, see `PLAYER_STATE1_HOSTILE_LOCK_ON`.
#define PLAYER_STATE1_PARALLEL (1 << 17) // "Parallel" mode, Z-Target without an actor lock-on
#define PLAYER_STATE1_18 (1 << 18)
#define PLAYER_STATE1_19 (1 << 19)
#define PLAYER_STATE1_20 (1 << 20)
#define PLAYER_STATE1_21 (1 << 21)
#define PLAYER_STATE1_SHIELDING (1 << 22) // Shielding in any form (regular, hylian shield as child, "shielding" with a two handed sword, etc.)
#define PLAYER_STATE1_23 (1 << 23)
#define PLAYER_STATE1_USING_BOOMERANG (1 << 24) // Currently using the boomerang. This includes all phases (aiming, throwing, and catching).
#define PLAYER_STATE1_BOOMERANG_THROWN (1 << 25) // Boomerang has been thrown and is flying in the air
#define PLAYER_STATE1_26 (1 << 26)
#define PLAYER_STATE1_27 (1 << 27)
#define PLAYER_STATE1_28 (1 << 28)
#define PLAYER_STATE1_29 (1 << 29)
#define PLAYER_STATE1_LOCK_ON_FORCED_TO_RELEASE (1 << 30) // Lock-on was released automatically, for example by leaving the lock-on leash range
#define PLAYER_STATE1_31 (1 << 31)

#define PLAYER_STATE2_0 (1 << 0)
#define PLAYER_STATE2_CAN_ACCEPT_TALK_OFFER (1 << 1) // Can accept a talk offer. "Speak" or "Check" is shown on the A button.
#define PLAYER_STATE2_2 (1 << 2)
#define PLAYER_STATE2_3 (1 << 3)
#define PLAYER_STATE2_4 (1 << 4)
#define PLAYER_STATE2_5 (1 << 5)
#define PLAYER_STATE2_6 (1 << 6)
#define PLAYER_STATE2_7 (1 << 7)
#define PLAYER_STATE2_8 (1 << 8)
#define PLAYER_STATE2_FORCE_SAND_FLOOR_SOUND (1 << 9) // Forces sand footstep sounds regardless of current floor type
#define PLAYER_STATE2_10 (1 << 10)
#define PLAYER_STATE2_11 (1 << 11)
#define PLAYER_STATE2_12 (1 << 12)
#define PLAYER_STATE2_LOCK_ON_WITH_SWITCH (1 << 13) // Actor lock-on is active, specifically with Switch Targeting. Hold Targeting checks the state of the Z button instead of this flag.
#define PLAYER_STATE2_14 (1 << 14)
#define PLAYER_STATE2_15 (1 << 15)
#define PLAYER_STATE2_DO_ACTION_ENTER (1 << 16) // Sets the "Enter On A" DoAction
#define PLAYER_STATE2_17 (1 << 17)
#define PLAYER_STATE2_CRAWLING (1 << 18) // Crawling through a crawlspace
#define PLAYER_STATE2_19 (1 << 19)
#define PLAYER_STATE2_NAVI_ACTIVE (1 << 20) // Navi is visible and active. Could be hovering idle near Link or hovering over other actors.
#define PLAYER_STATE2_21 (1 << 21)
#define PLAYER_STATE2_22 (1 << 22)
#define PLAYER_STATE2_23 (1 << 23)
#define PLAYER_STATE2_24 (1 << 24)
#define PLAYER_STATE2_25 (1 << 25)
#define PLAYER_STATE2_26 (1 << 26)
#define PLAYER_STATE2_USING_OCARINA (1 << 27) // Playing the ocarina or warping out from an ocarina warp song
#define PLAYER_STATE2_IDLE_FIDGET (1 << 28) // Playing a fidget idle animation (under typical circumstances, see `Player_ChooseNextIdleAnim` for more info)
#define PLAYER_STATE2_29 (1 << 29)
#define PLAYER_STATE2_30 (1 << 30)
#define PLAYER_STATE2_31 (1 << 31)

#define PLAYER_STATE3_0 (1 << 0)
#define PLAYER_STATE3_1 (1 << 1)
#define PLAYER_STATE3_2 (1 << 2)
#define PLAYER_STATE3_3 (1 << 3)
#define PLAYER_STATE3_4 (1 << 4)
#define PLAYER_STATE3_5 (1 << 5)
#define PLAYER_STATE3_RESTORE_NAYRUS_LOVE (1 << 6) // Set by ocarina effects actors when destroyed to signal Nayru's Love may be restored (see `ACTOROVL_ALLOC_ABSOLUTE`)
#define PLAYER_STATE3_FLYING_WITH_HOOKSHOT (1 << 7) // Flying in the air with the hookshot as it pulls Player toward its destination

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

typedef struct Player
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
    char            unk_165[0x003];
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
    void*           giObjectSegment; // also used for title card textures
    SkelAnime       skelAnime;
    Vec3s           jointTable[PLAYER_LIMB_BUF_COUNT];
    Vec3s           morphTable[PLAYER_LIMB_BUF_COUNT];
    Vec3s           blendTable[PLAYER_LIMB_BUF_COUNT];
    char            faceChange[0x4];
    Actor*          heldActor;
    Vec3f           leftHandPos;
    Vec3s           unk_3BC;
    Actor*          unk_3C4;
    Vec3f           unk_3C8;
    char            unk_3D4[0x058];
    s8              doorType;
    s8              doorDirection;
    s16             doorTimer;
    Actor*          doorActor;
    s16             gi; /* Extended to s16 */ // 0x424
    u16             getItemDirection;
    Actor*          interactRangeActor;
    s8              mountSide;
    char            unk_43D[0x003];
    Actor*          rideActor;
    u8              csAction;
    u8              prevCsAction;
    u8              cueId;
    u8              unk_447;
    Actor*          csActor; // Actor involved in a `csAction`. Typically the actor that invoked the cutscene.
    char            unk_44C[0x004];
    Vec3f           unk_450;
    Vec3f           unk_45C;
    char            unk_468[0x002];
    union {
        s16 haltActorsDuringCsAction; // If true, halt actors belonging to certain categories during a `csAction`
        s16 slidingDoorBgCamIndex; // `BgCamIndex` used during a sliding door cutscene
    } cv; // "Cutscene Variable": context dependent variable that has different meanings depending on what function is called
    s16             subCamId;
    char            unk_46E[0x02A];
    ColliderCylinder cylinder;
    ColliderQuad    meleeWeaponQuads[2];
    ColliderQuad    shieldQuad;
    Actor*          focusActor; // Actor that Player and the camera are looking at; Used for lock-on, talking, and more
    char            unk_668[0x004];
    s32             unk_66C;
    s32             meleeWeaponEffectIndex;
    void*           actionFunc;
    void*           ageProperties;
    u32             stateFlags1;
    u32             stateFlags2;
    Actor*          unk_684;
    Actor*          boomerangActor;
    Actor*          naviActor;
    s16             naviTextId;
    u8              state3;
    s8              exchangeItemId;
    Actor*          talkActor;
    float           talkActorDistance;
    char            unk_69c[0x004];
    float           unk_6a0;
    float           closestSecretDistSq;
    Actor*          unk_6a8;
    char            unk_6ac[0x1c];
    SkelAnime       upperSkelAnime;
    Vec3s           upperJointTable[PLAYER_LIMB_BUF_COUNT];
    Vec3s           upperMorphTable[PLAYER_LIMB_BUF_COUNT];
    void*           upperActionFunc;
    f32             upperAnimInterpWeight;
    s16             unk_834;
    s8              unk_836;
    u8              putAwayCooldownTimer;
    f32             speedXZ;
    s16             yaw;
    s16             parallelYaw;
    u16             underwaterTimer;
    s8              meleeWeaponAnimation;
    s8              meleeWeaponState;
    s8              unk_844;
    u8              unk_845;
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
    float           unk_854;
    float           unk_858;
    float           unk_85C;
    s16             unk_860;
    u8              drawGiId;
    float           unk_864;
    float           unk_868;
    float           unk_86C;
    float           unk_870;
    float           unk_874;
    float           unk_878;
    s16             unk_87C;
    s16             unk_87E;
    float           unk_880;
    float           yDistToLedge;
    float           distToInteractWall;
    u8              ledgeClimbType;
    u8              ledgeClimbDelayTimer;
    u8              unk_88e;
    u8              damageFlickerAnimCounter;
    u8              unk_890;
    u8              bodyShockTimer;
    u8              unk_892;
    u8              hoverBootsTimer;
    s16             fallStartHeight;
    s16             fallDistance;
    s16             floorPitch;
    s16             floorPitchAlt;
    s16             unk_89c;
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
    u8              isBurning;
    u8              flameTimers[PLAYER_BODYPART_MAX];
    u8              unk_a73;
    void*           afterPutAwayFunc;
    s8              invincibilityTimer;
    u8              floorTypeTimer;
    u8              floorProperty;
    u8              prevFloorType;
    float           prevControlStickMagnitude;
    s16             prevControlStickAngle;
    u16             prevFloorSfxOffset;
    s16             unk_a84;
    s8              unk_a86;
    u8              unk_a87;
    Vec3f           unk_a88;
}
Player;

_Static_assert(sizeof(Player) == 0xa84, "Player size is wrong");

ASSERT_OFFSET(Player, currentTunic,       0x13c);
ASSERT_OFFSET(Player, heldItemButton,     0x140);
ASSERT_OFFSET(Player, itemAction,         0x141);
ASSERT_OFFSET(Player, heldItemId,         0x142);
ASSERT_OFFSET(Player, boots,              0x143);
ASSERT_OFFSET(Player, heldItemAction,     0x144);
ASSERT_OFFSET(Player, mask,               0x14f);
ASSERT_OFFSET(Player, rightHandDLists,    0x150);
ASSERT_OFFSET(Player, leftHandDLists,     0x154);
ASSERT_OFFSET(Player, sheathDLists,       0x158);
ASSERT_OFFSET(Player, waistDLists,        0x15c);
ASSERT_OFFSET(Player, giObjectLoading,    0x160);
ASSERT_OFFSET(Player, giObjectDmaRequest, 0x164);
ASSERT_OFFSET(Player, gi,                 0x424);
ASSERT_OFFSET(Player, getItemDirection,   0x426);
ASSERT_OFFSET(Player, unk_684,            0x674);
ASSERT_OFFSET(Player, boomerangActor,     0x678);
ASSERT_OFFSET(Player, naviActor,          0x67c);
ASSERT_OFFSET(Player, naviTextId,         0x680);
ASSERT_OFFSET(Player, state3,             0x682);
ASSERT_OFFSET(Player, exchangeItemId,     0x683);
ASSERT_OFFSET(Player, drawGiId,           0x852);

typedef enum PlayerMeleeWeaponAnimation {
    /*  0 */ PLAYER_MWA_FORWARD_SLASH_1H,
    /*  1 */ PLAYER_MWA_FORWARD_SLASH_2H,
    /*  2 */ PLAYER_MWA_FORWARD_COMBO_1H,
    /*  3 */ PLAYER_MWA_FORWARD_COMBO_2H,
    /*  4 */ PLAYER_MWA_RIGHT_SLASH_1H,
    /*  5 */ PLAYER_MWA_RIGHT_SLASH_2H,
    /*  6 */ PLAYER_MWA_RIGHT_COMBO_1H,
    /*  7 */ PLAYER_MWA_RIGHT_COMBO_2H,
    /*  8 */ PLAYER_MWA_LEFT_SLASH_1H,
    /*  9 */ PLAYER_MWA_LEFT_SLASH_2H,
    /* 10 */ PLAYER_MWA_LEFT_COMBO_1H,
    /* 11 */ PLAYER_MWA_LEFT_COMBO_2H,
    /* 12 */ PLAYER_MWA_STAB_1H,
    /* 13 */ PLAYER_MWA_STAB_2H,
    /* 14 */ PLAYER_MWA_STAB_COMBO_1H,
    /* 15 */ PLAYER_MWA_STAB_COMBO_2H,
    /* 16 */ PLAYER_MWA_FLIPSLASH_START,
    /* 17 */ PLAYER_MWA_JUMPSLASH_START,
    /* 18 */ PLAYER_MWA_FLIPSLASH_FINISH,
    /* 19 */ PLAYER_MWA_JUMPSLASH_FINISH,
    /* 20 */ PLAYER_MWA_BACKSLASH_RIGHT,
    /* 21 */ PLAYER_MWA_BACKSLASH_LEFT,
    /* 22 */ PLAYER_MWA_HAMMER_FORWARD,
    /* 23 */ PLAYER_MWA_HAMMER_SIDE,
    /* 24 */ PLAYER_MWA_SPIN_ATTACK_1H,
    /* 25 */ PLAYER_MWA_SPIN_ATTACK_2H,
    /* 26 */ PLAYER_MWA_BIG_SPIN_1H,
    /* 27 */ PLAYER_MWA_BIG_SPIN_2H,
    /* 28 */ PLAYER_MWA_MAX
} PlayerMeleeWeaponAnimation;

int Player_IsBurningStickInRange(PlayState* play, Vec3f* pos, f32 xzRange, f32 yRange);

#endif
