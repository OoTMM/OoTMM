#ifndef COMBO_MM_EN_ELF_H
#define COMBO_MM_EN_ELF_H

#include <combo/common/actor.h>

typedef struct Actor_EnElf Actor_EnElf;

typedef void (*EnElfActionFunc)(Actor_EnElf*, GameState_Play*);
typedef void (*EnElfUnkFunc)(Actor_EnElf*, GameState_Play*);

struct Actor_EnElf
{
    /* 0x000 */ Actor base;
    /* 0x144 */ u8 skelAnime[0x44]; // SkelAnime
    /* 0x188 */ Vec3s jointTable[7];
    /* 0x1B2 */ Vec3s morphTable[7];
    /* 0x1DC */ Color_RGBAf innerColor;
    /* 0x1EC */ Color_RGBAf outerColor;
    /* 0x1FC */ u8 lightInfoGlow[0x10]; // LightInfo
    /* 0x20C */ void* lightNodeGlow; // z_Light
    /* 0x210 */ u8 lightInfoNoGlow[0x10]; // LightInfo
    /* 0x220 */ void* lightNodeNoGlow; // z_Light
    /* 0x224 */ Vec3f unk_224;
    /* 0x230 */ Actor* elfMsg;
    /* 0x234 */ Actor* unk_234;
    /* 0x238 */ f32 unk_238;
    /* 0x23C */ f32 unk_23C;
    /* 0x240 */ f32 unk_240;
    /* 0x244 */ s16 unk_244;
    /* 0x246 */ s16 unk_246;
    /* 0x248 */ s16 unk_248;
    /* 0x24A */ s16 unk_24A;
    /* 0x24C */ s16 unk_24C;
    /* 0x250 */ f32 unk_250;
    /* 0x254 */ f32 unk_254;
    /* 0x258 */ s16 unk_258;
    /* 0x25A */ u16 timer;
    /* 0x25C */ s16 unk_25C;
    /* 0x25E */ s16 disappearTimer;
    /* 0x260 */ s16 collectableFlag;
    /* 0x262 */ u16 fairyFlags;
    /* 0x264 */ u16 unk_264;
    /* 0x266 */ u16 unk_266;
    /* 0x268 */ u8 unk_268;
    /* 0x269 */ u8 unk_269;
    /* 0x26C */ EnElfUnkFunc unk_26C;
    /* 0x270 */ EnElfActionFunc actionFunc;

    /* Extended flags */
    /* 0x0274 */ Xflag xflag;
    /* 0x027A */ s16 itemGiven;
    /* 0x027C */ s16 extendedGi;
    /* 0x027E */ s16 extendedGiDraw;
};

_Static_assert(sizeof(Actor_EnElf) == 0x0280, "MM Actor_EnElf size is wrong");

#endif
