#include <combo.h>
#include <combo/global.h>
#include <combo/item.h>
#include "Obj_Snowball2.h"
#include <assets/mm/objects/object_goroiwa.h>

#define ENOBJSNOWBALL2_GET_3F(thisx) ((thisx)->params & 0x3F)
#define ENOBJSNOWBALL2_GET_7F00(thisx) (((thisx)->params >> 8) & 0x7F)

#define FLAGS (ACTOR_FLAG_THROW_ONLY)

void ObjSnowball2_Init(Actor_ObjSnowball2* this, PlayState* play);
void ObjSnowball2_Destroy(Actor_ObjSnowball2* this, PlayState* play);
void ObjSnowball2_Update(Actor_ObjSnowball2* this, PlayState* play);
void ObjSnowball2_Draw(Actor_ObjSnowball2* this, PlayState* play);

void func_80B38E20(Actor_ObjSnowball2* this);
void func_80B39C78(Actor_ObjSnowball2* this);
void func_80B39C9C(Actor_ObjSnowball2* this, PlayState* play);
void func_80B39F60(Actor_ObjSnowball2* this);
void func_80B39FA8(Actor_ObjSnowball2* this, PlayState* play);
void func_80B3A0D8(Actor_ObjSnowball2* this);
void func_80B3A13C(Actor_ObjSnowball2* this, PlayState* play);
void func_80B3A498(Actor_ObjSnowball2* this);
void func_80B3A500(Actor_ObjSnowball2* this, PlayState* play);

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00400000, 0x00, 0x02 },
            { 0x0583FFBE, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

Color_RGBA8 D_80B3A914 = { 250, 250, 250, 255 };

Color_RGBA8 D_80B3A918 = { 180, 180, 180, 255 };

Gfx* D_80B3A91C[] = {
    (void*)object_goroiwa_DL_0072F0,
    (void*)object_goroiwa_DL_0077D0,
    (void*)object_goroiwa_DL_007C60,
    (void*)object_goroiwa_DL_007C60,
};

Vec3f D_80B3A92C = { 0.0f, 0.3f, 0.0f };

void func_80B38E20(Actor_ObjSnowball2* this) {
    Matrix_SetTranslateRotateYXZ(this->actor.world.pos.x,
                                 this->actor.world.pos.y + (this->actor.shape.yOffset * this->actor.scale.y),
                                 this->actor.world.pos.z, &this->actor.shape.rot);
    Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
    Collider_UpdateSpheres(0, &this->collider);
}

void ObjSnowball2_Alias(Actor_ObjSnowball2* this)
{
    Xflag* xflag;
    xflag = &this->xflag;

    if (xflag->sceneId == SCE_MM_MOUNTAIN_VILLAGE_SPRING)
    {
        switch(xflag->id)
        {
            case 45:
                xflag->sceneId = SCE_MM_MOUNTAIN_VILLAGE_WINTER;
                xflag->id = 41;
                break;
            case 47:
            case 48:
            case 49:
            case 50:
                xflag->sceneId = SCE_MM_MOUNTAIN_VILLAGE_WINTER;
                xflag->id += 6;
                break;
            default:
                UNREACHABLE();
        }
    }
    if (xflag->sceneId == SCE_MM_PATH_SNOWHEAD && xflag->setupId == 1)
    {
        switch(xflag->id)
        {
            case 17:
            case 18:
            case 19:
                xflag->id -= 8;
                xflag->setupId = 0;
                break;
        }
    }
    if (xflag->sceneId == SCE_MM_SNOWHEAD && xflag->setupId == 1)
    {
        switch(xflag->id)
        {
            case 11: xflag->id = 43; xflag->setupId = 0; break;
            case 12: xflag->id = 44; xflag->setupId = 0; break;
            case 13: xflag->id = 45; xflag->setupId = 0; break;
            case 20: xflag->id = 42; xflag->setupId = 0; break;
        }
    }
}

static int ObjSnowball2_IsShuffled(Actor_ObjSnowball2* this)
{
    return !!(this->isExtended && !comboXflagsGet(&this->xflag));
}

static void ObjSnowball2_InitXflag(Actor_ObjSnowball2* this, PlayState* play)
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

    ObjSnowball2_Alias(this);

    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));
}

