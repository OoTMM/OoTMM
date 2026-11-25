#include <combo.h>
#include <combo/actor_id.h>
#include <combo/actor_ovl.h>
#include <combo/common/animation.h>
#include <combo/effect.h>
#include <combo/matrix.h>
#include <combo/magic.h>
#include <combo/mm/actor_arms_hook.h>
#include <combo/mm/dark_link.h>
#include <combo/sfx.h>
#include <combo/custom.h>
#include <combo/enemy_native.h>

extern void FOREIGN_ENEMY_API_SFX_GENERAL(u16 sfxId, Vec3f* pos, u8 token, f32* frequency, f32* volume, s8* reverb);

static f32 sDarkLinkSfxFrequency = 0.7950898f;
static s8 sDarkLinkSfxReverb = 35;
static PlayerAgeProperties sDarkLinkAdultProperties;
static u8 sDarkLinkAdultPropertiesLoaded;

#define DARK_LINK_COMBAT_IDLE ((PlayerAnimationHeader*)0x0400d468)
#define DARK_LINK_PERCH_JUMP  ((PlayerAnimationHeader*)0x0400dcd8)
#define DARK_LINK_PERCH_SLASH ((PlayerAnimationHeader*)0x0400d868)
#define DARK_LINK_PERCH_SLASH_END ((PlayerAnimationHeader*)0x0400d870)

static void DarkLink_PlayOotVoice(Actor* actor, u16 sfxId) {
    FOREIGN_ENEMY_API_SFX_GENERAL(sfxId, &actor->projectedPos, 4, &sDarkLinkSfxFrequency, &gSfxDefaultFreqAndVolScale,
                                  &sDarkLinkSfxReverb);
}

typedef struct {
    Player player;
    Input input;
    ColliderCylinder body;
    u8 alpha;
    u8 recovery;
    u8 stun;
    u8 staggerTimer;
    u8 jumpTimer;
    s16 rollHeading;
    f32 perchHeight;
    u8 swordPose;
    u8 swordSweep;
    Vec3f swordPoints[4];
    u8 shieldPose;
    u8 collidersReady;
    u8 falling;
    u8 swordPerch;
    u8 perchSwung;
    u8 counter;
    u8 staggerCount;
    u8 lastSwordAnim;
    u8 active;
    u8 jumpCounter;
    u8 rolling;
    u8 bodyPose;
    f32 bodyLimbY[3];
    Vec3f safeGround;
    u8 hasSafeGround;
} MmDarkLink;

static ColliderCylinderInit sBody = {
    { COL_MATERIAL_HIT5, AT_NONE, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1, COLSHAPE_CYLINDER },
    { ELEM_MATERIAL_UNK1, { 0, 0, 0 }, { 0xffcfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_ON },
    { 12, 60, 0, { 0, 0, 0 } },
};

static void DarkLink_UpdateBody(MmDarkLink* enemy) {
    Player* fighter = &enemy->player;
    if (enemy->bodyPose == 7) {
        f32 feet = (enemy->bodyLimbY[1] + enemy->bodyLimbY[2]) * .5f;
        f32 head = enemy->bodyLimbY[0] + 10.f;
        f32 height = fabsf(head - feet);
        if (fighter->stateFlags1 & (1u << 22))
            height *= .8f;
        enemy->body.dim.height = height;
        enemy->body.dim.yShift = MIN(head, feet) + fighter->actor.shape.yOffset * fighter->actor.scale.y;
    }
    Collider_UpdateCylinder(&fighter->actor, &enemy->body);
}

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x1),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(1, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(0, 0x1),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(0, 0x1),
    DMG_ENTRY(1, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(4, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0xE),
    DMG_ENTRY(0, 0x6),
    DMG_ENTRY(3, 0xD),
    DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0),
    DMG_ENTRY(1, 0x0),
    DMG_ENTRY(4, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0x0),
    DMG_ENTRY(8, 0x0),
    DMG_ENTRY(4, 0x0),
    DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0x0),
    DMG_ENTRY(0, 0x0),
} };

s32 FOREIGN_ENEMY_API_COLLISION_AC(PlayState*, CollisionCheckContext*, Collider*);

static Actor* DarkLink_ProjectileThreat(PlayState* play, Actor* actor) {
    s32 (*cylinderIntersects)(f32, f32, f32, Vec3f*, Vec3f*, Vec3f*, Vec3f*, Vec3f*) = (void*)0x800e8784;
    for (Actor* shot = play->actorCtx.actors[ACTORCAT_ITEMACTION].first; shot; shot = shot->next) {
        if (!shot->update || shot->speed <= 0.f)
            continue;
        if (shot->id != ACTOR_EN_ARROW && (shot->id != ACTOR_ARMS_HOOK || !((Actor_ArmsHook*)shot)->timer))
            continue;
        f32 speed = shot->speed * 10.f;
        Vec3f end = shot->world.pos;
        end.x += speed * Math_SinS(shot->world.rot.y);
        end.y += shot->velocity.y + shot->gravity * 10.f;
        end.z += speed * Math_CosS(shot->world.rot.y);
        Vec3f near;
        Vec3f far;
        if (Math_Vec3f_DistXYZ(&actor->world.pos, &shot->world.pos) <= 4000.f &&
            cylinderIntersects(actor->colChkInfo.cylRadius, actor->colChkInfo.cylHeight, 0.f, &actor->world.pos,
                               &shot->world.pos, &end, &near, &far))
            return shot;
    }
    return NULL;
}

void comboEnemyRandomDrop(PlayState*, Actor*, Vec3f*, s16);
void enemySetOotDropFlag(Actor*, ColliderElement*, s32);

