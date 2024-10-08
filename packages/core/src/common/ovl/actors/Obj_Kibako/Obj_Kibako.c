#include <combo.h>
#include <combo/item.h>
#include <combo/global.h>
#include <combo/custom.h>
#include "Obj_Kibako.h"

#define FLAGS (ACTOR_FLAG_OOT_4 | ACTOR_FLAG_OOT_26)

void ObjKibako_Init(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_Destroy(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_Update(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_Draw(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_SpawnCollectible(Actor_ObjKibako* this, GameState_Play* play);

static int ObjKibako_IsExtended(Actor_ObjKibako* this)
{
    return !!(this->isExtended && !comboXflagsGet(&this->xflag));
}

#if defined(GAME_OOT)
static void ObjKibako_Alias(Actor_ObjKibako* this)
{
}
#endif

#if defined(GAME_MM)
static void ObjKibako_Alias(Actor_ObjKibako* this)
{
}
#endif

static void ObjKibako_InitXflag(Actor_ObjKibako* this, GameState_Play* play)
{
    ComboItemOverride o;

    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->actor.room;
    this->xflag.sliceId = 0;
    this->xflag.id = this->actor.actorIndex;

    /* Fix the aliases */
    ObjKibako_Alias(this);

    /* Detect xflags */
    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));
}

static int ObjKibako_CsmcType(Actor_ObjKibako* this)
{
    ComboItemOverride o;

    if (!ObjKibako_IsExtended(this))
        o.gi = GI_NONE;
    else
        comboXflagItemOverride(&o, &this->xflag, 0);

    if (!csmcEnabled())
        return o.gi ? CSMC_MAJOR : CSMC_NORMAL;

    return csmcFromItem(o.gi);
}

static void ObjKibako_DrawWithTexture(Actor_ObjKibako* this, GameState_Play* play, u32 dlist, u32 baseTex)
{
    int type;
    u32 customVrom;
    void* data;

    type = ObjKibako_CsmcType(this);
    switch (type)
    {
    case CSMC_BOSS_KEY: customVrom = CUSTOM_CRATE_BOSS_KEY_ADDR; break; /* TODO: Need a texture */
    case CSMC_MAJOR: customVrom = CUSTOM_CHEST_MAJOR_FRONT_ADDR; break;
    case CSMC_KEY: customVrom = CUSTOM_CHEST_KEY_FRONT_ADDR; break;
    case CSMC_SPIDER: customVrom = CUSTOM_CHEST_SPIDER_FRONT_ADDR; break;
    case CSMC_FAIRY: customVrom = CUSTOM_CHEST_FAIRY_FRONT_ADDR; break;
    case CSMC_HEART: customVrom = CUSTOM_CHEST_HEART_FRONT_ADDR; break;
    case CSMC_SOUL: customVrom = CUSTOM_CHEST_SOUL_FRONT_ADDR; break;
    case CSMC_MAP_COMPASS: customVrom = CUSTOM_CHEST_MAP_FRONT_ADDR; break;
    default: customVrom = 0; break;
    }

    if (customVrom)
    {
        data = comboCacheGetFile(customVrom);
        if (!data)
            return;
    }
    else
    {
        data = (void*)(gSegments[baseTex >> 24] + (baseTex & 0xffffff));
    }

    /* Draw */
    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x08, data);
    CLOSE_DISPS();
    Gfx_DrawDListOpa(play, (void*)dlist);
}

#if defined(GAME_OOT)
# define OBJECT                 OBJECT_GAMEPLAY_DANGEON_KEEP
# define SEGADDR_CRATE_DL       ((void*)0x05005290)
# define SEGADDR_FRAGMENT_DL    ((void*)0x05005380)

void ObjKibako_SetupIdle(Actor_ObjKibako* this);
void ObjKibako_Idle(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_SetupHeld(Actor_ObjKibako* this);
void ObjKibako_Held(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_SetupThrown(Actor_ObjKibako* this);
void ObjKibako_Thrown(Actor_ObjKibako* this, GameState_Play* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000002, 0x00, 0x01 },
        { 0x4FC00748, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 12, 27, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sCCInfoInit = { 0, 12, 60, MASS_HEAVY };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 60, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void ObjKibako_SpawnCollectible(Actor_ObjKibako* this, GameState_Play* play)
{
    s16 collectible;

    if (ObjKibako_IsExtended(this))
    {
        EnItem00_DropCustom(play, &this->actor.world.pos, &this->xflag);
        return;
    }

    collectible = this->actor.params & 0x1F;
    if ((collectible >= 0) && (collectible < ITEM00_MAX)) {
        Item_DropCollectible(play, &this->actor.world.pos, collectible | (((this->actor.params >> 8) & 0x3F) << 8));
    }
}

void ObjKibako_ApplyGravity(Actor_ObjKibako* this) {
    this->actor.velocity.y += this->actor.gravity;
    if (this->actor.velocity.y < this->actor.minVelocityY) {
        this->actor.velocity.y = this->actor.minVelocityY;
    }
}

void ObjKibako_InitCollider(Actor_ObjKibako* this, GameState_Play* play)
{
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
}

void ObjKibako_Init(Actor_ObjKibako* this, GameState_Play* play)
{
    ObjKibako_InitXflag(this, play);
    this->actor.draw = ObjKibako_Draw;
    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->actor.gravity = -1.2f;
    this->actor.minVelocityY = -13.0f;
    ObjKibako_InitCollider(this, play);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, NULL, &sCCInfoInit);
    ObjKibako_SetupIdle(this);
}

void ObjKibako_Destroy(Actor_ObjKibako* this, GameState_Play* play)
{
    Collider_DestroyCylinder(play, &this->collider);
}

void ObjKibako_AirBreak(Actor_ObjKibako* this, GameState_Play* play)
{
    s16 angle;
    s32 i;
    Vec3f* breakPos = &this->actor.world.pos;
    Vec3f pos;
    Vec3f velocity;

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s16 phi_s0;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 6.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        pos.x += breakPos->x;
        pos.y += breakPos->y;
        pos.z += breakPos->z;
        temp_rand = Rand_ZeroOne();
        if (temp_rand < 0.1f) {
            phi_s0 = 0x60;
        } else if (temp_rand < 0.7f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, breakPos, -200, phi_s0, 10, 10, 0, (Rand_ZeroOne() * 30.0f) + 10.0f,
                             0, 32, 60, KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_DANGEON_KEEP, SEGADDR_FRAGMENT_DL);
    }

    SpawnSomeDust(play, &this->actor.world.pos, 40.0f, 3, 50, 140, 1);
}

void ObjKibako_WaterBreak(Actor_ObjKibako* this, GameState_Play* play) {
    s16 angle;
    s32 i;
    Vec3f* breakPos = &this->actor.world.pos;
    Vec3f pos;
    Vec3f velocity;

    pos = *breakPos;
    pos.y += this->actor.depthInWater;
    EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 500);

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s16 phi_s0;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.18f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.18f;
        pos.x += breakPos->x;
        pos.y += breakPos->y;
        pos.z += breakPos->z;
        temp_rand = Rand_ZeroOne();
        phi_s0 = (temp_rand < 0.2f) ? 0x40 : 0x20;
        EffectSsKakera_Spawn(play, &pos, &velocity, breakPos, -180, phi_s0, 30, 30, 0, (Rand_ZeroOne() * 30.0f) + 10.0f,
                             0, 32, 70, KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_DANGEON_KEEP, SEGADDR_FRAGMENT_DL);
    }
}

