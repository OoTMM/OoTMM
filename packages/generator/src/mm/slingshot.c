#include <combo.h>
#include <combo/custom.h>
#include <combo/mm/player.h>
#include <combo/player.h>
#include <combo/math.h>
#include <combo/global.h>
#include <combo/common/animation.h>
#include <assets/mm/objects/gameplay_keep.h>
#include "../../src/mm/ovl/actors/Custom_SlingshotProjectile/Custom_SlingshotProjecticle.h"

#define MM_PLAYER_SET_UPPER_ACTION                 0x8082F43C
#define MM_PLAYER_UPDATE_HOSTILE_LOCK_ON           0x8082FB68
#define MM_PLAYER_IS_Z_TARGETING                   0x8082FBE8
#define MM_PLAYER_REQUEST_RUMBLE                   0x8082DF48
#define MM_PLAYER_CHECK_FOR_IDLE_ANIM              0x8082ED94
#define MM_PLAYER_DECELERATE_TO_ZERO               0x80832F24
#define MM_PLAYER_CHECK_RANGED_SHIELD_DRAW         0x80830B88
#define MM_PLAYER_CHECK_RANGED_RESTART             0x80830F9C
#define MM_PLAYER_CHECK_RANGED_HOLD_RELEASE        0x80830FD4
#define MM_PLAYER_ANIM_TABLE                       0x8085BE84
#define MM_PLAYER_USE_HELD_ITEM                    0x80862B48
#define MM_PLAYER_HELD_ITEM_BUTTON_IS_HELD_DOWN    0x80862B4C

typedef s32 (*MmPlayerCheckForIdleAnimFunc)(Player* player);

typedef void (*MmPlayerRequestRumbleFunc)(
    Player* player,
    s32 arg1,
    s32 arg2,
    s32 arg3,
    s32 arg4
);

typedef void (*MmPlayerSetUpperActionFunc)(
    PlayState* play,
    Player* player,
    PlayerUpperActionFunc upperActionFunc
);

typedef s32 (*MmPlayerUpdateHostileLockOnFunc)(Player* player);
typedef s32 (*MmPlayerIsZTargetingFunc)(Player* player, PlayState* play);
typedef s32 (*MmPlayerCheckRangedShieldDrawFunc)(PlayState* play, Player* player);
typedef void (*MmPlayerDecelerateToZeroFunc)(Player* player);
typedef s32 (*MmPlayerCheckRangedRestartFunc)(PlayState* play);
typedef s32 (*MmPlayerCheckRangedHoldReleaseFunc)(PlayState* play);

#define Player_SetUpperAction \
    ((MmPlayerSetUpperActionFunc)OverlayAddr(MM_PLAYER_SET_UPPER_ACTION))

#define Player_UpdateHostileLockOn \
    ((MmPlayerUpdateHostileLockOnFunc)OverlayAddr(MM_PLAYER_UPDATE_HOSTILE_LOCK_ON))

#define Player_IsZTargeting \
    ((MmPlayerIsZTargetingFunc)OverlayAddr(MM_PLAYER_IS_Z_TARGETING))

#define Player_RequestRumble \
    ((MmPlayerRequestRumbleFunc)OverlayAddr(MM_PLAYER_REQUEST_RUMBLE))

#define Player_CheckForIdleAnim \
    ((MmPlayerCheckForIdleAnimFunc)OverlayAddr(MM_PLAYER_CHECK_FOR_IDLE_ANIM))

#define Player_DecelerateToZero \
    ((MmPlayerDecelerateToZeroFunc)OverlayAddr(MM_PLAYER_DECELERATE_TO_ZERO))

#define Player_CheckRangedShieldDraw \
    ((MmPlayerCheckRangedShieldDrawFunc)OverlayAddr(MM_PLAYER_CHECK_RANGED_SHIELD_DRAW))

#define Player_CheckRangedRestart \
    ((MmPlayerCheckRangedRestartFunc)OverlayAddr(MM_PLAYER_CHECK_RANGED_RESTART))

#define Player_CheckRangedHoldRelease \
    ((MmPlayerCheckRangedHoldReleaseFunc)OverlayAddr(MM_PLAYER_CHECK_RANGED_HOLD_RELEASE))

