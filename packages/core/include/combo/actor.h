#ifndef COMBO_ACTOR_H
#define COMBO_ACTOR_H

#include <combo/collision_check.h>
#include <combo/types.h>
#include <combo/actor_id.h>
#include <combo/util.h>
#include <combo/math/vec.h>
#include <combo/misc.h>

#define NBITS_TO_MASK(n)        ((1 << (n)) - 1)
#define PARAMS_GET_U(p, s, n)   (((p) >> (s)) & NBITS_TO_MASK(n))
#define PARAMS_GET_S(p, s, n)   (((p) & (NBITS_TO_MASK(n) << (s))) >> (s))

#define ACTOR_FLAG_OOT_5            (1 << 5)
#define ACTOR_FLAG_OOT_6            (1 << 6)
#define ACTOR_FLAG_OOT_9            (1 << 9)
#define ACTOR_FLAG_OOT_10           (1 << 10)
#define ACTOR_FLAG_OOT_13           (1 << 13)
#define ACTOR_FLAG_OOT_29           (1 << 29)
#define ACTOR_FLAG_OOT_30           (1 << 30)
#define ACTOR_FLAG_OOT_31           (1 << 31)

#define ACTOR_FLAG_MM_20                        (1 << 5)
#define ACTOR_FLAG_MM_INSIDE_CULLING_VOLUME     (1 << 6)
#define ACTOR_FLAG_MM_200                       (1 << 9)
#define ACTOR_FLAG_MM_400                       (1 << 10)
#define ACTOR_FLAG_MM_800                       (1 << 11)
#define ACTOR_FLAG_MM_2000                      (1 << 13)
#define ACTOR_FLAG_MM_100000                    (1 << 20)
#define ACTOR_FLAG_MM_200000                    (1 << 21)
#define ACTOR_FLAG_MM_400000                    (1 << 22)
#define ACTOR_FLAG_MM_10000000                  (1 << 28)
#define ACTOR_FLAG_MM_20000000                  (1 << 29)
#define ACTOR_FLAG_MM_80000000                  (1 << 31)

#if defined(GAME_OOT)
# define ACTOR_FLAG_GRASS_DESTROYED             (1 << 11)
# define ACTOR_FLAG_CARRY_X_ROT_INFLUENCE       (1 << 17)
# define ACTOR_FLAG_SFX_ACTOR_POS_2             (1 << 19)
# define ACTOR_AUDIO_FLAG_SFX_CENTERED_1        (1 << 20)
# define ACTOR_AUDIO_FLAG_SFX_CENTERED_2        (1 << 21)
# define ACTOR_FLAG_IGNORE_POINT_LIGHTS         (1 << 22)
# define ACTOR_FLAG_SFX_TIMER                   (1 << 28)
#endif

#if defined(GAME_MM)
# define ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCHES     (1 << 17)
# define ACTOR_FLAG_FOCUS_ACTOR_REFINDABLE       (1 << 19)
# define ACTOR_FLAG_CAMERA_DRIFT_ENABLED         (1 << 30)
#endif

#define ACTOR_FLAG_ATTENTION_ENABLED            (1 << 0)
#define ACTOR_FLAG_HOSTILE                      (1 << 2)
#define ACTOR_FLAG_FRIENDLY                     (1 << 3)
#define ACTOR_FLAG_UPDATE_CULLING_DISABLED      (1 << 4)
#define ACTOR_FLAG_DRAW_CULLING_DISABLED        (1 << 5)
#define ACTOR_FLAG_REACT_TO_LENS                (1 << 7)
#define ACTOR_FLAG_TALK                         (1 << 8)
#define ACTOR_FLAG_IGNORE_QUAKE                 (1 << 12)
#define ACTOR_FLAG_CAN_ATTACH_TO_ARROW          (1 << 14)
#define ACTOR_FLAG_ATTACHED_TO_ARROW            (1 << 15)
#define ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED     (1 << 16)
#define ACTOR_FLAG_TALK_WITH_C_UP               (1 << 18)
#define ACTOR_FLAG_THROW_ONLY                   (1 << 23)
#define ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT      (1 << 24)
#define ACTOR_FLAG_UPDATE_DURING_OCARINA        (1 << 25)
#define ACTOR_FLAG_CAN_PRESS_SWITCHES           (1 << 26)
#define ACTOR_FLAG_LOCK_ON_DISABLED             (1 << 27)
#define ACTOR_FLAG_UCODE_POINT_LIGHT_ENABLED    (1 << 28)