void ObjKibako_SetupIdle(Actor_ObjKibako* this) {
    this->actionFunc = ObjKibako_Idle;
    this->actor.colChkInfo.mass = MASS_HEAVY;
}

void ObjKibako_Idle(Actor_ObjKibako* this, GameState_Play* play)
{
    if (Actor_HasParent(&this->actor, play)) {
        ObjKibako_SetupHeld(this);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER) && (this->actor.depthInWater > 19.0f)) {
        ObjKibako_WaterBreak(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectible(this, play);
        Actor_Kill(&this->actor);
    } else if (this->collider.base.acFlags & AC_HIT) {
        ObjKibako_AirBreak(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectible(this, play);
        Actor_Kill(&this->actor);
    } else {
        Actor_MoveWithGravity(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 20.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR);
        if (!(this->collider.base.ocFlags1 & OC1_TYPE_PLAYER) && (this->actor.xzDistToPlayer > 28.0f)) {
            this->collider.base.ocFlags1 |= OC1_TYPE_PLAYER;
        }
        if (this->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&this->actor, &this->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            if (this->actor.xzDistToPlayer < 180.0f) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
            }
        }
        if (this->actor.xzDistToPlayer < 100.0f) {
            Actor_OfferCarry(&this->actor, play);
        }
    }
}

