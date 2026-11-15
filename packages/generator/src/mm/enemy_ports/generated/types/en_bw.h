#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_BW_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_BW_H

#include <combo.h>

#ifndef TORCH_SLUG_LIMB_BODY
#define TORCH_SLUG_LIMB_BODY 1
#endif
#ifndef TORCH_SLUG_LIMB_LEFT_EYESTALK_ROOT
#define TORCH_SLUG_LIMB_LEFT_EYESTALK_ROOT 3
#endif
#ifndef TORCH_SLUG_LIMB_LEFT_EYESTALK_ROOT_ROOT
#define TORCH_SLUG_LIMB_LEFT_EYESTALK_ROOT_ROOT 2
#endif
#ifndef TORCH_SLUG_LIMB_LEFT_LOWER_EYESTALK
#define TORCH_SLUG_LIMB_LEFT_LOWER_EYESTALK 6
#endif
#ifndef TORCH_SLUG_LIMB_LEFT_UPPER_EYESTALK
#define TORCH_SLUG_LIMB_LEFT_UPPER_EYESTALK 5
#endif
#ifndef TORCH_SLUG_LIMB_LEFT_UPPER_EYESTALK_ROOT
#define TORCH_SLUG_LIMB_LEFT_UPPER_EYESTALK_ROOT 4
#endif
#ifndef TORCH_SLUG_LIMB_MAX
#define TORCH_SLUG_LIMB_MAX 12
#endif
#ifndef TORCH_SLUG_LIMB_NONE
#define TORCH_SLUG_LIMB_NONE 0
#endif
#ifndef TORCH_SLUG_LIMB_RIGHT_EYESTALK_ROOT
#define TORCH_SLUG_LIMB_RIGHT_EYESTALK_ROOT 8
#endif
#ifndef TORCH_SLUG_LIMB_RIGHT_EYESTALK_ROOT_ROOT
#define TORCH_SLUG_LIMB_RIGHT_EYESTALK_ROOT_ROOT 7
#endif
#ifndef TORCH_SLUG_LIMB_RIGHT_LOWER_EYESTALK
#define TORCH_SLUG_LIMB_RIGHT_LOWER_EYESTALK 11
#endif
#ifndef TORCH_SLUG_LIMB_RIGHT_UPPER_EYESTALK
#define TORCH_SLUG_LIMB_RIGHT_UPPER_EYESTALK 10
#endif
#ifndef TORCH_SLUG_LIMB_RIGHT_UPPER_EYESTALK_ROOT
#define TORCH_SLUG_LIMB_RIGHT_UPPER_EYESTALK_ROOT 9
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_TorchSlugLimb
#define COMBO_ENEMY_ASSET_ENUM_TorchSlugLimb
typedef s32 TorchSlugLimb;
#endif
struct EnBw;
typedef void (*EnBwActionFunc)(struct EnBw*, struct PlayState*);

typedef struct EnBw {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[TORCH_SLUG_LIMB_MAX];
    Vec3s morphTable[TORCH_SLUG_LIMB_MAX];
    u8 unk_220;
    u8 unk_221;
    s16 unk_222;
    s16 unk_224;
    EnBwActionFunc actionFunc;
    Color_RGBA8 color1;
    u8 unk_230;
    u8 damageReaction;
    u8 unk_232;
    s16 unk_234;
    s16 unk_236;
    s16 unk_238;
    u8 unk_23A;
    u8 iceTimer;
    u8 unk_23C;
    f32 unk_240;
    f32 unk_244;
    f32 unk_248;
    f32 unk_24C;
    f32 unk_250;
    char unk_254[4];
    f32 unk_258;
    f32 unk_25C;
    f32 unk_260;
    Vec3f unk_264;
    Vec3f unk_270;
    Vec3f unk_27C;
    Vec3f unk_288;
    ColliderCylinder collider1;
    ColliderCylinder collider2;
} EnBw;

#endif
