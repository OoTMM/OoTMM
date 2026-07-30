#ifndef COMBO_OOT_OBJ_SWITCH_H
#define COMBO_OOT_OBJ_SWITCH_H

#include <combo/oot/collision_check.h>
#include <combo/actor.h>
#include <combo/common/color.h>

struct ObjSwitch;

typedef void (*ObjSwitchActionFunc)(struct ObjSwitch *, struct PlayState *);

typedef struct ObjSwitchJntSph
{
    /* 0x00 */ ColliderJntSph collider;
    /* 0x20 */ ColliderJntSphElement colliderElements[1];
} ObjSwitchJntSph;

typedef struct ObjSwitchTris
{
    /* 0x00 */ ColliderTris collider;
    /* 0x20 */ ColliderTrisElement colliderElements[2];
} ObjSwitchTris;

typedef struct ObjSwitch
{
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ ObjSwitchActionFunc actionFunc;
    /* 0x0168 */ s16 releaseTimer; // used for SUBTYPE_HOLD and SUBTYPE_HOLD_INVERTED
    /* 0x016A */ s16 disableAcTimer;
    /* 0x016C */ s16 cooldownTimer;
    /* 0x016E */ u8 cooldownOn;
    /* 0x0170 */ s16 eyeTexIndex;
    /* 0x0174 */ void *crystalSubtype1texture;
    /* 0x0178 */ u8 x1TexScroll;
    /* 0x0179 */ u8 y1TexScroll;
    /* 0x017A */ u8 x2TexScroll;
    /* 0x017B */ u8 y2TexScroll;
    /* 0x017C */ Color_RGB8 crystalColor;
    /* 0x017F */ u8 prevColFlags; // Either dynapoly interact flags or AC flags, of the previous frame
    union
    {
        /* 0x0180 */ ObjSwitchJntSph jntSph;
        /* 0x0180 */ ObjSwitchTris tris;
    };
} ObjSwitch; // size = 0x0258

#endif // COMBO_OOT_OBJ_SWITCH_H
