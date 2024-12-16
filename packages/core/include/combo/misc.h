#ifndef COMBO_MISC_H
#define COMBO_MISC_H

#include <ultra64.h>
#include <combo/math/vec.h>

#define PALIGN(val, align)  ((void*)ALIGN(((u32)(val)), align))

#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))

typedef struct PlayState PlayState;
typedef struct Actor Actor;

typedef struct {
    /* 0x0 */ Vec3f center;
    /* 0xC */ f32   radius;
} Spheref; /* size = 0x10 */

/*
The plane paramaters are of form `ax + by + cz + d = 0`
where `(a,b,c)` is the plane's normal vector and d is the originDist
 */
typedef struct {
    /* 0x00 */ Vec3f normal;
    /* 0x0C */ f32   originDist;
} Plane; /* size = 0x10 */

typedef struct {
    /* 0x00 */ Vec3f vtx[3];
    /* 0x24 */ Plane plane;
} TriNorm; /* size = 0x34 */

typedef struct {
    /* 0x0 */ s16   radius;
    /* 0x2 */ s16   height;
    /* 0x4 */ s16   yShift;
    /* 0x6 */ Vec3s pos;
} Cylinder16; /* size = 0xC */

typedef struct {
    /* 0x00 */ f32   radius;
    /* 0x04 */ f32   height;
    /* 0x08 */ f32   yShift;
    /* 0x0C */ Vec3f pos;
} Cylinderf; /* size = 0x18 */

typedef struct {
    /* 0x00 */ Vec3f point;
    /* 0x0C */ Vec3f dir;
} InfiniteLine; /* size = 0x18 */

typedef struct {
    /* 0x00 */ Vec3f a;
    /* 0x0C */ Vec3f b;
} Linef; /* size = 0x18 */

typedef struct {
    /* 0x0 */ f32 r; /* radius */
    /* 0x4 */ s16 pitch; /* depends on coordinate system. See below. */
    /* 0x6 */ s16 yaw; /* azimuthal angle */
} VecSphGeo; /* size = 0x8 */

typedef float MtxF_t[4][4];
typedef union {
    MtxF_t mf;
    struct {
        float xx, yx, zx, wx,
              xy, yy, zy, wy,
              xz, yz, zz, wz,
              xw, yw, zw, ww;
    };
} MtxF; /* size = 0x40 */

#define TRUNCF_BINANG(f) (s16)(s32)(f)

/* Angle conversion macros */
#define DEG_TO_BINANG(degrees) (s16)TRUNCF_BINANG((degrees) * (0x8000 / 180.0f))
#define RAD_TO_BINANG(radians) (s16)TRUNCF_BINANG((radians) * (0x8000 / M_PI))
#define RAD_TO_DEG(radians) ((radians) * (180.0f / M_PI))
#define DEG_TO_RAD(degrees) ((degrees) * (M_PI / 180.0f))
#define BINANG_TO_DEG(binang) ((f32)(binang) * (180.0f / 0x8000))
#define BINANG_TO_RAD(binang) ((f32)(binang) * (M_PI / 0x8000))
#define BINANG_TO_RAD_ALT(binang) (((f32)(binang) / (f32)0x8000) * M_PI)
#define BINANG_TO_RAD_ALT2(binang) (((f32)(binang) * M_PI) / 0x8000)

struct Actor;

typedef struct {
    /* 0x0 */ u32 dmgFlags; /* Toucher damage type flags. */
    /* 0x4 */ u8 effect; /* Damage Effect (Knockback, Fire, etc.) */
    /* 0x5 */ u8 damage; /* Damage or Stun Timer */
} ColliderTouchInit; /* size = 0x8 */

typedef struct {
    /* 0x0 */ u32 dmgFlags; /* Bumper exclusion mask */
    /* 0x4 */ u8 effect; /* Damage Effect (Knockback, Fire, etc.) */
    /* 0x5 */ u8 defense; /* Damage Resistance */
} ColliderBumpInit; /* size = 0x8 */

typedef struct {
    /* 0x00 */ u8 elemType; /* Affects sfx reaction when attacked by Link and hookability. Full purpose unknown. */
    /* 0x04 */ ColliderTouchInit toucher; /* Damage properties when acting as an AT collider */
    /* 0x0C */ ColliderBumpInit bumper; /* Damage properties when acting as an AC collider */
    /* 0x14 */ u8 toucherFlags; /* Information flags for AT collisions */
    /* 0x15 */ u8 bumperFlags; /* Information flags for AC collisions */
    /* 0x16 */ u8 ocElemFlags; /* Information flags for OC collisions */
} ColliderInfoInit; /* size = 0x18 */

/* Model has limbs with only rigid meshes */
typedef struct {
    /* 0x00 */ void** segment;
    /* 0x04 */ u8 limbCount;
} SkeletonHeader; /* size = 0x8 */

/* Model has limbs with flexible meshes */
typedef struct {
    /* 0x00 */ SkeletonHeader sh;
    /* 0x08 */ u8 dListCount;
} FlexSkeletonHeader; /* size = 0xC */

typedef struct SkelAnime {
    /* 0x00 */ u8 limbCount;      /* Number of limbs in the skeleton */
    /* 0x01 */ u8 mode;           /* 0: loop, 2: play once, 4: partial loop. +1 to interpolate between frames. */
    /* 0x02 */ u8 dListCount;     /* Number of display lists in a flexible skeleton */
    /* 0x03 */ s8 taper;          /* Tapering to use when morphing between animations. Only used by Door_Warp1. */
    /* 0x04 */ void** skeleton;   /* An array of pointers to limbs. Can be StandardLimb, LodLimb, or SkinLimb. */
    /* 0x08 */ void* animation;   /* Can be an AnimationHeader or PlayerAnimationHeader. */
    /* 0x0C */ f32 startFrame;    /* In mode 4, start of partial loop. */
    /* 0x10 */ f32 endFrame;      /* In mode 2, Update returns true when curFrame is equal to this. In mode 4, end of partial loop. */
    /* 0x14 */ f32 animLength;    /* Total number of frames in the current animation's file. */
    /* 0x18 */ f32 curFrame;      /* Current frame in the animation */
    /* 0x1C */ f32 playSpeed;     /* Multiplied by R_UPDATE_RATE / 3 to get the animation's frame rate. */
    /* 0x20 */ Vec3s* jointTable; /* Current translation of model and rotations of all limbs */
    /* 0x24 */ Vec3s* morphTable; /* Table of values used to morph between animations */
    /* 0x28 */ f32 morphWeight;   /* Weight of the current animation morph as a fraction in [0,1] */
    /* 0x2C */ f32 morphRate;     /* Reciprocal of the number of frames in the morph */
    /* 0x30 */ union {
                    s32 (*normal)(struct SkelAnime*);/* Can be Loop, Partial loop, Play once, Morph, or Tapered morph */
                    s32 (*player)(PlayState*, struct SkelAnime*); /* Loop, Play once, and Morph */
                } update;
    /* 0x34 */ s8 initFlags;      /* Flags used when initializing Player's skeleton */
    /* 0x35 */ u8 moveFlags;      /* Flags used for animations that move the actor in worldspace. */
    /* 0x36 */ s16 prevRot;       /* Previous rotation in worldspace. */
    /* 0x38 */ Vec3s prevTransl;  /* Previous modelspace translation. */
    /* 0x3E */ Vec3s baseTransl;  /* Base modelspace translation. */
} SkelAnime; /* size = 0x44 */

