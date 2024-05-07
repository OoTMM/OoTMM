#ifndef COMBO_ACTOR_H
#define COMBO_ACTOR_H

#include <combo/types.h>
#include <combo/actor_id.h>
#include <combo/util.h>
#include <combo/math/vec.h>
#include <combo/misc.h>

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

typedef struct Actor
{
    u16         id;
    u8          type;
    u8          room;
    s32         flags;
    PosRot      home;
    u16         variable;
    s8          objTableIndex;
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
    float       speedXZ;
    float       gravity;
    float       minVelocityY;
    CollisionPoly* wallPoly; /* Wall polygon the actor is touching */
    CollisionPoly* floorPoly; /* Floor polygon directly below the actor */
    u8          wallBgId; /* Bg ID of the wall polygon the actor is touching */
    u8          floorBgId; /* Bg ID of the floor polygon directly below the actor */
    s16         wallYaw; /* Y rotation of the wall polygon the actor is touching */
    f32         floorHeight; /* Y position of the floor polygon directly below the actor */
    f32         yDistToWater; /* Distance to the surface of active waterbox. Negative value means above water */
    u16         bgCheckFlags; /* Flags indicating how the actor is interacting with collision */
    s16         yawTowardsPlayer; /* Y rotation difference between the actor and the player */
    f32         xyzDistToPlayerSq; /* Squared distance between the actor and the player */
    f32         xzDistanceFromLink;
    f32         yDistanceFromLink;
    CollisionCheckInfo colChkInfo;
    Vec3s       rot2;
    char        unk_ba[2];
    float       modelOffsetY;
    char        unk_c0[0x4e];
    u16         messageId;
    u16         freezeTimer;
    char        unk_11A[0x6];
    Actor*      parent;
    Actor*      child;
    Actor*      prev;
    Actor*      next;
    void*       init;
    void*       fini;
    void*       update;
    void*       draw;
    void*       ovl;
}
Actor;

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
ASSERT_OFFSET(Actor, parent, X(0x118));
ASSERT_OFFSET(Actor, child, X(0x11c));
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

Actor* comboSpawnActor(ActorContext* actorCtx, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable);
Actor* comboSpawnActorEx(ActorContext* actorCtx, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3);
void* actorAddr(u16 actorId, u32 addr);

#endif