#define sPlayerUseHeldItem \
    (*(s32*)OverlayAddr(MM_PLAYER_USE_HELD_ITEM))

#define sHeldItemButtonIsHeldDown \
    (*(s32*)OverlayAddr(MM_PLAYER_HELD_ITEM_BUTTON_IS_HELD_DOWN))

#define GET_PLAYER_ANIM_CUSTOM(group, type) \
    (((PlayerAnimationHeader**)OverlayAddr(MM_PLAYER_ANIM_TABLE))[(group) * 6 + (type)])

#define PLAYER_UPPER_ROT_FLAGS(player) \
    (*(u16*)((u8*)(player) + 0xAA6))

#define CUSTOM_PLAYER_INPUT(play) (&(play)->state.input[0])

s32 Player_GetSlingshotAmmo(PlayState* play, Player* this, s32* itemPtr, s32* typePtr);
s32 Player_InitItemAction_CustomSlingshot(PlayState* play, Player* this);
s32 Player_StartSlingshotReady(Player* this, PlayState* play);
s32 Player_StartSlingshotShot(Player* this, PlayState* play);
s32 Player_UpdateSlingshotDraw(Player* this, PlayState* play);
s32 Player_UpdateSlingshotAimReady(Player* this, PlayState* play);
s32 Player_FireHeldSlingshotSeed(PlayState* play, Player* this);
s32 Player_EndSlingshotShot(Player* this, PlayState* play);
s32 Player_CheckSlingshotReadyOrStart(Player* this, PlayState* play);

static u16 sSlingshotSfxTable1[] = {
    NA_SE_IT_BOW_DRAW,
    NA_SE_IT_SLING_DRAW,
    NA_SE_IT_HOOKSHOT_READY,
};

static PlayerAnimationHeader* sSlingshotDrawAnimTable[] = {
    (PlayerAnimationHeader*)&gPlayerAnim_link_hook_walk2ready,
    (PlayerAnimationHeader*)&gPlayerAnim_link_bow_walk2ready,
    (PlayerAnimationHeader*)&gPlayerAnim_link_bow_walk2ready,
};

static PlayerAnimationHeader* sSlingshotHoldAnimTable[] = {
    (PlayerAnimationHeader*)&gPlayerAnim_link_hook_wait,
    (PlayerAnimationHeader*)&gPlayerAnim_link_bow_bow_wait,
    (PlayerAnimationHeader*)&gPlayerAnim_link_bow_bow_wait,
};

static u16 sSlingshotSfxTable2[] = {
    NA_SE_IT_BOW_FLICK,
    NA_SE_IT_SLING_FLICK,
};

static s32 CustomSlingshot_IsCurrentHeldItem(Player* this)
{
    return (this->heldItemAction == PLAYER_CUSTOM_IA_SLINGSHOT) &&
           (this->itemAction == PLAYER_CUSTOM_IA_SLINGSHOT);
}

static s32 CustomSlingshot_UseHeldItem(Player* this)
{
    return sPlayerUseHeldItem && CustomSlingshot_IsCurrentHeldItem(this);
}

static s32 CustomSlingshot_HeldItemButtonIsHeldDown(Player* this)
{
    return sHeldItemButtonIsHeldDown && CustomSlingshot_IsCurrentHeldItem(this);
}

s32 Player_InitItemAction_CustomSlingshot(PlayState* play, Player* this) {
    (void)play;

    this->stateFlags1 |= PLAYER_STATE1_MM_8;

    this->unk_B28 = -2;
    this->unk_ACC = 0;

    return true;
}

#define MM_UPG_BULLET_BAG gMmSave.info.inventory.upgrades.bulletBag

static s16 MM_GetSlingshotMax(void)
{
    return kMaxSeeds[MM_UPG_BULLET_BAG];
}

static s16 MM_GetSlingshotAmmo(void)
{
    s16 max = MM_GetSlingshotMax();

    if (gMmExtraAmmo.slingshotSeeds < 0)
    {
        gMmExtraAmmo.slingshotSeeds = 0;
    }

    if (gMmExtraAmmo.slingshotSeeds > max)
    {
        gMmExtraAmmo.slingshotSeeds = max;
    }

    return gMmExtraAmmo.slingshotSeeds;
}