void ObjSnowball2_DropCollectible(Actor_ObjSnowball2* this, PlayState* play) {
    s32 itemDrop;

    if (ObjSnowball2_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->actor.world.pos, &this->xflag);
        return;
    }

    if (!this->unk_1AE) {
        itemDrop = Item_CollectibleDropTable(ENOBJSNOWBALL2_GET_3F(&this->actor));
        if (itemDrop > ITEM00_NO_DROP) {
            Item_DropCollectible(play, &this->actor.world.pos, (ENOBJSNOWBALL2_GET_7F00(&this->actor) << 8) | itemDrop);
            this->unk_1AE = TRUE;
        }
    }
}

void func_80B38EFC(Actor_ObjSnowball2* this, PlayState* play) {
    s32 i;
    Vec3f spA4;
    Vec3f sp98;
    Vec3f sp8C;
    f32 sp88 = this->actor.world.pos.y + (this->actor.shape.yOffset * this->actor.scale.y);

    for (i = 0; i < 2; i++) {
        sp8C.x = ((Rand_ZeroOne() * 30.0f) - 15.0f) + this->actor.world.pos.x;
        sp8C.y = ((Rand_ZeroOne() * 20.0f) - 10.0f) + sp88;
        sp8C.z = ((Rand_ZeroOne() * 30.0f) - 15.0f) + this->actor.world.pos.z;

        sp98.x = (Rand_ZeroOne() - 0.5f) * 1.6f;
        sp98.y = -1.5f;
        sp98.z = (Rand_ZeroOne() - 0.5f) * 1.6f;

        spA4.x = sp98.x * -0.06f;
        spA4.y = sp98.y * -0.06f;
        spA4.z = sp98.z * -0.06f;

        func_800B0E48(play, &sp8C, &sp98, &spA4, &D_80B3A914, &D_80B3A918, (s32)(Rand_ZeroOne() * 40.0f) + 10,
                      (s32)(Rand_ZeroOne() * 30.0f) + 10);
    }
}

void func_80B39108(Actor_ObjSnowball2* this, PlayState* play) {
    s32 phi_s5;
    s16 phi_s1;
    s32 phi_v0;
    s32 i;
    u32 temp;
    s32 temp_s3;
    Vec3f spDC;
    Vec3f spD0;
    f32 temp_f2;
    f32 spC8 = this->actor.world.pos.y + (this->actor.shape.yOffset * this->actor.scale.y);
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;

    for (i = 0, phi_s5 = 0; i < 11; i++, phi_s5 += (0x10000 / 11)) {
        temp_s3 = (s32)(Rand_ZeroOne() * 5957.0f) + phi_s5;

        if ((Rand_Next() >> 0x1E) == 0) {
            phi_s1 = 0x20;
        } else {
            phi_s1 = 0x40;
        }

        if ((i & 3) == 0) {
            phi_s1 |= 1;
        }

        temp_f0 = Rand_ZeroOne();
        temp_f20 = (1.0f - SQ(temp_f0)) * 12.0f;
        temp_f22 = Math_SinS(temp_s3);
        temp_f24 = Math_CosS(temp_s3);
        temp_f12 = (Rand_ZeroOne() * 1.6f) - 0.8f;
        temp_f2 = fabsf(temp_f12) * temp_f12;

        spDC.x = temp_f22 * 4.0f;
        spDC.y = (temp_f2 * 6.0f) + 9.0f;
        spDC.z = temp_f24 * 4.0f;

        spD0.x = (temp_f22 * temp_f20) + this->actor.world.pos.x;
        spD0.y = (temp_f2 * temp_f20) + spC8;
        spD0.z = (temp_f24 * temp_f20) + this->actor.world.pos.z;
        temp = Rand_Next();

        if ((i & 3) == 0) {
            phi_v0 = 1;
        } else {
            phi_v0 = 0;
        }

        EffectSsKakera_Spawn(play, &spD0, &spDC, &spD0, -300, phi_s1, 30, 0, 0, (temp >> 0x1D) + 8, phi_v0, 0, 50, -1,
                             OBJECT_GOROIWA, D_80B3A91C[i & 3]);

        spD0.x += (Rand_ZeroOne() - 0.5f) * 40.0f;
        spD0.y += (Rand_ZeroOne() - 0.3f) * 45.0f;
        spD0.z += (Rand_ZeroOne() - 0.5f) * 40.0f;

        func_800B0E48(play, &spD0, &gZeroVec3f, &D_80B3A92C, &D_80B3A914, &D_80B3A918,
                      (s32)(Rand_ZeroOne() * 70.0f) + 10, (s32)(Rand_ZeroOne() * 100.0f) + 10);
    }
    spD0.y = (temp_f2 * temp_f20) + spC8;
}