void ObjKibako_SetupHeld(Actor_ObjKibako* this)
{
    this->actionFunc = ObjKibako_Held;
    this->actor.room = -1;
    //! @bug: This is an unsafe cast, although the sound effect will still play
    Player_PlaySfx((void*)&this->actor, NA_SE_PL_PULL_UP_WOODBOX);
}

void ObjKibako_Held(Actor_ObjKibako* this, GameState_Play* play)
{
    if (Actor_HasNoParent(&this->actor, play))
    {
        this->actor.room = play->roomCtx.curRoom.num;
        if (fabsf(this->actor.speed) < 0.1f)
        {
            Actor_PlaySfx(&this->actor, NA_SE_EV_PUT_DOWN_WOODBOX);
            ObjKibako_SetupIdle(this);
            this->collider.base.ocFlags1 &= ~OC1_TYPE_PLAYER;
        }
        else
        {
            ObjKibako_SetupThrown(this);
            ObjKibako_ApplyGravity(this);
            Actor_UpdatePos(&this->actor);
        }
        Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 20.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR);
    }
}

void ObjKibako_SetupThrown(Actor_ObjKibako* this)
{
    this->actor.velocity.x = Math_SinS(this->actor.world.rot.y) * this->actor.speed;
    this->actor.velocity.z = Math_CosS(this->actor.world.rot.y) * this->actor.speed;
    this->actor.colChkInfo.mass = 240;
    this->actionFunc = ObjKibako_Thrown;
}