typedef struct {
    /* 0x00 */ s16   id;
    /* 0x02 */ Vec3s pos;
    /* 0x08 */ Vec3s rot;
    /* 0x0E */ s16   params;
} ActorEntry; /* size = 0x10 */

typedef struct {
    struct {
        s8 room;    /* Room to switch to */
        s8 bgCamIndex; /* How the camera reacts during the transition. See `Camera_ChangeDoorCam` */
    } /* 0x00 */ sides[2]; /* 0 = front, 1 = back */
    /* 0x04 */ s16   id;
    /* 0x06 */ Vec3s pos;
    /* 0x0C */ s16   rotY;
    /* 0x0E */ s16   params;
} TransitionActorEntry; /* size = 0x10 */

typedef struct {
    /* 0x00 */ u8 playerEntryIndex;
    /* 0x01 */ u8 room;
} Spawn;

#define PATH_INDEX_NONE -1
#define ADDITIONAL_PATH_INDEX_NONE (u8)-1

typedef struct {
    /* 0x00 */ u8 count; /* number of points in the path */
    #if defined(GAME_MM)
    u8 additionalPathIndex;
    s16 customValue;
    #endif
    /* 0x04 */ Vec3s* points; /* Segment Address to the array of points */
} Path; /* size = 0x8 */

typedef struct {
    /* 0x00 */ u8 byte0;
    /* 0x01 */ u8 byte1;
    /* 0x02 */ u8 byte2;
    /* 0x03 */ u8 byte3;
} QuestHintCmd; /* size = 0x4 */

typedef struct {
    /* 0x00 */ OSContPad cur;
    /* 0x06 */ OSContPad prev;
    /* 0x0C */ OSContPad press; /* X/Y store delta from last frame */
    /* 0x12 */ OSContPad rel; /* X/Y store adjusted */
} Input; /* size = 0x18 */

typedef struct {
    /* 0x00 */ u32* vromStart;
    /* 0x04 */ u32* vromEnd;
} RomFile; /* size = 0x8 */

typedef struct ActorOverlay {
    /* 0x00 */ RomFile file;
    /* 0x08 */ void* vramStart;
    /* 0x0C */ void* vramEnd;
    /* 0x10 */ void* loadedRamAddr; // original name: "allocp"
    /* 0x14 */ struct ActorProfile* initInfo;
    /* 0x18 */ char* name;
    /* 0x1C */ u16 allocType; // bit 0: don't allocate memory, use actorContext->0x250? bit 1: Always keep loaded?
    /* 0x1E */ s8 numLoaded; // original name: "clients"
} ActorOverlay; // size = 0x20

// TODO: Move this into its own file
typedef struct {
    u8 unk_000[0x50]; // paramData, bunch of pointers
    Vec3f at;
    Vec3f eye;
    Vec3f up;
    Vec3f eyeNext;
    Vec3f quakeOffset;
    struct PlayState* play;
    Actor* focalActor;
    struct {
        Vec3f pos;
        Vec3s rot;
    } focalActorPosRot;
    Actor* target;
    struct {
        Vec3f pos;
        Vec3s rot;
    } targetPosRot;
    f32 rUpdateRateInv;
    f32 pitchUpdateRateInv;
    f32 yawUpdateRateInv;
    f32 xzOffsetUpdateRate;
    f32 yOffsetUpdateRate;
    f32 fovUpdateRate;
    f32 xzSpeed;
    f32 dist;
    f32 speedRatio;
    Vec3f playerToAtOffset;
    Vec3f playerPosDelta;
    f32 fov;
    f32 atLERPStepScale;
    f32 playerGroundY;
    Vec3f floorNorm;
    f32 waterYPos;
    s32 bgCamIndexBeforeUnderwater;
    s32 waterCamSetting;
    s32 waterQuakeIndex;
    void* data0;
    void* data1;
    s16 data2;
    s16 data3;
    s16 uid;
    char unk_132[2];
    Vec3s inputDir;
    Vec3s camDir;
    s16 status;
    s16 setting;
    s16 mode;
    s16 bgId; // bgId the camera is currently interacting with
    s16 bgCamIndex;
    s16 behaviorFlags; // includes flags on settings, modes, bgCam. All related to camera update behaviour
    s16 stateFlags;
    s16 childCamId;
    s16 waterDistortionTimer;
    s16 distortionFlags;
    s16 prevSetting;
    s16 nextBgCamIndex;
    s16 nextBgId;
    s16 roll;
    s16 viewFlags; // For setting params: at, eye, up, target, targetPos, fov, roll
    s16 animState;
    s16 timer;
    s16 parentCamId;
    s16 camId;
    s16 prevBgCamIndex;
    s16 csId;
#if defined(GAME_MM)
    Vec3f meshActorPos;
#endif
} Camera;

#if defined(GAME_OOT)
_Static_assert(sizeof(Camera) == 0x16c, "MM Camera size is wrong");
#elif defined(GAME_MM)
_Static_assert(sizeof(Camera) == 0x178, "MM Camera size is wrong");
#endif

typedef struct {
    /* 0x0 */ s16 frameCount;
} AnimationHeaderCommon; /* size = 0x2 */

typedef struct {
    /* 0x0 */ AnimationHeaderCommon common;
    /* 0x4 */ union {
                void* segmentVoid;
                struct PlayerAnimationFrame* linkAnimSegment;
            };
} PlayerAnimationHeader; /* size = 0x8 */

typedef struct {
    u32 cont:   1;
    u32 type:   4;
    u32 offset: 11;
    s32 value:  16;
} InitChainEntry;

#if defined(GAME_MM)
/* TODO: Move to En_Door_Warp1 once ported */
typedef enum
{
    ENDOORWARP1_FF_0,
    ENDOORWARP1_FF_1,
    ENDOORWARP1_FF_2,
    ENDOORWARP1_FF_3,
    ENDOORWARP1_FF_4,
    ENDOORWARP1_FF_5,
    ENDOORWARP1_FF_6
}
DoorWarp1Param;

/* TODO: Move to Item_B_Heart once ported */
typedef enum
{
    BHEART_PARAM_NORMAL,
    BHEART_PARAM_SMALL = 35
}
ItemBHeartParam;

typedef enum
{
    CLEAR_TAG_SMALL_EXPLOSION,
    CLEAR_TAG_LARGE_EXPLOSION,
    CLEAR_TAG_POP,
    CLEAR_TAG_SMALL_LIGHT_RAYS,
    CLEAR_TAG_LARGE_LIGHT_RAYS,
    CLEAR_TAG_SPLASH = 35,
    CLEAR_TAG_SMOKE = 200
}
ClearTagType;

#define BODYPART_NONE -1

// Camera is on
#define CAM_STATUS_CUT        0 // The camera is not updated at all
#define CAM_STATUS_WAIT       1 // There is minimally/partially updated, action function is not run
#define CAM_STATUS_UNK3       3 // The camera is mostly updated including running its action function, but data is not set to view
#define CAM_STATUS_ACTIVE     7 // The camera is fully updated, info is sent to view
// Camera is off
#define CAM_STATUS_INACTIVE   0x100

