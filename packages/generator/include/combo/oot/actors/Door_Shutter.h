#ifndef COMBO_OOT_DOOR_SHUTTER_H
#define COMBO_OOT_DOOR_SHUTTER_H

#include <combo/actor.h>

#define DOORSHUTTER_GET_SWITCH_FLAG(actor) PARAMS_GET_U((actor)->params, 0, 6)

struct DoorShutter;

typedef void (*DoorShutterActionFunc)(struct DoorShutter *, PlayState *);

typedef struct DoorShutter
{
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ s16 unk_164;
    /* 0x0166 */ s16 jabuDoorClosedAmount;
    /* 0x0168 */ s16 bossDoorTexIndex;
    /* 0x016A */ u8 doorType;
    /* 0x016B */ u8 styleType;
    /* 0x016C */ u8 gfxType;
    /* 0x016D */ s8 requiredObjBankIndex;
    /* 0x016E */ s8 unlockTimer;
    /* 0x016F */ s8 actionTimer;
    /* 0x0170 */ f32 barsClosedAmount;
    /* 0x0174 */ DoorShutterActionFunc actionFunc;
} DoorShutter; // size = 0x0178

#endif // COMBO_OOT_DOOR_SHUTTER_H
