#ifndef OVL_EN_MAG_H
#define OVL_EN_MAG_H

#include <combo/oot/play.h>

struct Actor_EnMag;

typedef struct Actor_EnMag
{
    Actor   actor;
    char    unk_14C[0x0004];
    Font    font;
    s16     effectFadeInState;
    s16     effectFadeInTimer;
    s16     globalState;
    f32     effectPrimLodFrac;
    f32     effectPrimColor[3];
    f32     effectEnvColor[3];
    f32     effectAlpha;
    f32     mainAlpha;
    f32     subAlpha;
    f32     copyrightAlpha;
    s16     unk_E30C;
    s16     effectScroll;
    char    unk_E310[0x0002];
    s16     copyrightAlphaStep;
    s16     fadeOutAlphaStep;
    s16     unk_E316;
    s16     unk_E318;
    s32     unk_E31C;
    s32     unk_E320;
    char    unk_E324[0x0004];

    /* Custom */
    void*   mmMagData;
    s16     mmEffectScrollSs[6];
    s16     mmEffectScrollTs[6];
    u8      mmEffectPrimColor[3];
    u8      mmEffectEnvColor[3];
}
Actor_EnMag;

typedef enum
{
    MAG_STATE_INITIAL,
    MAG_STATE_FADE_IN,
    MAG_STATE_DISPLAY,
    MAG_STATE_FADE_OUT,
    MAG_STATE_POST_DISPLAY
}
EnMag_GlobalState;

#endif
