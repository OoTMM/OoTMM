#include <combo.h>
#include <combo/item.h>
#include <combo/global.h>
#include <combo/custom.h>
#include "Obj_Flowerpot.h"

#define FLAGS 0

#define SLICE_POT   0
#define SLICE_GRASS 1

#define ENOBJFLOWERPOT_GET_3F(thisx) ((thisx)->actor.params & 0x3F)
#define ENOBJFLOWERPOT_GET_7F00(thisx) (((thisx)->actor.params >> 8) & 0x7F)

void ObjFlowerpot_Init(Actor_ObjFlowerpot* this, GameState_Play* play);
void ObjFlowerpot_Destroy(Actor_ObjFlowerpot* this, GameState_Play* play);
void ObjFlowerpot_Update(Actor_ObjFlowerpot* this, GameState_Play* play);
void ObjFlowerpot_Draw(Actor_ObjFlowerpot* this, GameState_Play* play);

void func_80A1C818(Actor_ObjFlowerpot* this);
void func_80A1C838(Actor_ObjFlowerpot* this, GameState_Play* play);
void func_80A1CBF8(Actor_ObjFlowerpot* this);
void func_80A1CC0C(Actor_ObjFlowerpot* this, GameState_Play* play);
void func_80A1CD10(Actor_ObjFlowerpot* this);
void func_80A1CEF4(Actor_ObjFlowerpot* this, GameState_Play* play);

u32 D_80A1D830;
MtxF D_80A1D838[8];
s16 D_80A1DA38;
s16 D_80A1DA3A;
s16 D_80A1DA3C;
s16 D_80A1DA3E;
s16 D_80A1DA40;

static ColliderJntSphElementInit sJntSphElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00400000, 0x00, 0x02 },
            { 0x05CBFFBE, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 100, 0 }, 12 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x0580C71C, 0x00, 0x00 },
            TOUCH_NONE | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 1, { { 0, 300, 0 }, 12 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    ARRAY_SIZE(sJntSphElementsInit),
    sJntSphElementsInit,
};

s16 D_80A1D3F8 = 0;

s16 D_80A1D3FC = 0;

s16 D_80A1D400 = 0;

u8 D_80A1D404 = TRUE;

Vec3f D_80A1D408 = { 0.0f, 20.0f, 0.0f };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_CONTINUE), ICHAIN_F32_DIV1000(minVelocityY, -20000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),   ICHAIN_F32(uncullZoneForward, 1600, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),   ICHAIN_F32(uncullZoneDownward, 100, ICHAIN_STOP),
};

static void ObjFlowerpot_Xflag(Xflag* dst, int slice, Actor_ObjFlowerpot* this)
{
    memcpy(dst, &this->xflags, sizeof(Xflag));
    dst->sliceId = slice;
}

static int ObjFlowerpot_IsShuffled(Actor_ObjFlowerpot* this, int slice)
{
    Xflag xflags;

    ObjFlowerpot_Xflag(&xflags, slice, this);
    return !!((this->isExtendedFlags & (1 << slice)) && (!comboXflagsGet(&xflags)));
}

static s16 ObjFlowerpot_ShuffledItem(Actor_ObjFlowerpot* this, int slice)
{
    ComboItemOverride o;
    Xflag xf;

    ObjFlowerpot_Xflag(&xf, slice, this);
    comboXflagItemOverride(&o, &xf, 0);
    if (o.gi && !comboXflagsGet(&xf))
        return o.gi;
    return 0;
}

static void ObjFlowerpot_Alias(Xflag* xf)
{
    switch (xf->sceneId)
    {
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
        xf->sceneId = SCE_MM_MOUNTAIN_VILLAGE_WINTER;
        xf->id -= 6;
        break;
    }
}

static void ObjFlowerpot_InitXflag(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    Xflag* xflag;

    xflag = &this->xflags;
    xflag->sceneId = play->sceneId;
    xflag->setupId = g.sceneSetupId;
    xflag->roomId = this->actor.room;
    xflag->sliceId = 0;
    xflag->id = this->actor.actorIndex;

    ObjFlowerpot_Alias(xflag);

    this->isExtendedFlags = 0;
    for (int i = 0; i < 2; ++i)
    {
        if (ObjFlowerpot_ShuffledItem(this, i))
            this->isExtendedFlags |= 1 << i;
    }
}

