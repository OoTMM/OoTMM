#include <actors/En_Kusa/En_Kusa.h>
#include <assets/oot/objects/gameplay_keep.h>
#include <assets/oot/objects/gameplay_field_keep.h>
#include <assets/oot/objects/object_kusa.h>

#define INSECT_TYPE_SPAWNED 1
#define FLAGS (ACTOR_FLAG_OOT_4 | ACTOR_FLAG_THROW_ONLY)

void EnKusa_Init(Actor* thisx, PlayState* play);
void EnKusa_Destroy(Actor* thisx, PlayState* play2);
void EnKusa_Update(Actor* thisx, PlayState* play);
void EnKusa_Draw(Actor* thisx, PlayState* play);

void EnKusa_SetupLiftedUp(EnKusa* this);
void EnKusa_SetupWaitForObject(EnKusa* this);
void EnKusa_SetupMain(EnKusa* this);
void EnKusa_SetupFall(EnKusa* this);
void EnKusa_SetupCut(EnKusa* this);
void EnKusa_SetupUprootedWaitRegrow(EnKusa* this);
void EnKusa_SetupRegrow(EnKusa* this);

void EnKusa_Fall(EnKusa* this, PlayState* play);
void EnKusa_WaitForObject(EnKusa* this, PlayState* play);
void EnKusa_Main(EnKusa* this, PlayState* play);
void EnKusa_LiftedUp(EnKusa* this, PlayState* play);
void EnKusa_CutWaitRegrow(EnKusa* this, PlayState* play);
void EnKusa_DoNothing(EnKusa* this, PlayState* play);
void EnKusa_UprootedWaitRegrow(EnKusa* this, PlayState* play);
void EnKusa_Regrow(EnKusa* this, PlayState* play);

static s16 rotSpeedXtarget = 0;
static s16 rotSpeedX = 0;
static s16 rotSpeedYtarget = 0;
static s16 rotSpeedY = 0;

ActorProfile En_Kusa_Profile = {
    /**/ ACTOR_EN_KUSA,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(EnKusa),
    /**/ EnKusa_Init,
    /**/ EnKusa_Destroy,
    /**/ EnKusa_Update,
    /**/ NULL,
};

OVL_INFO_ACTOR(ACTOR_EN_KUSA, En_Kusa_Profile);

static s16 sObjectIds[] = { OBJECT_GAMEPLAY_FIELD_KEEP, OBJECT_KUSA, OBJECT_KUSA };

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER | OC1_TYPE_2,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x4FC00758, 0x00, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 12, 44, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 30, MASS_IMMOVABLE };

static Vec3f sUnitDirections[] = {
    { 0.0f, 0.7071f, 0.7071f },
    { 0.7071f, 0.7071f, 0.0f },
    { 0.0f, 0.7071f, -0.7071f },
    { -0.7071f, 0.7071f, 0.0f },
};

static s16 sFragmentScales[] = { 108, 102, 96, 84, 66, 55, 42, 38 };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 400, ICHAIN_CONTINUE),         ICHAIN_F32_DIV1000(gravity, -3200, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(minVelocityY, -17000, ICHAIN_CONTINUE), ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),         ICHAIN_F32(uncullZoneDownward, 120, ICHAIN_STOP),
};

void EnKusa_Aliases(Xflag* xflag)
{
    switch (xflag->sceneId)
    {
    case SCE_OOT_MARKET_CHILD_NIGHT:
        xflag->sceneId = SCE_OOT_MARKET_CHILD_DAY;
        xflag->id += 16;
        break;
    case SCE_OOT_KAKARIKO_VILLAGE:
        switch (xflag->setupId)
        {
        case 1: xflag->id += 5; break;
        case 2: xflag->id += 12; break;
        case 3: xflag->id += 14; break;
        }
        xflag->setupId = 0;
        break;
    case SCE_OOT_ZORA_RIVER:
        if (xflag->setupId == 2)
        {
            xflag->id += 28;
            xflag->setupId = 0;
        }
        break;
    case SCE_OOT_KOKIRI_FOREST:
        if (xflag->setupId == 3)
        {
            xflag->setupId = 2;
            xflag->id -= 7;
        }
        if (xflag->setupId == 0 && xflag->roomId == 2 && xflag->id == 9)
        {
            xflag->roomId = 0;
            xflag->id = 62;
        }
        break;
    case SCE_OOT_LAKE_HYLIA:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            xflag->id -= 8;
        }
        break;
    case SCE_OOT_LOST_WOODS:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            switch (xflag->roomId)
            {
            case 2: xflag->id -= 2; break;
            case 7: xflag->id -= 1; break;
            case 8: xflag->id -= 1; break;
            }
        }
        break;
    }
}

