#include <combo/draw.h>
#include "En_Wonder_Item.h"

#define FLAGS 0

void EnWonderItem_Init(Actor* thisx, PlayState* play);
void EnWonderItem_Destroy(Actor* thisx, PlayState* play);
void EnWonderItem_Update(Actor* thisx, PlayState* play);

void EnWonderItem_MultitagFree(EnWonderItem* this, PlayState* play);
void EnWonderItem_ProximityDrop(EnWonderItem* this, PlayState* play);
void EnWonderItem_InteractSwitch(EnWonderItem* this, PlayState* play);
void EnWonderItem_ProximitySwitch(EnWonderItem* this, PlayState* play);
void EnWonderItem_MultitagOrdered(EnWonderItem* this, PlayState* play);
void EnWonderItem_BombSoldier(EnWonderItem* this, PlayState* play);
void EnWonderItem_RollDrop(EnWonderItem* this, PlayState* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 20, 30, 0, { 0, 0, 0 } },
};

static Vec3f sTagPointsFree[9];
static Vec3f sTagPointsOrdered[9];

void EnWonderItem_Destroy(Actor* thisx, PlayState* play) {
    EnWonderItem* this = (EnWonderItem*)thisx;

    if ((this->collider.dim.radius != 0) || (this->collider.dim.height != 0)) {
        Collider_DestroyCylinder(play, &this->collider);
    }
}

static void EnWonderItem_DropCustom(EnWonderItem* this, PlayState* play, s16 params)
{
    ComboItemQuery q;

    if (params & 0x8000)
    {
        Xflag_ItemQuery(&q, this->xflag, GI_NONE);
        Item_AddWithDecoy(play, &q);
        Xflag_Set(this->xflag);
    }
    else
        EnItem00_DropCustomEx(play, &this->actor.world.pos, this->xflag, XFDROP_NONE);
}

static void EnWonderItem_ItemDropCollectible(EnWonderItem* this, PlayState* play, const Vec3f* pos, s16 params)
{
    /* Check for vanilla */
    if (this->xflag == XFLAGID_NONE)
        Item_DropCollectible(play, pos, params);
    else
        EnWonderItem_DropCustom(this, play, params);
}

static void EnWonderItem_ItemDropCollectibleRandom(EnWonderItem* this, PlayState* play, const Vec3f* pos, s16 params)
{
    /* Check for vanilla */
    if (this->xflag == XFLAGID_NONE)
        Item_DropCollectibleRandom(play, NULL, pos, params);
    else
        EnWonderItem_DropCustom(this, play, params);
}

void EnWonderItem_DropCollectible(EnWonderItem* this, PlayState* play, s32 autoCollect) {
    static s16 dropTable[] = {
        ITEM00_NUT,            ITEM00_HEART_PIECE,  ITEM00_MAGIC_LARGE,   ITEM00_MAGIC_SMALL,
        ITEM00_RECOVERY_HEART, ITEM00_ARROWS_5,     ITEM00_ARROWS_10,     ITEM00_ARROWS_30,
        ITEM00_RUPEE_GREEN,    ITEM00_RUPEE_BLUE,   ITEM00_RUPEE_RED,     ITEM00_FLEXIBLE,
    };
    s32 i;
    s32 randomDrop;

    Sfx_PlaySfxCentered(NA_SE_SY_GET_ITEM);

    if (this->dropCount == 0) {
        this->dropCount++;
    }
    for (i = this->dropCount; i > 0; i--) {
        if (this->itemDrop < WONDERITEM_DROP_RANDOM) {
            if ((this->itemDrop == WONDERITEM_DROP_FLEXIBLE) || !autoCollect) {
                EnWonderItem_ItemDropCollectible(this, play, &this->actor.world.pos, dropTable[this->itemDrop]);
            } else {
                EnWonderItem_ItemDropCollectible(this, play, &this->actor.world.pos, dropTable[this->itemDrop] | 0x8000);
            }
        } else {
            randomDrop = this->itemDrop - WONDERITEM_DROP_RANDOM;
            if (!autoCollect) {
                EnWonderItem_ItemDropCollectibleRandom(this, play, &this->actor.world.pos, randomDrop);
            } else {
                EnWonderItem_ItemDropCollectibleRandom(this, play, &this->actor.world.pos, randomDrop | 0x8000);
            }
        }
    }
    if (this->switchFlag >= 0) {
        Flags_SetSwitch(play, this->switchFlag);
    }
    Actor_Kill(&this->actor);
}

static void EnWonderItem_DrawGlitter(EnWonderItem* this, PlayState* play)
{
    ComboItemOverride o;

    /* Check xflag */
    if (Xflag_Get(this->xflag))
    {
        this->actor.draw = NULL;
        return;
    }

    Xflag_ItemOverride(&o, this->xflag, GI_NONE);
    Draw_GlitterGi(play, &this->actor, o.gi, o.cloakGi);
}

