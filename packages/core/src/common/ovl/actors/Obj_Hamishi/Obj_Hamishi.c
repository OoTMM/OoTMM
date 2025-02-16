/*
 * File: z_obj_hamishi.c
 * Overlay: ovl_Obj_Hamishi
 * Description: Bronze Boulder
 */

#include "Obj_Hamishi.h"
#include <combo/common/actor_init.h>

#if defined(GAME_OOT)
# include "assets/oot/objects/gameplay_field_keep.h"
# define FLAGS 0
# define DAMAGE DMG_HAMMER
# define SILVER_ROCK_FRAGMENTS_DL gSilverRockFragmentsDL
# define SILVER_ROCK_DL gSilverRockDL
#else
# include "assets/mm/objects/gameplay_field_keep.h"
# define OBJHAMISHI_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)
# define FLAGS ACTOR_FLAG_UPDATE_CULLING_DISABLED
# define DAMAGE (DMG_POWDER_KEG | DMG_GORON_POUND | DMG_GORON_PUNCH)
# define SILVER_ROCK_FRAGMENTS_DL gameplay_field_keep_DL_006420
# define SILVER_ROCK_DL gameplay_field_keep_DL_0061E8
#endif

void ObjHamishi_Init(Actor_ObjHamishi* this, PlayState* play);
void ObjHamishi_Destroy(Actor_ObjHamishi* this, PlayState* play);
void ObjHamishi_Update(Actor_ObjHamishi* this, PlayState* play);
void ObjHamishi_Draw(Actor_ObjHamishi* this, PlayState* play);

ActorProfile Obj_Hamishi_Profile = {
    /**/ ACTOR_OBJ_HAMISHI,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_FIELD_KEEP,
    /**/ sizeof(Actor_ObjHamishi),
    /**/ (ActorFunc)ObjHamishi_Init,
    /**/ (ActorFunc)ObjHamishi_Destroy,
    /**/ (ActorFunc)ObjHamishi_Update,
    /**/ (ActorFunc)ObjHamishi_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_HARD,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, 0x00, 0x00 },
    #if defined(GAME_OOT)
        { 0x4FC1FFF6, 0x00, 0x00 },
        ATELEM_NONE,
    #else
        { 0x81C37FB6, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
    #endif
        ACELEM_ON,
        OCELEM_ON,
    },
    { 50, 70, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_IMMOVABLE };

static s16 sEffectScales[] = {
    145, 135, 115, 85, 75, 53, 45, 40, 35,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDistance, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 250, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 500, ICHAIN_STOP),
};

static void ObjHamishi_Alias(Actor_ObjHamishi* this)
{
    Xflag* xflag;

    xflag = &this->xflag;

    switch(xflag->sceneId)
    {
#if defined(GAME_OOT)
    case SCE_OOT_GORON_CITY:
        if(xflag->setupId == 2)
        {
            xflag->setupId = 0;
            switch(xflag->id)
            {
            case 1: xflag->id = 41; break;
            case 4: xflag->id = 45; break;
            case 2: xflag->id = 42; break;
            case 5: xflag->id = 46; break;
            case 3: xflag->id = 43; break;
            }
        }
        break;
    case SCE_OOT_DEATH_MOUNTAIN_CRATER:
        if(xflag->setupId == 2)
        {
            xflag->setupId = 0;
            switch(xflag->id)
            {
            case 4: xflag->id = 12; break;
            case 5: xflag->id = 13; break;
            case 6: xflag->id = 11; break;
            case 7: xflag->id = 14; break;
            }
        }
        break;
#endif
    }
}

static int ObjHamishi_IsShuffled(Actor_ObjHamishi* this)
{
    return !!(this->isExtended && !comboXflagsGet(&this->xflag));
}

