#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_TANRON1_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_TANRON1_H

#include <combo.h>

struct EnTanron1;

#define ENTANRON1_GET_100(thisx) ((thisx)->params & 0x100)

typedef struct {
    Vec3f unk_00;
    Vec3f unk_0C;
    s16 unk_18;
    s16 unk_1A;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20;
    s8 unk_22[0x2];
    u8 unk_24;
    s16 unk_26;
    s16 unk_28;
    s16 unk_2A;
    f32 unk_2C;
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    f32 unk_3C;
} EnTanron1Struct;

typedef struct EnTanron1 {
    Actor actor;
    u8 unk_144;
    s8 pad145[3];
    s16 unk_148;
    Vec3f unk_14C;
    s16 unk_158;
    f32 unk_15C;
    EnTanron1Struct* unk_160;
    Vec3f unk_3360;
    u16 mothCapacity;
    u16 mothDeathTimer;
    u8 standalone;
    ColliderCylinder standaloneCollider;
} EnTanron1;

#endif
