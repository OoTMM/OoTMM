#ifndef OOT_ACTOR_H
#define OOT_ACTOR_H

#include <combo/types.h>

#define AC_EN_BOX           0x0a
#define AC_DOOR_WARP1       0x5d
#define AC_ITEM_B_HEART     0x5f
#define AC_EN_MA1           0xe7
#define AC_ITEM_ETCETERA    0x10f
#define AC_EN_NIW_LADY      0x13c
#define AC_EN_SA            0x146
#define AC_EN_OWL           0x14d

typedef struct GameState_Play GameState_Play;

typedef struct PACKED ALIGNED(0x4)
{
    u16         id;
    u8          type;
    u8          room;
    s32         flags;
    char        unk[0xc];
    Vector3s    initRot;
    char        unk2[0x02];
    u16         variable;
    s8          objTableIndex;
    char        unk3[5];
    Vector3f    position;
    char        unk4[0x84];
    Vector3s    rot2;
    char        unk5[0x82];
}
Actor;

_Static_assert(sizeof(Actor) == 0x13c, "OoT Actor size is wrong");

Actor* SpawnActor(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable);
void   ActorDestroy(Actor* actor);
int    IsActorDead(Actor* actor);

#endif /* OOT_ACTOR_H */