static int ObjHamishi_DropCustom(Actor_ObjHamishi* this, PlayState* play)
{
    if (!ObjHamishi_IsShuffled(this))
        return 0;
    if(this->xflag.sceneId == SCE_OOT_DEATH_MOUNTAIN_CRATER && this->xflag.id == 14)
    {
        Player* player;
        player = GET_PLAYER(play);
        EnItem00_DropCustom(play, &player->actor.world.pos, &this->xflag);
    }
    else
    {
        EnItem00_DropCustom(play, &this->actor.world.pos, &this->xflag);
    }
    return 1;
}

static void ObjHamishi_InitXflag(Actor_ObjHamishi* this, PlayState* play)
{
    ComboItemOverride   o;
    Xflag*              xflag;

    /* Set the extended properties */
    xflag = &this->xflag;
    xflag->sceneId = play->sceneId;
    xflag->setupId = g.sceneSetupId;
    xflag->roomId = this->actor.room;
    xflag->sliceId = 0;
    xflag->id = this->actor.actorIndex;

    ObjHamishi_Alias(this);

    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));
}

void ObjHamishi_InitCollision(Actor_ObjHamishi* this, PlayState* play) {
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
}

void ObjHamishi_Shake(Actor_ObjHamishi* this) {
    if (this->shakeFrames > 0) {
        this->shakeFrames--;
        this->shakePosPhase += 5000;
        this->shakeRotPhase += 0xE10;

        Math_StepToF(&this->shakePosSize, 0.0f, 0.15f);
        Math_StepToF(&this->shakeRotSize, 0.0f, 40.0f);

        this->actor.world.pos.x = this->actor.home.pos.x + (Math_SinS(this->shakePosPhase * 4) * this->shakePosSize);
        this->actor.world.pos.z = this->actor.home.pos.z + (Math_CosS(this->shakePosPhase * 7) * this->shakePosSize);
        this->actor.shape.rot.x =
            this->actor.home.rot.x + (s16)(Math_SinS(this->shakeRotPhase * 4) * this->shakeRotSize);
        this->actor.shape.rot.z =
            this->actor.home.rot.z + (s16)(Math_CosS(this->shakeRotPhase * 7) * this->shakeRotSize);
    } else {
        Math_StepToF(&this->actor.world.pos.x, this->actor.home.pos.x, 1.0f);
        Math_StepToF(&this->actor.world.pos.z, this->actor.home.pos.z, 1.0f);
        Math_ScaledStepToS(&this->actor.shape.rot.x, this->actor.home.rot.x, 0xBB8);
        Math_ScaledStepToS(&this->actor.shape.rot.z, this->actor.home.rot.z, 0xBB8);
    }
}

void ObjHamishi_Break(Actor_ObjHamishi* this, PlayState* play) {

    Vec3f velocity;
    Vec3f pos;
    s16 phi_s0 = 1000;
    s16 gravity;
    s16 phi_v0;
    f32 temp_f20;
    f32 temp_f22;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sEffectScales); i++) {
        phi_s0 += 20000;

        temp_f20 = Rand_ZeroOne() * 10.0f;
        pos.x = (Math_SinS(phi_s0) * temp_f20) + this->actor.world.pos.x;
        pos.y = (Rand_ZeroOne() * 40.0f) + this->actor.world.pos.y + 5.0f;
        pos.z = (Math_CosS(phi_s0) * temp_f20) + this->actor.world.pos.z;

        temp_f20 = (Rand_ZeroOne() * 10.0f) + 2.0f;
        velocity.x = Math_SinS(phi_s0) * temp_f20;
        temp_f22 = Rand_ZeroOne();
        velocity.y = (Rand_ZeroOne() * i * 2.5f) + (temp_f22 * 15.0f);
        velocity.z = Math_CosS(phi_s0) * temp_f20;

        if (i == 0) {
            phi_v0 = 41;
            gravity = -450;
        } else if (i < 4) {
            phi_v0 = 37;
            gravity = -380;
        } else {
            phi_v0 = 69;
            gravity = -320;
        }

        EffectSsKakera_Spawn(play, &pos, &velocity, &this->actor.world.pos, gravity, phi_v0, 30, 5, 0, sEffectScales[i],
                             3, 0, 70, 1, OBJECT_GAMEPLAY_FIELD_KEEP, SILVER_ROCK_FRAGMENTS_DL);
    }

