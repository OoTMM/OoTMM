#include <combo.h>
#include <combo/custom.h>
#include <combo/collision.h>
#include <combo/math.h>

#define COS_OF_5_PI_DIV_8 -0.38268343f
#define OBJ_SWITCH_INIT_TRIS_COLLIDER_ADDR 0x8093AC6C
#define OBJ_SWITCH_IS_EYE_SWITCH_HIT_ADDR 0x8093BBD0
#define ARROW_TYPE_NORMAL_LIT 0
#define ARROW_TYPE_NORMAL_HORSE 1
#define ARROW_TYPE_NORMAL 2
#define ARROW_TYPE_FIRE 3
#define ARROW_TYPE_ICE 4
#define ARROW_TYPE_LIGHT 5
#define ARROW_TYPE_SLINGSHOT 6
#define ARROW_TYPE_DEKU_BUBBLE 7
#define ARROW_TYPE_DEKU_NUT 8


typedef struct ObjSwitch ObjSwitch;

typedef void (*ObjSwitchActionFunc)(ObjSwitch* this, PlayState* play);
typedef void (*ObjSwitchSetupActionFunc)(ObjSwitch* this);

struct ObjSwitch {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ObjSwitchActionFunc actionFunc;
    /* 0x160 */ s16 floorSwitchReleaseTimer;
    /* 0x162 */ s16 disableCrystalSwitchTimer;
    /* 0x164 */ s8 eyeTexIndex;
    /* 0x165 */ s8 sfxTimer;
    /* 0x166 */ u8 pad_166[0x2];
    /* 0x168 */ s32 crystalAnimTimer;
    /* 0x16C */ Color_RGB8 color;
    /* 0x16F */ u8 collisionFlags;
    /* 0x170 */ s8 floorSwitchPlayerSnapState;
    /* 0x171 */ s8 nextSwitchFlagState;
    /* 0x172 */ s8 isPlayingCutscene;
    /* 0x173 */ u8 pad_173;
    /* 0x174 */ ObjSwitchSetupActionFunc setupFunc;

    union {
        struct {
            /* 0x178 */ ColliderJntSph colliderJntSph;
            /* 0x198 */ ColliderJntSphElement colliderJntSphElements[1];
        };

        struct {
            /* 0x178 */ ColliderTris colliderTris;
            /* 0x198 */ ColliderTrisElement colliderTrisElements[2];
        };
    };

    /* 0x250 */ f32 floorSwitchUpScale;
    /* 0x254 */ f32 floorSwitchDownScale;
};

static void ObjSwitch_AddDekuBubbleToObjSwitchTrisCollider(ObjSwitch* this) {
    Actor* actor = &this->dyna.actor;

    if (actor->id != ACTOR_OBJ_SWITCH) {
        return;
    }
    this->colliderTrisElements[0].base.acDmgInfo.dmgFlags |= DMG_DEKU_BUBBLE;
    this->colliderTrisElements[1].base.acDmgInfo.dmgFlags |= DMG_DEKU_BUBBLE;
}

void ObjSwitch_InitTrisCollider_Custom(
    Actor* thisx,
    PlayState* play,
    ColliderTrisInit* init
) {
    ObjSwitch* this = (ObjSwitch*)thisx;
    s32 i;
    s32 j;
    Vec3f vtx[3];

    Collider_InitTris(play, &this->colliderTris);
    Collider_SetTris(play, &this->colliderTris, &this->dyna.actor, init, this->colliderTrisElements);
    ObjSwitch_AddDekuBubbleToObjSwitchTrisCollider(this);

    Matrix_Push();

    Matrix_SetTranslateRotateYXZ(
        this->dyna.actor.world.pos.x,
        this->dyna.actor.world.pos.y +
            this->dyna.actor.shape.yOffset * this->dyna.actor.scale.y,
        this->dyna.actor.world.pos.z,
        &this->dyna.actor.shape.rot
    );

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            Matrix_MultVec3f(&init->elements[i].dim.vtx[j], &vtx[j]);
        }

        Collider_SetTrisVertices(&this->colliderTris, i, &vtx[0], &vtx[1], &vtx[2]);
    }

    Matrix_Pop();
}

static s32 ObjSwitch_HitActorIsRealDekuBubble(Actor* acActor) {
    return acActor != NULL &&
           acActor->id == ACTOR_EN_ARROW &&
           acActor->params == ARROW_TYPE_DEKU_BUBBLE;
}

static s32 ObjSwitch_HitActorIsCustomSlingshot(Actor* acActor) {
    return acActor != NULL &&
           acActor->id == ACTOR_CUSTOM_SLINGSHOT_PROJECTILE;
}

static s32 ObjSwitch_HitActorIsVanillaEyeProjectile(Actor* acActor) {
    if (acActor == NULL) {
        return false;
    }

    if (acActor->id != ACTOR_EN_ARROW) {
        return false;
    }

    switch (acActor->params) {
    case ARROW_TYPE_NORMAL_LIT:
    case ARROW_TYPE_NORMAL_HORSE:
    case ARROW_TYPE_NORMAL:
    case ARROW_TYPE_FIRE:
    case ARROW_TYPE_ICE:
    case ARROW_TYPE_LIGHT:
        return true;

    case ARROW_TYPE_SLINGSHOT:
    case ARROW_TYPE_DEKU_BUBBLE:
    case ARROW_TYPE_DEKU_NUT:
    default:
        return false;
    }
}

s32 ObjSwitch_IsEyeSwitchHit_Custom(ObjSwitch* this) {
    Actor* acActor;
    Actor* actor = &this->dyna.actor;

    if ((this->colliderTris.base.acFlags & AC_HIT) &&
        !(this->collisionFlags & AC_HIT)) {
        acActor = this->colliderTris.base.ac;

        if (acActor != NULL) {
            Vec3f projectileForward;
            Vec3f switchForward;
            if (ObjSwitch_HitActorIsRealDekuBubble(acActor)) {
                return false;
            }
            if (ObjSwitch_HitActorIsCustomSlingshot(acActor)) {
                return true;
            }
            if (!ObjSwitch_HitActorIsVanillaEyeProjectile(acActor)) {
                return false;
            }
            Matrix_Push();

            Matrix_RotateYS(acActor->world.rot.y, MTXMODE_NEW);
            Matrix_RotateXS(acActor->world.rot.x, MTXMODE_APPLY);
            Matrix_MultVecZ(1.0f, &projectileForward);

            Matrix_RotateYS(actor->shape.rot.y, MTXMODE_NEW);
            Matrix_RotateXS(actor->shape.rot.x, MTXMODE_APPLY);
            Matrix_RotateZS(actor->shape.rot.z, MTXMODE_APPLY);
            Matrix_MultVecZ(1.0f, &switchForward);

            Matrix_Pop();

            if (Math3D_Cos(&projectileForward, &switchForward) < COS_OF_5_PI_DIV_8) {
                return true;
            }
        }
    }

    return false;
}

PATCH_FUNC(OBJ_SWITCH_INIT_TRIS_COLLIDER_ADDR, ObjSwitch_InitTrisCollider_Custom);
PATCH_FUNC(OBJ_SWITCH_IS_EYE_SWITCH_HIT_ADDR, ObjSwitch_IsEyeSwitchHit_Custom);