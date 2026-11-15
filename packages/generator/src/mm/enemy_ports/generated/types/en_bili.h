#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_BILI_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_BILI_H

#include <combo.h>

#ifndef BIRI_LIMB_CORE
#define BIRI_LIMB_CORE 2
#endif
#ifndef BIRI_LIMB_HOOD
#define BIRI_LIMB_HOOD 3
#endif
#ifndef BIRI_LIMB_MAX
#define BIRI_LIMB_MAX 5
#endif
#ifndef BIRI_LIMB_NONE
#define BIRI_LIMB_NONE 0
#endif
#ifndef BIRI_LIMB_ROOT
#define BIRI_LIMB_ROOT 1
#endif
#ifndef BIRI_LIMB_TENTACLES
#define BIRI_LIMB_TENTACLES 4
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_BiriLimb
#define COMBO_ENEMY_ASSET_ENUM_BiriLimb
typedef s32 BiriLimb;
#endif
typedef enum EnBiliType {
    EN_BILI_TYPE_NORMAL = -1,
    EN_BILI_TYPE_SPAWNED_BY_BARI,
    EN_BILI_TYPE_DIE_AFTER_ELECTRIFY
} EnBiliType;

struct EnBili;

typedef void (*EnBiliActionFunc)(struct EnBili*, struct PlayState*);

typedef struct EnBili {
    Actor actor;
    SkelAnime skelAnime;
    EnBiliActionFunc actionFunc;
    u8 tentaclesTexIndex;
    u8 playFlySfx;
    s16 timer;
    Vec3s jointTable[BIRI_LIMB_MAX];
    Vec3s morphTable[BIRI_LIMB_MAX];
    ColliderCylinder collider;
} EnBili;

#endif