void EnWonderItem_Init(Actor* thisx, PlayState* play) {
    static u32 damageFlags[] = {
        DMG_SLASH | DMG_DEKU_STICK,
        DMG_ARROW,
        DMG_HAMMER_SWING,
        DMG_EXPLOSIVE,
        DMG_SLINGSHOT,
        DMG_BOOMERANG,
        DMG_HOOKSHOT,
    };

    s16 colTypeIndex;
    EnWonderItem* this = (EnWonderItem*)thisx;
    s16 rotZover10;
    s16 tagIndex;

    this->xflag = Xflag_InitShuffled(thisx, play);
    if (play->sceneId == SCE_OOT_CASTLE_COURTYARD && Config_Flag(CFG_OOT_SKIP_ZELDA))
        this->xflag = XFLAGID_NONE;

    if (this->xflag != XFLAGID_NONE)
        this->actor.draw = EnWonderItem_DrawGlitter;

    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;

    this->wonderMode = PARAMS_GET_U(this->actor.params, 11, 5);
    this->itemDrop = PARAMS_GET_U(this->actor.params, 6, 5);
    this->switchFlag = PARAMS_GET_U(this->actor.params, 0, 6);
    if (this->switchFlag == 0x3F) {
        this->switchFlag = -1;
    }
    this->actor.attentionRangeType = ATTENTION_RANGE_1;
    if ((this->switchFlag >= 0) && Flags_GetSwitch(play, this->switchFlag) && this->xflag == XFLAGID_NONE) {
        Actor_Kill(&this->actor);
        return;
    }
    switch (this->wonderMode) {
        case WONDERITEM_MULTITAG_FREE:
            this->numTagPoints = this->actor.world.rot.z & 0xF;
            rotZover10 = 0;
            if (this->actor.world.rot.z >= 10) {
                rotZover10 = this->actor.world.rot.z / 10;
                this->timerMod = rotZover10 * 20;
            }
            this->numTagPoints = this->actor.world.rot.z - rotZover10 * 10;
            // i.e timerMod = rot.z / 10 seconds, numTagPoints = rot.z % 10
            this->updateFunc = EnWonderItem_MultitagFree;
            break;
        case WONDERITEM_TAG_POINT_FREE:
            tagIndex = this->actor.world.rot.z & 0xFF;
            sTagPointsFree[tagIndex] = this->actor.world.pos;
            Actor_Kill(&this->actor);
            break;
        case WONDERITEM_PROXIMITY_DROP:
            this->dropCount = this->actor.world.rot.z & 0xFF;
            this->updateFunc = EnWonderItem_ProximityDrop;
            break;
        case WONDERITEM_INTERACT_SWITCH:
            colTypeIndex = this->actor.world.rot.z & 0xFF;
            Collider_InitCylinder(play, &this->collider);
            Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
            this->collider.elem.acDmgInfo.dmgFlags = damageFlags[colTypeIndex];
            this->collider.dim.radius = 20;
            this->collider.dim.height = 30;
            this->updateFunc = EnWonderItem_InteractSwitch;
            break;
        case WONDERITEM_UNUSED:
            break;
        case WONDERITEM_MULTITAG_ORDERED:
            this->numTagPoints = this->actor.world.rot.z & 0xF;
            rotZover10 = 0;
            if (this->actor.world.rot.z >= 10) {
                rotZover10 = this->actor.world.rot.z / 10;
                this->timerMod = rotZover10 * 20;
            }
            this->numTagPoints = this->actor.world.rot.z - rotZover10 * 10;
            // i.e timerMod = rot.z / 10 seconds, numTagPoints = rot.z % 10
            this->updateFunc = EnWonderItem_MultitagOrdered;
            break;
        case WONDERITEM_TAG_POINT_ORDERED:
            tagIndex = this->actor.world.rot.z & 0xFF;
            sTagPointsOrdered[tagIndex] = this->actor.world.pos;
            Actor_Kill(&this->actor);
            break;
        case WONDERITEM_PROXIMITY_SWITCH:
            this->updateFunc = EnWonderItem_ProximitySwitch;
            break;
        case WONDERITEM_BOMB_SOLDIER:
            Collider_InitCylinder(play, &this->collider);
            Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
            this->collider.elem.acDmgInfo.dmgFlags = DMG_SLINGSHOT;
            this->unkPos = this->actor.world.pos;
            this->collider.dim.radius = 35;
            this->collider.dim.height = 75;
            this->updateFunc = EnWonderItem_BombSoldier;
            break;
        case WONDERITEM_ROLL_DROP:
            this->dropCount = this->actor.world.rot.z & 0xFF;
            this->updateFunc = EnWonderItem_RollDrop;
            break;
        default:
            Actor_Kill(&this->actor);
            break;
    }
}