void EnKusa_SetupAction(EnKusa* this, EnKusaActionFunc actionFunc) {
    this->timer = 0;
    this->actionFunc = actionFunc;
}

s32 EnKusa_SnapToFloor(EnKusa* this, PlayState* play, f32 yOffset) {
    CollisionPoly* groundPoly;
    Vec3f pos;
    s32 bgId;
    f32 floorY;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + 30.0f;
    pos.z = this->actor.world.pos.z;

    floorY = BgCheck_EntityRaycastDown4(&play->colCtx, &groundPoly, &bgId, &this->actor, &pos);

    if (floorY > BGCHECK_Y_MIN) {
        this->actor.world.pos.y = floorY + yOffset;
        Math_Vec3f_Copy(&this->actor.home.pos, &this->actor.world.pos);
        return true;
    } else {
        return false;
    }
}

void EnKusa_DropCollectible(EnKusa* this, PlayState* play) {
    s16 dropParams;

    if (Xflag_IsShuffled(&this->xflag))
    {
        EnItem00_DropCustom(play, &this->actor.world.pos, &this->xflag);
        return;
    }

    switch (PARAMS_GET_U(this->actor.params, 0, 2)) {
        case ENKUSA_TYPE_0:
        case ENKUSA_TYPE_2:
            dropParams = PARAMS_GET_U(this->actor.params, 8, 4);

            if (dropParams >= 0xD) {
                dropParams = 0;
            }
            Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, dropParams << 4);
            break;
        case ENKUSA_TYPE_1:
            if (Rand_ZeroOne() < 0.5f) {
                Item_DropCollectible(play, &this->actor.world.pos, ITEM00_SEEDS);
            } else {
                Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RECOVERY_HEART);
            }
            break;
    }
}

void EnKusa_UpdateVelY(EnKusa* this) {
    this->actor.velocity.y += this->actor.gravity;

    if (this->actor.velocity.y < this->actor.minVelocityY) {
        this->actor.velocity.y = this->actor.minVelocityY;
    }
}

void EnKusa_RandScaleVecToZero(Vec3f* vec, f32 scale) {
    scale += ((Rand_ZeroOne() * 0.2f) - 0.1f) * scale;
    vec->x -= vec->x * scale;
    vec->y -= vec->y * scale;
    vec->z -= vec->z * scale;
}

void EnKusa_SetScaleSmall(EnKusa* this) {
    this->actor.scale.y = 0.16000001f;
    this->actor.scale.x = 0.120000005f;
    this->actor.scale.z = 0.120000005f;
}

void EnKusa_SpawnFragments(EnKusa* this, PlayState* play) {
    Vec3f velocity;
    Vec3f pos;
    s32 i;
    s32 scaleIndex;
    Vec3f* dir;

    for (i = 0; i < ARRAY_COUNT(sUnitDirections); i++) {
        dir = &sUnitDirections[i];

        pos.x = this->actor.world.pos.x + (dir->x * this->actor.scale.x * 20.0f);
        pos.y = this->actor.world.pos.y + (dir->y * this->actor.scale.y * 20.0f) + 10.0f;
        pos.z = this->actor.world.pos.z + (dir->z * this->actor.scale.z * 20.0f);

        velocity.x = (Rand_ZeroOne() - 0.5f) * 8.0f;
        velocity.y = Rand_ZeroOne() * 10.0f;
        velocity.z = (Rand_ZeroOne() - 0.5f) * 8.0f;

        scaleIndex = (s32)(Rand_ZeroOne() * 111.1f) & 7;

        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, -100, 64, 40, 3, 0, sFragmentScales[scaleIndex], 0, 0, 80,
                             KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_KEEP, (void*)gCuttableShrubStalkDL);

        pos.x = this->actor.world.pos.x + (dir->x * this->actor.scale.x * 40.0f);
        pos.y = this->actor.world.pos.y + (dir->y * this->actor.scale.y * 40.0f) + 10.0f;
        pos.z = this->actor.world.pos.z + (dir->z * this->actor.scale.z * 40.0f);

        velocity.x = (Rand_ZeroOne() - 0.5f) * 6.0f;
        velocity.y = Rand_ZeroOne() * 10.0f;
        velocity.z = (Rand_ZeroOne() - 0.5f) * 6.0f;

        scaleIndex = (s32)(Rand_ZeroOne() * 111.1f) % 7;

        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, -100, 64, 40, 3, 0, sFragmentScales[scaleIndex], 0, 0, 80,
                             KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_KEEP, (void*)gCuttableShrubTipDL);
    }
}