#define ACTORCAT_SWITCH         0x00
#define ACTORCAT_BG             0x01
#define ACTORCAT_PLAYER         0x02
#define ACTORCAT_EXPLOSIVE      0x03
#define ACTORCAT_NPC            0x04
#define ACTORCAT_ENEMY          0x05
#define ACTORCAT_PROP           0x06
#define ACTORCAT_ITEMACTION     0x07
#define ACTORCAT_MISC           0x08
#define ACTORCAT_BOSS           0x09
#define ACTORCAT_DOOR           0x0a
#define ACTORCAT_CHEST          0x0b
#define ACTORCAT_MAX            0x0c

typedef struct CollisionPoly CollisionPoly;
typedef struct PlayState PlayState;
typedef struct ActorContext ActorContext;
typedef struct Actor Actor;

typedef void (*ActorCallback)(Actor*, PlayState*);

typedef struct
{
    Vec3f pos;
    Vec3s rot;
}
PosRot;

typedef struct
{
    Vec3s   rot;
    s16     face;
    f32     yOffset;
    void*   shadowDraw;
    f32     shadowScale;
    u8      shadowAlpha;
    u8      feetFloorFlags;
    u8      unk_16;
    u8      unk_17;
    Vec3f   feetPos[2];
}
ActorShape;

ASSERT_SIZE(ActorShape, 0x30);
ASSERT_OFFSET(ActorShape, rot,              0x00);
ASSERT_OFFSET(ActorShape, face,             0x06);
ASSERT_OFFSET(ActorShape, yOffset,          0x08);
ASSERT_OFFSET(ActorShape, shadowDraw,       0x0c);
ASSERT_OFFSET(ActorShape, shadowScale,      0x10);
ASSERT_OFFSET(ActorShape, shadowAlpha,      0x14);
ASSERT_OFFSET(ActorShape, feetFloorFlags,   0x15);
ASSERT_OFFSET(ActorShape, unk_16,           0x16);
ASSERT_OFFSET(ActorShape, unk_17,           0x17);
ASSERT_OFFSET(ActorShape, feetPos,          0x18);

typedef struct Actor
{
    u16         id;
    u8          type;
    u8          room;
    s32         flags;
    PosRot      home;
    s16         params;
    s8          objectSlot;
    s8          attentionRangeType;

#if defined(GAME_OOT)
    u16        sfx;
    u8         actorIndex;
    PosRot     world;
    PosRot     focus;
#endif

#if defined(GAME_MM)
    s16        halfDaysBits;
    u8         actorIndex;
    PosRot     world;
    s8         csId;
    u8         audioFlags;
    PosRot     focus;
    u16        sfx;
#endif

    float       targetArrowOffset;
    Vec3f       scale;
    Vec3f       velocity;
    float       speed;
    float       gravity;
    float       minVelocityY;
    CollisionPoly* wallPoly; /* Wall polygon the actor is touching */
    CollisionPoly* floorPoly; /* Floor polygon directly below the actor */
    u8          wallBgId; /* Bg ID of the wall polygon the actor is touching */
    u8          floorBgId; /* Bg ID of the floor polygon directly below the actor */
    s16         wallYaw; /* Y rotation of the wall polygon the actor is touching */
    f32         floorHeight; /* Y position of the floor polygon directly below the actor */
    f32         depthInWater; /* Distance to the surface of active waterbox. Negative value means above water */
    u16         bgCheckFlags; /* Flags indicating how the actor is interacting with collision */
    s16         yawTowardsPlayer; /* Y rotation difference between the actor and the player */
    f32         xyzDistToPlayerSq; /* Squared distance between the actor and the player */
    f32         xzDistToPlayer;
    f32         yDistanceFromLink;
    CollisionCheckInfo  colChkInfo;
    ActorShape          shape;
    Vec3f               projectedPos;
    float               projectedW;
    float               cullingVolumeDistance;
    float               cullingVolumeScale;
    float               cullingVolumeDownward;
    Vec3f               prevPos;
    u8                  isTargeted;
    u8                  targetPriority;
    u16                 messageId;
    u16                 freezeTimer;
    u16                 colorFilterParams;
    u8                  colorFilterTimer;
    u8                  isDrawn;
    u8                  dropFlag;
    u8                  hintId;
    Actor*              parent;
    Actor*              child;
    Actor*              prev;
    Actor*              next;
    void*               init;
    void*               fini;
    void*               update;
    void*               draw;
    void*               ovl;
}
Actor;