typedef enum {
    /* 0x00 */ CAM_FUNC_NONE,
    /* 0x01 */ CAM_FUNC_NORMAL0,
    /* 0x02 */ CAM_FUNC_NORMAL1,
    /* 0x03 */ CAM_FUNC_NORMAL2,
    /* 0x04 */ CAM_FUNC_NORMAL3,
    /* 0x05 */ CAM_FUNC_NORMAL4,
    /* 0x06 */ CAM_FUNC_PARALLEL0,
    /* 0x07 */ CAM_FUNC_PARALLEL1,
    /* 0x08 */ CAM_FUNC_PARALLEL2,
    /* 0x09 */ CAM_FUNC_PARALLEL3,
    /* 0x0A */ CAM_FUNC_PARALLEL4,
    /* 0x0B */ CAM_FUNC_KEEPON0,
    /* 0x0C */ CAM_FUNC_KEEPON1,
    /* 0x0D */ CAM_FUNC_KEEPON2,
    /* 0x0E */ CAM_FUNC_KEEPON3,
    /* 0x0F */ CAM_FUNC_KEEPON4,
    /* 0x10 */ CAM_FUNC_SUBJECT0,
    /* 0x11 */ CAM_FUNC_SUBJECT1,
    /* 0x12 */ CAM_FUNC_SUBJECT2,
    /* 0x13 */ CAM_FUNC_SUBJECT3,
    /* 0x14 */ CAM_FUNC_SUBJECT4,
    /* 0x15 */ CAM_FUNC_JUMP0,
    /* 0x16 */ CAM_FUNC_JUMP1,
    /* 0x17 */ CAM_FUNC_JUMP2,
    /* 0x18 */ CAM_FUNC_JUMP3,
    /* 0x19 */ CAM_FUNC_JUMP4,
    /* 0x1A */ CAM_FUNC_BATTLE0,
    /* 0x1B */ CAM_FUNC_BATTLE1,
    /* 0x1C */ CAM_FUNC_BATTLE2,
    /* 0x1D */ CAM_FUNC_BATTLE3,
    /* 0x1E */ CAM_FUNC_BATTLE4,
    /* 0x1F */ CAM_FUNC_FIXED0,
    /* 0x20 */ CAM_FUNC_FIXED1,
    /* 0x21 */ CAM_FUNC_FIXED2,
    /* 0x22 */ CAM_FUNC_FIXED3,
    /* 0x23 */ CAM_FUNC_FIXED4,
    /* 0x24 */ CAM_FUNC_DATA0,
    /* 0x25 */ CAM_FUNC_DATA1,
    /* 0x26 */ CAM_FUNC_DATA2,
    /* 0x27 */ CAM_FUNC_DATA3,
    /* 0x28 */ CAM_FUNC_DATA4,
    /* 0x29 */ CAM_FUNC_UNIQUE0,
    /* 0x2A */ CAM_FUNC_UNIQUE1,
    /* 0x2B */ CAM_FUNC_UNIQUE2,
    /* 0x2C */ CAM_FUNC_UNIQUE3,
    /* 0x2D */ CAM_FUNC_UNIQUE4,
    /* 0x2E */ CAM_FUNC_UNIQUE5,
    /* 0x2F */ CAM_FUNC_UNIQUE6,
    /* 0x30 */ CAM_FUNC_UNIQUE7,
    /* 0x31 */ CAM_FUNC_UNIQUE8,
    /* 0x32 */ CAM_FUNC_UNIQUE9,
    /* 0x33 */ CAM_FUNC_DEMO0,
    /* 0x34 */ CAM_FUNC_DEMO1,
    /* 0x35 */ CAM_FUNC_DEMO2,
    /* 0x36 */ CAM_FUNC_DEMO3,
    /* 0x37 */ CAM_FUNC_DEMO4,
    /* 0x38 */ CAM_FUNC_DEMO5,
    /* 0x39 */ CAM_FUNC_DEMO6,
    /* 0x3A */ CAM_FUNC_DEMO7,
    /* 0x3B */ CAM_FUNC_DEMO8,
    /* 0x3C */ CAM_FUNC_DEMO9,
    /* 0x3D */ CAM_FUNC_SPECIAL0,
    /* 0x3E */ CAM_FUNC_SPECIAL1,
    /* 0x3F */ CAM_FUNC_SPECIAL2,
    /* 0x40 */ CAM_FUNC_SPECIAL3,
    /* 0x41 */ CAM_FUNC_SPECIAL4,
    /* 0x42 */ CAM_FUNC_SPECIAL5,
    /* 0x43 */ CAM_FUNC_SPECIAL6,
    /* 0x44 */ CAM_FUNC_SPECIAL7,
    /* 0x45 */ CAM_FUNC_SPECIAL8,
    /* 0x46 */ CAM_FUNC_SPECIAL9,
    /* 0x47 */ CAM_FUNC_MAX
} CameraFuncType;

typedef enum {
    /* 0x00 */ CAM_DATA_Y_OFFSET,
    /* 0x01 */ CAM_DATA_01,
    /* 0x02 */ CAM_DATA_02,
    /* 0x03 */ CAM_DATA_PITCH_TARGET,
    /* 0x04 */ CAM_DATA_04,
    /* 0x05 */ CAM_DATA_05,
    /* 0x06 */ CAM_DATA_YAW_DIFF_RANGE,
    /* 0x07 */ CAM_DATA_FOV,
    /* 0x08 */ CAM_DATA_08,
    /* 0x09 */ CAM_DATA_INTERFACE_FLAGS,
    /* 0x0A */ CAM_DATA_10,
    /* 0x0B */ CAM_DATA_11,
    /* 0x0C */ CAM_DATA_12,
    /* 0x0D */ CAM_DATA_13,
    /* 0x0E */ CAM_DATA_14,
    /* 0x0F */ CAM_DATA_15,
    /* 0x10 */ CAM_DATA_16,
    /* 0x11 */ CAM_DATA_17,
    /* 0x12 */ CAM_DATA_18,
    /* 0x13 */ CAM_DATA_19,
    /* 0x14 */ CAM_DATA_20,
    /* 0x15 */ CAM_DATA_21
} CameraDataType;


#endif

typedef enum {
    /* 0x0 */ ICHAINTYPE_U8,            /* sets byte */
    /* 0x1 */ ICHAINTYPE_S8,
    /* 0x2 */ ICHAINTYPE_U16,           /* sets short */
    /* 0x3 */ ICHAINTYPE_S16,
    /* 0x4 */ ICHAINTYPE_U32,           /* sets word */
    /* 0x5 */ ICHAINTYPE_S32,
    /* 0x6 */ ICHAINTYPE_F32,           /* sets float */
    /* 0x7 */ ICHAINTYPE_F32_DIV1000,   /* sets float divided by 1000 */
    /* 0x8 */ ICHAINTYPE_VEC3F,         /* sets Vec3f members */
    /* 0x9 */ ICHAINTYPE_VEC3F_DIV1000, /* sets Vec3f members divided by 1000 */
    /* 0xA */ ICHAINTYPE_VEC3S          /* sets Vec3s members */
} InitChainType;

/**
 * ICHAIN macros generate an init chain entry of the following form:
 * * (e >> 31) & 0x0001 == Continue Parsing after this entry
 * * (e >> 27) & 0x000F == Type
 * * (e >> 16) & 0x07FF == Offset from start of instance to write initial value
 * *  e        & 0xFFFF == Initial Value
 *
 * Arguments:
 * * type ----- value from enum `InitChainType`
 * * member --- name of member inside `Actor` structure to use as the offset
 * * value ---- s16 value to use
 * * cont ----- ICHAIN_CONTINUE (or ICHAIN_STOP) to continue (or stop) parsing
 */
