#ifndef Z_EN_KAREBABA_H
#define Z_EN_KAREBABA_H

#include <combo.h>

#define DEKUBABA_LIMB_MAX 8

struct EnKarebaba;

typedef void (*EnKarebabaActionFunc)(struct EnKarebaba*, PlayState*);

typedef enum {
    ENKAREBABA_0,
    ENKAREBABA_1,
    KAREBABA_MINI
} KarebabaType;

typedef enum KarebabaBodyPart {
    KAREBABA_BODYPART_0,
    KAREBABA_BODYPART_1,
    KAREBABA_BODYPART_2,
    KAREBABA_BODYPART_3,
    KAREBABA_BODYPART_MAX
} KarebabaBodyPart;

typedef struct EnKarebaba {
    Actor actor;
    SkelAnime skelAnime;
    EnKarebabaActionFunc actionFunc;
    Vec3s jointTable[DEKUBABA_LIMB_MAX];
    Vec3s morphTable[DEKUBABA_LIMB_MAX];
    u8 drawDmgEffType;
    s16 timer;
    f32 drawDmgEffAlpha;
    f32 drawDmgEffScale;
    f32 drawDmgEffFrozenSteamScale;
    Vec3f bodyPartsPos[KAREBABA_BODYPART_MAX];
    CollisionPoly* boundFloor;
    ColliderCylinder attackCollider;
    ColliderCylinder hurtCollider;
} EnKarebaba;

#endif