typedef struct DynaPolyActor
{
    Actor   actor;
    s32     bgId;
    f32     unk_150;
    f32     unk_154;
    s16     unk_158;
    u32     transformFlags;
    u8      interactFlags;
    s16     unk_162;
}
DynaPolyActor;

#define COLORFILTER_COLORFLAG_GRAY 0x8000
#define COLORFILTER_COLORFLAG_RED  0x4000
#define COLORFILTER_COLORFLAG_BLUE 0x0000

#define COLORFILTER_INTENSITY_FLAG 0x8000

#define COLORFILTER_BUFFLAG_XLU    0x2000
#define COLORFILTER_BUFFLAG_OPA    0x0000

#if defined(GAME_MM)
typedef struct
{
    u32     count;
    Actor*  first;
    char    unk_8[0x4];
}
ActorList;

typedef enum
{
    TATL_HINT_ID_DEFAULT,
    TATL_HINT_ID_SNAPPER,
    TATL_HINT_ID_MINI_BABA,
    TATL_HINT_ID_MAD_JELLY,
    TATL_HINT_ID_SKULLTULA,
    TATL_HINT_ID_RED_CHUCHU,
    TATL_HINT_ID_BLUE_CHUCHU,
    TATL_HINT_ID_DEKU_BABA,
    TATL_HINT_ID_BIO_DEKU_BABA,
    TATL_HINT_ID_WILTED_DEKU_BABA,
    TATL_HINT_ID_DEXIHAND,
    TATL_HINT_ID_NEJIRON,
    TATL_HINT_ID_GIANT_BEE,
    TATL_HINT_ID_DODONGO,
    TATL_HINT_ID_DEEP_PYTHON,
    TATL_HINT_ID_DEATH_ARMOS,
    TATL_HINT_ID_DINOLFOS,
    TATL_HINT_ID_FIRE_KEESE,
    TATL_HINT_ID_KEESE,
    TATL_HINT_ID_ARMOS,
    TATL_HINT_ID_EENO,
    TATL_HINT_ID_DRAGONFLY,
    TATL_HINT_ID_IGOS_DU_IKANA,
    TATL_HINT_ID_GARO,
    TATL_HINT_ID_GARO_MASTER,
    TATL_HINT_ID_WART,
    TATL_HINT_ID_GOMESS,
    TATL_HINT_ID_GOHT,
    TATL_HINT_ID_BLUE_BUBBLE,
    TATL_HINT_ID_KINGS_LACKEYS_DARK_ROOM,
    TATL_HINT_ID_KINGS_LACKEYS_LIGHT_ROOM,
    TATL_HINT_ID_SKULLWALLTULA,
    TATL_HINT_ID_GOLD_SKULLTULA,
    TATL_HINT_ID_CAPTAIN_KEETA,
    TATL_HINT_ID_YELLOW_CHUCHU,
    TATL_HINT_ID_TAKKURI,
    TATL_HINT_ID_RED_BUBBLE,
    TATL_HINT_ID_HIPLOOP,
    TATL_HINT_ID_MASKED_HIPLOOP,
    TATL_HINT_ID_SHELLBLADE,
    TATL_HINT_ID_GYORG,
    TATL_HINT_ID_GYORG_STUNNED,
    TATL_HINT_ID_REDEAD,
    TATL_HINT_ID_2B,
    TATL_HINT_ID_BLACK_BOE,
    TATL_HINT_ID_GIBDO,
    TATL_HINT_ID_TWINMOLD,
    TATL_HINT_ID_WEARING_GIANTS_MASK,
    TATL_HINT_ID_WALLMASTER,
    TATL_HINT_ID_FLOORMASTER,
    TATL_HINT_ID_MAJORAS_MASK,
    TATL_HINT_ID_MAJORAS_INCARNATION,
    TATL_HINT_ID_MAJORAS_WRATH,
    TATL_HINT_ID_IRON_KNUCKLE,
    TATL_HINT_ID_36,
    TATL_HINT_ID_LIKE_LIKE,
    TATL_HINT_ID_38,
    TATL_HINT_ID_BEAMOS,
    TATL_HINT_ID_3A,
    TATL_HINT_ID_FREEZARD,
    TATL_HINT_ID_WHITE_BOE,
    TATL_HINT_ID_3D,
    TATL_HINT_ID_3E,
    TATL_HINT_ID_3F,
    TATL_HINT_ID_40,
    TATL_HINT_ID_41,
    TATL_HINT_ID_OCTOROK,
    TATL_HINT_ID_43,
    TATL_HINT_ID_POE,
    TATL_HINT_ID_GEKKO_SNAPPER,
    TATL_HINT_ID_BLUE_TEKTITE,
    TATL_HINT_ID_LEEVER,
    TATL_HINT_ID_PEAHAT,
    TATL_HINT_ID_PEAHAT_LARVA,
    TATL_HINT_ID_EYEGORE,
    TATL_HINT_ID_WIZROBE,
    TATL_HINT_ID_WOLFOS,
    TATL_HINT_ID_MAD_SCRUB,
    TATL_HINT_ID_4E,
    TATL_HINT_ID_4F,
    TATL_HINT_ID_POE_SISTER_MEG,
    TATL_HINT_ID_POE_SISTER_JO,
    TATL_HINT_ID_POE_SISTER_BETH,
    TATL_HINT_ID_POE_SISTER_AMY,
    TATL_HINT_ID_PIRATE,
    TATL_HINT_ID_STALCHILD,
    TATL_HINT_ID_ICE_KEESE,
    TATL_HINT_ID_WHITE_WOLFOS,
    TATL_HINT_ID_GUAY,
    TATL_HINT_ID_BIG_OCTO,
    TATL_HINT_ID_BIG_POE,
    TATL_HINT_ID_SKULLFISH,
    TATL_HINT_ID_DESBREKO,
    TATL_HINT_ID_GREEN_CHUCHU,
    TATL_HINT_ID_ODOLWA_PHASE_ONE,
    TATL_HINT_ID_GEKKO_GIANT_SLIME,
    TATL_HINT_ID_BAD_BAT,
    TATL_HINT_ID_REAL_BOMBCHU,
    TATL_HINT_ID_ODOLWA_CLOSE_TO_PHASE_TWO,
    TATL_HINT_ID_ODOLWA_PHASE_TWO,
    TATL_HINT_ID_MUSHROOM,
    TATL_HINT_ID_NONE = 0xFF
}
TatlHintId;