#define ICHAIN(type, member, value, cont)      \
        { cont, type, offsetof(Actor, member), value }

#define ICHAIN_U8(member, val, cont)            ICHAIN(ICHAINTYPE_U8, member, val, cont)
#define ICHAIN_S8(member, val, cont)            ICHAIN(ICHAINTYPE_S8, member, val, cont)
#define ICHAIN_U16(member, val, cont)           ICHAIN(ICHAINTYPE_U16, member, val, cont)
#define ICHAIN_S16(member, val, cont)           ICHAIN(ICHAINTYPE_S16, member, val, cont)
#define ICHAIN_U32(member, val, cont)           ICHAIN(ICHAINTYPE_U32, member, val, cont)
#define ICHAIN_S32(member, val, cont)           ICHAIN(ICHAINTYPE_S32, member, val, cont)
#define ICHAIN_F32(member, val, cont)           ICHAIN(ICHAINTYPE_F32, member, val, cont)
#define ICHAIN_F32_DIV1000(member, val, cont)   ICHAIN(ICHAINTYPE_F32_DIV1000, member, val, cont)
#define ICHAIN_VEC3F(member, val, cont)         ICHAIN(ICHAINTYPE_VEC3F, member, val, cont)
#define ICHAIN_VEC3F_DIV1000(member, val, cont) ICHAIN(ICHAINTYPE_VEC3F_DIV1000, member, val, cont)
#define ICHAIN_VEC3S(member, val, cont)         ICHAIN(ICHAINTYPE_VEC3S, member, val, cont)

#define ICHAIN_CONTINUE 1
#define ICHAIN_STOP     0

#define VTX(x,y,z,s,t,crnx,cgny,cbnz,a) { { { x, y, z }, 0, { s, t }, { crnx, cgny, cbnz, a } } }

/* Cannot be pushed by OC colliders */
#define MASS_IMMOVABLE 0xFF
/* Can only be pushed by OC colliders from actors with IMMOVABLE or HEAVY mass. */
#define MASS_HEAVY 0xFE