static int ObjFlowerpot_DropCustom(Actor_ObjFlowerpot* this, GameState_Play* play, int slice)
{
    Xflag xf;

    if (!ObjFlowerpot_IsShuffled(this, slice))
        return 0;
    ObjFlowerpot_Xflag(&xf, slice, this);
    EnItem00_DropCustom(play, &this->actor.world.pos, &xf);
    return 1;
}

void func_80A1B3D0(void)
{
    s32 i;
    f32 spB4;
    f32* ptr;
    f32 spAC;
    f32 spA8;
    f32 tempf1;
    f32 tempf2;
    f32 tempf3;
    f32 tempf4;
    f32 tempf5;
    f32 sp74[8];
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    f32 temp_f28;
    f32 temp_f0;
    f32 temp_f30;

    D_80A1DA38 += 70;
    D_80A1DA3A += 300;
    D_80A1DA3C += 700;
    D_80A1DA3E += 1300;
    D_80A1DA40 += 8900;

    temp_f28 = Math_SinS(D_80A1DA38);
    spB4 = Math_SinS(D_80A1DA3A);
    temp_f30 = Math_SinS(D_80A1DA3C);
    spAC = Math_SinS(D_80A1DA3E);
    spA8 = Math_SinS(D_80A1DA40);
    temp_f26 = Math_CosS(D_80A1DA38);
    temp_f20 = Math_CosS(D_80A1DA3A);
    temp_f22 = Math_CosS(D_80A1DA3C);
    temp_f24 = Math_CosS(D_80A1DA3E);
    temp_f0 = Math_CosS(D_80A1DA40);

    sp74[0] = (temp_f28 - temp_f20) * temp_f30 * temp_f26 * temp_f28 * 0.0012f;
    sp74[1] = (spB4 - temp_f22) * spAC * temp_f20 * temp_f28 * 0.0012f;
    sp74[2] = (temp_f30 - temp_f24) * temp_f22 * temp_f28 * temp_f26 * 0.0012f;
    sp74[3] = (spAC - temp_f20) * temp_f24 * spB4 * temp_f26 * 0.0012f;
    sp74[4] = (temp_f28 - temp_f22) * temp_f28 * spB4 * spA8 * 0.0013f;
    sp74[5] = (spB4 - temp_f24) * temp_f30 * spAC * spA8 * 0.0013f;
    sp74[6] = (temp_f30 - temp_f26) * temp_f26 * temp_f20 * temp_f0 * 0.0013f;
    sp74[7] = (spAC - temp_f20) * temp_f22 * temp_f24 * temp_f0 * 0.0013f;

    for (i = 0; i < ARRAY_SIZE(D_80A1D838); i++) {
        ptr = &D_80A1D838[i].xx;

        tempf1 = sp74[(i + 0) & 7];
        tempf2 = sp74[(i + 1) & 7];
        tempf3 = sp74[(i + 2) & 7];
        tempf4 = sp74[(i + 3) & 7];
        tempf5 = sp74[(i + 4) & 7];

        ptr[0] = sp74[1] * 0.2f;
        ptr[1] = tempf1;
        ptr[2] = tempf2;
        ptr[3] = 0.0f;

        ptr[4] = tempf3;
        ptr[5] = sp74[0];
        ptr[6] = tempf3;
        ptr[7] = 0.0f;

        ptr[8] = tempf4;
        ptr[9] = tempf5;
        ptr[10] = sp74[3] * 0.2f;
        ptr[11] = 0.0f;

        ptr[12] = 0.0f;
        ptr[13] = 0.0f;
        ptr[14] = 0.0f;
        ptr[15] = 0.0f;
    }
}

void func_80A1B840(MtxF* matrix) {
    MtxF* temp = Matrix_GetCurrent();
    f32* tmp = &temp->xx;
    f32* tmp2 = &matrix->xx;
    s32 i;

    for (i = 0; i < 16; i++) {
        *tmp++ += *tmp2++;
    }
}

void ObjFlowerpot_PotSpawnCollectible(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    ObjFlowerpot_DropCustom(this, play, SLICE_POT);
}