void EnKusa_SpawnBugs(EnKusa* this, PlayState* play) {
    s32 i;

    for (i = 0; i < 3; i++) {
        Actor* bug =
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_INSECT, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, Rand_ZeroOne() * 0xFFFF, 0, INSECT_TYPE_SPAWNED);

        if (bug == NULL) {
            break;
        }
    }
}

void EnKusa_InitCollider(Actor* thisx, PlayState* play) {
    EnKusa* this = (EnKusa*)thisx;

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
}

void EnKusa_Init(Actor* thisx, PlayState* play) {
    EnKusa* this = (EnKusa*)thisx;

    if (comboXflagInit(&this->xflag, thisx, play))
        EnKusa_Aliases(&this->xflag);

    Actor_ProcessInitChain(&this->actor, sInitChain);

    if (play->csCtx.state != CS_STATE_IDLE) {
        this->actor.uncullZoneForward += 1000.0f;
    }

    EnKusa_InitCollider(thisx, play);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, NULL, &sColChkInfoInit);

    if (this->actor.shape.rot.y == 0) {
        s16 rand = Rand_ZeroFloat(0x10000);

        this->actor.world.rot.y = rand;
        this->actor.home.rot.y = rand;
        this->actor.shape.rot.y = rand;
    }

    if (!EnKusa_SnapToFloor(this, play, 0.0f)) {
        Actor_Kill(&this->actor);
        return;
    }

    this->requiredObjectSlot = Object_GetSlot(&play->objectCtx, sObjectIds[PARAMS_GET_U(thisx->params, 0, 2)]);

    if (this->requiredObjectSlot < 0) {
        Actor_Kill(&this->actor);
        return;
    }

    EnKusa_SetupWaitForObject(this);
}

void EnKusa_Destroy(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnKusa* this = (EnKusa*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnKusa_SetupWaitForObject(EnKusa* this) {
    EnKusa_SetupAction(this, EnKusa_WaitForObject);
}

void EnKusa_WaitForObject(EnKusa* this, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, this->requiredObjectSlot)) {
        if (this->actor.flags & ACTOR_FLAG_GRASS_DESTROYED) {
            EnKusa_SetupCut(this);
        } else {
            EnKusa_SetupMain(this);
        }

        this->actor.draw = EnKusa_Draw;
        this->actor.objectSlot = this->requiredObjectSlot;
        this->actor.flags &= ~ACTOR_FLAG_OOT_4;
    }
}

void EnKusa_SetupMain(EnKusa* this) {
    EnKusa_SetupAction(this, EnKusa_Main);
    this->actor.flags &= ~ACTOR_FLAG_OOT_4;
}

void EnKusa_Main(EnKusa* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        EnKusa_SetupLiftedUp(this);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_PL_PULL_UP_PLANT);
    } else if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        EnKusa_SpawnFragments(this, play);
        EnKusa_DropCollectible(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_PLANT_BROKEN);

        if (PARAMS_GET_U(this->actor.params, 4, 1)) {
            EnKusa_SpawnBugs(this, play);
        }

        if (PARAMS_GET_U(this->actor.params, 0, 2) == ENKUSA_TYPE_0) {
            Actor_Kill(&this->actor);
            return;
        }

        EnKusa_SetupCut(this);
        this->actor.flags |= ACTOR_FLAG_GRASS_DESTROYED;
    } else {
        if (!(this->collider.base.ocFlags1 & OC1_TYPE_PLAYER) && (this->actor.xzDistToPlayer > 12.0f)) {
            this->collider.base.ocFlags1 |= OC1_TYPE_PLAYER;
        }

        if (this->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&this->actor, &this->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);

            if (this->actor.xzDistToPlayer < 400.0f) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
                if (this->actor.xzDistToPlayer < 100.0f) {
                    Actor_OfferCarry(&this->actor, play);
                }
            }
        }
    }
}

