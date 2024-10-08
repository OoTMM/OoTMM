#include <combo.h>
#include <combo/item.h>
#include <combo/global.h>
#include "Obj_Tsubo.h"

#define FLAGS (ACTOR_FLAG_MM_10 | ACTOR_FLAG_MM_800000 | ACTOR_FLAG_MM_CAN_PRESS_SWITCH)

#define OBJ_TSUBO_P000F(thisx)      ((thisx)->params & 0x0F)
#define OBJ_TSUBO_P001F(thisx)      ((thisx)->params & 0x1F)
#define OBJ_TSUBO_P003F(thisx)      ((thisx)->params & 0x3F)
#define OBJ_TSUBO_P0010(thisx)      (((thisx)->params >> 4) & 1)
#define OBJ_TSUBO_GET_TYPE(thisx)   (((thisx)->params >> 7) & 3)
#define OBJ_TSUBO_PFE00(thisx)      (((thisx)->params >> 9) & 0x7F)
#define OBJ_TSUBO_ZROT(thisx)       ((thisx)->home.rot.z)

typedef enum
{
    OBJ_TSUBO_TYPE_0,
    OBJ_TSUBO_TYPE_1,
    OBJ_TSUBO_TYPE_2,
    OBJ_TSUBO_TYPE_3
}
ObjTsuboType;