void ObjFlowerpot_GrassSpawnCollectible(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    if (ObjFlowerpot_DropCustom(this, play, SLICE_GRASS))
        return;

    if (!(this->unk_1EA & 4)) {
        s32 temp_v0 = Item_CollectibleDropTable(ENOBJFLOWERPOT_GET_3F(this));

        if (temp_v0 >= 0) {
            s32 params = ENOBJFLOWERPOT_GET_7F00(this);

            Item_DropCollectible(play, &this->actor.world.pos, temp_v0 | (params << 8));
            this->unk_1EA |= 4;
        }
    }
}

void func_80A1B994(Actor_ObjFlowerpot* this, GameState_Play* play) {
    SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
}

void func_80A1B9CC(Actor_ObjFlowerpot* this, GameState_Play* play) {
    SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_PLANT_BROKEN);
}

void func_80A1BA04(Actor_ObjFlowerpot* this, Vec3f* arg1) {
    Matrix_SetTranslateRotateYXZ(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z,
                                 &this->actor.shape.rot);
    Matrix_MultVec3f(&D_80A1D408, arg1);
}

void ObjFlowerpot_Break1(Actor_ObjFlowerpot* this, GameState_Play* play) {
    s32 i;
    Vec3f spD0;
    Vec3f spC4;
    Vec3f spB8;
    f32 temp_f0;
    f32 temp_f20;
    s16 phi_s3;
    s32 phi_s0;
    s32 phi_s1;

    spD0.x = this->actor.world.pos.x;
    spD0.y = this->actor.world.pos.y;
    spD0.z = this->actor.world.pos.z;

    for (i = 0, phi_s3 = 0; i < 18; i++, phi_s3 += 0xE38) {
        temp_f20 = (Rand_ZeroOne() * 8.0f) + 2.0f;

        spC4.x = Math_SinS((s32)(Rand_ZeroOne() * 3640.0f) + phi_s3) * temp_f20;
        spC4.y = Rand_ZeroOne() * 30.0f;
        spC4.z = Math_CosS((s32)(Rand_ZeroOne() * 3640.0f) + phi_s3) * temp_f20;

        spB8.x = spC4.x * 0.27f;
        spB8.y = (Rand_ZeroOne() * 6.0f) + 2.6f + ((30.0f - spC4.y) * 0.2f);
        spB8.z = spC4.z * 0.27f;

        Math_Vec3f_Sum(&spC4, &spD0, &spC4);

        temp_f0 = Rand_ZeroOne();
        if (temp_f0 < 0.2f) {
            phi_s0 = 32;
            phi_s1 = 0;
        } else if (temp_f0 < 0.6f) {
            phi_s0 = 65;
            phi_s1 = 1;
        } else {
            phi_s0 = 64;
            phi_s1 = 0;
        }

        EffectSsKakera_Spawn(play, &spC4, &spB8, &spC4, -600, phi_s0, 30, 0, 0, (Rand_ZeroOne() * 12.0f) + 16.6f, phi_s1, 0, 35, -1, OBJECT_FLOWERPOT, (Gfx*)0x060015B0);
    }

    spD0.y += 20.0f;

    SpawnSomeDust(play, &spD0, 30.0f, 2, 20, 50, 1);
    SpawnSomeDust(play, &spD0, 30.0f, 2, 10, 80, 1);
    SpawnSomeDust(play, &spD0, 30.0f, 1, 10, 40, 1);
}