void EnKusa_SetupLiftedUp(EnKusa* this) {
    EnKusa_SetupAction(this, EnKusa_LiftedUp);
    this->actor.room = -1;
    this->actor.flags |= ACTOR_FLAG_OOT_4;
}

void EnKusa_LiftedUp(EnKusa* this, PlayState* play) {
    if (Actor_HasNoParent(&this->actor, play)) {
        this->actor.room = play->roomCtx.curRoom.num;
        EnKusa_SetupFall(this);
        this->actor.velocity.x = this->actor.speed * Math_SinS(this->actor.world.rot.y);
        this->actor.velocity.z = this->actor.speed * Math_CosS(this->actor.world.rot.y);
        this->actor.colChkInfo.mass = 240;
        this->actor.gravity = -0.1f;
        EnKusa_UpdateVelY(this);
        EnKusa_RandScaleVecToZero(&this->actor.velocity, 0.005f);
        Actor_UpdatePos(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 7.5f, 35.0f, 0.0f,
                                UPDBGCHECKINFO_FLAG_OOT_0 | UPDBGCHECKINFO_FLAG_OOT_2 | UPDBGCHECKINFO_FLAG_OOT_6 |
                                    UPDBGCHECKINFO_FLAG_OOT_7);
        this->actor.gravity = -3.2f;
    }
}

void EnKusa_SetupFall(EnKusa* this) {
    EnKusa_SetupAction(this, EnKusa_Fall);
    rotSpeedXtarget = -0xBB8;
    rotSpeedYtarget = (Rand_ZeroOne() - 0.5f) * 1600.0f;
    rotSpeedX = 0;
    rotSpeedY = 0;
}

void EnKusa_Fall(EnKusa* this, PlayState* play) {
    Vec3f contactPos;

    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_WALL)) {
        if (!(this->actor.bgCheckFlags & BGCHECKFLAG_WATER)) {
            SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_PLANT_BROKEN);
        }
        EnKusa_SpawnFragments(this, play);
        EnKusa_DropCollectible(this, play);
        switch (PARAMS_GET_U(this->actor.params, 0, 2)) {
            case ENKUSA_TYPE_0:
            case ENKUSA_TYPE_2:
                Actor_Kill(&this->actor);
                break;

            case ENKUSA_TYPE_1:
                EnKusa_SetupUprootedWaitRegrow(this);
                break;
        }
        return;
    }

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) {
        contactPos.x = this->actor.world.pos.x;
        contactPos.y = this->actor.world.pos.y + this->actor.depthInWater;
        contactPos.z = this->actor.world.pos.z;
        EffectSsGSplash_Spawn(play, &contactPos, NULL, NULL, 0, 400);
        EffectSsGRipple_Spawn(play, &contactPos, 150, 650, 0);
        EffectSsGRipple_Spawn(play, &contactPos, 400, 800, 4);
        EffectSsGRipple_Spawn(play, &contactPos, 500, 1100, 8);
        this->actor.minVelocityY = -3.0f;
        rotSpeedX >>= 1;
        rotSpeedXtarget >>= 1;
        rotSpeedY >>= 1;
        rotSpeedYtarget >>= 1;
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_WATER_TOUCH;
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EV_DIVE_INTO_WATER_L);
    }

    EnKusa_UpdateVelY(this);
    Math_StepToS(&rotSpeedX, rotSpeedXtarget, 0x1F4);
    Math_StepToS(&rotSpeedY, rotSpeedYtarget, 0xAA);
    this->actor.shape.rot.x += rotSpeedX;
    this->actor.shape.rot.y += rotSpeedY;
    EnKusa_RandScaleVecToZero(&this->actor.velocity, 0.05f);
    Actor_UpdatePos(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 7.5f, 35.0f, 0.0f,
                            UPDBGCHECKINFO_FLAG_OOT_0 | UPDBGCHECKINFO_FLAG_OOT_2 | UPDBGCHECKINFO_FLAG_OOT_6 |
                                UPDBGCHECKINFO_FLAG_OOT_7);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void EnKusa_SetupCut(EnKusa* this) {
    switch (PARAMS_GET_U(this->actor.params, 0, 2)) {
        case ENKUSA_TYPE_2:
            EnKusa_SetupAction(this, EnKusa_DoNothing);
            break;
        case ENKUSA_TYPE_1:
            EnKusa_SetupAction(this, EnKusa_CutWaitRegrow);
            break;
    }
}