void ObjTsubo_Init(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_Destroy(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_Update(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_Draw(Actor_ObjTsubo* this, GameState_Play* play);

void ObjTsubo_PotBreak1(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_MagicPotBreak1(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_PotBreak2(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_MagicPotBreak2(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_PotBreak3(Actor_ObjTsubo* this, GameState_Play* play);
void ObjTsubo_MagicPotBreak3(Actor_ObjTsubo* this, GameState_Play* play);
void func_80928914(Actor_ObjTsubo* this);
void func_80928928(Actor_ObjTsubo* this, GameState_Play* play);
void func_809289B4(Actor_ObjTsubo* this);
void func_809289E4(Actor_ObjTsubo* this, GameState_Play* play);
void func_80928D6C(Actor_ObjTsubo* this);
void func_80928D80(Actor_ObjTsubo* this, GameState_Play* play);
void func_80928E74(Actor_ObjTsubo* this);
void func_80928F18(Actor_ObjTsubo* this, GameState_Play* play);
void func_809291DC(Actor_ObjTsubo* this);
void func_8092926C(Actor_ObjTsubo* this, GameState_Play* play);

s16 D_80929500 = 0;
s16 D_80929504 = 0;
s16 D_80929508 = 0;
s16 D_8092950C = 0;

typedef struct
{
    s16                         objectId;
    f32                         scale;
    Gfx*                        modelDL;
    Gfx*                        shardDL;
    s16                         radius;
    s16                         height;
    Actor_ObjTsubo_ActionFunc   breakPot1;
    Actor_ObjTsubo_ActionFunc   breakPot2;
    Actor_ObjTsubo_ActionFunc   breakPot3;
}
ObjTsuboData;

ObjTsuboData sPotTypeData[4] =
{
    { OBJECT_GAMEPLAY_DANGEON_KEEP, 0.197f,         (Gfx*)0x05017ea0, (Gfx*)0x05018090, 12, 32, ObjTsubo_PotBreak1, ObjTsubo_PotBreak2, ObjTsubo_PotBreak3 },
    { OBJECT_RACETSUBO,             0.29549998f,    (Gfx*)0x06000278, (Gfx*)0x06001610, 18, 45, ObjTsubo_MagicPotBreak1, ObjTsubo_MagicPotBreak2, ObjTsubo_MagicPotBreak3 },
    { OBJECT_TSUBO,                 0.197f,         (Gfx*)0x060017c0, (Gfx*)0x06001960, 12, 32, ObjTsubo_PotBreak1, ObjTsubo_PotBreak2, ObjTsubo_PotBreak3 },
    { OBJECT_GAMEPLAY_DANGEON_KEEP, 0.197f,         (Gfx*)0x05017ea0, (Gfx*)0x05018090, 12, 32, ObjTsubo_PotBreak1, ObjTsubo_PotBreak2, ObjTsubo_PotBreak3 },
};

static ColliderCylinderInit sCylinderInit =
{
    {
        COLTYPE_HARD,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00400000, 0x00, 0x02 },
        { 0x05CBFFBE, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 12, 30, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_CONTINUE),  ICHAIN_F32_DIV1000(minVelocityY, -20000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE), ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 100, ICHAIN_STOP),
};

static void ObjTsubo_Alias(Actor_ObjTsubo* this)
{
    Xflag* xflag;

    xflag = &this->xflag;

    /* Check for zora game pots */
    if (xflag->sceneId == SCE_MM_ZORA_CAPE && this->actor.params == 0x13f)
    {
        xflag->sceneId = SCE_MM_EXTRA;
        xflag->setupId = 0;
        xflag->roomId = 0;
        switch ((s16)(this->actor.home.pos.x))
        {
        case 0x04D0: xflag->id = 0; break;
        case 0x0560: xflag->id = 1; break;
        case 0x05BD: xflag->id = 2; break;
        case 0x056F: xflag->id = 3; break;
        case 0x0543: xflag->id = 4; break;
        default: UNREACHABLE(); break;
        }
        return;
    }

    switch (xflag->sceneId)
    {
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        xflag->sceneId = SCE_MM_SOUTHERN_SWAMP;
        xflag->id += 3;
        break;
    case SCE_MM_GORON_SHRINE:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            switch (xflag->roomId)
            {
            case 0: xflag->id -= 2; break;
            case 1: xflag->id += 3; break;
            }
        }
        break;
    case SCE_MM_GREAT_BAY_COAST:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            switch (xflag->id)
            {
            case 13: xflag->id = 123; break;
            case 14: xflag->id = 124; break;
            case 15: xflag->id = 125; break;
            case 16: xflag->id = 133; break;
            case 17: xflag->id = 119; break;
            case 18: xflag->id = 129; break;
            case 19: xflag->id = 118; break;
            case 20: xflag->id = 128; break;
            case 21: xflag->id = 130; break;
            case 22: xflag->id = 120; break;
            case 23: xflag->id = 121; break;
            case 24: xflag->id = 62;  break;
            case 25: xflag->id = 61;  break;
            case 26: xflag->id = 63;  break;
            case 27: xflag->id = 131;  break;
            default: UNREACHABLE(); break;
            }
        }
        break;
    case SCE_MM_ZORA_CAPE:
        if (xflag->setupId == 1)
        {
            xflag->setupId = 0;
            xflag->id += 9;
        }
        break;
    case SCE_MM_WOODFALL:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            xflag->id -= 36;
        }
        break;
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
        xflag->sceneId = SCE_MM_MOUNTAIN_VILLAGE_WINTER;
        xflag->id = 38;
        break;
    case SCE_MM_GORON_RACETRACK:
        xflag->setupId = 0;
        break;
    }
}

static int ObjTsubo_IsShuffled(Actor_ObjTsubo* this)
{
    if (!this->isExtended || comboXflagsGet(&this->xflag))
        return 0;
    return 1;
}

int func_809275C0(Actor_ObjTsubo* this, GameState_Play* play)
{
    s32 chestFlag = -1;
    s32 skulltulaParams = (OBJ_TSUBO_P001F(&this->actor) << 2) | 0xFF01;

    if (skulltulaParams & 3)
    {
        chestFlag = ((skulltulaParams >> 2) & 0xff);
    }

    return ((chestFlag < 0) || !Flags_GetTreasure(play, chestFlag));
}

void ObjTsubo_SpawnCollectibleFlexible(Actor_ObjTsubo* this, GameState_Play* play)
{
    if (ObjTsubo_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->actor.world.pos, &this->xflag);
        return;
    }

    if (!OBJ_TSUBO_P0010(&this->actor) && (OBJ_TSUBO_ZROT(&this->actor) != 2))
    {
        Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, OBJ_TSUBO_P000F(&this->actor) << 4);
    }
}

void ObjTsubo_SpawnCollectible(Actor_ObjTsubo* this, GameState_Play* play)
{
    s32 itemDrop;

    if (ObjTsubo_IsShuffled(this))
    {
        EnItem00_DropCustom(play, &this->actor.world.pos, &this->xflag);
        return;
    }

    if (!this->unk_197 && (OBJ_TSUBO_ZROT(&this->actor) != 2))
    {
        itemDrop = Item_CollectibleDropTable(OBJ_TSUBO_P003F(&this->actor));
        if (itemDrop > ITEM00_NO_DROP)
        {
            Item_DropCollectible(play, &this->actor.world.pos, (OBJ_TSUBO_PFE00(&this->actor) << 8) | itemDrop);
            this->unk_197 = TRUE;
        }
    }
}

void ObjTsubo_SpawnBoes(Actor_ObjTsubo* this, GameState_Play* play, s32 arg2)
{
    Actor_Spawn(&play->actorCtx, play, AC_EN_MKK, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 2);
}

void ObjTsubo_SpawnGoldSkulltula(Actor_ObjTsubo* this, GameState_Play* play, s32 arg2)
{
    Actor* child;
    s32 params;

    if (func_809275C0(this, play))
    {
        params = (OBJ_TSUBO_P001F(&this->actor) << 2) | 0xFF01;
        child = Actor_Spawn(&play->actorCtx, play, AC_EN_SW, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, Rand_Next() >> 0x10, 0, params);
        if (child != NULL) {
            child->parent = &this->actor;
            child->velocity.y = 0.0f;
            child->speed = 0.0f;
        }
    }
}

void func_80927818(Actor_ObjTsubo* this, GameState_Play* play, s32 arg2)
{
    if (OBJ_TSUBO_ZROT(&this->actor) == 1)
    {
        ObjTsubo_SpawnBoes(this, play, arg2);
    }
    else if (OBJ_TSUBO_ZROT(&this->actor) == 2)
    {
        ObjTsubo_SpawnGoldSkulltula(this, play, arg2);
    }
}

int ObjTsubo_IsSceneNotGohtOrTwinmold(Actor_ObjTsubo* this, GameState_Play* play)
{
    return (play->sceneId != SCE_MM_LAIR_GOHT) && (play->sceneId != SCE_MM_LAIR_TWINMOLD);
}

void func_8092788C(Actor_ObjTsubo* this, GameState_Play* play)
{
    if (!this->unk_197 && (play->roomCtx.curRoom.num != this->homeRoom))
    {
        this->unk_197 = TRUE;
    }
}

void ObjTsubo_Init(Actor_ObjTsubo* this, GameState_Play* play)
{
    ComboItemOverride o;
    s32 type;
    s32 sp2C;

    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->actor.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    /* Fix the aliases */
    ObjTsubo_Alias(this);

    /* Detect xflags */
    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));

    type = OBJ_TSUBO_GET_TYPE(&this->actor);
    sp2C = OBJ_TSUBO_ZROT(&this->actor);

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Actor_SetScale(&this->actor, sPotTypeData[type].scale);
    this->actor.shape.rot.z = 0;
    this->actor.world.rot.z = 0;
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    this->collider.dim.radius = sPotTypeData[type].radius;
    this->collider.dim.height = sPotTypeData[type].height;
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->requiredObjectSlot = Object_GetSlot(&play->objectCtx, sPotTypeData[type].objectId);
    if (this->requiredObjectSlot <= -1)
    {
        Actor_Kill(&this->actor);
        return;
    }

    this->actor.shape.shadowScale = 1.8f;
    this->homeRoom = this->actor.room;
    if ((type != OBJ_TSUBO_TYPE_3) && (sp2C != 2))
    {
        if (Item_CanDropBigFairy(play, OBJ_TSUBO_P003F(&this->actor), OBJ_TSUBO_PFE00(&this->actor)))
        {
            this->unk_198 = TRUE;
        }
    }
    if ((type == OBJ_TSUBO_TYPE_3) || (sp2C != 2) || !func_809275C0(this, play))
    {
        this->unk_19a = -1;
    }
    func_80928914(this);
}

