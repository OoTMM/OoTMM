#ifndef OVL_EN_BIGPAMET_H
#define OVL_EN_BIGPAMET_H

#include <combo.h>

struct Actor_EnBigpamet;

typedef void (*Actor_EnBigpametActionFunc)(struct Actor_EnBigpamet*, PlayState*);

typedef enum SpikedSnapperLimb
{
    SPIKED_SNAPPER_LIMB_NONE,
    SPIKED_SNAPPER_LIMB_BODY,
    SPIKED_SNAPPER_LIMB_SHELL,
    SPIKED_SNAPPER_LIMB_SPIKES,
    SPIKED_SNAPPER_LIMB_MAX
}
SpikedSnapperLimb;

typedef enum SnapperLimb
{
    SNAPPER_LIMB_NONE,
    SNAPPER_LIMB_BODY,
    SNAPPER_LIMB_HEAD,
    SNAPPER_LIMB_JAW,
    SNAPPER_LIMB_EYES,
    SNAPPER_LIMB_FRONT_LEFT_LEG,
    SNAPPER_LIMB_FRONT_LEFT_FOOT,
    SNAPPER_LIMB_FRONT_RIGHT_LEG,
    SNAPPER_LIMB_FRONT_RIGHT_FOOT,
    SNAPPER_LIMB_BACK_LEFT_LEG,
    SNAPPER_LIMB_BACK_LEFT_FOOT,
    SNAPPER_LIMB_BACK_RIGHT_LEG,
    SNAPPER_LIMB_BACK_RIGHT_FOOT,
    SNAPPER_LIMB_MAX
} SnapperLimb;

typedef enum
{
    ENBIGPAMET_0,
    ENBIGPAMET_1
}
Actor_EnBigpametParam;

typedef struct
{
    Vec3f unk_00;
    Vec3f unk_0C;
    Vec3s unk_18;
    f32 unk_20;
}
Actor_EnBigpametStruct;

typedef struct Actor_EnBigpamet
{
    Actor actor;
    SkelAnime snapperSkelAnime;
    Vec3s snapperJointTable[SNAPPER_LIMB_MAX];
    Vec3s snapperMorphTable[SNAPPER_LIMB_MAX];
    SkelAnime spikedSnapperSkelAnime;
    Vec3s spikedSnapperJointTable[SPIKED_SNAPPER_LIMB_MAX];
    Vec3s spikedSnapperMorphTable[SPIKED_SNAPPER_LIMB_MAX];
    Actor_EnBigpametActionFunc actionFunc;
    u8 unk_29C;
    s16 unk_29E;
    u8 pad2A0[2];
    s16 unk_2A2;
    f32 unk_2A4;
    f32 unk_2A8;
    f32 unk_2AC;
    ColliderCylinder collider;
    Actor_EnBigpametStruct unk_2FC[20];
}
Actor_EnBigpamet;

#endif