typedef enum
{
    ACTOR_DRAW_DMGEFF_FIRE,
    ACTOR_DRAW_DMGEFF_BLUE_FIRE,
    ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX = 10,
    ACTOR_DRAW_DMGEFF_FROZEN_SFX,
    ACTOR_DRAW_DMGEFF_LIGHT_ORBS = 20,
    ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS,
    ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL = 30,
    ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM,
    ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE
}
ActorDrawDamageEffectType;

typedef enum {
    /* 0 */ PLAYER_IMPACT_GORON_GROUND_POUND,
    /* 1 */ PLAYER_IMPACT_ZORA_BARRIER,
    /* 2 */ PLAYER_IMPACT_BONK // also activated by goron attack
} PlayerImpactType;

#define COLORFILTER_COLORFLAG_NONE 0xC000

Actor* func_800BC270(PlayState* play, Actor* actor, f32 distance, u32 dmgFlags);
s16 Actor_WorldPitchTowardPoint(Actor* actor, Vec3f* refPoint);
void Flags_SetClearTemp(PlayState* play, s32 roomNumber);
u8 Actor_ApplyDamage(Actor* actor);
void Actor_MoveWithoutGravity(Actor* actor);
s16 Actor_WorldYawTowardActor(Actor* actorA, Actor* actorB);
#endif