static void DarkLink_Init(Actor* actor, PlayState* play) {
    MmDarkLink* enemy = (MmDarkLink*)actor;
    Player* fighter = &enemy->player;
    fighter->transformation = MM_PLAYER_FORM_HUMAN;

    if (!sDarkLinkAdultPropertiesLoaded) {
        LoadFile(&sDarkLinkAdultProperties, CUSTOM_MM_ADULT_HUMAN_AGE_PROPERTIES_VROM,
                 sizeof(sDarkLinkAdultProperties));
        sDarkLinkAdultPropertiesLoaded = 1;
    }
    fighter->ageProperties = &sDarkLinkAdultProperties;
    fighter->csId = CS_ID_NONE;
    fighter->currentShield = 1;
    fighter->itemAction = fighter->heldItemAction = PLAYER_IA_SWORD_KOKIRI;
    fighter->heldItemId = ITEM_MM_SWORD_KOKIRI;
    for (int i = 0; i < 2; ++i)
        Effect_Add(play, &fighter->meleeWeaponEffectIndex[i], EFFECT_BLURE2, 0, 0, OverlayAddr(0x8085d30c));
    Effect_Add(play, &fighter->meleeWeaponEffectIndex[2], EFFECT_TIRE_MARK, 0, 0, OverlayAddr(0x8085d330));
    for (int i = 0; i < 3; ++i) {
        if (!Effect_GetByIndex(fighter->meleeWeaponEffectIndex[i])) {
            Actor_Kill(actor);
            return;
        }
    }
    ((void (*)(Player*, s32))0x80123bd4)(fighter, 2);
    play->playerInit(fighter, play, (FlexSkeletonHeader*)0x06004764);
    enemy->collidersReady = 1;
    play->func_18780(fighter, play);
    actor->gravity = -1.f;
    actor->minVelocityY = -20.f;
    actor->colChkInfo.damageTable = &sDamageTable;
    actor->colChkInfo.health = gSave.info.playerData.healthCapacity >> 3;
    actor->colChkInfo.cylRadius = 60;
    actor->colChkInfo.cylHeight = 100;
    fighter->cylinder.base.acFlags = 0;
    Collider_InitCylinder(play, &enemy->body);
    Collider_SetCylinder(play, &enemy->body, actor, &sBody);
    fighter->shieldCylinder.base.acFlags = 0;
    for (int i = 0; i < 2; ++i) {
        ColliderQuad* sword = &fighter->meleeWeaponQuads[i];
        sword->base.atFlags = AT_ON | AT_TYPE_ENEMY;
        sword->elem.atDmgInfo.damage = 8;
        sword->base.acFlags = AC_ON | AC_HARD | AC_TYPE_PLAYER;
        sword->base.colMaterial = COL_MATERIAL_METAL;
        sword->elem.acElemFlags = ACELEM_ON;
        sword->elem.acDmgInfo.dmgFlags = 0xffcfffff;
    }
    fighter->shieldQuad.base.atFlags = AT_ON | AT_TYPE_ENEMY;
    fighter->shieldQuad.base.acFlags = AC_ON | AC_HARD | AC_TYPE_PLAYER;
    fighter->shieldQuad.base.colMaterial = COL_MATERIAL_METAL;
    fighter->shieldQuad.elem.acDmgInfo.dmgFlags = 0xdfcfffff;
    enemy->alpha = 95;
    PlayerAnimation_PlayLoopSetSpeed(play, &fighter->skelAnime, DARK_LINK_COMBAT_IDLE, 0.f);
}

static void DarkLink_Destroy(Actor* actor, PlayState* play) {
    MmDarkLink* enemy = (MmDarkLink*)actor;
    Player* fighter = &enemy->player;
    comboMmDarkLinkPerch(actor, play, 0);
    for (int i = 0; i < 3; ++i) {
        if (Effect_GetByIndex(fighter->meleeWeaponEffectIndex[i]))
            Effect_Destroy(play, fighter->meleeWeaponEffectIndex[i]);
    }
    if (!enemy->collidersReady)
        return;
    Collider_DestroyCylinder(play, &enemy->body);
    Collider_DestroyCylinder(play, &fighter->cylinder);
    Collider_DestroyCylinder(play, &fighter->shieldCylinder);
    Collider_DestroyQuad(play, &fighter->meleeWeaponQuads[0]);
    Collider_DestroyQuad(play, &fighter->meleeWeaponQuads[1]);
    Collider_DestroyQuad(play, &fighter->shieldQuad);
}

static int DarkLink_Swing(MmDarkLink* enemy, PlayState* play, Player* player, s16* heading, f32* magnitude) {
    int lowHealth = gSave.info.playerData.health < 0x50;
    u8 delay = lowHealth ? 15 : 7;
    if (enemy->player.linearVelocity < 0.f || player->linearVelocity < 0.f ||
        (!enemy->swordPerch && (play->gameplayFrames & delay)))
        return 0;
    f32 noAttackChance = (lowHealth ? .3f : 0.f) + (enemy->alpha != 255 ? 2.f : 0.f);
    if (Rand_ZeroOne() < noAttackChance)
        return 0;
    if (enemy->swordPerch) {
        enemy->input.cur.button = BTN_B;
        return 1;
    }
    int swing = (int)(Rand_ZeroOne() * 7.f);
    if (swing == 1 || swing == 5) {
        *heading += 0x4000;
        *magnitude = 127.f;
    } else if (swing == 2 || swing == 6) {
        *heading -= 0x4000;
        *magnitude = 127.f;
    }
    enemy->input.cur.button = BTN_B;
    return 1;
}

static void DarkLink_OotPerchPoint(Player* player, Vec3f* perch) {
    s16 yaw = player->actor.shape.rot.y - 0x3e8;
    *perch = player->actor.world.pos;
    perch->x += 45.f * Math_SinS(yaw);
    perch->y += 26.3f;
    perch->z += 45.f * Math_CosS(yaw);
}

static int DarkLink_GetPerchPoint(Player* player, Vec3f* perch) {
    static const f32 lengths[] = { 3000.f, 3000.f, 4000.f, 5500.f };
    s32 sword = ((s32 (*)(Player*, s32))0x80124278)(player, player->heldItemAction);
    if (sword < 0 || sword >= 4)
        return 0;
    MtxF* hand = &player->leftHandMf;
    Vec3f base = {
        hand->xw + hand->xy * 400.f,
        hand->yw + hand->yy * 400.f,
        hand->zw + hand->zy * 400.f,
    };
    Vec3f blade = { hand->xx * lengths[sword], hand->yx * lengths[sword], hand->zx * lengths[sword] };
    f32 lengthSq = blade.x * blade.x + blade.y * blade.y + blade.z * blade.z;
    f32 horizontalSq = blade.x * blade.x + blade.z * blade.z;
    if (!(lengthSq >= 25.f && lengthSq <= 22500.f && horizontalSq >= 25.f) ||
        !(Math_Vec3f_DistXYZ(&base, &player->actor.world.pos) <= 100.f))
        return 0;
    DarkLink_OotPerchPoint(player, perch);
    f32 along = ((perch->x - base.x) * blade.x + (perch->z - base.z) * blade.z) / horizontalSq;
    along = MAX(.15f, MIN(.90f, along));
    perch->x = base.x + along * blade.x;
    perch->y = base.y + along * blade.y;
    perch->z = base.z + along * blade.z;
    return 1;
}

static int DarkLink_CheckPerch(Actor* actor, Player* player, PlayState* play, Vec3f* perch) {
    if (player->transformation != MM_PLAYER_FORM_HUMAN)
        return 0;
    DarkLink_OotPerchPoint(player, perch);
    Vec3f from = actor->world.pos;
    Vec3f to = *perch;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    from.y += 20.f;
    return fabsf(to.y - from.y) <= 80.f &&
           !BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &poly, 1, 1, 1, 0, &bgId);
}

static int DarkLink_SafeFloor(PlayState* play, CollisionPoly* poly, s32 bgId) {
    if (!poly || SurfaceType_GetFloorType(&play->colCtx, poly, bgId) == 9)
        return 0;
    s32 (*property)(CollisionContext*, CollisionPoly*, s32) = (void*)0x800c9b40;
    s32 (*exitIndex)(CollisionContext*, CollisionPoly*, s32) = (void*)0x800c99ac;
    s32 value = property(&play->colCtx, poly, bgId);
    return value != 5 && value != 12 && value != 13 && !exitIndex(&play->colCtx, poly, bgId);
}