typedef enum {
    /* 0x00 */ CAM_SET_NONE,
    /* 0x01 */ CAM_SET_NORMAL0, /* Generic camera 0, used in various places "NORMAL0" */
    /* 0x02 */ CAM_SET_NORMAL3, /* Generic camera 3, used in various places "NORMAL3" */
    /* 0x03 */ CAM_SET_PIVOT_DIVING, /* Player diving from the surface of the water to underwater not as zora "CIRCLE5" */
    /* 0x04 */ CAM_SET_HORSE, /* Reiding a horse "HORSE0" */
    /* 0x05 */ CAM_SET_ZORA_DIVING, /* Parallel's Pivot Diving, but as Zora. However, Zora does not dive like a human. So this setting appears to not be used "ZORA0" */
    /* 0x06 */ CAM_SET_PREREND_FIXED, /* Unused remnant of OoT: camera is fixed in position and rotation "PREREND0" */
    /* 0x07 */ CAM_SET_PREREND_PIVOT, /* Unused remnant of OoT: Camera is fixed in position with fixed pitch, but is free to rotate in the yaw direction 360 degrees "PREREND1" */
    /* 0x08 */ CAM_SET_DOORC, /* Generic room door transitions, camera moves and follows player as the door is open and closed "DOORC" */
    /* 0x09 */ CAM_SET_DEMO0, /* Unknown, possibly related to treasure chest game as goron? "DEMO0" */
    /* 0x0A */ CAM_SET_FREE0, /* Free Camera, manual control is given, no auto-updating eye or at "FREE0" */
    /* 0x0B */ CAM_SET_BIRDS_EYE_VIEW_0, /* Appears unused. Camera is a top-down view "FUKAN0" */
    /* 0x0C */ CAM_SET_NORMAL1, /* Generic camera 1, used in various places "NORMAL1" */
    /* 0x0D */ CAM_SET_NANAME, /* Unknown, slanted or tilted. Behaves identical to Normal0 except with added roll "NANAME" */
    /* 0x0E */ CAM_SET_CIRCLE0, /* Used in Curiosity Shop, Pirates Fortress, Mayor's Residence "CIRCLE0" */
    /* 0x0F */ CAM_SET_FIXED0, /* Used in Sakon's Hideout puzzle rooms, milk bar stage "FIXED0" */
    /* 0x10 */ CAM_SET_SPIRAL_DOOR, /* Exiting a Spiral Staircase "SPIRAL" */
    /* 0x11 */ CAM_SET_DUNGEON0, /* Generic dungeon camera 0, used in various places "DUNGEON0" */
    /* 0x12 */ CAM_SET_ITEM0, /* Getting an item and holding it above Player's head (from small chest, freestanding, npc, ...) "ITEM0" */
    /* 0x13 */ CAM_SET_ITEM1, /* Looking at player while playing the ocarina "ITEM1" */
    /* 0x14 */ CAM_SET_ITEM2, /* Bottles: drinking, releasing fairy, dropping fish "ITEM2" */
    /* 0x15 */ CAM_SET_ITEM3, /* Bottles: catching fish or bugs, showing an item "ITEM3" */
    /* 0x16 */ CAM_SET_NAVI, /* Song of Soaring, variations of playing Song of Time "NAVI" */
    /* 0x17 */ CAM_SET_WARP_PAD_MOON, /* Warp circles from Goron Trial on the moon "WARP0" */
    /* 0x18 */ CAM_SET_DEATH, /* Player death animation when health goes to 0 "DEATH" */
    /* 0x19 */ CAM_SET_REBIRTH, /* Unknown set with camDataId = -9 (it's not being revived by a fairy) "REBIRTH" */
    /* 0x1A */ CAM_SET_LONG_CHEST_OPENING, /* Long cutscene when opening a big chest with a major item "TREASURE" */
    /* 0x1B */ CAM_SET_MASK_TRANSFORMATION, /* Putting on a transformation mask "TRANSFORM" */
    /* 0x1C */ CAM_SET_ATTENTION, /* Unknown, set with camDataId = -15 "ATTENTION" */
    /* 0x1D */ CAM_SET_WARP_PAD_ENTRANCE, /* Warp pad from start of a dungeon to the boss-room "WARP1" */
    /* 0x1E */ CAM_SET_DUNGEON1, /* Generic dungeon camera 1, used in various places "DUNGEON1" */
    /* 0x1F */ CAM_SET_FIXED1, /* Fixes camera in place, used in various places eg. entering Stock Pot Inn, hiting a switch, giving witch a red potion, shop browsing "FIXED1" */
    /* 0x20 */ CAM_SET_FIXED2, /* Used in Pinnacle Rock after defeating Sea Monsters, and by Tatl in Fortress "FIXED2" */
    /* 0x21 */ CAM_SET_MAZE, /* Unused. Set to use Camera_Parallel2(), which is only Camera_Noop() "MAZE" */
    /* 0x22 */ CAM_SET_REMOTEBOMB, /* Unused. Set to use Camera_Parallel2(), which is only Camera_Noop(). But also related to Play_ChangeCameraSetting? "REMOTEBOMB" */
    /* 0x23 */ CAM_SET_CIRCLE1, /* Unknown "CIRCLE1" */
    /* 0x24 */ CAM_SET_CIRCLE2, /* Looking at far-away NPCs eg. Garo in Road to Ikana, Hungry Goron, Tingle "CIRCLE2" */
    /* 0x25 */ CAM_SET_CIRCLE3, /* Used in curiosity shop, goron racetrack, final room in Sakon's hideout, other places "CIRCLE3" */
    /* 0x26 */ CAM_SET_CIRCLE4, /* Used during the races on the doggy racetrack "CIRCLE4" */
    /* 0x27 */ CAM_SET_FIXED3, /* Used in Stock Pot Inn Toilet and Tatl cutscene after woodfall "FIXED3" */
    /* 0x28 */ CAM_SET_TOWER_ASCENT, /* Various climbing structures (Snowhead climb to the temple entrance) "TOWER0" */
    /* 0x29 */ CAM_SET_PARALLEL0, /* Unknown "PARALLEL0" */
    /* 0x2A */ CAM_SET_NORMALD, /* Unknown, set with camDataId = -20 "NORMALD" */
    /* 0x2B */ CAM_SET_SUBJECTD, /* Unknown, set with camDataId = -21 "SUBJECTD" */
    /* 0x2C */ CAM_SET_START0, /* Entering a room, either Dawn of a New Day reload, or entering a door where the camera is fixed on the other end "START0" */
    /* 0x2D */ CAM_SET_START2, /* Entering a scene, camera is put at a low angle eg. Grottos, Deku Palace, Stock Pot Inn "START2" */
    /* 0x2E */ CAM_SET_STOP0, /* Called in z_play "STOP0" */
    /* 0x2F */ CAM_SET_BOAT_CRUISE, /*  Koume's boat cruise "JCRUISING" */
    /* 0x30 */ CAM_SET_VERTICAL_CLIMB, /* Large vertical climbs, such as Mountain Village wall or Pirates Fortress ladder. "CLIMBMAZE" */
    /* 0x31 */ CAM_SET_SIDED, /* Unknown, set with camDataId = -24 "SIDED" */
    /* 0x32 */ CAM_SET_DUNGEON2, /* Generic dungeon camera 2, used in various places "DUNGEON2" */
    /* 0x33 */ CAM_SET_BOSS_ODOLWA, /* Odolwa's Lair, also used in GBT entrance: "BOSS_SHIGE" */
    /* 0x34 */ CAM_SET_KEEPBACK, /* Unknown. Possibly related to climbing something? "KEEPBACK" */
    /* 0x35 */ CAM_SET_CIRCLE6, /* Used in select regions from Ikana "CIRCLE6" */
    /* 0x36 */ CAM_SET_CIRCLE7, /* Unknown "CIRCLE7" */
    /* 0x37 */ CAM_SET_MINI_BOSS, /* Used during the various minibosses of the "CHUBOSS" */
    /* 0x38 */ CAM_SET_RFIXED1, /* Talking to Koume stuck on the floor in woods of mystery "RFIXED1" */
    /* 0x39 */ CAM_SET_TREASURE_CHEST_MINIGAME, /* Treasure Chest Shop in East Clock Town, minigame location "TRESURE1" */
    /* 0x3A */ CAM_SET_HONEY_AND_DARLING_1, /* Honey and Darling Minigames "BOMBBASKET" */
    /* 0x3B */ CAM_SET_CIRCLE8, /* Used by Stone Tower moving platforms, Falling eggs in Marine Lab, Bugs into soilpatch cutscene "CIRCLE8" */
    /* 0x3C */ CAM_SET_BIRDS_EYE_VIEW_1, /* Camera is a top-down view. Used in Fisherman's minigame and Deku Palace "FUKAN1" */
    /* 0x3D */ CAM_SET_DUNGEON3, /* Generic dungeon camera 3, used in various places "DUNGEON3" */
    /* 0x3E */ CAM_SET_TELESCOPE, /* Observatory telescope and Curiosity Shop Peep-Hole "TELESCOPE" */
    /* 0x3F */ CAM_SET_ROOM0, /* Certain rooms eg. inside the clock tower "ROOM0" */
    /* 0x40 */ CAM_SET_RCIRC0, /* Used by a few NPC cutscenes, focus close on the NPC "RCIRC0" */
    /* 0x41 */ CAM_SET_CIRCLE9, /* Used by Sakon Hideout entrance and Deku Palace Maze "CIRCLE9" */
    /* 0x42 */ CAM_SET_ONTHEPOLE, /* Somewhere in Snowhead Temple and Woodfall Temple "ONTHEPOLE" */
    /* 0x43 */ CAM_SET_INBUSH, /* Various bush environments eg. grottos, Swamp Spider House, Termina Field grass bushes, Deku Palace near bean "INBUSH" */
    /* 0x44 */ CAM_SET_BOSS_MAJORA, /* Majora's Lair: "BOSS_LAST" */
    /* 0x45 */ CAM_SET_BOSS_TWINMOLD, /* Twinmold's Lair: "BOSS_INI" */
    /* 0x46 */ CAM_SET_BOSS_GOHT, /* Goht's Lair: "BOSS_HAK" */
    /* 0x47 */ CAM_SET_BOSS_GYORG, /* Gyorg's Lair: "BOSS_KON" */
    /* 0x48 */ CAM_SET_CONNECT0, /* Smoothly and gradually return camera to Player after a cutscene "CONNECT0" */
    /* 0x49 */ CAM_SET_PINNACLE_ROCK, /* Pinnacle Rock pit "MORAY" */
    /* 0x4A */ CAM_SET_NORMAL2, /* Generic camera 2, used in various places "NORMAL2" */
    /* 0x4B */ CAM_SET_HONEY_AND_DARLING_2, /* "BOMBBOWL" */
    /* 0x4C */ CAM_SET_CIRCLEA, /* Unknown, Circle 10 "CIRCLEA" */
    /* 0x4D */ CAM_SET_WHIRLPOOL, /* Great Bay Temple Central Room Whirlpool "WHIRLPOOL" */
    /* 0x4E */ CAM_SET_CUCCO_SHACK, /* "KOKKOGAME" */
    /* 0x4F */ CAM_SET_GIANT, /* Giants Mask in Twinmold's Lair "GIANT" */
    /* 0x50 */ CAM_SET_SCENE0, /* Entering doors to a new scene "SCENE0" */
    /* 0x51 */ CAM_SET_ROOM1, /* Certain rooms eg. some rooms in Stock Pot Inn "ROOM1" */
    /* 0x52 */ CAM_SET_WATER2, /* Swimming as Zora in Great Bay Temple "WATER2" */
    /* 0x53 */ CAM_SET_WOODFALL_SWAMP, /* Woodfall inside the swamp, but not on the platforms, "SOKONASI" */
    /* 0x54 */ CAM_SET_FORCEKEEP, /* Unknown "FORCEKEEP" */
    /* 0x55 */ CAM_SET_PARALLEL1, /* Unknown "PARALLEL1" */
    /* 0x56 */ CAM_SET_START1, /* Used when entering the lens cave "START1" */
    /* 0x57 */ CAM_SET_ROOM2, /* Certain rooms eg. Deku King's Chamber, Ocean Spider House "ROOM2" */
    /* 0x58 */ CAM_SET_NORMAL4, /* Generic camera 4, used in Ikana Graveyard "NORMAL4" */
    /* 0x59 */ CAM_SET_ELEGY_SHELL, /* cutscene after playing elegy of emptyness and spawning a shell "SHELL" */
    /* 0x5A */ CAM_SET_DUNGEON4, /* Used in Pirates Fortress Interior, hidden room near hookshot "DUNGEON4" */
    /* 0x5B */ CAM_SET_MAX
} CameraSettingType;

