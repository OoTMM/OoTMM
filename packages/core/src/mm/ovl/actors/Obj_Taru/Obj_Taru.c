#include <combo.h>
#include <combo/global.h>
#include <combo/item.h>
#include <combo/custom.h>
#include "Obj_Taru.h"

#define FLAGS 0

#define OBJ_TARU_GET_100(thisx)             ((thisx)->dyna.actor.params & 0x100)
#define OBJ_TARU_GET_3F(thisx)              ((thisx)->dyna.actor.params & 0x3F)
#define OBJ_TARU_GET_SWITCH_FLAG(thisx)     ((thisx)->dyna.actor.params & 0x7F)
#define OBJ_TARU_GET_7F00(thisx)            (((thisx)->dyna.actor.params >> 8) & 0x7F)
#define OBJ_TARU_GET_80(thisx)              ((thisx)->dyna.actor.params & 0x80)

void ObjTaru_Init(Actor_ObjTaru* this, GameState_Play* play);
void ObjTaru_Destroy(Actor_ObjTaru* this, GameState_Play* play);
void ObjTaru_Update(Actor_ObjTaru* this, GameState_Play* play);
void ObjTaru_Draw(Actor_ObjTaru* this, GameState_Play* play);

void func_80B9C07C(Actor_ObjTaru* this, GameState_Play* play);
void func_80B9C174(Actor_ObjTaru* this, GameState_Play* play);
void func_80B9C1A0(Actor_ObjTaru* this, GameState_Play* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x80000508, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 30, 50, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 3300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 200, ICHAIN_STOP),
};

static int ObjTaru_IsShuffled(Actor_ObjTaru* this)
{
    return !!(this->isExtended && !comboXflagsGet(&this->xflag));
}

static int ObjTaru_DropCustom(Actor_ObjTaru* this, GameState_Play* play)
{
    if (!ObjTaru_IsShuffled(this))
        return 0;
    EnItem00_DropCustom(play, &this->dyna.actor.world.pos, &this->xflag);
    return 1;
}

static void ObjTaru_InitXflag(Actor_ObjTaru* this, GameState_Play* play)
{
    ComboItemOverride   o;
    Xflag*              xflag;

    /* Set the extended properties */
    xflag = &this->xflag;
    xflag->sceneId = play->sceneId;
    xflag->setupId = g.sceneSetupId;
    xflag->roomId = this->dyna.actor.room;
    xflag->sliceId = 0;
    xflag->id = this->dyna.actor.actorIndex;

    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));
}

void func_80B9B74C(Actor_ObjTaru* this, GameState_Play* play)
{
    Vec3f* thisPos = &this->dyna.actor.world.pos;
    Vec3f pos;
    Vec3f velocity;
    s16 angle;
    s32 i;

    for (i = 0, angle = 0; i < 0x10; i++, angle += 0x4E20) {
        f32 sin = Math_SinS(angle);
        f32 cos = Math_CosS(angle);
        f32 tempRand = Rand_ZeroOne() * 30.0f;
        s32 phi_s0;

        pos.x = sin * tempRand;
        pos.y = (Rand_ZeroOne() * 10.0f) + 2.0f;
        pos.z = cos * tempRand;

        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 10.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;

        pos.x += thisPos->x;
        pos.y += thisPos->y;
        pos.z += thisPos->z;

        tempRand = Rand_ZeroOne();
        if (tempRand < 0.05f) {
            phi_s0 = 0x60;
        } else if (tempRand < 0.7f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, -200, phi_s0, 28, 2, 0, (Rand_ZeroOne() * 30.0f) + 5.0f, 0, 0,
                             70, KAKERA_COLOR_NONE, OBJECT_KIBAKO2, (Gfx*)0x06001040);
    }
    SpawnSomeDust(play, thisPos, 90.0f, 6, 100, 160, 1);
}

void func_80B9B9C8(Actor_ObjTaru* this, GameState_Play* play) {
    s32 phi_s5;
    s32 i;
    s32 j;
    Vec3f spD8;
    Vec3f spCC;
    f32 sin;
    f32 cos;
    s32 phi_fp;
    Vec3f* thisPos = &this->dyna.actor.world.pos;
    f32 temp_fs0;
    f32 temp_fs1;

    sin = Math_SinS(this->dyna.actor.shape.rot.y);
    cos = Math_CosS(this->dyna.actor.shape.rot.y);

    if (OBJ_TARU_GET_100(this)) {
        phi_fp = 0;
        phi_s5 = 8;
    } else {
        phi_fp = 2;
        phi_s5 = 6;
    }

    for (i = 0; i < 4; i++) {
        for (j = phi_fp; j < phi_s5; j++) {
            temp_fs0 = Rand_CenteredFloat(10.0f) + -105.0f + (j * 30.0f);
            temp_fs1 = Rand_CenteredFloat(4.0f);
            spD8.x = temp_fs0 * cos;
            spD8.y = Rand_CenteredFloat(10.0f) + 15.0f + (i * 30.0f);
            spD8.z = temp_fs0 * sin;
            spCC.x = (spD8.x * 0.05f) + (temp_fs1 * sin);
            spCC.y = Rand_ZeroFloat(5.0f) + 2.0f;
            spCC.z = (spD8.z * 0.05f) + (temp_fs1 * cos);
            spD8.x += thisPos->x;
            spD8.y += thisPos->y;
            spD8.z += thisPos->z;
            EffectSsKakera_Spawn(play, &spD8, &spCC, &spD8, -0x64, 0x20, 0x1C, 4, 0, (Rand_ZeroOne() * 30.0f) + 5.0f, 0,
                                 0, 0x5A, KAKERA_COLOR_NONE, OBJECT_KIBAKO2, (Gfx*)0x06001100);
        }
    }
}