void ObjKibako_Thrown(Actor_ObjKibako* this, GameState_Play* play)
{
    if ((this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_WALL)) || (this->collider.base.atFlags & AT_HIT))
    {
        ObjKibako_AirBreak(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectible(this, play);
        Actor_Kill(&this->actor);
    }
    else if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH)
    {
        ObjKibako_WaterBreak(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectible(this, play);
        Actor_Kill(&this->actor);
    }
    else
    {
        ObjKibako_ApplyGravity(this);
        Actor_UpdatePos(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 20.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR);
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
}

void ObjKibako_Update(Actor_ObjKibako* this, GameState_Play* play)
{
    this->actionFunc(this, play);
}

void ObjKibako_Draw(Actor_ObjKibako* this, GameState_Play* play)
{
    ObjKibako_DrawWithTexture(this, play, (u32)SEGADDR_CRATE_DL, 0x05011ca0);
}
#endif

#if defined(GAME_MM)
# define OBJECT                                 OBJECT_GAMEPLAY_KEEP
# define KIBAKO_COLLECTIBLE_ID(thisx)           ((thisx)->actor.params & 0x3F)
# define KIBAKO_COLLECTIBLE_FLAG(thisx)         (((thisx)->actor.params >> 0x8) & 0x7F)
# define KIBAKO_BANK_INDEX(thisx)               (((thisx)->actor.params >> 0xF) & 1)
# define KIBAKO_BOMBER_CAN_HIDE_IN_BOX(thisx)   (((thisx)->actor.params >> 7) & 1)
# define SEGADDR_KEEP_CRATE_DL                  ((void*)0x05007890)
# define SEGADDR_KEEP_FRAGMENT_DL               ((void*)0x05007980)
# define SEGADDR_OBJ_CRATE_DL                   ((void*)0x06001180)
# define SEGADDR_OBJ_FRAGMENT_DL                ((void*)0x06001a70)

void func_809262BC(Actor_ObjKibako* this);
void func_80926318(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_AirBreak(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_WaterBreak(Actor_ObjKibako* this, GameState_Play* play);
void func_80926B40(Actor_ObjKibako* this);
void func_80926B54(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_SetupIdle(Actor_ObjKibako* this);
void ObjKibako_Idle(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_SetupHeld(Actor_ObjKibako* this);
void ObjKibako_Held(Actor_ObjKibako* this, GameState_Play* play);
void ObjKibako_SetupThrown(Actor_ObjKibako* this);
void ObjKibako_Thrown(Actor_ObjKibako* this, GameState_Play* play);

static s16 D_80927384 = 0;
static s16 D_80927388 = 0;
static s16 D_8092738C = 0;

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00400000, 0x00, 0x02 },
        { 0x058BC79C, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 15, 30, 0, { 0, 0, 0 } },
};

static s16 sObjectIds[] = { OBJECT_GAMEPLAY_DANGEON_KEEP, OBJECT_KIBAKO };

static Gfx* sKakeraDisplayLists[] = { SEGADDR_KEEP_FRAGMENT_DL, SEGADDR_OBJ_FRAGMENT_DL };

static Gfx* sDisplayLists[] = { SEGADDR_KEEP_CRATE_DL, SEGADDR_OBJ_CRATE_DL };

static u32 sTextures[] = { 0x050122c0, 0x06000000 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -1500, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(minVelocityY, -18000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 60, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 60, ICHAIN_STOP),
};

void ObjKibako_SpawnCollectible(Actor_ObjKibako* this, GameState_Play* play)
{
    s32 dropItem00Id;

    if (ObjKibako_IsExtended(this))
    {
        EnItem00_DropCustom(play, &this->actor.world.pos, &this->xflag);
        return;
    }

    if (this->isDropCollected == 0) {
        dropItem00Id = Item_CollectibleDropTable(KIBAKO_COLLECTIBLE_ID(this));
        if (dropItem00Id > ITEM00_NO_DROP) {
            Item_DropCollectible(play, &this->actor.world.pos,
                                 dropItem00Id | KIBAKO_COLLECTIBLE_FLAG(this) << 8);
            this->isDropCollected = 1;
        }
    }
}

void ObjKibako_SetShadow(Actor_ObjKibako* this) {
    if ((this->actor.projectedPos.z < 370.0f) && (this->actor.projectedPos.z > -10.0f)) {
        this->actor.shape.shadowDraw = ActorShadow_DrawSquare;
        this->actor.shape.shadowScale = 1.4f;
        this->actor.shape.shadowAlpha =
            (this->actor.projectedPos.z < 200.0f) ? 100 : (400 - ((s32)this->actor.projectedPos.z)) >> 1;
    } else {
        this->actor.shape.shadowDraw = NULL;
    }
}

void func_809262BC(Actor_ObjKibako* this) {
    s16 angle = this->actor.world.rot.y;

    if ((angle & 0x3FFF) != 0) {
        angle = Math_ScaledStepToS(&this->actor.world.rot.y, (s16)(angle + 0x2000) & 0xC000, 0x640);
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }
}

void func_80926318(Actor_ObjKibako* this, GameState_Play* play) {
    s16 angle;

    if (this->actor.xzDistToPlayer < 100.0f) {
        angle = this->actor.yawTowardsPlayer - GET_PLAYER(play)->actor.world.rot.y;
        if (ABS_ALT(angle) > 0x5555) {
            Actor_OfferGetItem(&this->actor, play, GI_NONE, 36.0f, 30.0f);
        }
    }
}

void func_80926394(Actor_ObjKibako* this, GameState_Play* play) {
    if ((this->isDropCollected == 0) && (play->roomCtx.curRoom.num != this->unk199)) {
        this->isDropCollected = 1;
    }
}

void ObjKibako_Init(Actor_ObjKibako* this, GameState_Play* play)
{
    s32 objectIndex;

    ObjKibako_InitXflag(this, play);
    objectIndex = KIBAKO_BANK_INDEX(this);
    Actor_ProcessInitChain(&this->actor, sInitChain);
    Actor_SetScale(&this->actor, 0.15f);
    if (objectIndex == 0) {
        this->actor.uncullZoneForward = 4000.0f;
    } else {
        this->actor.uncullZoneForward = 800.0f;
    }
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->objectSlot = Object_GetSlot(&play->objectCtx, sObjectIds[objectIndex]);
    if (this->objectSlot <= -1) {
        Actor_Kill(&this->actor);
        return;
    }
    this->unk199 = this->actor.room;
    func_80926B40(this);
}

void ObjKibako_Destroy(Actor_ObjKibako* this, GameState_Play* play)
{
    Collider_DestroyCylinder(play, &this->collider);
}

void ObjKibako_AirBreak(Actor_ObjKibako* this, GameState_Play* play)
{
    s16 angle;
    s32 i;
    Vec3f* worldPos = &this->actor.world.pos;
    Vec3f pos;
    Vec3f velocity;

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s16 phi_s0;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 6.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        pos.x += worldPos->x;
        pos.y += worldPos->y;
        pos.z += worldPos->z;
        temp_rand = Rand_ZeroOne();
        if (temp_rand < 0.1f) {
            phi_s0 = 0x60;
        } else if (temp_rand < 0.7f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }

        EffectSsKakera_Spawn(play, &pos, &velocity, worldPos, -200, phi_s0, 20, 0, 0, (Rand_ZeroOne() * 38.0f) + 10.0f,
                             0, 0, 60, -1, sObjectIds[KIBAKO_BANK_INDEX(this)],
                             sKakeraDisplayLists[KIBAKO_BANK_INDEX(this)]);
    }

    SpawnSomeDust(play, worldPos, 40.0f, 3, 0x32, 0x8C, 1);
    SpawnSomeDust(play, worldPos, 40.0f, 2, 0x14, 0x50, 1);
}

void ObjKibako_WaterBreak(Actor_ObjKibako* this, GameState_Play* play) {
    s16 angle;
    s32 i;
    Vec3f* worldPos = &this->actor.world.pos;
    Vec3f pos;
    Vec3f velocity;

    pos.y = worldPos->y + this->actor.depthInWater;
    for (angle = 0, i = 0; i < 5; i++, angle += 0x3333) {
        pos.x = worldPos->x + (Math_SinS(((s32)(Rand_ZeroOne() * 6000.0f)) + angle) * 15.0f);
        pos.z = worldPos->z + (Math_CosS(((s32)(Rand_ZeroOne() * 6000.0f)) + angle) * 15.0f);
        EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 350);
    }
    pos.x = worldPos->x;
    pos.z = worldPos->z;
    EffectSsGRipple_Spawn(play, &pos, 200, 600, 0);

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s16 phi_s0;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.18f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.18f;
        pos.x += worldPos->x;
        pos.y += worldPos->y;
        pos.z += worldPos->z;
        temp_rand = Rand_ZeroOne();
        phi_s0 = (temp_rand < 0.2f) ? 0x40 : 0x20;

        EffectSsKakera_Spawn(play, &pos, &velocity, worldPos, -180, phi_s0, 50, 5, 0, (Rand_ZeroOne() * 35.0f) + 10.0f,
                             0, 0, 70, -1, sObjectIds[KIBAKO_BANK_INDEX(this)],
                             sKakeraDisplayLists[KIBAKO_BANK_INDEX(this)]);
    }
}