void ObjFlowerpot_Break2(Actor_ObjFlowerpot* this, GameState_Play* play) {
    s32 i;
    Vec3f spC8;
    Vec3f spBC;
    Vec3f spB0;
    f32 temp_f20;
    s16 phi_s1;
    s32 phi_s0;

    spC8.x = this->actor.world.pos.x;
    spC8.y = this->actor.world.pos.y;
    spC8.z = this->actor.world.pos.z;

    for (i = 0, phi_s1 = 0; i < 14; i++, phi_s1 += 0x1249) {
        temp_f20 = (Rand_ZeroOne() * 8.0f) + 2.0f;

        spBC.x = Math_SinS((s32)(Rand_ZeroOne() * 4681.0f) + phi_s1) * temp_f20;
        spBC.y = Rand_ZeroOne() * 20.0f;
        spBC.z = Math_CosS((s32)(Rand_ZeroOne() * 4681.0f) + phi_s1) * temp_f20;

        spB0.x = spBC.x * 0.17f;
        spB0.y = (Rand_ZeroOne() * 5.0f) + 1.4f;
        spB0.z = spBC.z * 0.17f;

        Math_Vec3f_Sum(&spBC, &spC8, &spBC);

        if (Rand_ZeroOne() < 0.2f) {
            phi_s0 = 64;
        } else {
            phi_s0 = 32;
        }

        EffectSsKakera_Spawn(play, &spBC, &spB0, &spBC, -240, phi_s0, 40, 0, 0, (Rand_ZeroOne() * 20.0f) + 10.6f, 0, 0,
            42, -1, OBJECT_FLOWERPOT, (Gfx*)0x060015B0);
    }

    spBC.y = this->actor.world.pos.y + this->actor.depthInWater;

    for (phi_s1 = 0, i = 0; i < 4; i++, phi_s1 += 0x4000) {
        spBC.x = this->actor.world.pos.x + (Math_SinS((s32)(Rand_ZeroOne() * 7200.0f) + phi_s1) * 15.0f);
        spBC.z = this->actor.world.pos.z + (Math_CosS((s32)(Rand_ZeroOne() * 7200.0f) + phi_s1) * 15.0f);
        EffectSsGSplash_Spawn(play, &spBC, NULL, NULL, 0, 200);
    }

    spBC.x = this->actor.world.pos.x;
    spBC.z = this->actor.world.pos.z;
    EffectSsGSplash_Spawn(play, &spBC, NULL, NULL, 0, 350);
    EffectSsGRipple_Spawn(play, &spBC, 150, 650, 0);
}

void func_80A1C0FC(Actor_ObjFlowerpot* this, GameState_Play* play) {
    Vec3f spC4;
    Vec3f spB8;
    Vec3f spAC;
    f32 temp_f20;
    s16 phi_s0;
    s32 i;

    func_80A1BA04(this, &spC4);

    for (i = 0, phi_s0 = 0; i < 10; i++, phi_s0 += 0x1999) {
        temp_f20 = (Rand_ZeroOne() * 18.0f) + 2.0f;

        spB8.x = Math_SinS((s32)(Rand_ZeroOne() * 6553.0f) + phi_s0) * temp_f20;
        spB8.y = Rand_ZeroOne() * 15.0f;
        spB8.z = Math_CosS((s32)(Rand_ZeroOne() * 6553.0f) + phi_s0) * temp_f20;

        spAC.x = spB8.x * 0.23f;
        spAC.y = (Rand_ZeroOne() * 8.0f) + 1.5f;
        spAC.z = spB8.z * 0.23f;

        Math_Vec3f_Sum(&spB8, &spC4, &spB8);
        EffectSsKakera_Spawn(play, &spB8, &spAC, &spB8, -100, 64, 40, 0, 0, (Rand_ZeroOne() * 16.0f) + 14.0f, 0, 0, 80,
                             -1, OBJECT_FLOWERPOT, (Gfx*)0x060014F0);
    }
}

void func_80A1C328(Actor_ObjFlowerpot* this, GameState_Play* play) {
    Vec3f spC4;
    Vec3f spB8;
    Vec3f spAC;
    f32 temp_f20;
    s16 phi_s0;
    s32 i;

    func_80A1BA04(this, &spC4);

    for (i = 0, phi_s0 = 0; i < 10; i++, phi_s0 += 0x1999) {
        temp_f20 = (Rand_ZeroOne() * 18.0f) + 2.0f;

        spB8.x = Math_SinS((s32)(Rand_ZeroOne() * 6553.0f) + phi_s0) * temp_f20;
        spB8.y = Rand_ZeroOne() * 15.0f;
        spB8.z = Math_CosS((s32)(Rand_ZeroOne() * 6553.0f) + phi_s0) * temp_f20;

        spAC.x = spB8.x * 0.18f;
        spAC.y = (Rand_ZeroOne() * 4.0f) + 1.2f;
        spAC.z = spB8.z * 0.18f;

        Math_Vec3f_Sum(&spB8, &spC4, &spB8);
        EffectSsKakera_Spawn(play, &spB8, &spAC, &spB8, -80, 64, 44, 0, 0, (Rand_ZeroOne() * 16.0f) + 14.0f, 0, 0, 80,
                             -1, OBJECT_FLOWERPOT, (Gfx*)0x060014F0);
    }
}