void ObjTsubo_Destroy(Actor_ObjTsubo* this, GameState_Play* play)
{
    Collider_DestroyCylinder(play, &this->collider);
}

void ObjTsubo_PotBreak1(Actor_ObjTsubo* this, GameState_Play* play)
{
    s16 rot;
    s32 i;
    s32 phi_s0;
    Vec3f pos;
    Vec3f velocity;
    ObjTsuboData* typeData = &sPotTypeData[OBJ_TSUBO_GET_TYPE(&this->actor)];
    f32 randf;
    f32 sin;
    f32 cos;
    f32 scale;

    for (i = 0, rot = 0; i < 18; rot += 0x4E20, i++)
    {
        sin = Math_SinS(rot);
        cos = Math_CosS(rot);
        pos.x = sin * 8.0f;
        pos.y = Rand_ZeroOne() * 12.0f + 2.0f;
        pos.z = cos * 8.0f;
        velocity.x = pos.x * 0.23f;
        velocity.y = Rand_ZeroOne() * 5.0f + 2.5f;
        velocity.z = pos.z * 0.23f;
        Math_Vec3f_Sum(&pos, &this->actor.world.pos, &pos);
        randf = Rand_ZeroOne();
        if (randf < 0.2f)
            phi_s0 = 0x60;
        else if (randf < 0.6f)
            phi_s0 = 0x40;
        else
            phi_s0 = 0x20;
        scale = Rand_ZeroOne() * 110.0f + 15.0f;
        EffectSsKakera_Spawn(play, &pos, &velocity, &this->actor.world.pos, -260, phi_s0, 20, 0, 0, scale, 0, 0, 50, -1, typeData->objectId, typeData->shardDL);
    }

    SpawnSomeDust(play, &this->actor.world.pos, 30.0f, 2, 20, 50, TRUE);
    SpawnSomeDust(play, &this->actor.world.pos, 30.0f, 2, 10, 80, TRUE);
}