#if defined(GAME_OOT)
    func_80033480(play, &this->actor.world.pos, 140.0f, 6, 180, 90, 1);
    func_80033480(play, &this->actor.world.pos, 140.0f, 12, 80, 90, 1);
#else
    func_800BBFB0(play, &this->actor.world.pos, 140.0f, 6, 180, 90, 1);
    func_800BBFB0(play, &this->actor.world.pos, 140.0f, 12, 80, 90, 1);
#endif
}

#if defined(GAME_MM)
void ObjHamishi_SnapToFloor(Actor_ObjHamishi* this, PlayState* play) {
    Vec3f sp28;
    s32 bgId;

    sp28.x = this->actor.world.pos.x;
    sp28.y = this->actor.world.pos.y + 30.0f;
    sp28.z = this->actor.world.pos.z;

    this->actor.floorHeight =
        BgCheck_EntityRaycastFloor5(&play->colCtx, &this->actor.floorPoly, &bgId, &this->actor, &sp28);
}

s32 ObjHamishi_IsUnderwater(Actor_ObjHamishi* this, PlayState* play) {
    WaterBox* waterBox;
    f32 waterSurface;
    s32 bgId;

    if (WaterBox_GetSurfaceImpl(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z, &waterSurface,
                                &waterBox, &bgId) &&
        (this->actor.world.pos.y < waterSurface)) {
        return true;
    }
    return false;
}

#endif

void ObjHamishi_Init(Actor_ObjHamishi* this, PlayState* play) {

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ObjHamishi_InitXflag(this, play);

    if (play->csCtx.state != CS_STATE_IDLE) {
        this->actor.cullingVolumeDistance += 1000.0f;
    }
    if (this->actor.shape.rot.y == 0) {
        this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.home.rot.y = Rand_ZeroFloat(65536.0f);
    }

    ObjHamishi_InitCollision(this, play);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, NULL, &sColChkInfoInit);
#if defined(GAME_MM)
    ObjHamishi_SnapToFloor(this, play);
#endif

#if defined(GAME_OOT)
    if (!ObjHamishi_IsShuffled(this) && Flags_GetSwitch(play, PARAMS_GET_U(this->actor.params, 0, 6))) {
#else
    if (!ObjHamishi_IsShuffled(this) && Flags_GetSwitch(play, OBJHAMISHI_GET_SWITCH_FLAG(&this->actor))) {
#endif
        Actor_Kill(&this->actor);
        return;
    }

    this->actor.shape.yOffset = 80.0f;

#if defined(GAME_MM)
    if (ObjHamishi_IsUnderwater(this, play)) {
        this->unk_1A2 |= 1;
    }
#endif
}

void ObjHamishi_Destroy(Actor_ObjHamishi* this, PlayState* play) {
    Collider_DestroyCylinder(play, &this->collider);
}