#define CAM_ID_MAIN 0
#define CAM_ID_SUB_FIRST 1
#define CAM_ID_NONE -1

#define SUB_CAM_ID_DONE 0

typedef enum {
    /* 0 */ ROOM_BEHAVIOR_TYPE1_0,
    /* 1 */ ROOM_BEHAVIOR_TYPE1_1,
    /* 2 */ ROOM_BEHAVIOR_TYPE1_2,
    /* 3 */ ROOM_BEHAVIOR_TYPE1_3, /* unused */
    /* 4 */ ROOM_BEHAVIOR_TYPE1_4,
    /* 5 */ ROOM_BEHAVIOR_TYPE1_5
} RoomBehaviorType1;

typedef enum {
    /* 0 */ ROOM_BEHAVIOR_TYPE2_0,
    /* 1 */ ROOM_BEHAVIOR_TYPE2_1,
    /* 2 */ ROOM_BEHAVIOR_TYPE2_2,
    /* 3 */ ROOM_BEHAVIOR_TYPE2_HOT,
    /* 4 */ ROOM_BEHAVIOR_TYPE2_4,
    /* 5 */ ROOM_BEHAVIOR_TYPE2_5,
    /* 6 */ ROOM_BEHAVIOR_TYPE2_6
} RoomBehaviorType2;

#define SQ(x) ((x)*(x))
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define DECR(x) ((x) == 0 ? 0 : --(x))
#define CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
#define CLAMP_MAX(x, max) ((x) > (max) ? (max) : (x))
#define CLAMP_MIN(x, min) ((x) < (min) ? (min) : (x))

#define GET_ACTIVE_CAM(play) ((play)->cameraPtrs[(play)->activeCamId])

typedef struct {
    /* 0x0 */ u16 flags;         /* Only the bottom two bits are used, although others are set in objects */
    /* 0x2 */ s16 abscissa;      /* knot input value */
    /* 0x4 */ s16 leftGradient;  /* left derivative at the point */
    /* 0x6 */ s16 rightGradient; /* right derivative at the point */
    /* 0x8 */ f32 ordinate;      /* output value */
} CurveInterpKnot; /* size = 0xC */

typedef struct {
    /* 0x0 */ u8* knotCounts;
    /* 0x4 */ CurveInterpKnot* interpolationData;
    /* 0x8 */ s16* constantData;
    /* 0xC */ s16 unk_0C;     /* Set but not used, always 1 in objects */
    /* 0xE */ s16 frameCount; /* Not used, inferred from use in objects */
} CurveAnimationHeader; /* size = 0x10 */

typedef struct {
    /* 0x0 */ u8 child;
    /* 0x1 */ u8 sibling;
    /* 0x4 */ Gfx* dList[2];
} SkelCurveLimb; /* size = 0xC */

typedef struct {
    /* 0x0 */ SkelCurveLimb** limbs;
    /* 0x4 */ u8 limbCount;
} CurveSkeletonHeader; /* size = 0x8 */

typedef struct {
    /* 0x00 */ u8 limbCount;
    /* 0x04 */ SkelCurveLimb** skeleton;
    /* 0x08 */ CurveAnimationHeader* animation;
    /* 0x0C */ f32 unk_0C; /* set but not used */
    /* 0x10 */ f32 endFrame;
    /* 0x14 */ f32 playSpeed;
    /* 0x18 */ f32 curFrame;
    /* 0x1C */ s16 (*jointTable)[9];
} SkelCurve; /* size = 0x20 */

#define LIMB_DONE 0xFF

typedef union {
    struct {
        /* 0x00 */ u16 id; /* "dousa" */
        /* 0x02 */ u16 startFrame;
        /* 0x04 */ u16 endFrame;
        /* 0x06 */ Vec3us rot;
        /* 0x0C */ Vec3i startPos;
        /* 0x18 */ Vec3i endPos;
        /* 0x24 */ Vec3f normal;
    };
    s32 _words[12];
} CsCmdActorCue; /* size = 0x30 */

typedef union CutsceneData {
    s32 i;
    f32 f;
    s16 s[2];
    s8  b[4];
} CutsceneData;

typedef struct {
    /* 0x0 */ CutsceneData* script;
    /* 0x4 */ s16 nextEntrance;
    /* 0x6 */ u8 spawn;
    /* 0x7 */ u8 spawnFlags; /* See `CS_SPAWN_FLAG_` */
} CutsceneScriptEntry; /* size = 0x8 */

/* ZAPD compatibility typedefs */
/* TODO: Remove when ZAPD adds support for them */
#if defined(GAME_OOT)
typedef CutsceneScriptEntry CutsceneEntry;
#endif

#if defined(GAME_MM)
typedef struct {
    /* 0x00 */ s16 priority; // Lower means higher priority. -1 means it ignores priority
    /* 0x02 */ s16 length;
    /* 0x04 */ s16 csCamId; // Index of CsCameraEntry to use. Negative indices use sGlobalCamDataSettings. Indices 0 and above use CsCameraEntry from a sceneLayer
    /* 0x06 */ s16 scriptIndex;
    /* 0x08 */ s16 additionalCsId;
    /* 0x0A */ u8 endSfx;
    /* 0x0B */ u8 customValue; // 0 - 99: actor-specific custom value. 100+: spawn. 255: none
    /* 0x0C */ s16 hudVisibility;
    /* 0x0E */ u8 endCam;
    /* 0x0F */ u8 letterboxSize;
} CutsceneEntry; // size = 0x10

/* TODO consolidate type with oot's type */
typedef struct {
    /* 0x00 */ u8 scriptListCount;
    /* 0x04 */ CutsceneData* script;
    /* 0x08 */ u8 state;
    /* 0x0C */ f32 timer;
    /* 0x10 */ u16 curFrame;
    /* 0x12 */ u16 scriptIndex;
    /* 0x14 */ s32 subCamId;
    /* 0x18 */ u16 camEyeSplinePointsAppliedFrame; /* Remnant of OoT. Set but never used. */
    /* 0x1C */ u8 unk_1C[0xA]; /* Remnant of cam data from OoT */
    /* 0x24 */ CsCmdActorCue* playerCue;
    /* 0x28 */ CsCmdActorCue* actorCues[10]; /* "npcdemopnt" */
    /* 0x50 */ CutsceneScriptEntry* scriptList;
} CutsceneContext; /* size = 0x54 */

#define CURRENT_TIME ((void)0, gSaveContext.save.time)
#define SEGADDR_FROM_OFFSET(segment, offset) ((void*)(((segment) << 24) | offset))
#define CHECK_FLAG_ALL(flags, mask) (((flags) & (mask)) == (mask))

#define SEGADDR_CIRCLE_SHADOW_DL (OS_K0_TO_PHYSICAL(SEGADDR_FROM_OFFSET(4, 0x76BC0)))

#endif

typedef struct
{
    s32 active;
    Vec3f tip;
    Vec3f base;
}
WeaponInfo;