void ObjTsubo_MagicPotBreak1(Actor_ObjTsubo* this, GameState_Play* play)
{
    s16 rot;
    s32 phi_s0;
    s32 i;
    ObjTsuboData* typeData = &sPotTypeData[OBJ_TSUBO_GET_TYPE(&this->actor)];
    Vec3f pos;
    Vec3f velocity;
    f32 randf;
    f32 sin;
    f32 cos;
    f32 scale;

    for (i = 0, rot = 0; i < 20; rot += 0x4E20, i++) {
        sin = Math_SinS(rot);
        cos = Math_CosS(rot);
        pos.x = sin * 10.0f;
        pos.y = Rand_ZeroOne() * 20.0f + 2.0f;
        pos.z = cos * 10.0f;
        velocity.x = pos.x * 0.3f;
        velocity.y = Rand_ZeroOne() * 10.0f + 2.0f;
        velocity.z = pos.z * 0.3f;
        Math_Vec3f_Sum(&pos, &this->actor.world.pos, &pos);
        randf = Rand_ZeroOne();
        if (randf < 0.2f) {
            phi_s0 = 0xE0;
        } else if (randf < 0.6f) {
            phi_s0 = 0xC0;
        } else {
            phi_s0 = 0xA0;
        }
        scale = Rand_ZeroOne() * 160.0f + 15.0f;
        EffectSsKakera_Spawn(play, &pos, &velocity, &this->actor.world.pos, -340, phi_s0, 20, 0, 0, scale, 0, 0, 50, -1, typeData->objectId, typeData->shardDL);
    }
    SpawnSomeDust(play, &this->actor.world.pos, 50.0f, 2, 40, 50, TRUE);
    SpawnSomeDust(play, &this->actor.world.pos, 50.0f, 2, 20, 80, TRUE);
}