void func_80B39470(Actor_ObjSnowball2* this, PlayState* play) {
    Vec3f sp58;
    s32 phi_s0;
    s32 i;

    sp58.y = this->actor.world.pos.y + this->actor.depthInWater;

    for (phi_s0 = 0, i = 0; i < 5; i++, phi_s0 += (0x10000 / 5)) {
        sp58.x = this->actor.world.pos.x + (Math_SinS((s32)(Rand_ZeroOne() * 7200.0f) + phi_s0) * 15.0f);
        sp58.z = this->actor.world.pos.z + (Math_CosS((s32)(Rand_ZeroOne() * 7200.0f) + phi_s0) * 15.0f);
        EffectSsGSplash_Spawn(play, &sp58, NULL, NULL, 0, 200);
    }

    sp58.x = this->actor.world.pos.x;
    sp58.z = this->actor.world.pos.z;
    EffectSsGSplash_Spawn(play, &sp58, NULL, NULL, 0, 300);
}

void func_80B395C4(PlayState* play, Vec3f* arg1) {
    EffectSsGRipple_Spawn(play, arg1, 200, 700, 0);
}

void func_80B395EC(Actor_ObjSnowball2* this, PlayState* play) {
    Vec3f sp18;

    sp18.x = this->actor.world.pos.x;
    sp18.y = this->actor.world.pos.y + this->actor.depthInWater;
    sp18.z = this->actor.world.pos.z;
    func_80B395C4(play, &sp18);
}

void func_80B39638(PlayState* play, Vec3f* arg1) {
    static s16 D_80B3A938 = 0;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    Vec3f sp98;
    Vec3f sp8C;
    Vec3f sp80;
    f32 temp_f2;
    s32 i;

    D_80B3A938 += (s16)(Rand_Next() >> 0x11);

    for (i = 0; i < 2; i++) {
        D_80B3A938 += 0x8000;
        temp_f20 = Rand_ZeroOne();
        temp_f22 = Math_SinS(D_80B3A938) * temp_f20;
        temp_f24 = Rand_ZeroOne();
        temp_f2 = Math_CosS(D_80B3A938) * temp_f20;

        sp98.x = arg1->x + (temp_f22 * 5.0f);
        sp98.y = arg1->y + (temp_f24 * 25.0f);
        sp98.z = arg1->z + (temp_f2 * 5.0f);

        sp8C.x = temp_f22 * 1.55f;
        sp8C.y = temp_f24 * -0.7f;
        sp8C.z = temp_f2 * 1.55f;

        sp80.x = sp8C.x * -0.02f;
        sp80.y = sp8C.y * -0.025f;
        sp80.z = sp8C.z * -0.02f;

        EffectSsIceSmoke_Spawn(play, &sp98, &sp8C, &sp80, (s32)(Rand_ZeroOne() * 30.0f) + 68);
    }
}