void func_80A1C554(Actor_ObjFlowerpot* this) {
    if ((this->actor.projectedPos.z < 455.0f) && (this->actor.projectedPos.z > -10.0f)) {
        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        this->actor.shape.shadowScale = 1.9f;
        if (this->actor.projectedPos.z < 255.0f) {
            this->actor.shape.shadowAlpha = 200;
        } else {
            this->actor.shape.shadowAlpha = 455 - (s32)this->actor.projectedPos.z;
        }
    } else {
        this->actor.shape.shadowDraw = NULL;
    }
}

void func_80A1C5E8(Actor_ObjFlowerpot* this, GameState_Play* play) {
    Actor_UpdateBgCheckInfo(play, &this->actor, 18.0f, 15.0f, 0.0f,
                            UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR | UPDBGCHECKINFO_FLAG_MM_40);
}

void func_80A1C62C(Actor_ObjFlowerpot* this, GameState_Play* play) {
    if (!(this->unk_1EA & 4) && (play->roomCtx.curRoom.num != this->unk_1EC)) {
        this->unk_1EA |= 4;
    }
}

void ObjFlowerpot_Init(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    ObjFlowerpot_InitXflag(this, play);
    Actor_ProcessInitChain(&this->actor, sInitChain);

    if (this->actor.shape.rot.y == 0) {
        this->actor.shape.rot.y = this->actor.world.rot.y = Rand_Next() >> 0x10;
    }

    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    Matrix_SetTranslateRotateYXZ(this->actor.home.pos.x, this->actor.home.pos.y, this->actor.home.pos.z,
                                 &this->actor.shape.rot);
    Matrix_Scale(0.1f, 0.1f, 0.1f, MAT_MUL);
    Collider_UpdateSpheres(0, &this->collider);
    Collider_UpdateSpheres(1, &this->collider);

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->unk_1EC = this->actor.room;
    func_80A1C818(this);

    if (D_80A1D404) {
        D_80A1DA38 = Rand_Next() >> 0x10;
        D_80A1DA3A = Rand_Next() >> 0x10;
        D_80A1DA3C = Rand_Next() >> 0x10;
        D_80A1DA3E = Rand_Next() >> 0x10;
        D_80A1DA40 = Rand_Next() >> 0x10;
        D_80A1D404 = FALSE;
        func_80A1B3D0();
        D_80A1D830 = play->gameplayFrames;
    }
    this->unk_1EB = D_80A1D400 & 7;
    D_80A1D400++;
}

void ObjFlowerpot_Destroy(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    Collider_DestroyJntSph(play, &this->collider);
}

void func_80A1C818(Actor_ObjFlowerpot* this)
{
    this->actionFunc = func_80A1C838;
    this->unk_1EA |= 1;
}