void ObjTaru_DropCollectible(Actor_ObjTaru* this, GameState_Play* play) {
    s32 item;

    if (ObjTaru_DropCustom(this, play))
        return;

    item = Item_CollectibleDropTable(OBJ_TARU_GET_3F(this));
    if (item >= 0) {
        Item_DropCollectible(play, &this->dyna.actor.world.pos, (OBJ_TARU_GET_7F00(this) << 8) | item);
    }
}

void ObjTaru_Init(Actor_ObjTaru* this, GameState_Play* play)
{
    ObjTaru_InitXflag(this, play);
    DynaPolyActor_Init(&this->dyna, 0);
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);

    if (OBJ_TARU_GET_80(this)) {
        if (Flags_GetSwitch(play, OBJ_TARU_GET_SWITCH_FLAG(this))) {
            Actor_Kill(&this->dyna.actor);
        } else {
            DynaPolyActor_LoadMesh(play, &this->dyna, (void*)0x06001cb0);
        }
        if (OBJ_TARU_GET_100(this)) {
            this->dyna.actor.scale.x = 0.2f;
        }
    } else {
        DynaPolyActor_LoadMesh(play, &this->dyna, (void*)0x06000fc8);
        Collider_InitAndSetCylinder(play, &this->collider, &this->dyna.actor, &sCylinderInit);
        Collider_UpdateCylinder(&this->dyna.actor, &this->collider);

        if (Item_CanDropBigFairy(play, OBJ_TARU_GET_3F(this), OBJ_TARU_GET_7F00(this))) {
            this->unk_1AC = 1;
            this->dyna.actor.flags |= ACTOR_FLAG_MM_10;
        }
    }
    this->actionFunc = func_80B9C07C;
}