void func_80B39834(Actor_ObjSnowball2* this, PlayState* play) {
    s32 i;

    for (i = 0; i < 3; i++) {
        EffectSsBubble_Spawn(play, &this->actor.world.pos, 0.0f, 20.0f, 30.0f, (Rand_ZeroOne() * 0.11f) + 0.03f);
    }
}

void func_80B39908(Actor_ObjSnowball2* this, PlayState* play) {
    Vec3f spAC;
    Vec3f spA0;
    Vec3f sp94;
    s32 i;

    if (this->collider.elements[0].base.acElemFlags & AC_HIT) {
        Vec3s* hitPos = &this->collider.elements[0].base.acDmgInfo.hitPos;

        for (i = 0; i < 4; i++) {
            sp94.x = ((Rand_ZeroOne() * 14.0f) - 7.0f) + hitPos->x;
            sp94.y = ((Rand_ZeroOne() * 14.0f) - 7.0f) + hitPos->y;
            sp94.z = ((Rand_ZeroOne() * 14.0f) - 7.0f) + hitPos->z;

            spA0.x = (Rand_ZeroOne() - 0.5f) * 1.6f;
            spA0.y = -0.8f;
            spA0.z = (Rand_ZeroOne() - 0.5f) * 1.6f;

            spAC.x = spA0.x * -0.06f;
            spAC.y = spA0.y * -0.06f;
            spAC.z = spA0.z * -0.06f;

            func_800B0E48(play, &sp94, &spA0, &spAC, &D_80B3A914, &D_80B3A918, (s32)(Rand_ZeroOne() * 40.0f) + 10,
                          (s32)(Rand_ZeroOne() * 30.0f) + 10);
        }
    }
}

void func_80B39B28(Actor_ObjSnowball2* this, PlayState* play) {
    if ((this->unk_1AE == 0) && (play->roomCtx.curRoom.num != this->unk_1AF)) {
        this->unk_1AE = 1;
    }
}

void func_80B39B5C(Actor_ObjSnowball2* this, PlayState* play) {
    SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EV_SMALL_SNOWBALL_BROKEN);
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_CONTINUE),  ICHAIN_F32_DIV1000(minVelocityY, -20000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDistance, 2000, ICHAIN_CONTINUE), ICHAIN_F32(cullingVolumeScale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 100, ICHAIN_CONTINUE), ICHAIN_VEC3F_DIV1000(scale, 25, ICHAIN_STOP),
};

void ObjSnowball2_Init(Actor_ObjSnowball2* this, PlayState* play) {
    ObjSnowball2_InitXflag(this, play);
    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitJntSph(play, &this->collider);
    this->actor.shape.rot.x = 0;
    this->actor.shape.rot.z = 0;
    this->actor.shape.rot.y = Rand_Next() >> 0x10;
    ActorShape_Init(&this->actor.shape, 200.0f, NULL, 12.5f);
    this->actor.shape.shadowAlpha = 130;
    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    func_80B38E20(this);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->unk_1AF = this->actor.room;
    func_80B39C78(this);
}

void ObjSnowball2_Destroy(Actor_ObjSnowball2* this, PlayState* play) {
    Collider_DestroyJntSph(play, &this->collider);
}

void func_80B39C78(Actor_ObjSnowball2* this) {
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->unk_1AD = 0;
    this->actionFunc = func_80B39C9C;
}