void func_80926B40(Actor_ObjKibako* this) {
    this->actionFunc = func_80926B54;
}

void func_80926B54(Actor_ObjKibako* this, GameState_Play* play) {
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 18.0f, 15.0f, 0.0f,
                            UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR | UPDBGCHECKINFO_FLAG_MM_40);
    if (Object_IsLoaded(&play->objectCtx, this->objectSlot)) {
        this->actor.draw = ObjKibako_Draw;
        this->actor.objectSlot = this->objectSlot;
        ObjKibako_SetupIdle(this);
    }
}

void ObjKibako_SetupIdle(Actor_ObjKibako* this)
{
    this->actionFunc = ObjKibako_Idle;
}

void ObjKibako_Idle(Actor_ObjKibako* this, GameState_Play* play)
{
    if (Actor_HasParent(&this->actor, play))
    {
        ObjKibako_SetupHeld(this);
        this->actor.room = -1;
        this->actor.colChkInfo.mass = 120;
        if (Item_CollectibleDropTable2(KIBAKO_COLLECTIBLE_ID(this)) && !ObjKibako_IsExtended(this)) {
            ObjKibako_SpawnCollectible(this, play);
        }

        //! @bug: This function should only pass Player*: it uses *(this + 0x153), which is meant to be
        //! player->currentMask, but in this case is garbage in the collider
        Player_PlaySfx((void*)&this->actor, NA_SE_PL_PULL_UP_WOODBOX);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER) && (this->actor.depthInWater > 19.0f)) {
        ObjKibako_WaterBreak(this, play);
        ObjKibako_SpawnCollectible(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EV_DIVE_INTO_WATER_L);
        Actor_Kill(&this->actor);
    } else if (this->collider.base.acFlags & AC_HIT) {
        ObjKibako_AirBreak(this, play);
        ObjKibako_SpawnCollectible(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        Actor_Kill(&this->actor);
    } else {
        Actor_MoveWithGravity(&this->actor);
        func_809262BC(this);
        Actor_UpdateBgCheckInfo(play, &this->actor, 18.0f, 15.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR | UPDBGCHECKINFO_FLAG_MM_40);

        if (!(this->collider.base.ocFlags1 & OC1_TYPE_PLAYER) && (this->actor.xzDistToPlayer > 28.0f)) {
            this->collider.base.ocFlags1 |= OC1_TYPE_PLAYER;
        }

        if ((this->actor.colChkInfo.mass != MASS_IMMOVABLE) && (Math3D_Vec3fDistSq(&this->actor.world.pos, &this->actor.prevPos) < 0.01f))
        {
            this->actor.colChkInfo.mass = MASS_IMMOVABLE;
        }

        this->collider.base.acFlags &= ~AC_HIT;

        if (KIBAKO_BOMBER_CAN_HIDE_IN_BOX(this)) {
            Collider_UpdateCylinder(&this->actor, &this->collider);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

            if (this->actor.xzDistToPlayer < 800.0f) {
                CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
                func_80926318(this, play);
            }
        } else {
            if (this->actor.xzDistToPlayer < 800.0f) {
                Collider_UpdateCylinder(&this->actor, &this->collider);
                CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);

                if (this->actor.xzDistToPlayer < 180.0f) {
                    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
                    func_80926318(this, play);
                }
            }
        }
    }
}

