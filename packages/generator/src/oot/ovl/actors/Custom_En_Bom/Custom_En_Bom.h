#ifndef OVL_OOT_CUSTOM_EN_BOM_H
#define OVL_OOT_CUSTOM_EN_BOM_H

#include <combo.h>

struct EnBomMM;

typedef void (*EnBomMMActionFunc)(struct EnBomMM*, PlayState*);

#define ENBOMMM_GET_3(thisx) ((thisx)->shape.rot.x & 3)
#define ENBOMMM_GET_80(thisx) ((thisx)->shape.rot.z & 0x80)
#define ENBOMMM_GET_FF00(thisx) (((thisx)->shape.rot.z & 0xFF00) >> 8)

typedef enum BombType {
    /* 0 */ BOMB_TYPE_BODY,
    /* 1 */ BOMB_TYPE_EXPLOSION
} BombType;

// Passed via x rotation
typedef enum BombExplosiveTypeMM {
    /* 0 */ BOMB_EXPLOSIVE_TYPE_BOMB,
    /* 1 */ BOMB_EXPLOSIVE_TYPE_POWDER_KEG,
    /* 2 */ BOMB_EXPLOSIVE_TYPE_BLAST
} BombExplosiveType;

typedef struct EnBomMM {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider1;
    /* 0x190 */ ColliderJntSph collider2;
    /* 0x1B0 */ ColliderJntSphElement collider2Elements[1];
    /* 0x1F0 */ s16 timer;
    /* 0x1F2 */ s16 flashSpeedScale;
    /* 0x1F4 */ f32 unk_1F4;
    /* 0x1F8 */ u8 unk_1F8;
    /* 0x1F9 */ u8 isPowderKeg;
    /* 0x1FA */ s16 unk_1FA;
    /* 0x1FC */ u8 unk_1FC;
    /* 0x200 */ EnBomMMActionFunc actionFunc;
} EnBomMM; // size = 0x204

#endif