void func_80B39C9C(Actor_ObjSnowball2* this, PlayState* play) {
    s32 sp38 = (this->collider.base.acFlags & AC_HIT) != 0;

    if (sp38) {
        this->collider.base.acFlags &= ~AC_HIT;
    }

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.room = -1;
        this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        if (Item_CollectibleDropTable2(ENOBJSNOWBALL2_GET_3F(&this->actor))) {
            ObjSnowball2_DropCollectible(this, play);
        }
        func_80B38EFC(this, play);
        Player_PlaySfx(GET_PLAYER(play), NA_SE_PL_PULL_UP_SNOWBALL);
        func_80B39F60(this);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER) &&
               ((this->actor.shape.yOffset * this->actor.scale.y) < this->actor.depthInWater)) {
        func_80B3A498(this);
    } else if (sp38 && (this->collider.elements->base.acHitElem->atDmgInfo.dmgFlags & 0x0583FFBC)) {
        ObjSnowball2_DropCollectible(this, play);
        func_80B39108(this, play);
        func_80B39B5C(this, play);
        Actor_Kill(&this->actor);
        return;
    } else {
        if (sp38 && (this->collider.elements->base.acHitElem->atDmgInfo.dmgFlags & 2)) {
            func_80B39908(this, play);
        }

        if (this->unk_1AD == 0) {
            Actor_MoveWithGravity(&this->actor);
            Actor_UpdateBgCheckInfo(play, &this->actor, 15.0f, 15.0f, 0.0f,
                                    UPDBGCHECKINFO_FLAG_MM_4 | UPDBGCHECKINFO_FLAG_MM_40);
            if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
                (DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId) == NULL)) {
                this->unk_1AD = 1;
                this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
            }
        }

        if (this->actor.xzDistToPlayer < 800.0f) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            if (this->actor.xzDistToPlayer < 150.0f) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
                if (this->actor.xzDistToPlayer < 100.0f) {
                    if (ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, GET_PLAYER(play)->actor.world.rot.y)) >=
                        0x5556) {
                        Actor_OfferGetItem(&this->actor, play, GI_NONE, 36.0f, 30.0f);
                    }
                }
            }
        }
    }
}

void func_80B39F60(Actor_ObjSnowball2* this) {
    this->actor.world.pos.y += (600.0f - this->actor.shape.yOffset) * this->actor.scale.y;
    this->unk_1AC = 4;
    this->actor.shape.yOffset = 600.0f;
    this->actionFunc = func_80B39FA8;
}

void func_80B39FA8(Actor_ObjSnowball2* this, PlayState* play) {
    Vec3f sp30;
    s32 sp2C;

    func_80B39B28(this, play);

    if (this->unk_1AC > 0) {
        this->unk_1AC--;
        func_80B38EFC(this, play);
    }

    if (Actor_HasNoParent(&this->actor, play)) {
        this->actor.room = play->roomCtx.curRoom.num;
        this->actor.speed *= 3.8f;
        this->actor.velocity.y *= 0.4f;
        this->actor.gravity = -2.8f;
        Actor_MoveWithGravity(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 15.0f, 15.0f, 0.0f,
                                UPDBGCHECKINFO_FLAG_MM_1 | UPDBGCHECKINFO_FLAG_MM_4 | UPDBGCHECKINFO_FLAG_MM_40);
        func_80B3A0D8(this);
    } else {
        sp30.x = this->actor.world.pos.x;
        sp30.y = this->actor.world.pos.y + 20.0f;
        sp30.z = this->actor.world.pos.z;
        this->actor.floorHeight =
            BgCheck_EntityRaycastFloor5(&play->colCtx, &this->actor.floorPoly, &sp2C, &this->actor, &sp30);
    }
}

void func_80B3A0D8(Actor_ObjSnowball2* this) {
    this->unk_1AC = 60;
    this->actor.colChkInfo.mass = 180;
    this->unk_1A8 = Rand_S16Offset(-5000, 4000);
    this->unk_1AA = Rand_S16Offset(-800, 1600);
    this->actionFunc = func_80B3A13C;
}