void ObjTsubo_PotBreak2(Actor_ObjTsubo* this, GameState_Play* play2) {
    GameState_Play* play = play2;
    s16 rot;
    s32 i;
    s32 phi_s0;
    Vec3f pos;
    Vec3f velocity;
    Vec3f* worldPos = &this->actor.world.pos;
    ObjTsuboData* typeData = &sPotTypeData[OBJ_TSUBO_GET_TYPE(&this->actor)];
    f32 sin;
    f32 cos;
    f32 scale;

    pos.y = worldPos->y + this->actor.depthInWater;

    for (rot = 0, i = 0; i < 5; i++, rot += 0x10000 / 5)
    {
        pos.x = worldPos->x + Math_SinS((s32)(Rand_ZeroOne() * 6000) + rot) * 15.0f;
        pos.z = worldPos->z + Math_CosS((s32)(Rand_ZeroOne() * 6000) + rot) * 15.0f;
        EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 350);
    }

    pos.x = worldPos->x;
    pos.z = worldPos->z;
    EffectSsGRipple_Spawn(play, &pos, 200, 600, 0);
    for (i = 0, rot = 0; i < 13; rot += 0x4E20, i++) {
        sin = Math_SinS(rot);
        cos = Math_CosS(rot);
        pos.x = sin * 8.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cos * 8.0f;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        Math_Vec3f_Sum(&pos, worldPos, &pos);
        if (Rand_ZeroOne() < .2f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        scale = Rand_ZeroOne() * 105.0f + 10.0f;
        EffectSsKakera_Spawn(play, &pos, &velocity, worldPos, -170, phi_s0, 50, 5, 0, scale, 0, 0, 70, -1, typeData->objectId, typeData->shardDL);
    }
}

void ObjTsubo_MagicPotBreak2(Actor_ObjTsubo* this, GameState_Play* play2)
{
    GameState_Play* play = play2;
    s16 rot;
    s32 i;
    Vec3f pos;
    Vec3f velocity;
    ObjTsuboData* typeData = &sPotTypeData[OBJ_TSUBO_GET_TYPE(&this->actor)];
    Vec3f* worldPos = &this->actor.world.pos;
    f32 sin;
    f32 cos;
    s32 phi_s0;
    s16 scale;

    pos.y = this->actor.world.pos.y + this->actor.depthInWater;

    for (rot = 0, i = 0; i < 5; i++, rot += 0x10000 / 5)
    {
        pos.x = worldPos->x + Math_SinS((s32)(Rand_ZeroOne() * 6000) + rot) * 30.0f;
        pos.z = worldPos->z + Math_CosS((s32)(Rand_ZeroOne() * 6000) + rot) * 30.0f;
        EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 350);
    }
    pos.x = worldPos->x;
    pos.z = worldPos->z;
    EffectSsGRipple_Spawn(play, &pos, 300, 700, 0);
    for (i = 0, rot = 0; i < 15; rot += 0x4E20, i++)
    {
        sin = Math_SinS(rot);
        cos = Math_CosS(rot);
        pos.x = sin * 10.0f;
        pos.y = (Rand_ZeroOne() * 15.0f) + 2.0f;
        pos.z = cos * 10.0f;
        velocity.x = pos.x * 0.3f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.3f;
        Math_Vec3f_Sum(&pos, worldPos, &pos);
        if (Rand_ZeroOne() < 0.2f) {
            phi_s0 = 0xC0;
        } else {
            phi_s0 = 0xA0;
        }
        scale = (Rand_ZeroOne() * 150.0f) + 10.0f;
        EffectSsKakera_Spawn(play, &pos, &velocity, worldPos, -170, phi_s0, 50, 5, 0, scale, 0, 0, 70, -1, typeData->objectId, typeData->shardDL);
    }
}

void ObjTsubo_PotBreak3(Actor_ObjTsubo* this, GameState_Play* play)
{
    s16 rot;
    s32 phi_s0;
    Vec3f pos;
    Vec3f velocity;
    ObjTsuboData* typeData = &sPotTypeData[OBJ_TSUBO_GET_TYPE(&this->actor)];
    f32 randf;
    f32 temp_f20;
    f32 cos;
    f32 sin;
    s32 i;

    for (i = 0, rot = 0; i < 13; i++, rot += 0x4E20)
    {
        randf = Rand_ZeroOne();
        temp_f20 = (1.0f - SQ(randf)) * 4.8f + 3.2f;
        sin = Math_SinS(rot);
        cos = Math_CosS(rot);

        pos.x = sin * temp_f20;
        pos.y = (Rand_ZeroOne() * 15.0f) + 2.0f;
        pos.z = cos * temp_f20;

        velocity.x = pos.x * 0.4f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 4.0f;
        velocity.z = pos.z * 0.4f;

        Math_Vec3f_Sum(&pos, &this->actor.world.pos, &pos);

        if (Rand_ZeroOne() < 0.2f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }

        EffectSsKakera_Spawn(play, &pos, &velocity, &this->actor.world.pos, -170, phi_s0, 50, 3, 0, (Rand_ZeroOne() * 105.0f) + 10.0f, 0, 0, 70, -1, typeData->objectId, typeData->shardDL);
    }

    for (i = 0; i < 7; i++)
    {
        EffectSsBubble_Spawn(play, &this->actor.world.pos, 20.0f, 30.0f, 40.0f, (Rand_ZeroOne() * 0.06f) + 0.09f);
        EffectSsBubble_Spawn(play, &this->actor.world.pos, 10.0f, 10.0f, 10.0f, (Rand_ZeroOne() * 0.08f) + 0.09f);
    }
}

