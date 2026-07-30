#ifndef COMBO_OOT_BG_HIDAN_KOUSI_H
#define COMBO_OOT_BG_HIDAN_KOUSI_H

#include <combo/actor.h>

struct BgHidanKousi;

typedef void (*BgHidanKousiActionFunc)(struct BgHidanKousi *, struct PlayState *);

typedef struct BgHidanKousi
{
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ BgHidanKousiActionFunc actionFunc;
    /* 0x0168 */ s16 unk_168;
} BgHidanKousi; // size = 0x016C

#endif // COMBO_OOT_BG_HIDAN_KOUSI_H