void func_80B3A13C(Actor_ObjSnowball2* this, PlayState* play) {
    s32 sp38 = (this->collider.base.atFlags & AT_HIT) != 0;
    s32 sp34 = (this->collider.base.ocFlags1 & OC1_HIT) != 0;
    s32 sp30;

    if (sp38) {
        this->collider.base.atFlags &= ~AT_HIT;
    }

    if (sp34) {
        this->collider.base.ocFlags1 &= ~OC1_HIT;
    }

    if (this->unk_1AC > 0) {
        this->unk_1AC--;
    }

    func_80B39B28(this, play);

    if ((((this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL)) || sp38) &&
         !(this->actor.bgCheckFlags & BGCHECKFLAG_WATER)) ||
        (this->unk_1AC <= 0)) {
        ObjSnowball2_DropCollectible(this, play);
        func_80B39108(this, play);
        func_80B39B5C(this, play);
        Actor_Kill(&this->actor);
        return;
    }

    sp30 = FALSE;
    if (this->actor.bgCheckFlags & (BGCHECKFLAG_WATER | BGCHECKFLAG_WATER_TOUCH)) {
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) || (this->actor.speed > 3.0f)) {
            if (this->actor.depthInWater < (1200.0f * this->actor.scale.y)) {
                func_80B39470(this, play);
                func_80B395EC(this, play);
            }
            if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) {
                Actor_PlaySfx(&this->actor, NA_SE_EV_BOMB_DROP_WATER);
            }
        } else if ((((play->gameplayFrames % 16) == 0) || ((Rand_Next() >> 0x10) == 0)) &&
                   (this->actor.depthInWater < (1200.0f * this->actor.scale.y))) {
            func_80B395EC(this, play);
        }

        func_80B39834(this, play);
        this->unk_1A8 >>= 1;
        this->unk_1AA >>= 1;

        if (sp34) {
            this->actor.speed *= 0.8f;
        } else {
            this->actor.speed *= 0.65f;
        }
        this->actor.velocity.y *= 0.27f;
        this->actor.gravity *= 0.27f;
        if (this->actor.speed < 0.4f) {
            sp30 = TRUE;
        }
    } else {
        if (sp34) {
            this->actor.speed *= 0.8f;
        } else {
            this->actor.speed *= 0.96f;
        }
        this->actor.velocity.y *= 0.96f;
    }

    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 15.0f, 15.0f, 0.0f,
                            UPDBGCHECKINFO_FLAG_MM_1 | UPDBGCHECKINFO_FLAG_MM_4 | UPDBGCHECKINFO_FLAG_MM_40);
    this->actor.shape.rot.x += this->unk_1A8;
    this->actor.shape.rot.y += this->unk_1AA;
    func_80B38E20(this);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    if (sp30) {
        func_80B3A498(this);
    } else {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
}

void func_80B3A498(Actor_ObjSnowball2* this) {
    this->actor.home.pos.x = this->actor.world.pos.x;
    this->unk_1AC = 46;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actor.home.pos.y = this->actor.world.pos.y + this->actor.depthInWater;
    this->actor.home.pos.z = this->actor.world.pos.z;
    this->actor.world.pos.y += this->actor.shape.yOffset * this->actor.scale.y;
    this->actor.shape.yOffset = 0.0f;
    this->actor.speed = 0.0f;
    this->actionFunc = func_80B3A500;
}

