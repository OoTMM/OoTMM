#ifndef COMBO_ACTOR_H
#define COMBO_ACTOR_H

#include <combo/types.h>
#include <combo/actor_id.h>
#include <combo/util.h>
#include <combo/math/vec.h>
#include <combo/misc.h>

#define ACTOR_FLAG_OOT_0    (1 <<  0)
#define ACTOR_FLAG_OOT_1    (1 <<  1)
#define ACTOR_FLAG_OOT_2    (1 <<  2)
#define ACTOR_FLAG_OOT_3    (1 <<  3)
#define ACTOR_FLAG_OOT_4    (1 <<  4)
#define ACTOR_FLAG_OOT_5    (1 <<  5)
#define ACTOR_FLAG_OOT_6    (1 <<  6)
#define ACTOR_FLAG_OOT_7    (1 <<  7)
#define ACTOR_FLAG_OOT_8    (1 <<  8)
#define ACTOR_FLAG_OOT_9    (1 <<  9)
#define ACTOR_FLAG_OOT_10   (1 <<  10)
#define ACTOR_FLAG_OOT_11   (1 <<  11)
#define ACTOR_FLAG_OOT_12   (1 <<  12)
#define ACTOR_FLAG_OOT_13   (1 <<  13)
#define ACTOR_FLAG_OOT_14   (1 <<  14)
#define ACTOR_FLAG_OOT_15   (1 <<  15)
#define ACTOR_FLAG_OOT_16   (1 <<  16)
#define ACTOR_FLAG_OOT_17   (1 <<  17)
#define ACTOR_FLAG_OOT_18   (1 <<  18)
#define ACTOR_FLAG_OOT_19   (1 <<  19)
#define ACTOR_FLAG_OOT_20   (1 <<  20)
#define ACTOR_FLAG_OOT_21   (1 <<  21)
#define ACTOR_FLAG_OOT_22   (1 <<  22)
#define ACTOR_FLAG_OOT_23   (1 <<  23)
#define ACTOR_FLAG_OOT_24   (1 <<  24)
#define ACTOR_FLAG_OOT_25   (1 <<  25)
#define ACTOR_FLAG_OOT_26   (1 <<  26)
#define ACTOR_FLAG_OOT_27   (1 <<  27)
#define ACTOR_FLAG_OOT_28   (1 <<  28)
#define ACTOR_FLAG_OOT_29   (1 <<  29)
#define ACTOR_FLAG_OOT_30   (1 <<  30)
#define ACTOR_FLAG_OOT_31   (1 <<  31)

#define ACTOR_FLAG_MM_1                         (1 <<  0)
#define ACTOR_FLAG_MM_2                         (1 <<  1)
#define ACTOR_FLAG_MM_4                         (1 <<  2)
#define ACTOR_FLAG_MM_8                         (1 <<  3)
#define ACTOR_FLAG_MM_10                        (1 <<  4)
#define ACTOR_FLAG_MM_20                        (1 <<  5)
#define ACTOR_FLAG_MM_40                        (1 <<  6)
#define ACTOR_FLAG_MM_80                        (1 <<  7)
#define ACTOR_FLAG_MM_100                       (1 <<  8)
#define ACTOR_FLAG_MM_200                       (1 <<  9)
#define ACTOR_FLAG_MM_400                       (1 <<  10)
#define ACTOR_FLAG_MM_800                       (1 <<  11)
#define ACTOR_FLAG_MM_1000                      (1 <<  12)
#define ACTOR_FLAG_MM_2000                      (1 <<  13)
#define ACTOR_FLAG_MM_4000                      (1 <<  14)
#define ACTOR_FLAG_MM_8000                      (1 <<  15)
#define ACTOR_FLAG_MM_10000                     (1 <<  16)
#define ACTOR_FLAG_MM_CAN_PRESS_HEAVY_SWITCH    (1 <<  17)
#define ACTOR_FLAG_MM_40000                     (1 <<  18)
#define ACTOR_FLAG_MM_80000                     (1 <<  19)
#define ACTOR_FLAG_MM_100000                    (1 <<  20)
#define ACTOR_FLAG_MM_200000                    (1 <<  21)
#define ACTOR_FLAG_MM_400000                    (1 <<  22)
#define ACTOR_FLAG_MM_800000                    (1 <<  23)
#define ACTOR_FLAG_MM_1000000                   (1 <<  24)
#define ACTOR_FLAG_MM_2000000                   (1 <<  25)
#define ACTOR_FLAG_MM_CAN_PRESS_SWITCH          (1 <<  26)
#define ACTOR_FLAG_MM_8000000                   (1 <<  27)
#define ACTOR_FLAG_MM_10000000                  (1 <<  28)
#define ACTOR_FLAG_MM_20000000                  (1 <<  29)
#define ACTOR_FLAG_MM_40000000                  (1 <<  30)
#define ACTOR_FLAG_MM_80000000                  (1 <<  31)

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
typedef struct GameState_Play GameState_Play;
typedef struct ActorContext ActorContext;
typedef struct Actor Actor;

typedef void (*ActorCallback)(Actor*, GameState_Play*);

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
    u16         variable;
    s8          objectSlot;
    s8          targetMode;

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
    float               uncullZoneForward;
    float               uncullZoneScale;
    float               uncullZoneDownward;
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

#if defined(GAME_MM)
typedef struct
{
    u32     count;
    Actor*  first;
    char    unk_8[0x4];
}
ActorList;

typedef enum TargetMode
{
    TARGET_MODE_0,
    TARGET_MODE_1,
    TARGET_MODE_2,
    TARGET_MODE_3,
    TARGET_MODE_4,
    TARGET_MODE_5,
    TARGET_MODE_6,
    TARGET_MODE_7,
    TARGET_MODE_8,
    TARGET_MODE_9,
    TARGET_MODE_10,
    TARGET_MODE_MAX
}
TargetMode;

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

#define COLORFILTER_COLORFLAG_NONE 0xC000
#define COLORFILTER_COLORFLAG_GRAY 0x8000
#define COLORFILTER_COLORFLAG_RED  0x4000
#define COLORFILTER_COLORFLAG_BLUE 0x0000

#define COLORFILTER_INTENSITY_FLAG 0x8000

#define COLORFILTER_BUFFLAG_XLU    0x2000
#define COLORFILTER_BUFFLAG_OPA    0x0000

#endif

#if defined(GAME_OOT)
# define X(x) (x)
#else
# define X(x) (x + 8)
#endif

ASSERT_OFFSET(Actor, id,        0x000);
ASSERT_OFFSET(Actor, type,      0x002);
ASSERT_OFFSET(Actor, room,      0x003);
ASSERT_OFFSET(Actor, home,      0x008);
ASSERT_OFFSET(Actor, variable,  0x01c);
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
_Static_assert(sizeof(Actor) == 0x13c, "OoT Actor size is wrong");
#endif

#if defined(GAME_MM)
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
#define DYNA_INTERACT_ACTOR_ON_SWITCH (1 << 3) // Like the ACTOR_ON_TOP flag but only actors with ACTOR_FLAG_CAN_PRESS_SWITCH
#define DYNA_INTERACT_ACTOR_ON_HEAVY_SWITCH (1 << 4) // Like the ACTOR_ON_TOP flag but only actors with ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCH

void DynaPolyActor_Init(DynaPolyActor* dynaActor, s32 transformFlags);
void Actor_UpdatePos(Actor *actor);

#endif