typedef enum AttentionRangeType
{
    ATTENTION_RANGE_0,  // 70   / 140
    ATTENTION_RANGE_1,  // 170  / 255
    ATTENTION_RANGE_2,  // 280  / 5600
    ATTENTION_RANGE_3,  // 350  / 525   (default)
    ATTENTION_RANGE_4,  // 700  / 1050
    ATTENTION_RANGE_5,  // 1000 / 1500
    ATTENTION_RANGE_6,  // 100  / 105.36842
    ATTENTION_RANGE_7,  // 140  / 163.33333
    ATTENTION_RANGE_8,  // 240  / 576
    ATTENTION_RANGE_9,  // 280  / 280000
    #if defined(GAME_MM)
    ATTENTION_RANGE_10, // 2500 / 3750
    #endif
    ATTENTION_RANGE_MAX
}
AttentionRangeType;

#if defined(GAME_OOT)
# define X(x) (x)
#else
# define X(x) (x + 8)
#endif

ASSERT_OFFSET(Actor, id,        0x000);
ASSERT_OFFSET(Actor, type,      0x002);
ASSERT_OFFSET(Actor, room,      0x003);
ASSERT_OFFSET(Actor, home,      0x008);
ASSERT_OFFSET(Actor, params,    0x01c);
ASSERT_OFFSET(Actor, parent,    X(0x118));
ASSERT_OFFSET(Actor, child,     X(0x11c));
ASSERT_OFFSET(Actor, prev,      X(0x120));
ASSERT_OFFSET(Actor, next,      X(0x124));
ASSERT_OFFSET(Actor, init,      X(0x128));
ASSERT_OFFSET(Actor, fini,      X(0x12c));
ASSERT_OFFSET(Actor, update,    X(0x130));
ASSERT_OFFSET(Actor, draw,      X(0x134));
ASSERT_OFFSET(Actor, ovl,       X(0x138));

#undef X

#if defined(GAME_OOT)
ASSERT_OFFSET(Actor, sfx, 0x020);
_Static_assert(sizeof(Actor) == 0x13c, "OoT Actor size is wrong");
#endif

#if defined(GAME_MM)
ASSERT_OFFSET(Actor, sfx, 0x050);
_Static_assert(sizeof(Actor) == 0x144, "MM Actor size is wrong");
_Static_assert(sizeof(ActorList) == 0xC, "MM ActorList size is wrong");
#endif

typedef struct
{
    s16 eyeTexIndex;
    s16 blinkTimer;
}
BlinkInfo;

ASSERT_SIZE(BlinkInfo, 0x4);
ASSERT_OFFSET(BlinkInfo, eyeTexIndex, 0x0);
ASSERT_OFFSET(BlinkInfo, blinkTimer, 0x2);

void* actorAddr(u16 actorId, u32 addr);

#define DYNA_TRANSFORM_POS (1 << 0) // Position of the actors on top follows the dynapoly actor's movement.
#define DYNA_TRANSFORM_ROT_Y (1 << 1) // The Y rotation of the actors on top follows the dynapoly actor's Y rotation.