typedef enum AnimationMode {
    /* 0 */ ANIMMODE_LOOP,
    /* 1 */ ANIMMODE_LOOP_INTERP,
    /* 2 */ ANIMMODE_ONCE,
    /* 3 */ ANIMMODE_ONCE_INTERP,
    /* 4 */ ANIMMODE_LOOP_PARTIAL,
    /* 5 */ ANIMMODE_LOOP_PARTIAL_INTERP
} AnimationMode;

typedef enum {
    /* 0 */ CS_STATE_IDLE,
    /* 1 */ CS_STATE_START,
    /* 2 */ CS_STATE_RUN,
    /* 3 */ CS_STATE_STOP,
    /* 4 */ CS_STATE_RUN_UNSTOPPABLE
} CutsceneState;

typedef enum {
    /*   -1 */ CS_ID_NONE = -1,
    // CsId's 0 - 119 are sceneLayer-specific and index `CutsceneEntry`
    /* 0x78 */ CS_ID_GLOBAL_78 = 120,
    /* 0x79 */ CS_ID_GLOBAL_79,
    /* 0x7A */ CS_ID_GLOBAL_7A,
    /* 0x7B */ CS_ID_GLOBAL_ELEGY,
    /* 0x7C */ CS_ID_GLOBAL_TALK,
    /* 0x7D */ CS_ID_GLOBAL_DOOR,
    /* 0x7E */ CS_ID_GLOBAL_RETURN_TO_CAM, // smoothly return to the previous camera
    /* 0x7F */ CS_ID_GLOBAL_END
} CutsceneId;

typedef enum RespawnMode {
    /* 0 */ RESPAWN_MODE_DOWN,                          /* "RESTART_MODE_DOWN" */
    /* 1 */ RESPAWN_MODE_RETURN,                        /* "RESTART_MODE_RETURN" */
    /* 2 */ RESPAWN_MODE_TOP,                           /* "RESTART_MODE_TOP" */
    /* 3 */ RESPAWN_MODE_UNK_3,                         /* Related to grottos */
    /* 4 */ RESPAWN_MODE_GORON,                         /* "RESTART_MODE_GORON" */
    /* 5 */ RESPAWN_MODE_ZORA,                          /* "RESTART_MODE_ZORA" */
    /* 6 */ RESPAWN_MODE_DEKU,                          /* "RESTART_MODE_NUTS" */
    /* 7 */ RESPAWN_MODE_HUMAN,                         /* "RESTART_MODE_CHILD" */
    /* 8 */ RESPAWN_MODE_MAX
} RespawnMode;

typedef enum CustomRespawnMode {
    /* 0 */ CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE,
} CustomRespawnMode;

#if defined(GAME_MM)
# define gGrottoData    (gSaveContext.respawn[RESPAWN_MODE_UNK_3].data)
#endif

typedef enum {
    /*  0 */ TRANS_TYPE_WIPE,
    /*  1 */ TRANS_TYPE_TRIFORCE,
    /*  2 */ TRANS_TYPE_FADE_BLACK,
    /*  3 */ TRANS_TYPE_FADE_WHITE,
    /*  4 */ TRANS_TYPE_FADE_BLACK_FAST,
    /*  5 */ TRANS_TYPE_FADE_WHITE_FAST,
    /*  6 */ TRANS_TYPE_FADE_BLACK_SLOW,
    /*  7 */ TRANS_TYPE_FADE_WHITE_SLOW,
    /*  8 */ TRANS_TYPE_WIPE_FAST,
    /*  9 */ TRANS_TYPE_FILL_WHITE_FAST, /* Not actually fast due to bug in Play_UpdateTransition */
    /* 10 */ TRANS_TYPE_FILL_WHITE,
    /* 11 */ TRANS_TYPE_INSTANT,
    /* 12 */ TRANS_TYPE_FILL_BROWN,
    /* 13 */ TRANS_TYPE_FADE_WHITE_CS_DELAYED,
    /* 14 */ TRANS_TYPE_SANDSTORM_PERSIST,
    /* 15 */ TRANS_TYPE_SANDSTORM_END,
    /* 16 */ TRANS_TYPE_CS_BLACK_FILL,
    /* 17 */ TRANS_TYPE_FADE_WHITE_INSTANT,
    /* 18 */ TRANS_TYPE_FADE_GREEN,
    /* 19 */ TRANS_TYPE_FADE_BLUE,
    /* 20 */ TRANS_TYPE_FADE_DYNAMIC, /* Chooses Black or White based on time of day */
    /* 21 */ TRANS_TYPE_CIRCLE,
    /* 22 */ TRANS_TYPE_WIPE5,
    /* transition types 23 - 31 are unused */
    /* transition types 32 - 39 are Wipe4 TODO needs macro */
    /* 38 */ TRANS_TYPE_38 = 38,
    /* transition types 40 - 63 are unused */
    /* transition types 64 - 127 are Wipe3 TODO needs macro */
    /* 64 */ TRANS_TYPE_64 = 64,
    /* 70 */ TRANS_TYPE_70 = 70,
    /* 72 */ TRANS_TYPE_72 = 72,
    /* 73 */ TRANS_TYPE_73,
    /* 80 */ TRANS_TYPE_80 = 80,
    /* 86 */ TRANS_TYPE_86 = 86
} TransitionType;

#define BGCHECKFLAG_GROUND (1 << 0) /* Standing on the ground */
#define BGCHECKFLAG_GROUND_TOUCH (1 << 1) /* Has touched the ground (only active for 1 frame) */
#define BGCHECKFLAG_GROUND_LEAVE (1 << 2) /* Has left the ground (only active for 1 frame) */
#define BGCHECKFLAG_WALL (1 << 3) /* Touching a wall */
#define BGCHECKFLAG_CEILING (1 << 4) /* Touching a ceiling */
#define BGCHECKFLAG_WATER (1 << 5) /* In water */
#define BGCHECKFLAG_WATER_TOUCH (1 << 6) /* Has touched water (reset when leaving water) */
#define BGCHECKFLAG_GROUND_STRICT (1 << 7) /* Strictly on ground (BGCHECKFLAG_GROUND has some leeway) */
#define BGCHECKFLAG_CRUSHED (1 << 8) /* Crushed between a floor and ceiling (triggers a void for player) */
#define BGCHECKFLAG_PLAYER_WALL_INTERACT (1 << 9) /* Only set/used by player, related to interacting with walls */