void ObjTsubo_MagicPotBreak3(Actor_ObjTsubo* this, GameState_Play* play)
{
}

void func_80928914(Actor_ObjTsubo* this)
{
    this->actionFunc = func_80928928;
}

void func_80928928(Actor_ObjTsubo* this, GameState_Play* play)
{
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 15.0f, 15.0f, 0.0f, 0x44);
    if (Object_IsLoaded(&play->objectCtx, this->requiredObjectSlot))
    {
        this->actor.objectSlot = this->requiredObjectSlot;
        func_809289B4(this);
    }
}

void func_809289B4(Actor_ObjTsubo* this)
{
    this->actor.draw = ObjTsubo_Draw;
    this->actor.flags |= 0x10;
    this->unk_195 = FALSE;
    this->actionFunc = func_809289E4;
}

void func_809289E4(Actor_ObjTsubo* this, GameState_Play* play)
{
    ObjTsuboData* typeData;
    s32 type;
    s32 acHit;

    type = OBJ_TSUBO_GET_TYPE(&this->actor);
    acHit = (this->collider.base.acFlags & AC_HIT) != 0;

    if (acHit)
    {
        this->collider.base.acFlags &= ~AC_HIT;
    }
    if (this->collider.base.ocFlags1 & OC1_HIT)
    {
        this->collider.base.ocFlags1 &= ~OC1_HIT;
    }
    if (Actor_HasParent(&this->actor, play))
    {
        this->actor.room = -1;
        this->actor.flags |= 0x10;
        if ((type != OBJ_TSUBO_TYPE_3) && Item_CollectibleDropTable2(OBJ_TSUBO_P003F(&this->actor)))
        {
            ObjTsubo_SpawnCollectible(this, play);
        }
        func_80927818(this, play, 0);
        //! @bug: This function should only pass Player*: it uses *(this + 0x153), which is meant to be
        //! player->currentMask, but in this case is garbage in the collider
        Player_PlaySfx((Actor_Player*)&this->actor, NA_SE_PL_PULL_UP_POT);
        func_80928D6C(this);
    }
    else if ((this->unk_19b != 0) || (acHit && (this->collider.elem.acHitElem->atDmgInfo.dmgFlags & 0x058BFFBC)))
    {
        typeData = &sPotTypeData[type];
        this->unk_19b = 0;
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER) && (this->actor.depthInWater > 15.0f))
        {
            typeData->breakPot3(this, play);
        }
        else
        {
            typeData->breakPot1(this, play);
        }
        if (type == OBJ_TSUBO_TYPE_3)
        {
            ObjTsubo_SpawnCollectibleFlexible(this, play);
        }
        else
        {
            ObjTsubo_SpawnCollectible(this, play);
        }
        func_80927818(this, play, 1);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        if (ObjTsubo_IsSceneNotGohtOrTwinmold(this, play))
        {
            Actor_Kill(&this->actor);
        }
        else
        {
            func_809291DC(this);
        }
    } else {
        if (!this->unk_195) {
            Actor_MoveWithGravity(&this->actor);
            Actor_UpdateBgCheckInfo(play, &this->actor, 15.0f, 15.0f, 0.0f, 0x44);
            if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId) == NULL))
            {
                this->unk_195 = TRUE;
                this->actor.flags &= ~0x10;
            }
        }
        if ((this->actor.xzDistToPlayer < 800.0f) || (gSaveContext.save.entrance == ENTR_MM_GORON_RACETRACK_ALT))
        {
            Collider_UpdateCylinder(&this->actor, &this->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            if (this->actor.xzDistToPlayer < 150.0f) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
                if (this->actor.xzDistToPlayer < 100.0f) {
                    s16 yawDiff = this->actor.yawTowardsPlayer - GET_PLAYER(play)->actor.world.rot.y;
                    s32 absYawDiff = ABS_ALT(yawDiff);

                    if (absYawDiff > (0x10000 / 3)) {
                        Actor_OfferGetItem(&this->actor, play, GI_NONE, 36.0f, 30.0f);
                    }
                }
            }
        }
    }
}