void ObjHamishi_Update(Actor_ObjHamishi* this, PlayState* play) {
        ObjHamishi_Shake(this);

    if ((this->collider.base.acFlags & AC_HIT))
    {
        this->collider.base.acFlags &= ~AC_HIT;
        if(this->collider.elem.acHitElem->atDmgInfo.dmgFlags & DAMAGE)
        {
#if defined(GAME_MM)
            this->unk_1A1 = 5;
            if(this->collider.elem.acHitElem->atDmgInfo.dmgFlags & DMG_GORON_POUND)
            {
                this->unk_1A0 = 26;
            }
            else
            {
                this->unk_1A0 = 11;
            }
            if(this->collider.elem.acHitElem->atDmgInfo.dmgFlags & DMG_POWDER_KEG)
            {
                this->hitCount = 1;
            }
#endif
            this->hitCount++;
            if (this->hitCount < 2) {
                this->shakeFrames = 15;
                this->shakePosSize = 2.0f;
                this->shakeRotSize = 400.0f;
            } else {
                ObjHamishi_DropCustom(this, play);
                ObjHamishi_Break(this, play);
                SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
#if defined(GAME_OOT)
                Flags_SetSwitch(play, PARAMS_GET_U(this->actor.params, 0, 6));
#else
                Flags_SetSwitch(play, OBJHAMISHI_GET_SWITCH_FLAG(&this->actor));
#endif
                Actor_Kill(&this->actor);
            }
        }
    }

#if defined(GAME_OOT)
    if (this->actor.xzDistToPlayer < 600.0f) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
#endif

#if defined(GAME_MM)
    if (this->actor.update != NULL) {
        if (this->unk_1A1 > 0) {
            this->unk_1A1--;
            if (this->unk_1A1 == 0) {
                this->collider.base.colMaterial = COL_MATERIAL_HARD;
            } else {
                this->collider.base.colMaterial = COL_MATERIAL_NONE;
            }
        }

        if (this->unk_1A0 > 0) {
            this->unk_1A0--;
        } else if ((this->actor.flags & ACTOR_FLAG_MM_INSIDE_CULLING_VOLUME) && (this->actor.xzDistToPlayer < 1000.0f)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }

        if (this->actor.xzDistToPlayer < 600.0f) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        }
    }
#endif
}

static int ObjHamishi_CsmcType(Actor_ObjHamishi* this, PlayState* play)
{
    ComboItemOverride o;

    if (!ObjHamishi_IsShuffled(this))
        return CSMC_NORMAL;

    if (!csmcEnabled())
        return CSMC_MAJOR;

    comboXflagItemOverride(&o, &this->xflag, 0);
    return csmcFromItemCloaked(o.gi, o.cloakGi);
}

static void ObjHamishi_DrawCamc(Actor_ObjHamishi* this, PlayState* play) {
    int camcType;

    camcType = ObjHamishi_CsmcType(this, play);
    if(camcType != CSMC_NORMAL)
    {
        const Color_RGB8*   color;
        color = csmcTypeColor(camcType);
#if defined(GAME_OOT)
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
#else
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
#endif
        Gfx_DrawFlameColor(play, color->r << 24 | color->g << 16 | color->b << 8 | 0xcc, 4.5f, 120.f);
    }
}

void ObjHamishi_Draw(Actor_ObjHamishi* this, PlayState* play) {

    OPEN_DISPS(play->state.gfxCtx);
#if defined(GAME_OOT)
    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 170, 130, 255);
    gSPDisplayList(POLY_OPA_DISP++, SILVER_ROCK_DL);
    ObjHamishi_DrawCamc(this, play);
#else
    if ((this->actor.projectedPos.z <= 2150.0f) || ((this->unk_1A2 & 1) && (this->actor.projectedPos.z < 2250.0f))) {
        this->actor.shape.shadowAlpha = 160;
        Gfx_SetupDL25_Opa(play->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x08, D_801AEFA0);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 170, 130, 255);
        gSPDisplayList(POLY_OPA_DISP++, SILVER_ROCK_DL);
        } else if (this->actor.projectedPos.z < 2250.0f) {
            f32 sp20 = (2250.0f - this->actor.projectedPos.z) * 2.55f;

            this->actor.shape.shadowAlpha = sp20 * 0.627451f;
            Gfx_SetupDL25_Xlu(play->state.gfxCtx);

            gSPSegment(POLY_XLU_DISP++, 0x08, D_801AEF88);
            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 170, 130, (s32)sp20);
            gSPDisplayList(POLY_XLU_DISP++, SILVER_ROCK_DL);
        } else {
            this->actor.shape.shadowAlpha = 0;
        }
        ObjHamishi_DrawCamc(this, play);
#endif
    CLOSE_DISPS();
}

OVL_INFO_ACTOR(ACTOR_OBJ_HAMISHI, Obj_Hamishi_Profile);