typedef enum {
    /* 0x00 */ CAM_MODE_NORMAL, /* "NORMAL" */
    /* 0x01 */ CAM_MODE_JUMP, /* "JUMP" */
    /* 0x02 */ CAM_MODE_GORONDASH, /* "GORONDASH" */
    /* 0x03 */ CAM_MODE_DEKUSHOOT, /* "NUTSSHOT" */
    /* 0x04 */ CAM_MODE_BOWARROWZ, /* "BOWARROWZ" */
    /* 0x05 */ CAM_MODE_DEKUFLY, /* "NUTSFLY" */
    /* 0x06 */ CAM_MODE_FIRSTPERSON, /* "SUBJECT" */
    /* 0x07 */ CAM_MODE_FOLLOWBOOMERANG, /* "BOOKEEPON" */
    /* 0x08 */ CAM_MODE_ZORAFIN, /* "ZORAFIN" */
    /* 0x09 */ CAM_MODE_FOLLOWTARGET, /* "KEEPON" */
    /* 0x0A */ CAM_MODE_TARGET, /* "PARALLEL" */
    /* 0x0B */ CAM_MODE_TALK, /* "TALK" */
    /* 0x0C */ CAM_MODE_SLINGSHOT, /* "PACHINCO" */
    /* 0x0D */ CAM_MODE_BOWARROW, /* "BOWARROW" */
    /* 0x0E */ CAM_MODE_BATTLE, /* "BATTLE" */
    /* 0x0F */ CAM_MODE_DEKUHIDE, /* "NUTSHIDE" */
    /* 0x10 */ CAM_MODE_STILL, /* "STILL" */
    /* 0x11 */ CAM_MODE_CHARGE, /* "CHARGE" */
    /* 0x12 */ CAM_MODE_CLIMB, /* "CLIMB" */
    /* 0x13 */ CAM_MODE_CLIMBZ, /* "CLIMBZ" */
    /* 0x14 */ CAM_MODE_HOOKSHOT, /* "FOOKSHOT" */
    /* 0x15 */ CAM_MODE_FREEFALL, /* "FREEFALL" */
    /* 0x16 */ CAM_MODE_HANG, /* "HANG" */
    /* 0x17 */ CAM_MODE_HANGZ, /* "HANGZ */
    /* 0x18 */ CAM_MODE_PUSHPULL, /* "PUSHPULL" */
    /* 0x19 */ CAM_MODE_DEKUFLYZ, /* "CNUTSFLYZ" */
    /* 0x1A */ CAM_MODE_GORONJUMP, /* "GORONJUMP" */
    /* 0x1B */ CAM_MODE_BOOMERANG, /* "BOOMERANG" */
    /* 0x1C */ CAM_MODE_CHARGEZ, /* "CHARGEZ" */
    /* 0x1D */ CAM_MODE_ZORAFINZ, /* "ZORAFINZ" */
    /* 0x1E */ CAM_MODE_MAX
} CameraModeType;

typedef enum {
    /* 0x00 */ DO_ACTION_ATTACK,
    /* 0x01 */ DO_ACTION_CHECK,
    /* 0x02 */ DO_ACTION_ENTER,
    /* 0x03 */ DO_ACTION_RETURN,
    /* 0x04 */ DO_ACTION_OPEN,
    /* 0x05 */ DO_ACTION_JUMP,
    /* 0x06 */ DO_ACTION_DECIDE,
    /* 0x07 */ DO_ACTION_DIVE,
    /* 0x08 */ DO_ACTION_FASTER,
    /* 0x09 */ DO_ACTION_THROW,
    /* 0x0A */ DO_ACTION_NONE,  /* in do_action_static, the texture at this position is NAVI, however this value is in practice the "No Action" value */
    /* 0x0B */ DO_ACTION_CLIMB,
    /* 0x0C */ DO_ACTION_DROP,
    /* 0x0D */ DO_ACTION_DOWN,
    /* 0x0E */ DO_ACTION_QUIT,
    /* 0x0F */ DO_ACTION_SPEAK,
    /* 0x10 */ DO_ACTION_NEXT,
    /* 0x11 */ DO_ACTION_GRAB,
    /* 0x12 */ DO_ACTION_STOP,
    /* 0x13 */ DO_ACTION_PUTAWAY,
    /* 0x14 */ DO_ACTION_REEL,
    /* 0x15 */ DO_ACTION_INFO,
    /* 0x16 */ DO_ACTION_WARP,
    /* 0x17 */ DO_ACTION_SNAP,
    /* 0x18 */ DO_ACTION_EXPLODE,
    /* 0x19 */ DO_ACTION_DANCE,
    /* 0x1A */ DO_ACTION_MARCH,
    /* 0x1B */ DO_ACTION_1,
    /* 0x1C */ DO_ACTION_2,
    /* 0x1D */ DO_ACTION_3,
    /* 0x1E */ DO_ACTION_4,
    /* 0x1F */ DO_ACTION_5,
    /* 0x20 */ DO_ACTION_6,
    /* 0x21 */ DO_ACTION_7,
    /* 0x22 */ DO_ACTION_8,
    /* 0x23 */ DO_ACTION_CURL,
    /* 0x24 */ DO_ACTION_SURFACE,
    /* 0x25 */ DO_ACTION_SWIM,
    /* 0x26 */ DO_ACTION_PUNCH,
    /* 0x27 */ DO_ACTION_POUND,
    /* 0x28 */ DO_ACTION_HOOK,
    /* 0x29 */ DO_ACTION_SHOOT,
    /* 0x2A */ DO_ACTION_MAX,
    /* 0x2A */ TATL_STATE_2A = DO_ACTION_MAX,
    /* 0x2B */ TATL_STATE_2B,
    /* 0x2C */ TATL_STATE_2C
} DoAction;

typedef struct {
    /* 0x0 */ s8 sceneId;
    /* 0x1 */ s8 spawnNum;
    /* 0x2 */ u16 flags;
} EntranceTableEntry; /* size = 0x4 */

#define DO_ACTION_TEX_WIDTH  48
#define DO_ACTION_TEX_HEIGHT 16
#define DO_ACTION_TEX_SIZE ((DO_ACTION_TEX_WIDTH * DO_ACTION_TEX_HEIGHT) / 2)

#define UPDBGCHECKINFO_FLAG_OOT_0   (1 << 0) /* check wall */
#define UPDBGCHECKINFO_FLAG_OOT_1   (1 << 1) /* check ceiling */
#define UPDBGCHECKINFO_FLAG_OOT_2   (1 << 2) /* check floor and water */
#define UPDBGCHECKINFO_FLAG_OOT_3   (1 << 3)
#define UPDBGCHECKINFO_FLAG_OOT_4   (1 << 4)
#define UPDBGCHECKINFO_FLAG_OOT_5   (1 << 5) /* unused */
#define UPDBGCHECKINFO_FLAG_OOT_6   (1 << 6) /* disable water ripples */
#define UPDBGCHECKINFO_FLAG_OOT_7   (1 << 7) /* alternate wall check? */

#define UPDBGCHECKINFO_FLAG_MM_1        (1 << 0) /* check wall */
#define UPDBGCHECKINFO_FLAG_MM_2        (1 << 1) /* check ceiling */
#define UPDBGCHECKINFO_FLAG_MM_4        (1 << 2) /* check floor and water */
#define UPDBGCHECKINFO_FLAG_MM_8        (1 << 3)
#define UPDBGCHECKINFO_FLAG_MM_10       (1 << 4)
#define UPDBGCHECKINFO_FLAG_MM_20       (1 << 5)
#define UPDBGCHECKINFO_FLAG_MM_40       (1 << 6)
#define UPDBGCHECKINFO_FLAG_MM_80       (1 << 7)
#define UPDBGCHECKINFO_FLAG_MM_100      (1 << 8)
#define UPDBGCHECKINFO_FLAG_MM_200      (1 << 9)
#define UPDBGCHECKINFO_FLAG_MM_400      (1 << 10) /* check water */
#define UPDBGCHECKINFO_FLAG_MM_800      (1 << 11)

#define UPDBGCHECKINFO_FLAG_WALL        (1 << 0)
#define UPDBGCHECKINFO_FLAG_CEILING     (1 << 1)
#define UPDBGCHECKINFO_FLAG_FLOOR       (1 << 2)

#endif /* TYPES_H */