static int DarkLink_LedgePath(Player* fighter, PlayState* play, s16* heading) {
    Actor* actor = &fighter->actor;
    s16 inward = actor->wallYaw + 0x8000;
    if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND) || !actor->wallPoly || fighter->ledgeClimbType < 2 ||
        fighter->yDistToLedge <= 0.f || fighter->yDistToLedge > 120.f || ABS((s16)(inward - *heading)) >= 0x2800)
        return 0;
    f32 distance = MAX(25.f, MIN(55.f, fighter->distToInteractWall + 20.f));
    Vec3f top = actor->world.pos;
    top.x += distance * Math_SinS(inward);
    top.z += distance * Math_CosS(inward);
    top.y += fighter->yDistToLedge + 16.f;
    CollisionPoly* poly;
    s32 bgId = 0;
    f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &top);
    if (!DarkLink_SafeFloor(play, poly, bgId) || fabsf(top.y - 16.f - floor) > 20.f)
        return 0;
    Vec3f head = top;
    Vec3f hit;
    top.y = floor + 2.f;
    head.y = floor + 58.f;
    if (BgCheck_EntityLineTest1(&play->colCtx, &top, &head, &hit, &poly, 0, 1, 0, 0, &bgId))
        return 0;
    *heading = inward;
    return 1;
}

static int DarkLink_HopPath(Actor* actor, PlayState* play, s16 heading) {
    if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND) || (actor->bgCheckFlags & BGCHECKFLAG_WALL) ||
        actor->xzDistToPlayer < 55.f || actor->xzDistToPlayer > 140.f)
        return 0;
    Vec3f middle = actor->world.pos;
    middle.x += 28.f * Math_SinS(heading);
    middle.z += 28.f * Math_CosS(heading);
    middle.y += 26.f;
    CollisionPoly* poly;
    s32 bgId = 0;
    f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &middle);
    if (DarkLink_SafeFloor(play, poly, bgId) && fabsf(floor - actor->world.pos.y) <= 8.f)
        return 0;
    Vec3f land = middle;
    land.x += 28.f * Math_SinS(heading);
    land.z += 28.f * Math_CosS(heading);
    floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &land);
    if (!DarkLink_SafeFloor(play, poly, bgId) || floor - actor->world.pos.y > 12.f || actor->world.pos.y - floor > 18.f)
        return 0;
    Vec3f start = actor->world.pos;
    Vec3f hit;
    start.y += 25.f;
    land.y = floor + 25.f;
    return !BgCheck_EntityLineTest1(&play->colCtx, &start, &land, &hit, &poly, 1, 1, 0, 0, &bgId);
}

static void DarkLink_Reposition(MmDarkLink* enemy, PlayState* play, Player* player) {
    Actor* actor = &enemy->player.actor;
    Vec3f spot = player->actor.world.pos;
    spot.x -= 120.f * Math_SinS(player->actor.shape.rot.y);
    spot.z -= 120.f * Math_CosS(player->actor.shape.rot.y);
    spot.y = actor->home.pos.y + 40.f;
    Vec3f probeFrom = player->actor.focus.pos;
    if (Math_Vec3f_DistXYZ(&spot, &actor->home.pos) > 800.f) {
        f32 distance = 25.f + Rand_ZeroOne() * 20.f;
        s16 offset = (Rand_ZeroOne() - .5f) * 4000.f;
        s16 yaw = Math_Vec3f_Yaw(&actor->home.pos, &player->actor.world.pos);
        actor->shape.rot.y = actor->world.rot.y = yaw;
        probeFrom = actor->home.pos;
        probeFrom.y += 40.f;
        spot = probeFrom;
        spot.x += distance * Math_SinS(yaw + offset);
        spot.z += distance * Math_CosS(yaw + offset);
    }
    CollisionPoly* poly;
    s32 bgId = 0;
    Vec3f hit;
    f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &spot);
    if (DarkLink_SafeFloor(play, poly, bgId) && fabsf(floor - actor->home.pos.y) <= 160.f &&
        !BgCheck_EntityLineTest1(&play->colCtx, &probeFrom, &spot, &hit, &poly, 1, 0, 0, 0, &bgId)) {
        spot.y = floor;
    } else {
        spot = actor->home.pos;
        if (enemy->hasSafeGround) {
            Vec3f saved = enemy->safeGround;
            saved.y += 20.f;
            floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &saved);
            if (DarkLink_SafeFloor(play, poly, bgId) && fabsf(floor - enemy->safeGround.y) < 20.f) {
                spot = enemy->safeGround;
                spot.y = floor;
            }
        }
    }
    actor->world.pos = actor->prevPos = spot;
    enemy->perchHeight = actor->shape.yOffset = 0.f;
    enemy->player.invincibilityTimer = 0;
    enemy->player.stateFlags1 &= ~(PLAYER_STATE1_MM_20000000 | PLAYER_STATE1_MM_80000000);
    enemy->player.stateFlags2 &= ~PLAYER_STATE2_MM_80000000;
    enemy->player.fallStartHeight = spot.y;
    enemy->player.fallDistance = 0;
    actor->velocity.y = actor->speed = enemy->player.linearVelocity = 0.f;
    Actor_UpdateBgCheckInfo(play, actor, 20.f, 30.f, 35.f, 5);
    enemy->recovery = enemy->swordPerch = 0;
    if (enemy->alpha != 255)
        enemy->staggerCount = enemy->staggerTimer = 0;
    actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    play->func_18780(&enemy->player, play);
}

static void DarkLink_HoldStunnedPose(Player* fighter, PlayState* play) {
    (void)fighter;
    (void)play;
}

static void DarkLink_SwordStandPose(Player* fighter, PlayState* play) {
    MmDarkLink* enemy = (MmDarkLink*)fighter;
    fighter->skelAnime.moveFlags = 0;
    fighter->linearVelocity = fighter->actor.speed = fighter->actor.velocity.y = 0.f;
    if (enemy->perchSwung == 1) {
        enemy->perchSwung = 2;
        fighter->unk_ADD = 0;
        fighter->meleeWeaponAnimation = PLAYER_MWA_FORWARD_SLASH_1H;
        fighter->meleeWeaponState = 0;
        ((void (*)(Player*, s32))OverlayAddr(0x8083375c))(fighter, PLAYER_MWA_FORWARD_SLASH_1H);
        PlayerAnimation_PlayOnce(play, &fighter->skelAnime, DARK_LINK_PERCH_SLASH);
        DarkLink_PlayOotVoice(&fighter->actor, 0x6800);
        return;
    }
    int finished = PlayerAnimation_Update(play, &fighter->skelAnime);
    if (fighter->skelAnime.animation == DARK_LINK_PERCH_SLASH) {
        fighter->meleeWeaponState = fighter->skelAnime.curFrame >= 1.f && fighter->skelAnime.curFrame <= 4.f;
        if (finished) {
            fighter->meleeWeaponState = 0;
            PlayerAnimation_PlayOnce(play, &fighter->skelAnime, DARK_LINK_PERCH_SLASH_END);
        }
    } else if (fighter->skelAnime.animation == DARK_LINK_PERCH_JUMP) {
        if (enemy->swordPerch <= 24)
            PlayerAnimation_Change(play, &fighter->skelAnime, DARK_LINK_COMBAT_IDLE,
                                   1.f, 0.f, 28.f, 0, -3.f);
    } else if (fighter->skelAnime.animation == DARK_LINK_PERCH_SLASH_END) {
        if (finished)
            PlayerAnimation_PlayLoopSetSpeed(play, &fighter->skelAnime, DARK_LINK_COMBAT_IDLE, 1.f);
    } else if (fighter->skelAnime.animation != DARK_LINK_COMBAT_IDLE) {
        PlayerAnimation_PlayLoopSetSpeed(play, &fighter->skelAnime, DARK_LINK_COMBAT_IDLE, 1.f);
    }
}