void func_80A1C838(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    if (Actor_HasParent(&this->actor, play))
    {
        func_80A1CBF8(this);
        this->actor.room = -1;
        this->actor.colChkInfo.mass = 180;
        this->actor.flags |= ACTOR_FLAG_MM_10;
        if (Item_CollectibleDropTable2(ENOBJFLOWERPOT_GET_3F(this)))
        {
            ObjFlowerpot_GrassSpawnCollectible(this, play);
        }

        //! @bug: This function should only pass Player*: it uses *(this + 0x153), which is meant to be
        //! player->currentMask, but in this case is garbage in the collider
        Player_PlaySfx((void*)&this->actor, NA_SE_PL_PULL_UP_POT);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER) && (this->actor.depthInWater > 19.0f)) {
        if (!(this->unk_1EA & 2)) {
            ObjFlowerpot_GrassSpawnCollectible(this, play);
            func_80A1C328(this, play);
            func_80A1B9CC(this, play);
            this->unk_1EA |= 2;
        }
        ObjFlowerpot_PotSpawnCollectible(this, play);
        ObjFlowerpot_Break2(this, play);
        func_80A1B994(this, play);
        Actor_Kill(&this->actor);
    } else if ((this->collider.elements[0].elem.bumperFlags & BUMP_HIT) &&
               (this->collider.elements[0].elem.acHitElem->atDmgInfo.dmgFlags & 0x058BFFBC)) {
        if (!(this->unk_1EA & 2)) {
            ObjFlowerpot_GrassSpawnCollectible(this, play);
            func_80A1C0FC(this, play);
            func_80A1B9CC(this, play);
            this->unk_1EA |= 2;
        }
        ObjFlowerpot_PotSpawnCollectible(this, play);
        ObjFlowerpot_Break1(this, play);
        func_80A1B994(this, play);
        Actor_Kill(&this->actor);
    } else {
        if (this->collider.elements[1].elem.bumperFlags & BUMP_HIT) {
            if (!(this->unk_1EA & 2)) {
                this->unk_1EA |= 2;
                this->collider.elements[1].elem.bumperFlags &= ~BUMP_ON;
                func_80A1C0FC(this, play);
                ObjFlowerpot_GrassSpawnCollectible(this, play);
                func_80A1B9CC(this, play);
            }
        }

        if (this->unk_1EA & 1) {
            Actor_MoveWithGravity(&this->actor);
            func_80A1C5E8(this, play);
            if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
                if (this->actor.colChkInfo.mass == MASS_IMMOVABLE) {
                    if (DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId) == NULL) {
                        this->actor.flags &= ~ACTOR_FLAG_MM_10;
                        this->unk_1EA &= ~0x1;
                    }
                } else if (Math3D_Vec3fDistSq(&this->actor.world.pos, &this->actor.prevPos) < 0.01f) {
                    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
                }
            }
        }

        if (!(this->collider.base.ocFlags1 & OC1_TYPE_PLAYER) && (this->actor.xzDistToPlayer > 28.0f)) {
            this->collider.base.ocFlags1 |= OC1_TYPE_PLAYER;
        }

        this->collider.base.acFlags &= ~AC_HIT;

        if (this->actor.xzDistToPlayer < 600.0f) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);

            if (this->actor.xzDistToPlayer < 180.0f) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

                if (this->actor.xzDistToPlayer < 100.0f) {
                    s16 temp_v0_3 = this->actor.yawTowardsPlayer - GET_PLAYER(play)->actor.world.rot.y;

                    if (ABS_ALT(temp_v0_3) >= 0x5556) {
                        Actor_OfferGetItem(&this->actor, play, GI_NONE, 36.0f, 30.0f);
                    }
                }
            }
        }
    }
}

void func_80A1CBF8(Actor_ObjFlowerpot* this)
{
    this->actionFunc = func_80A1CC0C;
}

void func_80A1CC0C(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    func_80A1C62C(this, play);

    if (Actor_HasNoParent(&this->actor, play)) {
        this->actor.room = play->roomCtx.curRoom.num;
        if (fabsf(this->actor.speed) < 0.1f) {
            func_80A1C818(this);
            Player_PlaySfx(GET_PLAYER(play), NA_SE_PL_PUT_DOWN_POT);
            this->collider.base.ocFlags1 &= ~OC1_TYPE_PLAYER;
        } else {
            Actor_MoveWithGravity(&this->actor);
            func_80A1CD10(this);
        }
        func_80A1C5E8(this, play);
    } else {
        Vec3f sp30;
        s32 sp2C;

        sp30.x = this->actor.world.pos.x;
        sp30.y = this->actor.world.pos.y + 20.0f;
        sp30.z = this->actor.world.pos.z;
        this->actor.floorHeight = BgCheck_EntityRaycastDown4(&play->colCtx, &this->actor.floorPoly, &sp2C, &this->actor, &sp30);
    }
}

