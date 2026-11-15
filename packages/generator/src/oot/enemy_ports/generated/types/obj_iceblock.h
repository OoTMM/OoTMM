#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_OBJ_ICEBLOCK_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_OBJ_ICEBLOCK_H

#include <combo.h>

struct ObjIceblock;

typedef void (*ObjIceblockActionFunc)(struct ObjIceblock*, PlayState*);
typedef void (*ObjIceblockExtendedDrawFunc)(struct ObjIceblock*, PlayState*);

#define ICEBLOCK_GET_SNAP_ROT(thisx) (((thisx)->params >> 1) & 1)
#define ICEBLOCK_GET_ICEBERG(thisx) ((thisx)->params & 1)

typedef struct {
    s16 unk_00;
    s16 unk_02;
    f32 unk_04;
    f32 unk_08;
    f32 unk_0C;
} ObjIceBlockUnkStruct;

typedef struct {
    CollisionPoly* unk_00;
    f32 unk_04;
    s32 unk_08;
    f32 unk_0C;
} ObjIceBlockUnkStruct2;

typedef struct {
    f32 unk_00;
    f32 unk_04;
} ObjIceBlockUnkStruct3;

typedef struct {
    f32 unk_00;
    f32 unk_04;
    f32 unk_08;
    s16 unk_0C;
    s16 unk_0E;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    s16 unk_16;
    s16 unk_18;
    f32 unk_1C;
    s16 unk_20;
    s16 unk_22;
} ObjIceBlockUnkStruct4;

typedef struct ObjIceblock {
    DynaPolyActor dyna;
    ColliderCylinder collider;
    ObjIceblockActionFunc actionFunc;
    ObjIceblockExtendedDrawFunc extendedDrawFunc;
    s32 unk_1B0;
    ObjIceBlockUnkStruct unk_1B4[4];
    ObjIceBlockUnkStruct2 unk_1F4[5];
    f32 unk_244;
    Vec3f unk_248;
    Vec3s unk_254;
    f32 unk_25C;
    f32 unk_260;
    f32* unk_264;
    f32 unk_268;
    s16 unk_26C;
    s16 unk_26E[4];
    s16 unk_276;
    s16 unk_278;
    ObjIceBlockUnkStruct4 unk_27C;
    s16 stateTimer;
    s16 unk_2A2;
    f32 unk_2A4;
    f32 unk_2A8;
    s16 unk_2AC;
    s16 meltTimer;
    s8 unk_2B0;
    s8 spawnCutsceneTimer;
    f32 unk_2B4;
} ObjIceblock;

#endif