static void DarkLink_PlacePerch(MmDarkLink* enemy, Vec3f* target) {
    Actor* actor = &enemy->player.actor;
    if (enemy->swordPerch < 27) {
        Math_SmoothStepToF(&actor->world.pos.x, target->x, 1.f, 5.f, 0.f);
        Math_SmoothStepToF(&actor->world.pos.z, target->z, 1.f, 5.f, 0.f);
    }
    f32 footY = 0.f;
    if (enemy->swordPerch < 27 && enemy->bodyPose == 7)
        footY = MIN(enemy->bodyLimbY[1], enemy->bodyLimbY[2]);
    f32 height = actor->scale.y > 0.f ? (target->y - actor->world.pos.y - footY) / actor->scale.y : 0.f;
    Math_SmoothStepToF(&enemy->perchHeight, height, 1.f, 2000.f, 0.f);
    actor->shape.yOffset = enemy->perchHeight;
    actor->prevPos = actor->world.pos;
}

static void DarkLink_Update(Actor* actor, PlayState* play) {
    MmDarkLink* enemy = (MmDarkLink*)actor;
    Player* fighter = &enemy->player;
    Input* input = &enemy->input;
    Player* player = GET_PLAYER(play);
    int playerClashed = comboEnemyDarkLinkClashedWithPlayer(fighter, player);

    if (actor->colChkInfo.health && enemy->alpha < 255)
        enemy->alpha = MIN(255, enemy->alpha + 4);
    if (!actor->colChkInfo.health || enemy->recovery) {
        int finished = PlayerAnimation_Update(play, &fighter->skelAnime);
        Actor_MoveWithGravity(actor);

        Actor_UpdateBgCheckInfo(play, actor, 20.f, 30.f, 35.f,
                                actor->colChkInfo.health ? 1 : actor->velocity.y > 0.f ? 7 :
                                                                                         5);
        if (!actor->colChkInfo.health) {
            if (actor->velocity.y > 0.f && (actor->bgCheckFlags & BGCHECKFLAG_CEILING))
                actor->velocity.y = 0.f;
            if (actor->bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH)
                Actor_PlaySfx(actor, NA_SE_PL_BOUND);
            if (finished && enemy->falling && (actor->bgCheckFlags & BGCHECKFLAG_GROUND)) {
                enemy->falling = 0;
                PlayerAnimation_PlayOnce(play, &fighter->skelAnime, (PlayerAnimationHeader*)0x0400dc80);
            }
            if (!enemy->falling)
                Math_StepToF(&actor->speed, 0.f, .75f);
            if (enemy->alpha <= 13) {
                Actor_Kill(actor);
            } else {
                enemy->alpha -= 13;
                actor->shape.shadowAlpha = MAX(0, actor->shape.shadowAlpha - 13);
            }
        } else if ((actor->floorHeight >= -30000.f && actor->world.pos.y < actor->floorHeight - 160.f) ||
                   (actor->floorHeight < -30000.f && actor->world.pos.y < actor->home.pos.y - 160.f) ||
                   !--enemy->recovery) {
            DarkLink_Reposition(enemy, play, player);
        }
        if (enemy->staggerTimer && !--enemy->staggerTimer)
            enemy->counter = enemy->staggerCount = 0;
        actor->focus.pos = actor->world.pos;
        actor->focus.pos.y += 20.f;
        return;
    }
    Collider* sword = &fighter->meleeWeaponQuads[0].base;
    Collider* swordTip = &fighter->meleeWeaponQuads[1].base;
    if ((sword->acFlags | swordTip->acFlags) & AC_BOUNCED) {
        sword->acFlags &= ~(AC_HIT | AC_BOUNCED);
        swordTip->acFlags &= ~(AC_HIT | AC_BOUNCED);
        sword->atFlags |= AT_BOUNCED;
        swordTip->atFlags |= AT_BOUNCED;
        enemy->body.base.acFlags &= ~AC_HIT;
        if (enemy->lastSwordAnim != fighter->meleeWeaponAnimation) {
            enemy->lastSwordAnim = fighter->meleeWeaponAnimation;
            ++enemy->staggerCount;
        }
        if (enemy->counter && gSave.info.playerData.health < 0x50) {
            enemy->counter = 0;
            enemy->staggerTimer = 50;
        }
    }
    if (enemy->body.base.acFlags & AC_HIT) {
        enemy->body.base.acFlags &= ~AC_HIT;
        int canTakeHit = !enemy->swordPerch && fighter->invincibilityTimer == 0 &&
                         !(fighter->stateFlags1 & PLAYER_STATE1_MM_4000000) &&
                         !((sword->atFlags | swordTip->atFlags) & AT_HIT);
        if (canTakeHit && actor->colChkInfo.damageEffect == 1) {
            enemy->stun = 80;
            enemy->jumpTimer = enemy->jumpCounter = enemy->rolling = 0;
            fighter->invincibilityTimer = 0;
            fighter->linearVelocity = actor->speed = 0.f;
            actor->colChkInfo.damage = 0;
            /* DarkLink_Draw always renders through POLY_XLU_DISP. */
            Actor_SetColorFilter(actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_XLU, 80);
        } else if (canTakeHit && actor->colChkInfo.damage) {
            Actor_ApplyDamage(actor);
            AudioSfx_PlaySfx(NA_SE_PL_DAMAGE, &actor->projectedPos, 4, &sDarkLinkSfxFrequency,
                             &gSfxDefaultFreqAndVolScale, &sDarkLinkSfxReverb);
            actor->colChkInfo.damage = 0;
            actor->world.rot.y = actor->yawTowardsPlayer + 0x8000;
            actor->shape.rot.y = actor->yawTowardsPlayer;
            actor->speed = 8.f;
            actor->velocity.y = 6.f;
            actor->gravity = -1.f;
            actor->minVelocityY = -20.f;
            actor->bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH);
            fighter->linearVelocity = actor->speed;
            fighter->meleeWeaponState = 0;
            enemy->swordPose = enemy->shieldPose = 0;
            enemy->stun = 0;
            if (enemy->swordPerch)
                comboMmDarkLinkPerch(actor, play, 0);
            enemy->swordPerch = enemy->jumpTimer = enemy->jumpCounter = enemy->rolling = 0;
            enemy->perchHeight = actor->shape.yOffset = 0.f;
            if (!actor->colChkInfo.health) {
                actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
                fighter->linearVelocity = actor->speed = actor->velocity.y = 6.f;
                enemy->falling = 1;
                Enemy_StartFinishingBlow(play, actor);
                comboEnemyRandomDrop(play, actor, &actor->world.pos, 0xc0);
                DarkLink_PlayOotVoice(actor, 0x681d);
                PlayerAnimation_PlayOnce(play, &fighter->skelAnime, (PlayerAnimationHeader*)0x0400dc78);
            } else {
                actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                Actor_SetColorFilter(actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_XLU, 12);
                enemySetOotDropFlag(actor, &enemy->body.elem, true);
                enemy->recovery = 64;
                DarkLink_PlayOotVoice(actor, 0x6805);
                PlayerAnimation_PlayOnce(play, &fighter->skelAnime, (PlayerAnimationHeader*)0x0400dbb8);
            }
            return;
        }
    }
    input->prev = input->cur;
    input->cur.button = 0;
    input->cur.stick_x = 0;
    input->cur.stick_y = 0;
    Actor* projectile = DarkLink_ProjectileThreat(play, actor);
    if (!projectile) {
        for (Actor* explosive = play->actorCtx.actors[ACTORCAT_EXPLOSIVE].first; explosive;
             explosive = explosive->next) {
            if (explosive->update && explosive->params == 1 &&
                Math_Vec3f_DistXYZ(&actor->world.pos, &explosive->world.pos) <= 80.f + explosive->shape.rot.z * 10.f) {
                projectile = explosive;
                break;
            }
        }
    }
    int activating = !enemy->active;
    if (activating) {
        actor->shape.rot.y = actor->world.rot.y = actor->yawTowardsPlayer;
        Vec3f spot = actor->home.pos;
        spot.x += 25.f * Math_SinS(actor->shape.rot.y);
        spot.z += 25.f * Math_CosS(actor->shape.rot.y);
        Vec3f hit;
        CollisionPoly* poly;
        s32 bgId;
        if (!BgCheck_EntityLineTest1(&play->colCtx, &actor->world.pos, &spot, &hit, &poly, 1, 0, 0, 0, &bgId)) {
            Vec3f probe = spot;
            probe.y += 30.f;
            f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &probe);
            if (DarkLink_SafeFloor(play, poly, bgId) && fabsf(floor - spot.y) < 25.f) {
                spot.y = floor;
                actor->world.pos = actor->prevPos = spot;
            }
        }
        if (actor->xzDistToPlayer > 120.f && player->lockOnActor != actor && !projectile) {
            if (enemy->stun)
                --enemy->stun;
            PlayerAnimation_Update(play, &fighter->skelAnime);
            actor->focus.pos = actor->world.pos;
            actor->focus.pos.y += 20.f;
            DarkLink_UpdateBody(enemy);
            FOREIGN_ENEMY_API_COLLISION_AC(play, &play->colChkCtx, &enemy->body.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &enemy->body.base);
            return;
        }
        enemy->active = 1;
        fighter->skelAnime.playSpeed = 1.f;
        play->func_18780(fighter, play);
    }
    s16 heading = actor->yawTowardsPlayer;
    f32 magnitude = 0.f;
    f32 distance = actor->xzDistToPlayer;
    u8 attack = player->meleeWeaponAnimation;
    int spin = player->meleeWeaponState && attack >= PLAYER_MWA_SPIN_ATTACK_1H && attack <= PLAYER_MWA_BIG_SPIN_2H;
    int backflip = 0;
    int startRoll = 0;
    int perchRelease = 0;
    int pursuitHop = 0;
    int lockedOn = player->lockOnActor == actor;
    Vec3f perchTarget = actor->world.pos;
    int perchInterrupted = enemy->swordPerch && !comboMmDarkLinkPerch(actor, play, 1);
    if (enemy->swordPerch &&
        (perchInterrupted || distance > 100.f || !DarkLink_GetPerchPoint(player, &perchTarget))) {
        comboMmDarkLinkPerch(actor, play, 0);
        if (!perchInterrupted)
            player->skelAnime.curFrame = 3.f;
        enemy->swordPerch = 0;
        actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        fighter->meleeWeaponState = 0;
        backflip = perchRelease = 1;
    }
    if (!activating && distance <= 610.f && fighter->invincibilityTimer >= 0) {
        input->cur.button = BTN_Z;
        fighter->lockOnActor = &player->actor;
    } else {
        fighter->lockOnActor = NULL;
    }
    if (perchRelease) {
        input->cur.button = 0;
    } else if (enemy->stun) {
        input->cur.button = 0;
    } else if (!enemy->swordPerch && enemy->rolling) {
        heading = enemy->rollHeading;
        magnitude = 127.f;
    } else if (!enemy->swordPerch && projectile) {
        enemy->rollHeading = heading;
        startRoll = 1;
        magnitude = 127.f;
        input->cur.button = BTN_A;
        if (activating)
            fighter->lockOnActor = NULL;
    } else if (activating) {
        input->cur.button = 0;
    } else if (!enemy->swordPerch && enemy->jumpTimer) {
        if (!enemy->jumpCounter && (actor->bgCheckFlags & BGCHECKFLAG_GROUND)) {
            actor->world.rot.y = actor->shape.rot.y = heading;
            if (enemy->alpha < 255) {
                heading += 0x8000;
                magnitude = 127.f;
            }
            input->cur.button |= BTN_A;
            enemy->jumpCounter = 1;
            fighter->invincibilityTimer = 10;
        } else if (enemy->jumpCounter && enemy->alpha == 255 && actor->velocity.y > 0.f) {
            input->cur.button |= BTN_B;
        }
    } else if (!enemy->swordPerch && (player->meleeWeaponState || player->actor.velocity.y > -3.f) &&
               attack == PLAYER_MWA_JUMPSLASH_START) {
        actor->world.rot.y = actor->shape.rot.y = heading;
        heading += (play->gameplayFrames & 1) ? 0x4000 : -0x4000;
        magnitude = 127.f;
        input->cur.button |= BTN_A;
        enemy->jumpTimer = 15;
        enemy->jumpCounter = 0;
    } else if (enemy->swordPerch) {
        --enemy->swordPerch;
        player->linearVelocity = player->actor.speed = 0.f;
        if (enemy->swordPerch && player->skelAnime.curFrame > 2.f)
            player->skelAnime.curFrame = 2.f;
        if (!enemy->swordPerch || (player->invincibilityTimer > 0 && !fighter->meleeWeaponState)) {
            player->skelAnime.curFrame = 3.f;
            comboMmDarkLinkPerch(actor, play, 0);
            enemy->swordPerch = 0;
            actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            actor->world.rot.y = actor->shape.rot.y = actor->yawTowardsPlayer;
            fighter->meleeWeaponState = 0;
            backflip = perchRelease = 1;
        } else if (enemy->swordPerch < 16 && !enemy->perchSwung &&
                   fabsf(actor->world.pos.x - perchTarget.x) <= 5.f &&
                   fabsf(actor->world.pos.z - perchTarget.z) <= 5.f) {
            enemy->perchSwung = DarkLink_Swing(enemy, play, player, &heading, &magnitude) ? 1 : 3;
        } else if (enemy->swordPerch == 19) {
            DarkLink_PlayOotVoice(actor, 0x6814);
        }
    } else if (!fighter->meleeWeaponState && player->actor.world.pos.y > actor->world.pos.y + 25.f &&
               (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && distance <= 300.f) {
        magnitude = 127.f;
        if (DarkLink_LedgePath(fighter, play, &heading))
            input->cur.button |= BTN_A;
        else
            pursuitHop = DarkLink_HopPath(actor, play, heading);
    } else if (player->meleeWeaponState && distance <= 120.f &&
               (s16)(actor->yawTowardsPlayer + 0x8000 - player->actor.shape.rot.y) != -32768 &&
               (s16)(actor->yawTowardsPlayer - actor->world.rot.y) != -32768) {
        if (attack == PLAYER_MWA_STAB_1H && distance < 90.f) {
            if (distance <= 85.f && lockedOn && !player->invincibilityTimer && !fighter->meleeWeaponState &&
                !enemy->counter && (actor->bgCheckFlags & BGCHECKFLAG_GROUND) &&
                DarkLink_CheckPerch(actor, player, play, &perchTarget) && comboMmDarkLinkPerch(actor, play, 1)) {
                player->meleeWeaponState = 0;
                player->actor.speed = player->linearVelocity = 0.f;
                fighter->linearVelocity = 0.f;
                f32 playSpeed = player->skelAnime.playSpeed;
                player->skelAnime.curFrame = 2.f;
                player->skelAnime.playSpeed = 0.f;
                PlayerAnimation_Update(play, &player->skelAnime);
                player->skelAnime.playSpeed = playSpeed;
                input->cur.button = BTN_A;
                actor->velocity.y = 0.f;
                fighter->invincibilityTimer = -7;
                enemy->swordPerch = 27;
                enemy->perchSwung = 0;
                enemy->perchHeight = 0.f;
                enemy->swordSweep = 0;
                enemy->jumpTimer = enemy->jumpCounter = enemy->rolling = 0;
                actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            } else {
                backflip = 1;
            }
        } else if (attack < PLAYER_MWA_STAB_1H) {
            if (attack >= PLAYER_MWA_LEFT_SLASH_1H)
                heading -= 0x4000;
            else if (attack >= PLAYER_MWA_RIGHT_SLASH_1H)
                heading += 0x4000;
            magnitude = attack >= PLAYER_MWA_RIGHT_SLASH_1H ? 127.f : 0.f;
            input->cur.button = BTN_B;
            if (!fighter->meleeWeaponState)
                enemy->counter = 1;
        } else if (attack <= PLAYER_MWA_STAB_COMBO_2H ||
                   (attack >= PLAYER_MWA_GORON_PUNCH_LEFT && attack <= PLAYER_MWA_GORON_PUNCH_BUTT)) {
            input->cur.button = BTN_R;
        } else {
            backflip = 1;
        }
    } else {
        s16 facingDiff = player->actor.shape.rot.y - actor->shape.rot.y;
        s32 facing = facingDiff < 0 ? -facingDiff : facingDiff;
        int shielding = (player->stateFlags1 & 0x400000) != 0;
        int approach = 0;
        if (distance > 70.f && distance <= 90.f && facing >= 0x7800 && (lockedOn || !shielding)) {
            DarkLink_Swing(enemy, play, player, &heading, &magnitude);
        } else if (!fighter->meleeWeaponState && (distance <= 70.f || (distance <= 80.f && player->meleeWeaponState))) {
            if (!DarkLink_Swing(enemy, play, player, &heading, &magnitude) && !enemy->counter)
                backflip = 1;
        } else if (distance <= 50.f) {
            approach = 1;
        } else if (distance > 100.f) {
            backflip = distance < 280.f && spin;
            approach = !backflip;
        } else {
            approach =
                (facing >= 0x3000 && facing < 0x7800) || !DarkLink_Swing(enemy, play, player, &heading, &magnitude);
        }
        if (approach) {
            magnitude = 127.f;
            if (!lockedOn)
                Math_SmoothStepToS(&heading, player->actor.shape.rot.y + 0x7fff, 1, 0x2328, 0);
            pursuitHop = DarkLink_HopPath(actor, play, heading);
        }
    }
    if (backflip && (perchRelease || (actor->bgCheckFlags & BGCHECKFLAG_GROUND))) {
        actor->world.rot.y = actor->shape.rot.y = actor->yawTowardsPlayer;
        heading = actor->yawTowardsPlayer + 0x8000;
        magnitude = 127.f;
        input->cur.button = BTN_Z | BTN_A;
        if (!perchRelease) {
            fighter->invincibilityTimer = 10;
            enemy->counter = 0;
        }
    }
    s16 angle = Play_GetCamera(play, CAM_ID_MAIN)->camDir.y - heading;
    input->cur.stick_x = magnitude * Math_SinS(angle);
    input->cur.stick_y = magnitude * Math_CosS(angle);
    if (!activating && enemy->alpha < 255 && (play->gameplayFrames & 7) == 0)
        ++enemy->alpha;
    if (gSaveContext.magicState == MAGIC_STATE_METER_FLASH_1 && !spin) {
        input->cur.button = BTN_R;
        input->cur.stick_x = input->cur.stick_y = 0;
        pursuitHop = 0;
    }
    int stunned = enemy->stun != 0;
    if (stunned) {
        --enemy->stun;
        fighter->stateFlags1 &= ~PLAYER_STATE1_MM_4000000;
        input->cur.button = 0;
        input->cur.stick_x = input->cur.stick_y = 0;
        fighter->invincibilityTimer = 0;
        fighter->linearVelocity = actor->speed = 0.f;
    }
    if (((input->cur.button & BTN_B) || startRoll) && !activating && distance <= 610.f &&
        fighter->invincibilityTimer >= 0)
        input->cur.button |= BTN_Z;
    input->prev.button &= ~(BTN_A | BTN_B);
    if (input->cur.button & BTN_R) {
        if (enemy->counter || fighter->meleeWeaponState)
            input->cur.button &= ~BTN_R;
        else
            input->cur.button = BTN_R;
    }
    if (enemy->swordPerch) {
        input->cur.button = BTN_Z;
        input->cur.stick_x = input->cur.stick_y = 0;
        fighter->lockOnActor = &player->actor;
        actor->bgCheckFlags |= BGCHECKFLAG_GROUND;
        fighter->linearVelocity = actor->speed = actor->velocity.y = 0.f;
    }
    input->press.button = input->cur.button & ~input->prev.button;
    input->rel.button = input->prev.button & ~input->cur.button;
    ((void (*)(Input*))0x8008524c)(input);
    input->press.stick_x += (s8)(input->cur.stick_x - input->prev.stick_x);
    input->press.stick_y += (s8)(input->cur.stick_y - input->prev.stick_y);
    if (enemy->swordPerch) {
        if (enemy->swordPerch == 27) {
            fighter->skelAnime.moveFlags = 0;
            fighter->meleeWeaponState = 0;
            PlayerAnimation_PlayOnce(play, &fighter->skelAnime, DARK_LINK_PERCH_JUMP);
        }
        fighter->actionFunc = DarkLink_SwordStandPose;
        DarkLink_PlacePerch(enemy, &perchTarget);
        actor->world.rot.y = actor->shape.rot.y = Math_Vec3f_Yaw(&actor->world.pos, &player->actor.world.pos);
        fighter->currentYaw = actor->shape.rot.y;
        DarkLink_SwordStandPose(fighter, play);
        if (fighter->invincibilityTimer < 0)
            ++fighter->invincibilityTimer;
        else if (fighter->invincibilityTimer > 0)
            --fighter->invincibilityTimer;
        for (int i = 0; i < 2; ++i)
            fighter->meleeWeaponQuads[i].elem.atDmgInfo.damage = 8;
        actor->focus.pos = actor->world.pos;
        actor->focus.pos.y += 20.f + enemy->perchHeight * actor->scale.y;
        DarkLink_UpdateBody(enemy);
        return;
    }
    if (fighter->actionFunc == DarkLink_SwordStandPose) {
        fighter->meleeWeaponState = 0;
        play->func_18780(fighter, play);
    }
    PlayerActionFunc action = fighter->actionFunc;
    if (stunned)
        fighter->actionFunc = DarkLink_HoldStunnedPose;
    play->playerUpdate(fighter, play, input);
    if (!stunned && fighter->skelAnime.animation == (PlayerAnimationHeader*)0x0400e410)
        PlayerAnimation_PlayLoopSetSpeed(play, &fighter->skelAnime, DARK_LINK_COMBAT_IDLE, 1.f);
    if (stunned && fighter->actionFunc == DarkLink_HoldStunnedPose)
        fighter->actionFunc = action;
    if ((fighter->stateFlags2 & PLAYER_STATE2_MM_80000000) ||
        (actor->world.pos.y < actor->home.pos.y - 160.f &&
         (actor->floorHeight < -30000.f || actor->world.pos.y < actor->floorHeight - 160.f))) {
        if (enemy->swordPerch)
            comboMmDarkLinkPerch(actor, play, 0);
        DarkLink_Reposition(enemy, play, player);
        return;
    }
    if (!enemy->swordPerch && (actor->bgCheckFlags & BGCHECKFLAG_GROUND) &&
        DarkLink_SafeFloor(play, actor->floorPoly, actor->floorBgId)) {
        enemy->safeGround = actor->world.pos;
        enemy->hasSafeGround = 1;
    }
    if (pursuitHop && (actor->bgCheckFlags & BGCHECKFLAG_GROUND) && !fighter->meleeWeaponState) {
        actor->shape.rot.y = actor->world.rot.y = heading;
        ((void (*)(Player*, PlayState*, s32))OverlayAddr(0x80839860))(fighter, play, 0);
    }
    if (perchRelease && (input->cur.button & BTN_A) &&
        fighter->skelAnime.animation != (PlayerAnimationHeader*)0x0400d7f0) {
        ((void (*)(Player*, PlayState*, s32))OverlayAddr(0x80839860))(fighter, play, 2);
    }

    for (int i = 0; i < 2; ++i)
        fighter->meleeWeaponQuads[i].elem.atDmgInfo.damage = 8;

    if (startRoll)
        fighter->invincibilityTimer = 20;
    if (startRoll || enemy->rolling)
        enemy->rolling = fighter->invincibilityTimer > 0;

    if (fighter->linearVelocity == -18.f) {
        fighter->linearVelocity = distance > 80.f ? 1.2f : distance < 70.f ? -1.5f : 1.f;
        s32 threshold = (s32)((Rand_ZeroOne() - .5f) * 2.f) + 6;
        if (gSave.info.playerData.health < 0x50)
            threshold = (s32)((Rand_ZeroOne() - .5f) * 2.f) + 3;
        if (threshold < enemy->staggerCount) {
            fighter->skelAnime.playSpeed *= .6f;
            AudioSfx_PlaySfx(NA_SE_PL_DAMAGE, &actor->projectedPos, 4, &sDarkLinkSfxFrequency,
                             &gSfxDefaultFreqAndVolScale, &sDarkLinkSfxReverb);
            enemy->staggerTimer = enemy->staggerCount = 0;
        }
    }
    if (playerClashed && player->linearVelocity == -18.f)
        player->linearVelocity = actor->xzDistToPlayer > 80.f ? 1.2f : actor->xzDistToPlayer < 70.f ? -1.5f : 1.f;
    if (enemy->counter == 1 && !fighter->meleeWeaponState)
        enemy->counter = 0;
    if (enemy->counter == 1) {
        enemy->counter = 2;
        fighter->meleeWeaponState = 1;
        fighter->skelAnime.curFrame =
            MAX(0.f, MIN(fighter->skelAnime.endFrame, player->skelAnime.curFrame - player->skelAnime.playSpeed));
        fighter->skelAnime.playSpeed = player->skelAnime.playSpeed;
        PlayerAnimation_Update(play, &fighter->skelAnime);
        s32 (*resetQuadAT)(PlayState*, Collider*) = (void*)0x800e1fd4;
        resetQuadAT(play, sword);
        resetQuadAT(play, swordTip);
    }
    if (enemy->staggerTimer && !--enemy->staggerTimer)
        enemy->counter = enemy->staggerCount = 0;
    if (enemy->perchHeight != 0.f) {
        actor->world.pos.y += enemy->perchHeight * actor->scale.y;
        enemy->perchHeight = 0.f;
    }
    actor->shape.yOffset = enemy->perchHeight;
    if (enemy->jumpTimer && !--enemy->jumpTimer)
        enemy->jumpCounter = 0;
    actor->focus.pos = actor->world.pos;
    actor->focus.pos.y += 20.f;
    DarkLink_UpdateBody(enemy);
    int invulnerable = fighter->invincibilityTimer != 0;
    enemy->body.base.colMaterial = invulnerable ? COL_MATERIAL_NONE : COL_MATERIAL_HIT5;
    enemy->body.elem.elemMaterial = invulnerable ? ELEM_MATERIAL_UNK5 : ELEM_MATERIAL_UNK1;
    if (!enemy->swordPerch) {
        if (fighter->invincibilityTimer <= 0 && !(fighter->stateFlags1 & PLAYER_STATE1_MM_4000000))
            FOREIGN_ENEMY_API_COLLISION_AC(play, &play->colChkCtx, &enemy->body.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &enemy->body.base);
    }
}

