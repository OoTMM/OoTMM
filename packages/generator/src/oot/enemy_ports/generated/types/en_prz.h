#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_PRZ_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_PRZ_H

#include <combo.h>

#ifndef OBJECT_PR_1_LIMB_01
#define OBJECT_PR_1_LIMB_01 1
#endif
#ifndef OBJECT_PR_1_LIMB_02
#define OBJECT_PR_1_LIMB_02 2
#endif
#ifndef OBJECT_PR_1_LIMB_03
#define OBJECT_PR_1_LIMB_03 3
#endif
#ifndef OBJECT_PR_1_LIMB_04
#define OBJECT_PR_1_LIMB_04 4
#endif
#ifndef OBJECT_PR_1_LIMB_05
#define OBJECT_PR_1_LIMB_05 5
#endif
#ifndef OBJECT_PR_1_LIMB_06
#define OBJECT_PR_1_LIMB_06 6
#endif
#ifndef OBJECT_PR_1_LIMB_07
#define OBJECT_PR_1_LIMB_07 7
#endif
#ifndef OBJECT_PR_1_LIMB_08
#define OBJECT_PR_1_LIMB_08 8
#endif
#ifndef OBJECT_PR_1_LIMB_09
#define OBJECT_PR_1_LIMB_09 9
#endif
#ifndef OBJECT_PR_1_LIMB_MAX
#define OBJECT_PR_1_LIMB_MAX 10
#endif
#ifndef OBJECT_PR_1_LIMB_NONE
#define OBJECT_PR_1_LIMB_NONE 0
#endif
#ifndef PR_2_LIMB_BODY
#define PR_2_LIMB_BODY 1
#endif
#ifndef PR_2_LIMB_JAW
#define PR_2_LIMB_JAW 3
#endif
#ifndef PR_2_LIMB_MAX
#define PR_2_LIMB_MAX 5
#endif
#ifndef PR_2_LIMB_NONE
#define PR_2_LIMB_NONE 0
#endif
#ifndef PR_2_LIMB_SKULL
#define PR_2_LIMB_SKULL 2
#endif
#ifndef PR_2_LIMB_TAIL
#define PR_2_LIMB_TAIL 4
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_ObjectPr1Limb
#define COMBO_ENEMY_ASSET_ENUM_ObjectPr1Limb
typedef s32 ObjectPr1Limb;
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_Pr2Limb
#define COMBO_ENEMY_ASSET_ENUM_Pr2Limb
typedef s32 Pr2Limb;
#endif
struct EnPrz;

typedef void (*EnPrzActionFunc)(struct EnPrz*, PlayState*);

#define ENPRZ_GET(thisx) ((thisx)->params & 0xFF)

typedef struct EnPrz {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[PR_2_LIMB_MAX];
    Vec3s morphTable[PR_2_LIMB_MAX];
    EnPrzActionFunc actionFunc;
    u8 unk_1C8;
    Vec3f unk_1CC;
    Vec3f unk_1D8;
    s16 unk_1E4;
    s16 unk_1E6;
    s16 unk_1E8;
    s16 unk_1EA;
    s16 unk_1EC;
    s16 unk_1EE;
    s8 unk1F0[2];
    s16 unk_1F2;
    s8 unk1F4[8];
    s16 unk_1FC;
    s16 unk_1FE;
    s8 unk200[2];
    s16 unk_202;
    s32 animIndex;
    f32 unk_208;
    f32 unk_20C;
    f32 unk_210;
    f32 unk_214;
    f32 unk_218;
    s8 unk21C[4];
    Actor* unk_220;
    ColliderCylinder collider;
} EnPrz;

#endif