void func_80A1CD10(Actor_ObjFlowerpot* this) {
    f32 sp1C;

    this->actionFunc = func_80A1CEF4;
    this->unk_1E8 = 64;

    if (this->unk_1EA & 2) {
        sp1C = (Rand_ZeroOne() * 1.2f) - 1.1f;
        if (sp1C < -0.9f) {
            sp1C = -0.9f;
        }
        D_80A1D3F8 = sp1C * 8000.0f;
        D_80A1D3FC = ((Rand_ZeroOne() - 0.5f) * 3800.0f) * (fabsf(sp1C) + 0.1f);
        this->actor.shape.yOffset = -71.5f;
        this->actor.world.pos.y += 7.15f;
    } else {
        sp1C = (Rand_ZeroOne() - 0.78f) * 1.3f;
        if (sp1C < -0.78f) {
            sp1C = -0.78f;
        } else if (sp1C > 0.22000003f) {
            sp1C = 0.22000003f;
        }
        D_80A1D3F8 = sp1C * 6200.0f;
        D_80A1D3FC = (Rand_ZeroOne() - 0.5f) * 4200.0f;
        this->actor.shape.yOffset = -110.0f;
        this->actor.world.pos.y += 11.0f;
    }
}

void func_80A1CEF4(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    s32 sp28 = this->collider.elements[0].elem.toucherFlags & TOUCH_HIT;

    if (sp28) {
        this->collider.elements[0].elem.toucherFlags &= ~TOUCH_ON;
    }

    if (this->unk_1E8 > 0) {
        this->unk_1E8--;
    }

    func_80A1C62C(this, play);

    if ((this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_WALL)) || sp28 ||
        (this->unk_1E8 <= 0)) {
        if (!(this->unk_1EA & 2)) {
            ObjFlowerpot_GrassSpawnCollectible(this, play);
            func_80A1C0FC(this, play);
            func_80A1B9CC(this, play);
            this->unk_1EA |= 2;
        }
        ObjFlowerpot_PotSpawnCollectible(this, play);
        ObjFlowerpot_Break1(this, play);
        func_80A1B994(this, play);
        Actor_Kill(&this->actor);
        return;
    }

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) {
        if (!(this->unk_1EA & 2)) {
            ObjFlowerpot_GrassSpawnCollectible(this, play);
            func_80A1C328(this, play);
            func_80A1B9CC(this, play);
            this->unk_1EA |= 2;
        }
        ObjFlowerpot_PotSpawnCollectible(this, play);
        ObjFlowerpot_Break2(this, play);
        func_80A1B994(this, play);
        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EV_DIVE_INTO_WATER_L);
        Actor_Kill(&this->actor);
        return;
    }

    Actor_MoveWithGravity(&this->actor);

    if (!(this->unk_1EA & 2)) {
        D_80A1D3F8 += TRUNCF_BINANG(this->actor.shape.rot.x * -0.06f);
    } else {
        Math_StepToS(&D_80A1D3F8, 0, 80);
        Math_StepToS(&D_80A1D3FC, 0, 20);
    }

    this->actor.shape.rot.x += D_80A1D3F8;
    this->actor.shape.rot.y += D_80A1D3FC;

    func_80A1C5E8(this, play);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
}

void ObjFlowerpot_Update(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    this->actionFunc(this, play);
    func_80A1C554(this);

    if ((D_80A1D830 != play->gameplayFrames) && (play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_0)) {
        func_80A1B3D0();
        D_80A1D830 = play->gameplayFrames;
    }
}

static int ObjFlowerpot_CsmcType(Actor_ObjFlowerpot* this, int slice)
{
    s16 gi;

    gi = ObjFlowerpot_ShuffledItem(this, slice);
    if (gi == GI_NONE)
        return CSMC_NORMAL;
    if (!csmcEnabled())
        return CSMC_MAJOR;
    return csmcFromItem(gi);
}