static void DarkLink_PostLimbDraw(PlayState* play, s32 limb, Gfx** dList, Vec3s* rot, Actor* actor, Gfx** gfx) {
    MmDarkLink* enemy = (MmDarkLink*)actor;
    Vec3f quad[4];
    if (limb == PLAYER_LIMB_LEFT_HAND) {
        static Vec3f bladeBase = { 0.f, 400.f, 0.f };
        static Vec3f bladeTip = { 4000.f, 400.f, 0.f };
        WeaponInfo* blade = &enemy->player.meleeWeaponInfo[0];
        Matrix_MultVec3f(&bladeBase, &blade->base);
        Matrix_MultVec3f(&bladeTip, &blade->tip);
        static Vec3f sword[4] = {
            { 0.f, 1400.f, -1000.f },
            { 5200.f, -400.f, 1000.f },
            { 0.f, -400.f, 1000.f },
            { 5200.f, 1400.f, -1000.f },
        };
        f32 reach = 4000.f;
        if (enemy->player.unk_ADD >= 3) {
            ++enemy->player.unk_ADD;
            reach *= 1.f + (9 - enemy->player.unk_ADD) * .1f;
        }
        sword[1].x = sword[3].x = reach + 1200.f;
        for (int i = 0; i < 4; ++i)
            Matrix_MultVec3f(&sword[i], &quad[i]);

        int attacking = !enemy->stun && !enemy->recovery && actor->colChkInfo.health &&
                        enemy->player.meleeWeaponState > 0 &&
                        (enemy->player.meleeWeaponAnimation < PLAYER_MWA_SPIN_ATTACK_1H ||
                         (enemy->player.stateFlags2 & PLAYER_STATE2_MM_20000));
        enemy->swordPose = 0;
        if (enemy->swordSweep && attacking) {
            for (int i = 0; i < 2; ++i) {
                Vec3f* base = &quad[i * 2];
                Vec3f* tip = &quad[i * 2 + 1];
                Vec3f* oldBase = &enemy->swordPoints[i * 2];
                Vec3f* oldTip = &enemy->swordPoints[i * 2 + 1];
                if (base->x == oldBase->x && base->y == oldBase->y && base->z == oldBase->z && tip->x == oldTip->x &&
                    tip->y == oldTip->y && tip->z == oldTip->z) {
                    ((s32(*)(PlayState*, Collider*))0x800e1fd4)(play, &enemy->player.meleeWeaponQuads[i].base);
                    continue;
                }
                Collider_SetQuadVertices(&enemy->player.meleeWeaponQuads[i], base, tip, oldBase, oldTip);
                enemy->swordPose |= 1 << i;
                CollisionCheck_SetAT(play, &play->colChkCtx, &enemy->player.meleeWeaponQuads[i].base);
                if (enemy->counter)
                    FOREIGN_ENEMY_API_COLLISION_AC(play, &play->colChkCtx, &enemy->player.meleeWeaponQuads[i].base);
            }
        }
        if (!enemy->swordSweep && attacking) {
            for (int i = 0; i < 2; ++i)
                ((s32(*)(PlayState*, Collider*))0x800e1fd4)(play, &enemy->player.meleeWeaponQuads[i].base);
        }
        void* trail = Effect_GetByIndex(enemy->player.meleeWeaponEffectIndex[0]);
        if (trail && attacking && !(enemy->player.stateFlags1 & PLAYER_STATE1_MM_400000))
            EffectBlure_AddVertex(trail, &blade->tip, &blade->base);
        else if (trail && enemy->swordSweep)
            ((void (*)(void*))0x800a8514)(trail);
        for (int i = 0; i < 4; ++i)
            enemy->swordPoints[i] = quad[i];
        enemy->swordSweep = attacking;
    } else if (limb == PLAYER_LIMB_RIGHT_HAND) {
        static Vec3f shield[4] = {
            { -4500.f, -3000.f, -600.f },
            { 1500.f, -3000.f, -600.f },
            { -4500.f, 3000.f, -600.f },
            { 1500.f, 3000.f, -600.f },
        };
        for (int i = 0; i < 4; ++i)
            Matrix_MultVec3f(&shield[i], &quad[i]);
        Collider_SetQuadVertices(&enemy->player.shieldQuad, &quad[0], &quad[1], &quad[2], &quad[3]);
        enemy->shieldPose = 1;
        if (!enemy->stun && !enemy->recovery && actor->colChkInfo.health &&
            (enemy->player.stateFlags1 & PLAYER_STATE1_MM_400000)) {
            FOREIGN_ENEMY_API_COLLISION_AC(play, &play->colChkCtx, &enemy->player.shieldQuad.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &enemy->player.shieldQuad.base);
        }
    } else if (limb == PLAYER_LIMB_LEFT_FOOT || limb == PLAYER_LIMB_RIGHT_FOOT) {
        static Vec3f foot = { 200.f, 300.f, 0.f };
        Matrix_MultVec3f(&foot, &actor->shape.feetPos[limb == PLAYER_LIMB_RIGHT_FOOT]);
    }
    if (limb == PLAYER_LIMB_HEAD || limb == PLAYER_LIMB_LEFT_FOOT || limb == PLAYER_LIMB_RIGHT_FOOT) {
        static Vec3f origin;
        Vec3f part;
        Matrix_MultVec3f(&origin, &part);
        s32 bodyPart = limb == PLAYER_LIMB_HEAD ? 0 : limb == PLAYER_LIMB_LEFT_FOOT ? 1 : 2;
        if (enemy->swordPerch && bodyPart != 0)
            part.y = actor->shape.feetPos[limb == PLAYER_LIMB_RIGHT_FOOT].y;
        enemy->bodyLimbY[bodyPart] = part.y - actor->world.pos.y - actor->shape.yOffset * actor->scale.y;
        enemy->bodyPose |= 1 << bodyPart;
    }
}