void func_80928D6C(Actor_ObjTsubo* this)
{
    this->actionFunc = func_80928D80;
}

void func_80928D80(Actor_ObjTsubo* this, GameState_Play* play)
{
    Vec3f pos;
    s32 bgId;

    func_8092788C(this, play);
    if (Actor_HasNoParent(&this->actor, play)) {
        this->actor.room = play->roomCtx.curRoom.num;
        Actor_MoveWithGravity(&this->actor);
        this->actor.flags &= ~ACTOR_FLAG_MM_CAN_PRESS_SWITCH;
        Actor_UpdateBgCheckInfo(play, &this->actor, 15.0f, 15.0f, 0.0f, 0x01 | 0x04 | 0x40 | 0x80);
        func_80928E74(this);
    } else {
        pos.x = this->actor.world.pos.x;
        pos.y = this->actor.world.pos.y + 20.0f;
        pos.z = this->actor.world.pos.z;
        this->actor.floorHeight = BgCheck_EntityRaycastDown4(&play->colCtx, &this->actor.floorPoly, &bgId, &this->actor, &pos);
    }
}

void func_80928E74(Actor_ObjTsubo* this)
{
    this->unk_194 = 80;
    this->actor.colChkInfo.mass = 180;
    D_80929500 = (Rand_ZeroOne() - 0.78f) * 4500.0f;
    D_80929508 = (Rand_ZeroOne() - 0.5f) * 2000.0f;
    D_80929504 = 0;
    D_8092950C = 0;
    this->actionFunc = func_80928F18;
}

void func_80928F18(Actor_ObjTsubo* this, GameState_Play* play)
{
    ObjTsuboData* typeData;
    s32 type = OBJ_TSUBO_GET_TYPE(&this->actor);
    s32 atHit = (this->collider.base.atFlags & AT_HIT) != 0;

    if (atHit) {
        this->collider.base.atFlags &= ~AT_HIT;
    }
    func_8092788C(this, play);

    if (this->unk_194 > 0) {
        this->unk_194--;
    }
    typeData = &sPotTypeData[type];
    if ((this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_WALL)) || atHit ||
        (this->unk_194 <= 0)) {
        typeData->breakPot1(this, play);
        if (type == OBJ_TSUBO_TYPE_3) {
            ObjTsubo_SpawnCollectibleFlexible(this, play);
        } else {
            ObjTsubo_SpawnCollectible(this, play);
        }
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        if (ObjTsubo_IsSceneNotGohtOrTwinmold(this, play)) {
            Actor_Kill(&this->actor);
            return;
        }

        func_809291DC(this);
    } else if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) {
        typeData->breakPot2(this, play);
        if (type == OBJ_TSUBO_TYPE_3) {
            ObjTsubo_SpawnCollectibleFlexible(this, play);
        } else {
            ObjTsubo_SpawnCollectible(this, play);
        }
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EV_DIVE_INTO_WATER_L);
        if (ObjTsubo_IsSceneNotGohtOrTwinmold(this, play)) {
            Actor_Kill(&this->actor);
            return;
        }

        func_809291DC(this);
    } else {
        Actor_MoveWithGravity(&this->actor);
        Math_StepToS(&D_80929504, D_80929500, 150);
        Math_StepToS(&D_8092950C, D_80929508, 150);
        this->actor.shape.rot.x += D_80929504;
        this->actor.shape.rot.y += D_8092950C;
        Actor_UpdateBgCheckInfo(play, &this->actor, 15.0f, 15.0f, 0.0f, 0x01 | 0x04 | 0x40 | 0x80);
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
}

