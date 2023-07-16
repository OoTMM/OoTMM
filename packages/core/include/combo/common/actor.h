#ifndef COMBO_COMMON_ACTOR_H
#define COMBO_COMMON_ACTOR_H

#if !defined(__ASSEMBLER__)
#include <combo/types.h>
#endif

#if defined(GAME_OOT)
# define AC_ITEM_GIVER              0x003
# define AC_CUSTOM_WARP             0x005
# define AC_CUSTOM_TRIGGERS         0x006
# define AC_EN_DOOR                 0x009
# define AC_EN_BOX                  0x00a
# define AC_ITEM00                  0x015
# define AC_DOOR_SHUTTER            0x02e
# define AC_EN_XC                   0x048
# define AC_DOOR_WARP1              0x05d
# define AC_ITEM_B_HEART            0x05f
# define AC_BG_TOKI_SWD             0x06c
# define AC_EN_DU                   0x098
# define AC_EN_MA1                  0x0e7
# define AC_ITEM_OCARINA            0x0f1
# define AC_ITEM_ETCETERA           0x10f
# define AC_EN_SKJ                  0x115
# define AC_EN_G_SWITCH             0x117
# define AC_BG_SPOT03_TAKI          0x11f
# define AC_EN_DIVING_GAME          0x124
# define AC_EN_NIW_LADY             0x13c
# define AC_EN_GM                   0x13d
# define AC_EN_MS                   0x13e
# define AC_EN_SA                   0x146
# define AC_EN_BOM_BOWL_MAN         0x14b
# define AC_EN_BOM_BOWL_PIT         0x14c
# define AC_EN_OWL                  0x14d
# define AC_EN_ZK                   0x164
# define AC_EN_EX_ITEM              0x168
# define AC_EN_SSH                  0x188
# define AC_EN_GO2                  0x1ae
# define AC_EN_COW                  0x1c6
# define AC_EN_GS                   0x1b9
#endif

#if defined(GAME_MM)
# define AC_EN_GIRLA        0x002
# define AC_EN_DOOR         0x005
# define AC_EN_BOX          0x006
# define AC_ITEM00          0x00e
# define AC_DOOR_SHUTTER    0x01e
# define AC_ITEM_GIVER      0x01f
# define AC_CUSTOM_WARP     0x023
# define AC_CUSTOM_TRIGGERS 0x025
# define AC_DOOR_WARP1      0x038
# define AC_ITEM_B_HEART    0x03a
# define AC_EN_COW          0x0f3
# define AC_DM_HINA         0x140
# define AC_EN_TAB          0x263
#endif

#if !defined(__ASSEMBLER__)
typedef struct GameState_Play GameState_Play;
typedef struct Actor Actor;

typedef void (*ActorCallback)(Actor*, GameState_Play*);

typedef struct PACKED ALIGNED(0x4) Actor
{
    u16         id;
    u8          type;
    u8          room;
    s32         flags;
    Vec3f       initPos;
    Vec3s       initRot;
    char        unk2[0x02];
    u16         variable;
    s8          objTableIndex;
    char        unk3[5];
    Vec3f       position;
    Vec3s       speedRot;
    u16         unk_36;
#if defined(GAME_MM)
    u32         unk_mm0;
#endif
    Vec3f       pos3;
    Vec3s       rot1;
#if defined(GAME_MM)
    u32         unk_mm1;
#endif
    char        unk_4a[0x06];
    Vec3f       scale;
    Vec3f       velocity;
    float       speedXZ;
    float       gravity;
    float       minVelocityY;
    char        unk_74[0x1c];
    float       xzDistanceFromLink;
    char        unk_94[0x20];
    Vec3s       rot2;
    char        unk_ba[2];
    float       modelOffsetY;
    char        unk_c0[0x4e];
    u16         messageId;
    u16         freezeTimer;
    char        unk_11A[0x6];
    Actor*      attachedA;
    Actor*      attachedB;
    Actor*      prev;
    Actor*      next;
    void*       init;
    void*       fini;
    void*       update;
    void*       draw;
    void*       ovl;
}
Actor;

typedef struct
{
    u32     count;
    Actor*  first;
}
ActorList;

#if defined(GAME_OOT)
_Static_assert(sizeof(Actor) == 0x13c, "OoT Actor size is wrong");
#endif

#if defined(GAME_MM)
_Static_assert(sizeof(Actor) == 0x144, "MM Actor size is wrong");
#endif
#endif

#endif
