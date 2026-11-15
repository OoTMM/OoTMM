#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_PR_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_PR_H

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
struct EnPr;

typedef void (*EnPrActionFunc)(struct EnPr*, PlayState*);

#define ENPR_GET_FF00(thisx) (((thisx)->params >> 8) & 0xFF)

#define ENPR_FF00_MAX 20
#define ENPR_FF00_MIN 0

typedef enum EnPrBodyPart {
    ENPR_BODYPART_0,
    ENPR_BODYPART_1,
    ENPR_BODYPART_2,
    ENPR_BODYPART_3,
    ENPR_BODYPART_4,
    ENPR_BODYPART_5,
    ENPR_BODYPART_6,
    ENPR_BODYPART_7,
    ENPR_BODYPART_8,
    ENPR_BODYPART_9,
    ENPR_BODYPART_MAX
} EnPrBodyPart;

typedef struct EnPr {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[OBJECT_PR_1_LIMB_MAX];
    Vec3s morphTable[OBJECT_PR_1_LIMB_MAX];
    EnPrActionFunc actionFunc;
    u8 unk_204;
    s16 unk_206;
    s16 unk_208;
    s16 unk_20A;
    s16 unk_20C;
    s16 unk_20E;
    s16 unk_210;
    s16 unk_212;
    s16 unk_214;
    s16 unk_216;
    s8 unk218[4];
    s32 animIndex;
    s8 unk220[0x8];
    s32 bodyPartsCount;
    s16 unk_22C;
    s16 drawDmgEffAlpha;
    s16 drawDmgEffType;
    f32 unk_234;
    f32 unk_238;
    Vec3f bodyPartsPos[ENPR_BODYPART_MAX];
    f32 unk_2B4;
    f32 unk_2B8;
    f32 animEndFrame;
    f32 unk_2C0;
    f32 unk_2C4;
    f32 unk_2C8;
    f32 unk_2CC;
    s16 unk_2D0;
    s16 unk_2D2;
    Vec3f unk_2D4;
    Vec3f unk_2E0;
    s8 unk2EC[0xC];
    Actor* unk_2F8;
    ColliderCylinder collider;
} EnPr;

#endif
