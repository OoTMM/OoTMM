#ifndef COMBO_OOT_EN_ELF_H
#define COMBO_OOT_EN_ELF_H

#include <combo/common/actor.h>
#include <combo/item.h>

typedef struct Actor_EnElf Actor_EnElf;

typedef void (*EnElfActionFunc)(Actor_EnElf*, GameState_Play*);
typedef void (*EnElfUnkFunc)(Actor_EnElf*, GameState_Play*);

struct Actor_EnElf
{
    /* 0x0000 */ Actor base;
    /* 0x013C */ u8 skelAnime[0x44]; // SkelAnime
    /* 0x0180 */ Vec3s jointTable[15];
    /* 0x01DA */ Vec3s morphTable[15];
    /* 0x0234 */ Color_RGBAf innerColor;
    /* 0x0244 */ Color_RGBAf outerColor;
    /* 0x0254 */ u8 lightInfoGlow[0x10]; // LightInfo
    /* 0x0264 */ void* lightNodeGlow; // LightNode
    /* 0x0268 */ u8 lightInfoNoGlow[0x10]; // LightInfo
    /* 0x0278 */ void* lightNodeNoGlow; // LightNode
    /* 0x027C */ Vec3f unk_28C;
    /* 0x0288 */ void* elfMsg; // ElfMsg
    /* 0x028C */ f32 unk_29C;
    /* 0x0290 */ f32 unk_2A0;
    /* 0x0294 */ f32 unk_2A4;
    /* 0x0298 */ s16 unk_2A8;
    /* 0x029A */ s16 unk_2AA;
    /* 0x029C */ s16 unk_2AC;
    /* 0x029E */ s16 unk_2AE;
    /* 0x02A0 */ s16 unk_2B0;
    /* 0x02A4 */ f32 unk_2B4;
    /* 0x02A8 */ f32 unk_2B8;
    /* 0x02AC */ s16 unk_2BC;
    /* 0x02AE */ u16 timer;
    /* 0x02B0 */ s16 unk_2C0;
    /* 0x02B2 */ s16 disappearTimer;
    /* 0x02B4 */ u16 fairyFlags;
    /* 0x02B6 */ u8 unk_2C6;
    /* 0x02B7 */ u8 unk_2C7;
    /* 0x02B8 */ EnElfUnkFunc func_2C8;
    /* 0x02BC */ EnElfActionFunc actionFunc;

    /* Extended flags */
    /* 0x02C0 */ Xflag xflag;
    /* 0x02C6 */ s16 itemGiven;
    /* 0x02C8 */ s16 extendedGi;
    /* 0x02CA */ s16 extendedGiDraw;
};

void EnElf_ItemQuery(ComboItemQuery* q, Actor_EnElf* this);
void EnElf_Draw(Actor_EnElf* this, GameState_Play* play);

_Static_assert(sizeof(Actor_EnElf) == 0x02CC, "OoT Actor_EnElf size is wrong");

#endif
