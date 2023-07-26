#ifndef COMBO_COMMON_ACTOR_H
#define COMBO_COMMON_ACTOR_H

#if !defined(__ASSEMBLER__)
#include <combo/types.h>
#endif

#if defined(GAME_OOT)
# define AC_EN_TEST                 0x002
# define AC_ITEM_GIVER              0x003
# define AC_CUSTOM_WARP             0x005
# define AC_CUSTOM_TRIGGERS         0x006
# define AC_EN_DOOR                 0x009
# define AC_EN_BOX                  0x00a
# define AC_EN_OKUTA                0x00e
# define AC_EN_WALLMAS              0x011
# define AC_EN_DODONGO              0x012
# define AC_EN_FIREFLY              0x013
# define AC_ITEM00                  0x015
# define AC_EN_TITE                 0x01b
# define AC_EN_PEEHAT               0x01d
# define AC_EN_ZF                   0x025
# define AC_BOSS_DODONGO            0x027
# define AC_BOSS_GOMA               0x028
# define AC_EN_GOMA                 0x02b
# define AC_EN_BUBBLE               0x02d
# define AC_DOOR_SHUTTER            0x02e
# define AC_EN_DODOJR               0x02f
# define AC_EN_BILI                 0x034
# define AC_EN_TP                   0x035
# define AC_EN_ST                   0x037
# define AC_EN_BW                   0x038
# define AC_EN_EIYER                0x03a
# define AC_EN_XC                   0x048
# define AC_EN_MB                   0x04b
# define AC_BOSS_GANONDROF          0x052
# define AC_EN_AM                   0x054
# define AC_EN_DEKUBABA             0x055
# define AC_DOOR_WARP1              0x05d
# define AC_ITEM_B_HEART            0x05f
# define AC_EN_DEKUNUTS             0x060
# define AC_EN_VALI                 0x063
# define AC_EN_BB                   0x069
# define AC_BG_TOKI_SWD             0x06c
# define AC_EN_VM                   0x08a
# define AC_EN_FLOORMAS             0x08e
# define AC_EN_RD                   0x090
# define AC_EN_SW                   0x095
# define AC_BOSS_FD                 0x096
# define AC_EN_DU                   0x098
# define AC_EN_FD                   0x099
# define AC_EN_DH                   0x0a4
# define AC_EN_DHA                  0x0a5
# define AC_BOSS_VA                 0x0ba
# define AC_EN_SYATEKI_MAN          0x0c1
# define AC_BOSS_MO                 0x0c4
# define AC_EN_SB                   0x0c5
# define AC_EN_BIGOKUTA             0x0c6
# define AC_EN_KAREBABA             0x0c7
# define AC_BG_BDAN_OBJECTS         0x0c8
# define AC_BOSS_TW                 0x0dc
# define AC_EN_RR                   0x0dd
# define AC_EN_ANUBICE              0x0e0
# define AC_EN_MA1                  0x0e7
# define AC_BOSS_SST                0x0e9
# define AC_EN_NY                   0x0ec
# define AC_ITEM_OCARINA            0x0f1
# define AC_EN_ANUBICE_TAG          0x0f6
# define AC_ITEM_ETCETERA           0x10f
# define AC_EN_IK                   0x113
# define AC_EN_SKJ                  0x115
# define AC_EN_G_SWITCH             0x117
# define AC_EN_TUBO_TRAP            0x11d
# define AC_BG_SPOT03_TAKI          0x11f
# define AC_EN_FZ                   0x121
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
# define AC_EN_WEIYER               0x18c
# define AC_EN_HINTNUTS             0x192
# define AC_EN_GO2                  0x1ae
# define AC_EN_WF                   0x1af
# define AC_EN_GS                   0x1b9
# define AC_EN_CROW                 0x1c0
# define AC_EN_COW                  0x1c6

#endif

#if defined(GAME_MM)
# define AC_EN_GIRLA        0x002
# define AC_EN_DOOR         0x005
# define AC_EN_BOX          0x006
# define AC_EN_OKUTA        0x008
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

# define AC_EN_BIGOKUTA     0x1a8
# define AC_EN_WALLMAS      0x00a
# define AC_EN_DODONGO      0x00b
# define AC_EN_FIREFLY      0x00c
# define AC_EN_TITE         0x012
# define AC_EN_PEEHAT       0x014
# define AC_EN_DINOFOS      0x019
# define AC_EN_ST           0x024
# define AC_EN_AM           0x032
# define AC_EN_FAMOS        0x02d
# define AC_EN_DEKUBABA     0x033
# define AC_EN_KAREBABA     0x066
# define AC_EN_DEKUNUTS     0x03b
# define AC_EN_BBFALL       0x03c
# define AC_EN_BB           0x03e
# define AC_EN_VM           0x047
# define AC_EN_RD           0x04c
# define AC_EN_TALK_GIBUD   0x1da
# define AC_EN_RAILGIBUD    0x235
# define AC_EN_SW           0x050
# define AC_EN_SB           0x064
# define AC_EN_RR           0x06c
# define AC_EN_IK           0x084
# define AC_EN_FZ           0x08f
# define AC_EN_WF           0x0ec
# define AC_EN_CROW         0x0f1
# define AC_EN_TUBO_TRAP    0x08d
# define AC_EN_FLOORMAS     0x04a
# define AC_EN_SLIME        0x14a
# define AC_EN_DRAGON       0x206
# define AC_EN_PR           0x14b
# define AC_EN_PR2          0x180
# define AC_EN_PRZ          0x181
# define AC_EN_WDHAND       0x1d1
# define AC_EN_GRASSHOPPER  0x109
# define AC_EN_SNOWMAN      0x1e6
# define AC_EN_EGOL         0x184
# define AC_EN_PP           0x1e9
# define AC_EN_RAT          0x16f
# define AC_EN_THIEFBIRD    0x291
# define AC_EN_MKK          0x164
# define AC_EN_BAGUO        0x155
# define AC_BOSS_05         0x12d
# define AC_EN_JSO          0x113
# define AC_EN_JSO2         0x182
# define AC_EN_WIZ          0x15d
# define AC_EN_WIZ_BROCK    0x15e
# define AC_EN_DEATH        0x043
# define AC_EN_PAMETFROG    0x007
# define AC_EN_BIGSLIME     0x065
# define AC_EN_BAT          0x15b
# define AC_EN_BIGPAMET     0x144
# define AC_BOSS_04         0x12c
# define AC_EN_BSB          0x28f
# define AC_EN_KNIGHT       0x115
# define AC_BOSS_06         0x12e
# define AC_BOSS_01         0x129
# define AC_BOSS_HAKUGIN    0x1dd
# define AC_BOSS_03         0x12b
# define AC_BOSS_02         0x12a
# define AC_EN_KAME         0x1ba

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

Actor* comboSpawnActor(void* unk, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable);
Actor* comboSpawnActorEx(void* unk, GameState_Play *play, short actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int ex1, int ex2, int ex3);

#endif

#endif