void EnWonderItem_MultitagFree(EnWonderItem* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 prevTagFlags = this->tagFlags;
    s32 i;
    s32 mask;
    f32 dx;
    f32 dy;
    f32 dz;

    for (i = 0, mask = 1; i < this->numTagPoints; i++, mask <<= 1) {
        if (!(prevTagFlags & mask)) {
            dx = player->actor.world.pos.x - sTagPointsFree[i].x;
            dy = player->actor.world.pos.y - sTagPointsFree[i].y;
            dz = player->actor.world.pos.z - sTagPointsFree[i].z;

            if (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 50.0f) {
                this->tagFlags |= mask;
                this->tagCount++;
                this->timer = this->timerMod + 81;
                return;
            }
        }
    }
    if (this->timer == 1) {
        Actor_Kill(&this->actor);
        return;
    }
    if (this->tagCount == this->numTagPoints) {
        if (this->switchFlag >= 0) {
            Flags_SetSwitch(play, this->switchFlag);
        }
        EnWonderItem_DropCollectible(this, play, true);
    }
}

void EnWonderItem_ProximityDrop(EnWonderItem* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((this->actor.xzDistToPlayer < 50.0f) && (fabsf(this->actor.world.pos.y - player->actor.world.pos.y) < 30.0f)) {
        EnWonderItem_DropCollectible(this, play, true);
    }
}

void EnWonderItem_InteractSwitch(EnWonderItem* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        EnWonderItem_DropCollectible(this, play, false);
    }
}

void EnWonderItem_ProximitySwitch(EnWonderItem* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((this->actor.xzDistToPlayer < 50.0f) && (fabsf(this->actor.world.pos.y - player->actor.world.pos.y) < 30.0f)) {
        if (this->switchFlag >= 0) {
            Flags_SetSwitch(play, this->switchFlag);
        }
        Actor_Kill(&this->actor);
    }
}

void EnWonderItem_MultitagOrdered(EnWonderItem* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 prevTagFlags = this->tagFlags;
    s32 i;
    s32 mask;
    f32 dx;
    f32 dy;
    f32 dz;

    for (i = 0, mask = 1; i < this->numTagPoints; i++, mask <<= 1) {
        if (!(prevTagFlags & mask)) {
            dx = player->actor.world.pos.x - sTagPointsOrdered[i].x;
            dy = player->actor.world.pos.y - sTagPointsOrdered[i].y;
            dz = player->actor.world.pos.z - sTagPointsOrdered[i].z;

            if (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 50.0f) {
                if (prevTagFlags & mask) {
                    return;
                } else if (i == this->nextTag) {
                    this->tagFlags |= mask;
                    this->tagCount++;
                    this->nextTag++;
                    this->timer = this->timerMod + 81;
                    return;
                } else {
                    Actor_Kill(&this->actor);
                    return;
                }
            }
        }
    }
    if (this->timer == 1) {
        Actor_Kill(&this->actor);
        return;
    }
    if (this->tagCount == this->numTagPoints) {
        EnWonderItem_DropCollectible(this, play, true);
    }
}

void EnWonderItem_BombSoldier(EnWonderItem* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HEISHI2, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.yawTowardsPlayer, 0, 9);
        if (this->switchFlag >= 0) {
            Flags_SetSwitch(play, this->switchFlag);
        }
        Actor_Kill(&this->actor);
    }
}

void EnWonderItem_RollDrop(EnWonderItem* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((this->actor.xzDistToPlayer < 50.0f) && (player->invincibilityTimer < 0) &&
        (fabsf(this->actor.world.pos.y - player->actor.world.pos.y) < 30.0f)) {
        EnWonderItem_DropCollectible(this, play, true);
    }
}

void EnWonderItem_Update(Actor* thisx, PlayState* play) {
    EnWonderItem* this = (EnWonderItem*)thisx;

    if (Xflag_Get(this->xflag))
        this->xflag = XFLAGID_NONE;

    if (this->timer != 0) {
        this->timer--;
    }
    this->updateFunc(this, play);

    if (this->wonderMode == WONDERITEM_UNUSED) {
        Actor_SetFocus(&this->actor, this->unkHeight);
    }
    if ((this->wonderMode == WONDERITEM_INTERACT_SWITCH) || (this->wonderMode == WONDERITEM_BOMB_SOLDIER)) {
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}

static ActorProfile En_Wonder_Item_Profile = {
    /**/ ACTOR_EN_WONDER_ITEM,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(EnWonderItem),
    /**/ EnWonderItem_Init,
    /**/ EnWonderItem_Destroy,
    /**/ EnWonderItem_Update,
    /**/ NULL,
};

OVL_INFO_ACTOR(ACTOR_EN_WONDER_ITEM, En_Wonder_Item_Profile);