#define DYNA_INTERACT_ACTOR_ON_TOP (1 << 0) // There is an actor standing on the collision of the dynapoly actor
#define DYNA_INTERACT_PLAYER_ON_TOP (1 << 1) // The player actor is standing on the collision of the dynapoly actor
#define DYNA_INTERACT_PLAYER_ABOVE (1 << 2) // The player is directly above the collision of the dynapoly actor (any distance above)
#define DYNA_INTERACT_ACTOR_ON_SWITCH (1 << 3) // Like the ACTOR_ON_TOP flag but only actors with ACTOR_FLAG_CAN_PRESS_SWITCHES
#define DYNA_INTERACT_ACTOR_ON_HEAVY_SWITCH (1 << 4) // Like the ACTOR_ON_TOP flag but only actors with ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCHES

void DynaPolyActor_Init(DynaPolyActor* dynaActor, s32 transformFlags);
void Actor_UpdatePos(Actor *actor);

#if defined(GAME_MM)
s32 func_800B90AC(PlayState* play, Actor* actor, CollisionPoly* polygon, s32 bgId, Vec3f* arg4);
void func_800BBFB0(PlayState* play, Vec3f* position, f32 arg2, s32 arg3, s16 arg4, s16 scaleStep, u8 arg6);
s32 Actor_SetPlayerImpact(PlayState* play, PlayerImpactType type, s32 timer, f32 dist, Vec3f* pos);
void Actor_RequestQuakeAndRumble(Actor* actor, PlayState* play, s16 quakeY, s16 quakeDuration);
#endif
void Actor_SetClosestSecretDistance(Actor* actor, PlayState* play);