void ObjKibako_SetupHeld(Actor_ObjKibako* this) {
    this->actionFunc = ObjKibako_Held;
}

void ObjKibako_Held(Actor_ObjKibako* this, GameState_Play* play)
{
    Vec3f pos;
    s32 bgId;

    func_80926394(this, play);
    if (Actor_HasNoParent(&this->actor, play)) {
        this->actor.room = play->roomCtx.curRoom.num;
        if (fabsf(this->actor.speed) < 0.1f) {
            ObjKibako_SetupIdle(this);
            this->collider.base.ocFlags1 &= ~OC1_TYPE_PLAYER;
            Actor_PlaySfx(&this->actor, NA_SE_EV_PUT_DOWN_WOODBOX);
        } else {
            Actor_MoveWithGravity(&this->actor);
            ObjKibako_SetupThrown(this);
            this->actor.flags &= ~ACTOR_FLAG_MM_CAN_PRESS_SWITCH;
        }
        Actor_UpdateBgCheckInfo(play, &this->actor, 18.0f, 15.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR | UPDBGCHECKINFO_FLAG_MM_40);
    } else {
        pos.x = this->actor.world.pos.x;
        pos.y = this->actor.world.pos.y + 20.0f;
        pos.z = this->actor.world.pos.z;
        this->actor.floorHeight = BgCheck_EntityRaycastDown4(&play->colCtx, &this->actor.floorPoly, &bgId, &this->actor, &pos);
    }
}

