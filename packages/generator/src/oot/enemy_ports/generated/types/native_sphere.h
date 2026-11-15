#ifndef COMBO_ENEMY_NATIVE_TYPES_SPHERE_H
#define COMBO_ENEMY_NATIVE_TYPES_SPHERE_H

#include <combo.h>

#if defined(GAME_OOT)
typedef struct
{
    Collider base;
    ColliderElement elem;
    ColliderJntSphElementDim dim;
} ColliderSphere;

typedef struct
{
    ColliderInit base;
    ColliderElementInit elem;
    ColliderJntSphElementDimInit dim;
} ColliderSphereInit;

_Static_assert(sizeof(ColliderSphere) == 0x58, "Retained sphere collision storage");
_Static_assert(sizeof(ColliderSphereInit) == 0x2c, "Retained sphere init storage");
_Static_assert(__builtin_offsetof(ColliderSphere, elem) == sizeof(Collider), "Native sphere element placement");
_Static_assert(__builtin_offsetof(ColliderSphere, dim) == sizeof(Collider) + sizeof(ColliderElement), "Native sphere geometry placement");
#define COLSHAPE_SPHERE 4
s32 comboEnemyInitAndSetSphere(PlayState* play, ColliderSphere* sphere, Actor* actor, ColliderSphereInit* init);
s32 comboEnemyDestroySphere(PlayState* play, ColliderSphere* sphere);
#define Collider_InitAndSetSphere comboEnemyInitAndSetSphere
#define Collider_DestroySphere comboEnemyDestroySphere
#endif

#endif