#if defined(GAME_OOT)
typedef enum NaviEnemy {
    /* 0x00 */ NAVI_ENEMY_DEFAULT,
    /* 0x01 */ NAVI_ENEMY_GOHMA,
    /* 0x02 */ NAVI_ENEMY_GOHMA_EGG,
    /* 0x03 */ NAVI_ENEMY_GOHMA_LARVA,
    /* 0x04 */ NAVI_ENEMY_SKULLTULA,
    /* 0x05 */ NAVI_ENEMY_BIG_SKULLTULA,
    /* 0x06 */ NAVI_ENEMY_TAILPASARAN,
    /* 0x07 */ NAVI_ENEMY_DEKU_BABA,
    /* 0x08 */ NAVI_ENEMY_BIG_DEKU_BABA,
    /* 0x09 */ NAVI_ENEMY_WITHERED_DEKU_BABA,
    /* 0x0A */ NAVI_ENEMY_DEKU_SCRUB,
    /* 0x0B */ NAVI_ENEMY_UNUSED_B,
    /* 0x0C */ NAVI_ENEMY_KING_DODONGO,
    /* 0x0D */ NAVI_ENEMY_DODONGO,
    /* 0x0E */ NAVI_ENEMY_BABY_DODONGO,
    /* 0x0F */ NAVI_ENEMY_LIZALFOS,
    /* 0x10 */ NAVI_ENEMY_DINOLFOS,
    /* 0x11 */ NAVI_ENEMY_FIRE_KEESE,
    /* 0x12 */ NAVI_ENEMY_KEESE,
    /* 0x13 */ NAVI_ENEMY_ARMOS,
    /* 0x14 */ NAVI_ENEMY_BARINADE,
    /* 0x15 */ NAVI_ENEMY_PARASITIC_TENTACLE,
    /* 0x16 */ NAVI_ENEMY_SHABOM,
    /* 0x17 */ NAVI_ENEMY_BIRI,
    /* 0x18 */ NAVI_ENEMY_BARI,
    /* 0x19 */ NAVI_ENEMY_STINGER,
    /* 0x1A */ NAVI_ENEMY_PHANTOM_GANON_PHASE_2,
    /* 0x1B */ NAVI_ENEMY_STALFOS,
    /* 0x1C */ NAVI_ENEMY_BLUE_BUBBLE,
    /* 0x1D */ NAVI_ENEMY_WHITE_BUBBLE,
    /* 0x1E */ NAVI_ENEMY_GREEN_BUBBLE,
    /* 0x1F */ NAVI_ENEMY_SKULLWALLTULA,
    /* 0x20 */ NAVI_ENEMY_GOLD_SKULLTULA,
    /* 0x21 */ NAVI_ENEMY_VOLVAGIA,
    /* 0x22 */ NAVI_ENEMY_FLARE_DANCER,
    /* 0x23 */ NAVI_ENEMY_TORCH_SLUG,
    /* 0x24 */ NAVI_ENEMY_RED_BUBBLE,
    /* 0x25 */ NAVI_ENEMY_MORPHA,
    /* 0x26 */ NAVI_ENEMY_DARK_LINK,
    /* 0x27 */ NAVI_ENEMY_SHELL_BLADE,
    /* 0x28 */ NAVI_ENEMY_SPIKE,
    /* 0x29 */ NAVI_ENEMY_BONGO_BONGO,
    /* 0x2A */ NAVI_ENEMY_REDEAD,
    /* 0x2B */ NAVI_ENEMY_PHANTOM_GANON_PHASE_1,
    /* 0x2C */ NAVI_ENEMY_UNUSED_2C,
    /* 0x2D */ NAVI_ENEMY_GIBDO,
    /* 0x2E */ NAVI_ENEMY_DEAD_HANDS_HAND,
    /* 0x2F */ NAVI_ENEMY_DEAD_HAND,
    /* 0x30 */ NAVI_ENEMY_WALLMASTER,
    /* 0x31 */ NAVI_ENEMY_FLOORMASTER,
    /* 0x32 */ NAVI_ENEMY_TWINROVA_KOUME,
    /* 0x33 */ NAVI_ENEMY_TWINROVA_KOTAKE,
    /* 0x34 */ NAVI_ENEMY_IRON_KNUCKLE_NABOORU,
    /* 0x35 */ NAVI_ENEMY_IRON_KNUCKLE,
    /* 0x36 */ NAVI_ENEMY_SKULL_KID_ADULT,
    /* 0x37 */ NAVI_ENEMY_LIKE_LIKE,
    /* 0x38 */ NAVI_ENEMY_UNUSED_38,
    /* 0x39 */ NAVI_ENEMY_BEAMOS,
    /* 0x3A */ NAVI_ENEMY_ANUBIS,
    /* 0x3B */ NAVI_ENEMY_FREEZARD,
    /* 0x3C */ NAVI_ENEMY_UNUSED_3C,
    /* 0x3D */ NAVI_ENEMY_GANONDORF,
    /* 0x3E */ NAVI_ENEMY_GANON,
    /* 0x3F */ NAVI_ENEMY_SKULL_KID,
    /* 0x40 */ NAVI_ENEMY_SKULL_KID_FRIENDLY,
    /* 0x41 */ NAVI_ENEMY_SKULL_KID_MASK,
    /* 0x42 */ NAVI_ENEMY_OCTOROK,
    /* 0x43 */ NAVI_ENEMY_POE_COMPOSER,
    /* 0x44 */ NAVI_ENEMY_POE,
    /* 0x45 */ NAVI_ENEMY_RED_TEKTITE,
    /* 0x46 */ NAVI_ENEMY_BLUE_TEKTITE,
    /* 0x47 */ NAVI_ENEMY_LEEVER,
    /* 0x48 */ NAVI_ENEMY_PEAHAT,
    /* 0x49 */ NAVI_ENEMY_PEAHAT_LARVA,
    /* 0x4A */ NAVI_ENEMY_MOBLIN,
    /* 0x4B */ NAVI_ENEMY_MOBLIN_CLUB,
    /* 0x4C */ NAVI_ENEMY_WOLFOS,
    /* 0x4D */ NAVI_ENEMY_MAD_SCRUB,
    /* 0x4E */ NAVI_ENEMY_BUSINESS_SCRUB,
    /* 0x4F */ NAVI_ENEMY_DAMPES_GHOST,
    /* 0x50 */ NAVI_ENEMY_POE_SISTER_MEG,
    /* 0x51 */ NAVI_ENEMY_POE_SISTER_JOELLE,
    /* 0x52 */ NAVI_ENEMY_POE_SISTER_BETH,
    /* 0x53 */ NAVI_ENEMY_POE_SISTER_AMY,
    /* 0x54 */ NAVI_ENEMY_GERUDO_THIEF,
    /* 0x55 */ NAVI_ENEMY_STALCHILD,
    /* 0x56 */ NAVI_ENEMY_ICE_KEESE,
    /* 0x57 */ NAVI_ENEMY_WHITE_WOLFOS,
    /* 0x58 */ NAVI_ENEMY_GUAY,
    /* 0x59 */ NAVI_ENEMY_BIGOCTO,
    /* 0x5A */ NAVI_ENEMY_BIG_POE,
    /* 0x5B */ NAVI_ENEMY_TWINROVA,
    /* 0x5C */ NAVI_ENEMY_POE_WASTELAND,
    /* 0xFF */ NAVI_ENEMY_NONE = 0xFF
} NaviEnemy;
#endif