static void DarkLink_Draw(Actor* actor, PlayState* play) {
    void* adjustPlayerPose = (void*)0x80125580;
    static Gfx translucent[] = {
        gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2 | Z_UPD),
        gsDPSetAlphaCompare(G_AC_THRESHOLD),
        gsSPEndDisplayList(),
    };
    MmDarkLink* enemy = (MmDarkLink*)actor;
    SkelAnime* skel = &enemy->player.skelAnime;
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, MIN(enemy->alpha, 160));
    gSPSegment(POLY_XLU_DISP++, 0x0C, translucent);
    ((void (*)(Actor*, PlayState*, s32))0x800b8118)(actor, play, 0);
    POLY_XLU_DISP = SkelAnime_DrawFlex(play, skel->skeleton, skel->jointTable, skel->dListCount, adjustPlayerPose,
                                       DarkLink_PostLimbDraw, actor, POLY_XLU_DISP);
    CLOSE_DISPS();
}

static ActorProfile sDarkLinkProfile = {
    0xc33,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED |
        ACTOR_FLAG_DRAW_CULLING_DISABLED,
    0x1020,
    sizeof(MmDarkLink),
    DarkLink_Init,
    DarkLink_Destroy,
    DarkLink_Update,
    DarkLink_Draw,
};
OVL_INFO_NATIVE_ENEMY(0x33, sDarkLinkProfile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_OOT_EN_TORCH2, sDarkLinkProfile, DarkLink_Init, DarkLink_Destroy,
                     DarkLink_Update, DarkLink_Draw);
static const u32 kOrigin[] = { 0x4e505431, 0, 0x33, 0x20, 0x6eb383c0 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_TORCH2_ORIGIN, kOrigin);
