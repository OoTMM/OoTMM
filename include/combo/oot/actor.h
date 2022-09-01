#ifndef OOT_ACTOR_H
#define OOT_ACTOR_H

#include <combo/types.h>

#define AC_ITEM_CUSTOM      0x001
#define AC_EN_BOX           0x00a
#define AC_ITEM00           0x015
#define AC_DOOR_WARP1       0x05d
#define AC_ITEM_B_HEART     0x05f
#define AC_EN_DU            0x098
#define AC_EN_MA1           0x0e7
#define AC_ITEM_OCARINA     0x0f1
#define AC_ITEM_ETCETERA    0x10f
#define AC_BG_SPOT03_TAKI   0x11f
#define AC_EN_DIVING_GAME   0x124
#define AC_EN_NIW_LADY      0x13c
#define AC_EN_SA            0x146
#define AC_EN_OWL           0x14d

typedef struct GameState_Play GameState_Play;

typedef struct PACKED ALIGNED(0x4) Actor
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
    char        unk5[0x6e];
    void*       init;
    void*       fini;
    void*       update;
    void*       draw;
    void*       ovl;
}
Actor;

_Static_assert(sizeof(Actor) == 0x13c, "OoT Actor size is wrong");

Actor* SpawnActor(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable);
void   ActorDestroy(Actor* actor);
int    HasActorGivenItem(Actor* actor);

#endif /* OOT_ACTOR_H */