void func_80B3A500(Actor_ObjSnowball2* this, PlayState* play) {
    f32 phi_f0;
    f32 temp_f14 = this->actor.home.pos.y - this->actor.world.pos.y;
    f32 temp_f12 = this->actor.scale.y * 600.0f;

    this->unk_1AC--;

    this->actor.speed *= 0.7f;

    this->unk_1A8 >>= 1;
    this->unk_1AA >>= 1;

    this->actor.shape.rot.x += this->unk_1A8;
    this->actor.shape.rot.y += this->unk_1AA;

    if (temp_f14 < -temp_f12) {
        this->actor.gravity = this->actor.scale.y * -40.0f;
        phi_f0 = 0.94f;
    } else if (temp_f12 < temp_f14) {
        this->actor.gravity = this->actor.scale.y * 24.0f;
        phi_f0 = 0.8f;
    } else if (temp_f12 > 0.001f) {
        this->actor.gravity = (((1.6f * temp_f14) / temp_f12) + -1.0f + 0.6f) * 0.5f * 40.0f * this->actor.scale.y;
        phi_f0 = (((-0.13999999f * temp_f14) / temp_f12) + 0.94f + 0.8f) * 0.5f;
    } else {
        this->actor.gravity = 0.0f;
        phi_f0 = 1.0f;
    }

    this->actor.velocity.y *= phi_f0;
    this->actor.velocity.y += this->actor.gravity;
    this->actor.world.pos.y += this->actor.velocity.y;

    if (((play->gameplayFrames % 16) == 0) || ((Rand_Next() >> 0x10) == 0)) {
        func_80B395C4(play, &this->actor.home.pos);
    }

    if (this->unk_1AC <= 0) {
        Actor_Kill(&this->actor);
        return;
    }

    if (this->unk_1AC < 20) {
        this->actor.scale.x -= 0.00125f;
        this->actor.scale.y = this->actor.scale.x;
        this->actor.scale.z = this->actor.scale.x;

        if ((this->unk_1AC >= 6) && (temp_f14 < temp_f12)) {
            func_80B39638(play, &this->actor.home.pos);
        }

        if (this->unk_1AC == 10) {
            ObjSnowball2_DropCollectible(this, play);
        }

        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EV_ICE_MELT_LEVEL - SFX_FLAG);
    } else {
        func_80B38E20(this);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
}

void ObjSnowball2_Update(Actor_ObjSnowball2* this, PlayState* play) {
    this->actionFunc(this, play);

    if (this->actor.projectedPos.z < 460.0f) {
        if (this->actor.projectedPos.z > 200.0f) {
            this->actor.shape.shadowAlpha = (460 - (s32)this->actor.projectedPos.z) >> 1;
            this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        } else if (this->actor.projectedPos.z > -10.0f) {
            this->actor.shape.shadowAlpha = 130;
            this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        } else {
            this->actor.shape.shadowDraw = NULL;
        }
    } else {
        this->actor.shape.shadowDraw = NULL;
    }
}

static int ObjSnowball2_CsmcType(Actor_ObjSnowball2* this)
{
    ComboItemOverride o;

    if(!ObjSnowball2_IsShuffled(this))
        return CSMC_NORMAL;

    if(!csmcEnabled())
        return CSMC_MAJOR;

    comboXflagItemOverride(&o, &this->xflag, 0);
    return csmcFromItemCloaked(o.gi, o.cloakGi);
}

void ObjSnowball2_Draw(Actor_ObjSnowball2* this, PlayState* play) {
    int type;
    const Color_RGB8* color;
    u8 gray;

    OPEN_DISPS(play->state.gfxCtx);
    type = ObjSnowball2_CsmcType(this);
    switch(type)
    {
        case CSMC_NORMAL:
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
            break;
        case CSMC_SPIDER:
            gray = 0x90;
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, gray, gray, gray, 255);
            break;
        default:
            color = csmcTypeColor(type);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, color->r, color->g, color->b, 255);
            break;

    }
    CLOSE_DISPS();
    Gfx_DrawDListOpa(play, (void*)object_goroiwa_DL_008B90);
}

ActorProfile Obj_Snowball2_InitVars = {
    ACTOR_OBJ_SNOWBALL2,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GOROIWA,
    sizeof(Actor_ObjSnowball2),
    (ActorFunc)ObjSnowball2_Init,
    (ActorFunc)ObjSnowball2_Destroy,
    (ActorFunc)ObjSnowball2_Update,
    (ActorFunc)ObjSnowball2_Draw,
};

OVL_INFO_ACTOR(ACTOR_OBJ_SNOWBALL2, Obj_Snowball2_InitVars)