void EnKusa_CutWaitRegrow(EnKusa* this, PlayState* play) {
    if (this->timer >= 120) {
        EnKusa_SetupRegrow(this);
    }
}

void EnKusa_DoNothing(EnKusa* this, PlayState* play) {
}

void EnKusa_SetupUprootedWaitRegrow(EnKusa* this) {
    this->actor.world.pos.x = this->actor.home.pos.x;
    this->actor.world.pos.y = this->actor.home.pos.y - 9.0f;
    this->actor.world.pos.z = this->actor.home.pos.z;
    EnKusa_SetScaleSmall(this);
    this->actor.shape.rot = this->actor.home.rot;
    EnKusa_SetupAction(this, EnKusa_UprootedWaitRegrow);
}

void EnKusa_UprootedWaitRegrow(EnKusa* this, PlayState* play) {
    if (this->timer > 120) {
        if (Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y, 0.6f)) {
            if (this->timer >= 170) {
                EnKusa_SetupRegrow(this);
            }
        }
    }
}

void EnKusa_SetupRegrow(EnKusa* this) {
    EnKusa_SetupAction(this, EnKusa_Regrow);
    EnKusa_SetScaleSmall(this);
    this->actor.shape.rot = this->actor.home.rot;
    this->actor.flags &= ~ACTOR_FLAG_GRASS_DESTROYED;
}

void EnKusa_Regrow(EnKusa* this, PlayState* play) {
    s32 isFullyGrown = true;

    isFullyGrown &= Math_StepToF(&this->actor.scale.y, 0.4f, 0.014f);
    isFullyGrown &= Math_StepToF(&this->actor.scale.x, 0.4f, 0.011f);
    this->actor.scale.z = this->actor.scale.x;

    if (isFullyGrown) {
        Actor_SetScale(&this->actor, 0.4f);
        EnKusa_SetupMain(this);
        this->collider.base.ocFlags1 &= ~OC1_TYPE_PLAYER;
    }
}

void EnKusa_Update(Actor* thisx, PlayState* play) {
    EnKusa* this = (EnKusa*)thisx;

    this->timer++;

    this->actionFunc(this, play);

    if (this->actor.flags & ACTOR_FLAG_GRASS_DESTROYED) {
        this->actor.shape.yOffset = -6.25f;
    } else {
        this->actor.shape.yOffset = 0.0f;
    }
}

static void EnKusa_DrawImpl(EnKusa* this, PlayState* play) {
    static Gfx* dLists[] = { gFieldBushDL, object_kusa_DL_000140, object_kusa_DL_000140 };

    if (this->actor.flags & ACTOR_FLAG_GRASS_DESTROYED) {
        Gfx_DrawDListOpa(play, object_kusa_DL_0002E0);
    } else {
        Gfx_DrawDListOpa(play, dLists[PARAMS_GET_U(this->actor.params, 0, 2)]);
    }
}

void EnKusa_Draw(Actor* thisx, PlayState* play)
{
    EnKusa* this;
    ComboItemOverride o;
    int alt;

    this = (EnKusa*)thisx;
    if (Xflag_IsShuffled(&this->xflag))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    /* Prepare */
    if ((this->actor.params & 3) == 0)
        alt = 0;
    else
        alt = 1;
    csmcGrassPreDraw(play, o.gi, CSMC_GRASS_NORMAL, alt, 0);

    /* Draw the actor */
    EnKusa_DrawImpl(this, play);
}
