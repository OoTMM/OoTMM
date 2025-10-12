#include "En_Snowwd.h"
#include <assets/mm/objects/object_snowwd.h>

#define FLAGS 0x00000000

void EnSnowwd_Init(Actor* thisx, PlayState* play);
void EnSnowwd_Destroy(Actor* thisx, PlayState* play);
void EnSnowwd_Update(Actor* thisx, PlayState* play);
void EnSnowwd_Draw(Actor* thisx, PlayState* play);

void EnSnowwd_Idle(EnSnowwd* this, PlayState* play);

ActorProfile En_Snowwd_Profile = {
    /**/ ACTOR_EN_SNOWWD,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_SNOWWD,
    /**/ sizeof(EnSnowwd),
    /**/ EnSnowwd_Init,
    /**/ EnSnowwd_Destroy,
    /**/ EnSnowwd_Update,
    /**/ EnSnowwd_Draw,
};

OVL_INFO_ACTOR(ACTOR_EN_SNOWWD, En_Snowwd_Profile);

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_TREE,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK5,
        { 0x00000000, 0x00, 0x00 },
        { 0x0100020A, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 18, 60, 0, { 0, 0, 0 } },
};

static void EnSnowwd_Alias(Xflag* xf)
{
    switch (xf->sceneId)
    {
    case SCE_MM_PATH_MOUNTAIN_VILLAGE:
        if (xf->setupId == 1)
        {
            xf->setupId = 0;
            xf->id -= 5;
        }
        break;
    case SCE_MM_PATH_SNOWHEAD:
        if (xf->setupId == 1)
        {
            xf->setupId = 0;
            switch (xf->id)
            {
            case 00: xf->id = 24; break;
            case 01: xf->id = 23; break;
            case 02: xf->id = 22; break;
            case 03: xf->id = 25; break;
            }
        }
        break;
    }
}

void EnSnowwd_Init(Actor* thisx, PlayState* play) {
    EnSnowwd* this = (EnSnowwd*)thisx;

    if (comboXflagInit(&this->xflag, thisx, play)) {
        EnSnowwd_Alias(&this->xflag);
    }

    SNOWWD_DROPPED_COLLECTIBLE(thisx) = false;
    this->actor.home.rot.y = 0;
    this->timer = 0;
    this->actor.cullingVolumeDistance = 4000.0f;
    this->actor.cullingVolumeScale = 2000.0f;
    this->actor.cullingVolumeDownward = 2400.0f;
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Actor_SetScale(&this->actor, 1.0f);
    this->actionFunc = EnSnowwd_Idle;
}

void EnSnowwd_Destroy(Actor* thisx, PlayState* play) {
    EnSnowwd* this = (EnSnowwd*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

static int EnSnowwd_DropCustom(EnSnowwd* this, PlayState* play)
{
    if (Xflag_IsShuffled(&this->xflag))
    {
        EnItem00_DropCustom(play, &this->actor.world.pos, &this->xflag);
        return true;
    }
    return false;
}

void EnSnowwd_Idle(EnSnowwd* this, PlayState* play) {
    static Vec3f sAccel = { 0.0f, 0.0f, 0.0f };
    static Vec3f sVelocity = { 0.0f, -4.0f, 0.0f };
    static Color_RGBA8 sPrimColor = { 255, 255, 255, 255 };
    static Color_RGBA8 sEnvColor = { 200, 200, 220, 0 };

    Actor* thisx = &this->actor;
    f32 wobbleAmplitude;
    Vec3f pos;

    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_PlaySfx(thisx, NA_SE_IT_REFLECTION_WOOD);
    }
    if (thisx->home.rot.y != 0) {
        this->timer = 21;
        thisx->home.rot.y = 0;
        if (!EnSnowwd_DropCustom(this, play) && !SNOWWD_DROPPED_COLLECTIBLE(&this->actor)) {
            if (SNOWWD_GET_DROP_TABLE(&this->actor) < 16) {
                pos = thisx->world.pos;
                pos.y += 200.0f;
                Item_DropCollectibleRandom(play, NULL, &pos, SNOWWD_GET_DROP_TABLE(&this->actor) * 16);
            }
            SNOWWD_DROPPED_COLLECTIBLE(&this->actor) = true;
        }
    }
    if (thisx->xzDistToPlayer < 600.0f) {
        Collider_UpdateCylinder(thisx, &this->collider);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }

    // Wobble from impact and dust particles
    if (this->timer > 0) {
        this->timer--;
        wobbleAmplitude = Math_SinS((this->timer ^ 0xFFFF) * 0x3332) * 250.0f;
        thisx->shape.rot.x = Math_CosS(thisx->yawTowardsPlayer - thisx->shape.rot.y) * wobbleAmplitude;
        thisx->shape.rot.z = Math_SinS(thisx->yawTowardsPlayer - thisx->shape.rot.y) * wobbleAmplitude;
        pos = thisx->world.pos;
        pos.x += Rand_CenteredFloat(80.0f);
        pos.y += 100.0f + Rand_ZeroFloat(30.0f);
        pos.z += Rand_CenteredFloat(80.0f);
        func_800B0EB0(play, &pos, &sVelocity, &sAccel, &sPrimColor, &sEnvColor, 200, 10, 20);
    }
}

void EnSnowwd_Update(Actor* thisx, PlayState* play) {
    EnSnowwd* this = (EnSnowwd*)thisx;

    this->actionFunc(this, play);
}

void EnSnowwd_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(gSnowTreeSnowLeavesTex));
    gSPDisplayList(POLY_OPA_DISP++, gSnowTreeDL);

    CLOSE_DISPS();
}
