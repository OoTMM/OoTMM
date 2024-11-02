#ifndef OVL_BOSS_04_H
#define OVL_BOSS_04_H

#include <combo.h>
#include <assets/mm/objects/object_boss04.h>

struct Actor_Boss04;

typedef void (*Actor_Boss04ActionFunc)(struct Actor_Boss04*, PlayState*);

typedef enum WartLimb {
    WART_LIMB_NONE,
    WART_LIMB_ROOT,
    WART_LIMB_BODY,
    WART_LIMB_EYE_ROOT,
    WART_LIMB_EYE,
    WART_LIMB_TOP_EYELID_ROOT,
    WART_LIMB_TOP_EYELID,
    WART_LIMB_BOTTOM_EYELID_ROOT,
    WART_LIMB_BOTTOM_EYELID,
    WART_LIMB_MAX
} WartLimb;

typedef struct Actor_Boss04 {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[WART_LIMB_MAX];
    Vec3s morphTable[WART_LIMB_MAX];
    s16 unk_1F4;
    u8 unk_1F6;
    u8 unk_1F7;
    s16 unk_1F8;
    s16 unk_1FA;
    char unk_1FC[2];
    s16 unk_1FE;
    s16 unk_200;
    Actor_Boss04ActionFunc actionFunc;
    ColliderJntSph collider1;
    ColliderJntSphElement collider1Elements[1];
    ColliderJntSph collider2;
    ColliderJntSphElement collider2Elements[1];
    s16 unk_2C8;
    f32 unk_2CC;
    f32 unk_2D0;
    s16 unk_2D4;
    s16 unk_2D6;
    s16 unk_2D8;
    s16 unk_2DA;
    s16 unk_2DC;
    s16 unk_2DE;
    s16 unk_2E0;
    s16 unk_2E2;
    Vec3f unk_2E4[16];
    Vec3f unk_3A4[15];
    Vec3f unk_458[15];
    Vec3f unk_50C[11];
    Vec3f unk_590[11];
    Vec3f unk_614[6];
    Vec3f unk_65C[6];
    Vec3f unk_6A4;
    Vec3f unk_6B0;
    Vec3f unk_6BC;
    Vec3f unk_6C8;
    f32 unk_6D4;
    f32 unk_6D8;
    f32 unk_6DC;
    f32 unk_6E0;
    f32 unk_6E4;
    f32 unk_6E8;
    char unk_6EC[4];
    f32 unk_6F0;
    s16 unk_6F4;
    s16 unk_6F6;
    f32 unk_6F8;
    f32 unk_6FC;
    f32 unk_700;
    u32 unk_704;
    s16 unk_708;
    s16 subCamId;
    Vec3f subCamEye;
    Vec3f subCamAt;
    char unk_724[4];
    f32 unk_728;
    char unk_72C[0x10];
    f32 unk_73C;
    char unk_740[4];
    f32 subCamFov;
    s16 subCamAtOscillator;
    u8 unk_74A;
}
Actor_Boss04;

#endif
