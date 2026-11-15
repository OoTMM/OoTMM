#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_BA_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_BA_H

#include <combo.h>

struct EnBa;

typedef void (*EnBaActionFunc)(struct EnBa*, struct PlayState*);

typedef enum EnBaType {
    EN_BA_TENTACLE_RED,
    EN_BA_TENTACLE_GREEN,
    EN_BA_TENTACLE_GRAY,
    EN_BA_DEAD_BLOB
} EnBaType;

typedef struct EnBa {
    Actor actor;
    s32 unk_14C;
    EnBaActionFunc actionFunc;
    s16 upperParams;
    s16 unk_156;
    Vec3f unk_158[14];
    Vec3f unk_200[14];
    Vec3s unk_2A8[14];
    Vec3f unk_2FC;
    Vec3f unk_308;
    f32 unk_314;
    s16 unk_318;
    s16 unk_31A;
    s16 unk_31C;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[2];
} EnBa;

#endif