static void MM_SetSlingshotAmmo(s16 ammo)
{
    s16 max = MM_GetSlingshotMax();

    if (ammo < 0)
    {
        ammo = 0;
    }

    if (ammo > max)
    {
        ammo = max;
    }

    gMmExtraAmmo.slingshotSeeds = ammo;
}

s32 Player_GetSlingshotAmmo(PlayState* play, Player* this, s32* itemPtr, s32* typePtr)
{
    (void)play;
    (void)this;

    *itemPtr = ITEM_MM_SLINGSHOT;
    *typePtr = ACTOR_CUSTOM_SLINGSHOT_PROJECTILE;

    return MM_GetSlingshotAmmo();
}

s32 Player_StartSlingshotShot(Player* this, PlayState* play) {
    Player_SetUpperAction(play, this, Player_UpdateSlingshotDraw);

    this->stateFlags3 |= PLAYER_STATE3_MM_40;
    this->unk_ACC = 14;

    if (this->unk_B28 >= 0) {
        s32 var_v1 = ABS(this->unk_B28);
        s32 item;
        s32 projectileType;

        Player_PlaySfx(this, sSlingshotSfxTable1[var_v1 - 1]);

        if (Player_GetSlingshotAmmo(play, this, &item, &projectileType) > 0) {
            if (this->unk_B28 >= 0) {
                this->heldActor = Actor_SpawnAsChild(
                    &play->actorCtx,
                    &this->actor,
                    play,
                    ACTOR_CUSTOM_SLINGSHOT_PROJECTILE,
                    this->actor.world.pos.x,
                    this->actor.world.pos.y,
                    this->actor.world.pos.z,
                    0,
                    this->actor.shape.rot.y,
                    0,
                    0
                );
            }
        }
    }

    return true;
}

s32 Player_StartSlingshotReady(Player* this, PlayState* play) {
    if (!Player_StartSlingshotShot(this, play)) {
        return false;
    }

    PlayerAnimation_PlayOnce(play, &this->skelAnimeUpper, (PlayerAnimationHeader*)&gPlayerAnim_link_bow_bow_ready);

    if (this->stateFlags1 & PLAYER_STATE1_MM_800000) {
        PlayerAnimation_PlayLoop(play, &this->skelAnime, (PlayerAnimationHeader*)&gPlayerAnim_link_uma_anim_walk);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
               !Player_UpdateHostileLockOn(this)) {
        PlayerAnimation_PlayLoop(play, &this->skelAnime, GET_PLAYER_ANIM_CUSTOM(0, this->modelAnimType));
    }

    return true;
}

static s32 CustomSlingshot_DrawOrAimRanged(Player* this, PlayState* play) {
    if ((this->unk_AA5 == 0) ||
        (this->unk_AA5 == 3)) {
        if (Player_IsZTargeting(this, play) ||
            (this->lockOnActor != NULL) ||
            (Camera_CheckValidMode(Play_GetCamera(play, CAM_ID_MAIN), 4) == 0)) {
            return true;
        }

        this->unk_AA5 = 3;
    }

    return false;
}

static s32 CustomSlingshot_CheckReadyOrIdle(Player* this, PlayState* play) {
    if ((this->doorType == 0) &&
        !(this->stateFlags1 & PLAYER_STATE1_MM_2000000)) {
        if (CustomSlingshot_UseHeldItem(this)) {
            if (Player_StartSlingshotReady(this, play)) {
                return CustomSlingshot_DrawOrAimRanged(this, play);
            }
        }
    }

    return false;
}

s32 Player_FireHeldSlingshotSeed(PlayState* play, Player* this)
{
    (void)play;

    if (this->heldActor != NULL)
    {
        if (MM_GetSlingshotAmmo() > 0)
        {
            MM_SetSlingshotAmmo(MM_GetSlingshotAmmo() - 1);
        }

        Player_RequestRumble(this, 150, 10, 150, 0);

        this->unk_D57 = 4;

        this->heldActor->parent = NULL;
        this->actor.child = NULL;
        this->heldActor = NULL;

        return true;
    }

    return false;
}