void ObjTaru_Destroy(Actor_ObjTaru* this, GameState_Play* play)
{
    if (!OBJ_TARU_GET_80(this)) {
        Collider_DestroyCylinder(play, &this->collider);
    }
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

s32 func_80B9BF7C(Actor_ObjTaru* this)
{
    s32 phi_a3 = FALSE;

    if ((!OBJ_TARU_GET_80(this)) && (this->collider.base.acFlags & AC_HIT)) {
        Actor* ac = this->collider.base.ac;

        this->collider.base.acFlags &= ~AC_HIT;
        if (ac != NULL) {
            if (this->collider.elem.acHitElem->atDmgInfo.dmgFlags & 0x80000000) {
                phi_a3 = FALSE;
                if (Math3D_Vec3fDistSq(&this->dyna.actor.world.pos, &ac->world.pos) < SQ(160.0f)) {
                    phi_a3 = TRUE;
                }
            } else if (this->collider.elem.acHitElem->atDmgInfo.dmgFlags & 8) {
                if (Math3D_Vec3fDistSq(&this->dyna.actor.world.pos, &ac->world.pos) < SQ(100.0f)) {
                    phi_a3 = TRUE;
                }
            } else if (this->collider.elem.acHitElem->atDmgInfo.dmgFlags & 0x500) {
                phi_a3 = TRUE;
            }
        }
    } else if (this->dyna.actor.home.rot.z != 0) {
        phi_a3 = TRUE;
    }
    return phi_a3;
}

void func_80B9C07C(Actor_ObjTaru* this, GameState_Play* play) {
    if (func_80B9BF7C(this)) {
        if (!OBJ_TARU_GET_80(this)) {
            func_80B9B74C(this, play);
        } else {
            func_80B9B9C8(this, play);
        }
        SoundSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 0x14, NA_SE_EV_WOODBOX_BREAK);
        if (OBJ_TARU_GET_80(this)) {
            this->actionFunc = func_80B9C1A0;
        } else {
            this->dyna.actor.flags |= ACTOR_FLAG_MM_10;
            DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
            this->dyna.actor.draw = NULL;
            this->actionFunc = func_80B9C174;
        }
    } else {
        if (this->dyna.actor.xzDistToPlayer < 600.0f) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

void func_80B9C174(Actor_ObjTaru* this, GameState_Play* play)
{
    ObjTaru_DropCollectible(this, play);
    Actor_Kill(&this->dyna.actor);
}

void func_80B9C1A0(Actor_ObjTaru* this, GameState_Play* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        CutsceneManager_StartWithPlayerCs(this->dyna.actor.csId, &this->dyna.actor);
        Flags_SetSwitch(play, OBJ_TARU_GET_SWITCH_FLAG(this));
        Actor_Kill(&this->dyna.actor);
        return;
    }

    CutsceneManager_Queue(this->dyna.actor.csId);
}

void ObjTaru_Update(Actor_ObjTaru* this, GameState_Play* play)
{
    if (!OBJ_TARU_GET_80(this))
    {
        if (this->unk_1AC != 0) {
            play->actorCtx.flags |= ACTORCTX_FLAG_3;
        }
    }
    this->actionFunc(this, play);
}

static int ObjTaru_CsmcType(Actor_ObjTaru* this)
{
    ComboItemOverride o;

    if (!ObjTaru_IsShuffled(this))
        return CSMC_NORMAL;

    if (!csmcEnabled())
        return CSMC_MAJOR;

    comboXflagItemOverride(&o, &this->xflag, 0);
    return csmcFromItem(o.gi);
}

static const Gfx sListLoaderTextureDefault[] = {
    gsDPLoadTextureBlock(0x06000670, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, 4, 5, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx sListLoaderTopDefault[] = {
    gsDPLoadTextureBlock(0x06000A70, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, 5, 4, 0, 0),
    gsSPEndDisplayList(),
};

static const Gfx sListLoaderCustom[] = {
    gsDPLoadTextureBlock(0x0a000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 6, 0, 0),
    gsSPEndDisplayList(),
};


static void ObjTaru_DrawBarrel(Actor_ObjTaru* this, GameState_Play* play)
{
    int type;
    u32 textureAddr;
    void* tex;

    type = ObjTaru_CsmcType(this);
    OPEN_DISPS(play->gs.gfx);
    if (type == CSMC_NORMAL)
    {
        gSPSegment(POLY_OPA_DISP++, 0x08, sListLoaderTextureDefault);
        gSPSegment(POLY_OPA_DISP++, 0x09, sListLoaderTopDefault);
    }
    else
    {
        switch (type)
        {
        case CSMC_BOSS_KEY: textureAddr = CUSTOM_POT_BOSSKEY_SIDE_ADDR; break;
        case CSMC_MAJOR: textureAddr = CUSTOM_POT_MAJOR_SIDE_ADDR; break;
        case CSMC_KEY: textureAddr = CUSTOM_POT_KEY_SIDE_ADDR; break;
        case CSMC_SPIDER: textureAddr = CUSTOM_POT_SPIDER_SIDE_ADDR; break;
        case CSMC_FAIRY: textureAddr = CUSTOM_POT_FAIRY_SIDE_ADDR; break;
        case CSMC_HEART: textureAddr = CUSTOM_POT_HEART_SIDE_ADDR; break;
        case CSMC_SOUL: textureAddr = CUSTOM_POT_SOUL_SIDE_ADDR; break;
        case CSMC_MAP_COMPASS: textureAddr = CUSTOM_POT_MAP_SIDE_ADDR; break;
        default: UNREACHABLE(); break;
        }

        tex = comboCacheGetFile(textureAddr);
        if (!tex)
            return;
        gSPSegment(POLY_OPA_DISP++, 0x08, sListLoaderCustom);
        gSPSegment(POLY_OPA_DISP++, 0x09, sListLoaderCustom);
        gSPSegment(POLY_OPA_DISP++, 0x0a, tex);
    }
    CLOSE_DISPS();

    Gfx_DrawDListOpa(play, (Gfx*)0x06000420);
}

static void ObjTaru_DrawPlank(Actor_ObjTaru* this, GameState_Play* play)
{
    Gfx_DrawDListOpa(play, (Gfx*)0x06001140);
}

void ObjTaru_Draw(Actor_ObjTaru* this, GameState_Play* play)
{
    if (OBJ_TARU_GET_80(this))
        ObjTaru_DrawPlank(this, play);
    else
        ObjTaru_DrawBarrel(this, play);
}

ActorProfile ObjTaru_InitVars = {
    AC_OBJ_TARU,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_TARU,
    sizeof(Actor_ObjTaru),
    (ActorFunc)ObjTaru_Init,
    (ActorFunc)ObjTaru_Destroy,
    (ActorFunc)ObjTaru_Update,
    (ActorFunc)ObjTaru_Draw,
};

OVL_ACTOR_INFO(AC_OBJ_TARU, ObjTaru_InitVars);
