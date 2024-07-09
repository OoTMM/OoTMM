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
    PosRot     world;
    PosRot     focus;
#endif

#if defined(GAME_MM)
    s16        halfDaysBits;
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
    char                unk_11A[0x6];
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

void DynaPolyActor_Init(DynaPolyActor* dynaActor, s32 transformFlags);
void Actor_UpdatePos(Actor *actor);

#endif