void func_809291DC(Actor_ObjTsubo* this) {
    this->actor.draw = NULL;
    this->actor.flags |= 0x10;

    Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
    this->actor.shape.rot.z = 0;
    this->actor.world.rot.z = 0;
    this->unk_197 = FALSE;
    this->actor.shape.rot.x = this->actor.home.rot.x;
    this->actor.world.rot.x = this->actor.home.rot.x;
    this->actor.velocity.y = 0.0f;
    this->actor.speed = 0.0f;
    this->actor.shape.rot.y = this->actor.home.rot.y;
    this->actor.world.rot.y = this->actor.home.rot.y;
    Actor_SetScale(&this->actor, 0.0f);
    this->unk_194 = 127;
    this->actionFunc = func_8092926C;
}

void func_8092926C(Actor_ObjTsubo* this, GameState_Play* play) {
    f32 scale;

    if (this->unk_194 > 0) {
        this->unk_194--;
        if (this->unk_194 == 0) {
            this->actor.draw = ObjTsubo_Draw;
        }
    } else {
        scale = sPotTypeData[OBJ_TSUBO_GET_TYPE(&this->actor)].scale;
        if (Math_StepToF(&this->actor.scale.x, scale, scale * 0.1f)) {
            this->actor.flags |= ACTOR_FLAG_MM_CAN_PRESS_SWITCH;
            func_809289B4(this);
        }
        this->actor.scale.z = this->actor.scale.y = this->actor.scale.x;
    }
}

void ObjTsubo_Update(Actor_ObjTsubo* this, GameState_Play* play)
{
    this->actionFunc(this, play);
    if (this->actor.draw == NULL) {
        this->actor.shape.shadowDraw = NULL;
    } else if (this->actor.projectedPos.z < 811.0f) {
        if (this->actor.projectedPos.z > 300.0f) {
            this->actor.shape.shadowAlpha = (811 - (s32)this->actor.projectedPos.z) >> 1;
            this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        } else if (this->actor.projectedPos.z > -10.0f) {
            this->actor.shape.shadowAlpha = 255;
            this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        } else {
            this->actor.shape.shadowDraw = NULL;
        }
    } else {
        this->actor.shape.shadowDraw = NULL;
    }
    if (!this->unk_197) {
        if (this->unk_198) {
            play->actorCtx.flags |= ACTORCTX_FLAG_3;
            this->actor.flags |= 0x10;
        }
        if (this->unk_19a >= 0) {
            if (this->unk_19a == 0) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_STALGOLD_ROLL);
                if (Rand_ZeroOne() < 0.1f) {
                    this->unk_19a = Rand_S16Offset(40, 80);
                } else {
                    this->unk_19a = 8;
                }
            } else {
                this->unk_19a--;
            }
        }
    }
}

void ObjTsubo_Draw(Actor_ObjTsubo* this, GameState_Play* play)
{
    ComboItemOverride o;
    int type;

    /* Get the default pot drawing */
    switch ((this->actor.params >> 7) & 3)
    {
    case 0:
    case 3:
        type = CSMC_POT_NORMAL_DANGEON;
        break;
    case 1:
        type = CSMC_POT_MAGIC;
        break;
    case 2:
        type = CSMC_POT_NORMAL;
        break;
    }

    if (ObjTsubo_IsShuffled(this))
        comboXflagItemOverride(&o, &this->xflag, 0);
    else
        o.gi = 0;

    csmcPotPreDraw(&this->actor, play, o.gi, type);
    Gfx_DrawDListOpa(play, sPotTypeData[OBJ_TSUBO_GET_TYPE(&this->actor)].modelDL);
}

ActorProfile ObjTsubo_InitVars =
{
    AC_OBJ_TSUBO,
    ACTORCAT_PROP,
    FLAGS,
    1,
    sizeof(Actor_ObjTsubo),
    (ActorFunc)ObjTsubo_Init,
    (ActorFunc)ObjTsubo_Destroy,
    (ActorFunc)ObjTsubo_Update,
    NULL,
};

OVL_ACTOR_INFO(AC_OBJ_TSUBO, ObjTsubo_InitVars);
