#ifndef OVL_EN_GOROIWA
#define OVL_EN_GOROIWA

#include <combo/actor.h>

struct Actor_EnGoroiwa;

typedef void (*Actor_EnGoroiwaActionFunc)(struct Actor_EnGoroiwa*, GameState_Play*);
typedef s32 (*Actor_EnGoroiwaUnkFunc)(struct Actor_EnGoroiwa*);

#define ENGOROIWA_GET_C000(thisx) (((thisx)->params >> 0xE) & 3)
#define ENGOROIWA_GET_3000(thisx) (((thisx)->params >> 0xC) & 3)
#define ENGOROIWA_GET_400(thisx) (((thisx)->params >> 0xA) & 1)
#define ENGOROIWA_GET_300(thisx) (((thisx)->params >> 0x8) & 3)
#define ENGOROIWA_GET_PATH_INDEX(thisx) ((thisx)->params & 0xFF)

#define ENGOROIWA_PATH_INDEX_NONE 0xFF

typedef enum
{
    ENGOROIWA_300_0,
    ENGOROIWA_300_1,
    ENGOROIWA_300_3 = 3
}
Actor_EnGoroiwaParam300;

typedef enum
{
    ENGOROIWA_400_0,
    ENGOROIWA_400_1
}
Actor_EnGoroiwaParam400;

typedef enum
{
    ENGOROIWA_3000_0,
    ENGOROIWA_3000_1,
    ENGOROIWA_3000_2
}
Actor_EnGoroiwaParam3000;

typedef enum
{
    ENGOROIWA_C000_1 = 1,
    ENGOROIWA_C000_2
}
 Actor_EnGoroiwaParamC000;

typedef struct
 {
    Vec3f unk_00;
    f32 unk_0C;
    f32 unk_10;
    f32 unk_14;
    f32 unk_18;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20;
    s16 unk_22;
    s16 unk_24;
    s16 unk_26;
    CollisionPoly* unk_28;
    u8 unk_2C;
    u8 unk_2D;
}
 Actor_EnGoroiwaStruct; // size = 0x30

typedef struct Actor_EnGoroiwa
{
    Actor actor;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[1];
    Actor_EnGoroiwaActionFunc actionFunc;
    Vec3f unk_1A8;
    Vec3f unk_1B4;
    f32 unk_1C0;
    f32 unk_1C4;
    s16 unk_1C8;
    s16 unk_1CA;
    s16 unk_1CC;
    s16 unk_1CE;
    Vec3s* pathPoints;
    s16 unk_1D4;
    s16 unk_1D6;
    s16 unk_1D8;
    s16 unk_1DA;
    f32 unk_1DC;
    f32 unk_1E0;
    s8 unk_1E4;
    u8 unk_1E5;
    s8 unk_1E6;
    s8 unk_1E7;
    Actor_EnGoroiwaStruct unk_1E8[2];
    s32 unk_248;
}
Actor_EnGoroiwa; // size = 0x24C

#endif