s32 Player_UpdateSlingshotDraw(Player* this, PlayState* play) {
    s32 index;
    s32 temp;

    temp = 1;
    index = temp;

    Math_ScaledStepToS(&this->upperLimbRot.z, 0x4B0, 0x190);
    PLAYER_UPPER_ROT_FLAGS(this) |= 0x04;

    if ((this->unk_ACE == 0) &&
        (Player_CheckForIdleAnim(this) == 0) &&
        (this->skelAnime.animation == (PlayerAnimationHeader*)&gPlayerAnim_link_bow_side_walk)) {
        PlayerAnimation_PlayOnce(play, &this->skelAnimeUpper, sSlingshotDrawAnimTable[index]);
        this->unk_ACE = -1;
    } else if (PlayerAnimation_Update(play, &this->skelAnimeUpper)) {
        PlayerAnimation_PlayLoop(play, &this->skelAnimeUpper, sSlingshotHoldAnimTable[index]);
        this->unk_ACE = 1;
    } else if (this->unk_ACE == 1) {
        this->unk_ACE = 2;
    }

    if (this->unk_ACC >= 0xB) {
        this->unk_ACC--;
    }

    CustomSlingshot_DrawOrAimRanged(this, play);

    if ((this->unk_ACE > 0) &&
    ((this->unk_B28 < 0) ||
     !CustomSlingshot_HeldItemButtonIsHeldDown(this))) {
        Player_SetUpperAction(play, this, Player_UpdateSlingshotAimReady);

        if (this->unk_B28 >= 0) {
            if (!Player_FireHeldSlingshotSeed(play, this)) {
                Player_PlaySfx(this, sSlingshotSfxTable2[this->unk_B28 - 1]);
            }
        }

        this->unk_ACC = 0xA;
        Player_DecelerateToZero(this);
    } else {
        this->stateFlags3 |= PLAYER_STATE3_MM_40;
    }

    return true;
}

s32 Player_UpdateSlingshotAimReady(Player* this, PlayState* play) {
    s32 animFinished = PlayerAnimation_Update(play, &this->skelAnimeUpper);

    if (!Player_CheckRangedShieldDraw(play, this) &&
    (((this->unk_B28 < 0) && CustomSlingshot_HeldItemButtonIsHeldDown(this)) ||
     ((animFinished || (this->transformation != MM_PLAYER_FORM_DEKU)) &&
      CustomSlingshot_UseHeldItem(this)))) {
        this->unk_B28 = ABS(this->unk_B28);

        if (Player_StartSlingshotShot(this, play)) {
            PlayerAnimation_PlayOnce(
                play,
                &this->skelAnimeUpper,
                (PlayerAnimationHeader*)&gPlayerAnim_link_bow_bow_shoot_next
            );
        }
    } else {
        if (this->unk_ACC != 0) {
            this->unk_ACC--;
        }

        if (Player_IsZTargeting(this, play) ||
            (this->unk_AA5 != 0) ||
            (this->stateFlags1 & PLAYER_STATE1_MM_20)) {
            if (this->unk_ACC == 0) {
                this->unk_ACC++;
            }

            return true;
        }

        Player_SetUpperAction(play, this, Player_EndSlingshotShot);

        PlayerAnimation_PlayOnce(
            play,
            &this->skelAnimeUpper,
            (PlayerAnimationHeader*)&gPlayerAnim_link_bow_bow_shoot_end
        );

        this->unk_ACC = 0;
    }

    return true;
}

s32 Player_EndSlingshotShot(Player* this, PlayState* play) {
    if (!(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) ||
        PlayerAnimation_Update(play, &this->skelAnimeUpper)) {
        Player_SetUpperAction(play, this, Player_CheckSlingshotReadyOrStart);
    }

    return true;
}

s32 Player_CheckSlingshotReadyOrStart(Player* this, PlayState* play) {
    if (this->unk_B28 >= 0) {
        this->unk_B28 = -this->unk_B28;
    }

    if (!Player_CheckRangedShieldDraw(play, this) &&
        !CustomSlingshot_CheckReadyOrIdle(this, play)) {
        return false;
    }

    return true;
}