void ObjKibako_SetupThrown(Actor_ObjKibako* this) {
    f32 temp;

    temp = (Rand_ZeroOne() - 0.5f) * 1000.0f;
    D_80927388 = temp;
    D_80927384 = (Rand_ZeroOne() - 2.0f) * 1500.0f;
    D_8092738C = temp * 3.0f;
    this->timer = 80;
    this->actionFunc = ObjKibako_Thrown;
}

void ObjKibako_Thrown(Actor_ObjKibako* this, GameState_Play* play)
{
    s32 atHit;

    atHit = (this->collider.base.atFlags & AT_HIT) != 0;
    if (atHit) {
        this->collider.base.atFlags &= ~AT_HIT;
    }
    func_80926394(this, play);
    if (this->timer > 0) {
        this->timer--;
    }
    if ((this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_WALL)) || atHit ||
        (this->timer <= 0)) {
        ObjKibako_AirBreak(this, play);
        ObjKibako_SpawnCollectible(this, play);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        Actor_Kill(&this->actor);
    } else {
        if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) {
            ObjKibako_WaterBreak(this, play);
            ObjKibako_SpawnCollectible(this, play);
            SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
            SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EV_DIVE_INTO_WATER_L);
            Actor_Kill(&this->actor);
        } else {
            if (this->actor.velocity.y < -0.05f) {
                this->actor.gravity = -2.3f;
            }
            Actor_MoveWithGravity(&this->actor);
            Math_StepToS(&D_80927384, 0, 0xA0);
            Math_StepToS(&D_8092738C, D_80927388, 0xA0);
            this->actor.shape.rot.x = (s16)(this->actor.shape.rot.x + D_80927384);
            this->actor.shape.rot.y = (s16)(this->actor.shape.rot.y + D_8092738C);
            Actor_UpdateBgCheckInfo(play, &this->actor, 18.0f, 15.0f, 0.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR | UPDBGCHECKINFO_FLAG_MM_40);
            Collider_UpdateCylinder(&this->actor, &this->collider);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

void ObjKibako_Update(Actor_ObjKibako* this, GameState_Play* play)
{
    this->actionFunc(this, play);
    ObjKibako_SetShadow(this);
}

void ObjKibako_Draw(Actor_ObjKibako* this, GameState_Play* play)
{
    ObjKibako_DrawWithTexture(this, play, (u32)sDisplayLists[KIBAKO_BANK_INDEX(this)], sTextures[KIBAKO_BANK_INDEX(this)]);
}
#endif

ActorProfile Obj_Kibako_InitVars =
{
    AC_OBJ_KIBAKO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT,
    sizeof(Actor_ObjKibako),
    (ActorFunc)ObjKibako_Init,
    (ActorFunc)ObjKibako_Destroy,
    (ActorFunc)ObjKibako_Update,
    NULL,
};

OVL_ACTOR_INFO(AC_OBJ_KIBAKO, Obj_Kibako_InitVars);