typedef enum NpcTalkState {
    /* 0x0 */ NPC_TALK_STATE_IDLE, // NPC not currently talking to player
    /* 0x1 */ NPC_TALK_STATE_TALKING, // NPC is currently talking to player
    /* 0x2 */ NPC_TALK_STATE_ACTION, // An NPC-defined action triggered in the conversation
    /* 0x3 */ NPC_TALK_STATE_ITEM_GIVEN // NPC finished giving an item and text box is done
} NpcTalkState;

typedef enum NpcTrackingMode {
    /* 0x0 */ NPC_TRACKING_PLAYER_AUTO_TURN, // Determine tracking mode based on player position, see Npc_UpdateAutoTurn
    /* 0x1 */ NPC_TRACKING_NONE, // Don't track the target (usually the player)
    /* 0x2 */ NPC_TRACKING_HEAD_AND_TORSO, // Track target by turning the head and the torso
    /* 0x3 */ NPC_TRACKING_HEAD, // Track target by turning the head
    /* 0x4 */ NPC_TRACKING_FULL_BODY // Track target by turning the body, torso and head
} NpcTrackingMode;

typedef struct NpcInteractInfo {
    /* 0x00 */ s16 talkState;
    /* 0x02 */ s16 trackingMode;
    /* 0x04 */ s16 autoTurnTimer;
    /* 0x06 */ s16 autoTurnState;
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ f32 yOffset; // Y position offset to add to actor position when calculating angle to target
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x4];
} NpcInteractInfo;

// Converts a number of bits to a bitmask, helper for params macros
// e.g. 3 becomes 0b111 (7)
#define NBITS_TO_MASK(n) \
    ((1 << (n)) - 1)

// Extracts the `n`-bit value at position `s` in `p`, shifts then masks
// Unsigned variant, no possibility of sign extension
#define PARAMS_GET_U(p, s, n) \
    (((p) >> (s)) & NBITS_TO_MASK(n))

// Extracts the `n`-bit value at position `s` in `p`, masks then shifts
// Signed variant, possibility of sign extension
#define PARAMS_GET_S(p, s, n) \
    (((p) & (NBITS_TO_MASK(n) << (s))) >> (s))

// Extracts all bits past position `s` in `p`
#define PARAMS_GET_NOMASK(p, s) \
    ((p) >> (s))

// Extracts the `n`-bit value at position `s` in `p` without shifting it from its current position
#define PARAMS_GET_NOSHIFT(p, s, n) \
    ((p) & (NBITS_TO_MASK(n) << (s)))

// Generates a bitmask for bit position `s` of length `n`
#define PARAMS_MAKE_MASK(s, n) PARAMS_GET_NOSHIFT(~0, s, n)

#if defined(GAME_OOT)
void func_80033480(struct PlayState* play, Vec3f* posBase, f32 randRangeDiameter, s32 amountMinusOne, s16 scaleBase,
                   s16 scaleStep, u8 arg6);
void Actor_SetObjectDependency(struct PlayState* play, Actor* actor);
#else
extern Gfx D_801AEFA0[];
extern Gfx D_801AEF88[];
#endif

#endif