static const Gfx sListLoaderPotDefault[] = {
    gsDPLoadTextureBlock(0x06000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 4, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx sListLoaderPotCustom[] = {
    gsDPLoadTextureBlock(0x09000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};

static void ObjFlowerpot_DrawPot(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    int type;
    u32 customTextureAddr;
    void* customTexture;

    OPEN_DISPS(play->gs.gfx);
    type = ObjFlowerpot_CsmcType(this, SLICE_POT);
    if (type == CSMC_NORMAL)
    {
        gSPSegment(POLY_OPA_DISP++, 0x08, sListLoaderPotDefault);
    }
    else
    {
        switch (type)
        {
        case CSMC_BOSS_KEY: customTextureAddr = CUSTOM_POT_BOSSKEY_SIDE_ADDR; break;
        case CSMC_MAJOR: customTextureAddr = CUSTOM_POT_MAJOR_SIDE_ADDR; break;
        case CSMC_KEY: customTextureAddr = CUSTOM_POT_KEY_SIDE_ADDR; break;
        case CSMC_SPIDER: customTextureAddr = CUSTOM_POT_SPIDER_SIDE_ADDR; break;
        case CSMC_FAIRY: customTextureAddr = CUSTOM_POT_FAIRY_SIDE_ADDR; break;
        case CSMC_HEART: customTextureAddr = CUSTOM_POT_HEART_SIDE_ADDR; break;
        case CSMC_SOUL: customTextureAddr = CUSTOM_POT_SOUL_SIDE_ADDR; break;
        case CSMC_MAP_COMPASS: customTextureAddr = CUSTOM_POT_MAP_SIDE_ADDR; break;
        default: UNREACHABLE(); break;
        }

        customTexture = comboCacheGetFile(customTextureAddr);
        if (!customTexture)
            return;
        gSPSegment(POLY_OPA_DISP++, 0x08, sListLoaderPotCustom);
        gSPSegment(POLY_OPA_DISP++, 0x09, customTexture);
    }
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, 0x060012e0);
    CLOSE_DISPS();
}

static const Gfx sListLoaderGrassDefault[] = {
    gsDPLoadTextureBlock(0x06000800, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx sListLoaderGrassCustom[] = {
    gsDPLoadTextureBlock(0x09000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, 5, 5, 0, 0),
    gsSPEndDisplayList(),
};

static void ObjFlowerpot_DrawGrass(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    int type;
    void* customTexture;
    const Color_RGB8* color;

    OPEN_DISPS(play->gs.gfx);
    type = ObjFlowerpot_CsmcType(this, SLICE_GRASS);
    if (type == CSMC_NORMAL)
    {
        gSPSegment(POLY_OPA_DISP++, 0x08, sListLoaderGrassDefault);
    }
    else
    {
        customTexture = comboCacheGetFile(CUSTOM_GRASS_ALT_ADDR);
        if (!customTexture)
            return;
        color = csmcTypeColor(type);
        gSPSegment(POLY_OPA_DISP++, 0x08, sListLoaderGrassCustom);
        gSPSegment(POLY_OPA_DISP++, 0x09, customTexture);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, color->r, color->g, color->b, 255);
    }

    if ((play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_0) && (this->actionFunc == func_80A1C838))
    {
        if ((this->actor.projectedPos.z > -150.0f) && (this->actor.projectedPos.z < 400.0f))
        {
            func_80A1B840(&D_80A1D838[this->unk_1EB]);
            gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        }
    }
    gSPDisplayList(POLY_OPA_DISP++, 0x06001408);
    CLOSE_DISPS();
}

void ObjFlowerpot_Draw(Actor_ObjFlowerpot* this, GameState_Play* play)
{
    Gfx_SetupDL25_Opa(play->gs.gfx);
    ObjFlowerpot_DrawPot(this, play);

    if ((this->actionFunc != func_80A1C838) || (this->unk_1EA & 1)) {
        Collider_UpdateSpheres(0, &this->collider);
        if (!(this->unk_1EA & 2)) {
            Collider_UpdateSpheres(1, &this->collider);
        }
    }

    if (!(this->unk_1EA & 2))
        ObjFlowerpot_DrawGrass(this, play);
}

ActorProfile ObjFlowerpot_InitVars =
{
    AC_OBJ_FLOWERPOT,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_FLOWERPOT,
    sizeof(Actor_ObjFlowerpot),
    (ActorFunc)ObjFlowerpot_Init,
    (ActorFunc)ObjFlowerpot_Destroy,
    (ActorFunc)ObjFlowerpot_Update,
    (ActorFunc)ObjFlowerpot_Draw,
};

OVL_ACTOR_INFO(AC_OBJ_FLOWERPOT, ObjFlowerpot_InitVars);
