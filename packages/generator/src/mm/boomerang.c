#include <combo.h>
#include <combo/mm/boomerang.h>
#include <combo/mm/player.h>
#include <combo/custom_animations.h>
#include <assets/mm/objects/gameplay_keep.h>
#include <combo/common/animation.h>
#include <combo/controller.h>

#include "actors/Custom_Boomerang/Custom_Boomerang.h"

/* ============================================================================
 * MM Player Overlay Function Addresses
 * ========================================================================== */

#define MM_PLAYER_SET_UPPER_ACTION               0x8082F43C
#define MM_PLAYER_DRAW_BOW_OR_HOOKSHOT           0x808306F8
#define MM_PLAYER_IS_Z_TARGETING                 0x8082FBE8
#define MM_PLAYER_TRY_SHIELD                     0x80830B88
#define MM_PLAYER_USE_HELD_ITEM                  0x80862B48
#define MM_PLAYER_HELD_ITEM_BUTTON_IS_HELD_DOWN  0x80862B4C
#define MM_PLAYER_ANIM_SFX_PLAY_VOICE            0x8082DF8C
#define MM_PLAYER_SET_PARALLEL                   0x8083133C
#define MM_PLAYER_UPDATE_HOSTILE_LOCK_ON         0x8082FB68
#define MM_PLAYER_ANIM_TABLE                     0x8085BE84

#define MM_PLAYER_IS_HOLDING_HOOKSHOT            0x801240C8
#define MM_PLAYER_CHECK_HOSTILE_LOCK_ON          0x80123420
#define MM_PLAYER_ANIMATION_PLAY_LOOP            0x80136650
#define MM_ANIM_TASK_QUEUE_ADD_COPY              0x80135A28

#define CAMERA_CHECK_VALID_MODE                  0x800DF86C

/* ============================================================================
 * Player State / Animation Constants
 * ========================================================================== */

#ifndef PLAYER_ANIMGROUP_wait
#define PLAYER_ANIMGROUP_wait 0
#endif

#ifndef PLAYER_ANIMTYPE_MAX
#define PLAYER_ANIMTYPE_MAX 6
#endif

/* ============================================================================
 * Function Pointer Types
 * ========================================================================== */

typedef s32 (*MmPlayerTryShieldFunc)(PlayState* play, Player* player);

typedef void (*MmPlayerSetUpperActionFunc)(
    PlayState* play,
    Player* player,
    PlayerUpperActionFunc upperActionFunc
);

typedef s32 (*MmPlayerDrawBowOrHookshotFunc)(Player* player, PlayState* play);
typedef s32 (*MmPlayerIsZTargetingFunc)(Player* player, PlayState* play);
typedef s32 (*MmPlayerIsHoldingHookshotFunc)(Player* player);
typedef s32 (*MmPlayerCheckHostileLockOnFunc)(Player* player);
typedef s32 (*MmPlayerUpdateHostileLockOnFunc)(Player* player);

typedef void (*MmPlayerAnimSfxPlayVoiceFunc)(Player* player, u16 sfxId);
typedef void (*MmPlayerSetParallelFunc)(Player* player);

typedef s32 (*CameraCheckValidModeFunc)(Camera* camera, s16 mode);

typedef void (*PlayerAnimationPlayLoopFunc)(
    PlayState* play,
    SkelAnime* skelAnime,
    PlayerAnimationHeader* animation
);

typedef void (*MmAnimTaskQueueAddCopyFunc)(
    PlayState* play,
    s32 limbCount,
    Vec3s* dst,
    Vec3s* src
);

/* ============================================================================
 * Overlay Functions / Data
 * ========================================================================== */

#define Player_TryShield \
    ((MmPlayerTryShieldFunc)OverlayAddr(MM_PLAYER_TRY_SHIELD))

#define Player_SetUpperAction \
    ((MmPlayerSetUpperActionFunc)OverlayAddr(MM_PLAYER_SET_UPPER_ACTION))

#define Player_DrawBowOrHookshot \
    ((MmPlayerDrawBowOrHookshotFunc)OverlayAddr(MM_PLAYER_DRAW_BOW_OR_HOOKSHOT))

#define Player_IsZTargeting \
    ((MmPlayerIsZTargetingFunc)OverlayAddr(MM_PLAYER_IS_Z_TARGETING))

#define Player_AnimSfx_PlayVoice \
    ((MmPlayerAnimSfxPlayVoiceFunc)OverlayAddr(MM_PLAYER_ANIM_SFX_PLAY_VOICE))

#define Player_SetParallel \
    ((MmPlayerSetParallelFunc)OverlayAddr(MM_PLAYER_SET_PARALLEL))

#define Player_UpdateHostileLockOn \
    ((MmPlayerUpdateHostileLockOnFunc)OverlayAddr(MM_PLAYER_UPDATE_HOSTILE_LOCK_ON))

#define sPlayerUseHeldItem \
    (*(s32*)OverlayAddr(MM_PLAYER_USE_HELD_ITEM))

#define sPlayerHeldItemButtonIsHeldDown \
    (*(s32*)OverlayAddr(MM_PLAYER_HELD_ITEM_BUTTON_IS_HELD_DOWN))

#define GET_PLAYER_ANIM_CUSTOM(group, type) \
    (((PlayerAnimationHeader**)OverlayAddr(MM_PLAYER_ANIM_TABLE))[(group) * PLAYER_ANIMTYPE_MAX + (type)])

/* ============================================================================
 * Static Functions / Data
 * ========================================================================== */

#define Camera_CheckValidMode \
    ((CameraCheckValidModeFunc)CAMERA_CHECK_VALID_MODE)

#define Player_IsHoldingHookshot \
    ((MmPlayerIsHoldingHookshotFunc)MM_PLAYER_IS_HOLDING_HOOKSHOT)

#define PlayerAnimation_PlayLoop_MM \
    ((PlayerAnimationPlayLoopFunc)MM_PLAYER_ANIMATION_PLAY_LOOP)

#define AnimTaskQueue_AddCopy \
    ((MmAnimTaskQueueAddCopyFunc)MM_ANIM_TASK_QUEUE_ADD_COPY)

#define Player_CheckHostileLockOn \
    ((MmPlayerCheckHostileLockOnFunc)MM_PLAYER_CHECK_HOSTILE_LOCK_ON)

/* ============================================================================
 * Player Field Helpers
 * ========================================================================== */

#define PLAYER_FOCUS_ACTOR(player) \
    (*(Actor**)((u8*)(player) + 0x730))

#define PLAYER_ZORA_BOOMERANG_ACTOR(player) \
    (*(Actor**)((u8*)(player) + 0xA7C))

/* ============================================================================
 * Forward Declarations
 * ========================================================================== */

s32 Player_InitItemAction_CustomBoomerang(Player* player, PlayState* play);
s32 Player_InitItemAction_CustomBoomerang_Upper(Player* player, PlayState* play);

/* Original OoT address name: func_80834D2C */
s32 Player_SetupRangedOrCustomBoomerangAction(Player* player, PlayState* play);

/* Original OoT address name: func_80834E44 */
s32 Player_CheckBButtonPressedWithAmmo(PlayState* play);

/* Original OoT address name: func_80834EB8 */
s32 Player_CheckCustomBoomerangAimReady(Player* player, PlayState* play);

/* Original OoT address name: func_80834F2C */
s32 Player_TryStartHeldItemAction(Player* player, PlayState* play);

/* Original OoT address name: func_80835B60 */
s32 Player_CustomBoomerang_WaitForReturn(Player* player, PlayState* play);

/* Original OoT address name: func_80835C08 */
s32 Player_CustomBoomerang_PlayCatchAnimation(Player* player, PlayState* play);

/* Original OoT address name: func_80835884 */
s32 Player_CustomBoomerang_StartReadyAnimation(Player* player, PlayState* play);

/* Original OoT address name: func_808358F0 */
s32 Player_CustomBoomerang_HoldReadyUntilRelease(Player* player, PlayState* play);

/* Original OoT address name: func_808359FC */
s32 Player_CustomBoomerang_ThrowOnRelease(Player* player, PlayState* play);

/* Original OoT address name: func_808332E4 */
s32 Player_IsCustomBoomerangUpperActionActive(Player* player);

/* Original OoT address name: func_808334B4 */
s32 Player_ShouldUseCustomBoomerangWaitAnim(Player* player);

/* Original OoT address name: func_808334E4 */
PlayerAnimationHeader* Player_GetCustomBoomerangWaitAnimRight(Player* player);

/* Original OoT address name: func_80833528 */
PlayerAnimationHeader* Player_GetCustomBoomerangWaitAnimLeft(Player* player);

/* Original OoT address name: func_808335B0 */
PlayerAnimationHeader* Player_GetCustomBoomerangSideWalkAnimRight(Player* player);

/* Original OoT address name: func_808335F4 */
PlayerAnimationHeader* Player_GetCustomBoomerangSideWalkAnimLeft(Player* player);

/* ============================================================================
 * Item Action Init / Gate
 * ========================================================================== */

/* Original OoT address name: func_80834E44 */
s32 Player_CheckBButtonPressedWithAmmo(PlayState* play) {
    return (play->bButtonAmmoPlusOne > 0) &&
           CHECK_BTN_ALL(play->state.input[0].press.button, BTN_B);
}

/* Original OoT address name: func_80834F2C */
s32 Player_TryStartHeldItemAction(Player* player, PlayState* play) {
    if ((player->doorType == 0) &&
        !(player->stateFlags1 & PLAYER_STATE1_MM_2000000)) {
        if (sPlayerUseHeldItem || Player_CheckBButtonPressedWithAmmo(play)) {
            if (Player_SetupRangedOrCustomBoomerangAction(player, play)) {
                return Player_CheckCustomBoomerangAimReady(player, play);
            }
        }
    }

    return false;
}

/*
 * This used to be func_80835800.
 */
s32 Player_InitItemAction_CustomBoomerang(Player* player, PlayState* play) {
    if (Player_TryShield(play, player)) {
        return true;
    }

    if (player->stateFlags1 & PLAYER_STATE1_MM_2000000) {
        Player_SetUpperAction(play, player, Player_CustomBoomerang_WaitForReturn);
        return true;
    }

    if (Player_TryStartHeldItemAction(player, play)) {
        return true;
    }

    return false;
}

/*
 * Init hook used by Player_RunCustomItemActionInitFunc.
 */
s32 Player_InitItemAction_CustomBoomerang_Upper(Player* player, PlayState* play) {
    player->stateFlags1 |= PLAYER_STATE1_MM_1000000;
    return true;
}

/* ============================================================================
 * Boomerang Catch
 * ========================================================================== */

/* Original OoT address name: func_80835B60 */
s32 Player_CustomBoomerang_WaitForReturn(Player* player, PlayState* play) {
    if (Player_TryShield(play, player)) {
        return true;
    }

    if (!(player->stateFlags1 & PLAYER_STATE1_MM_2000000)) {
        Player_SetUpperAction(play, player, Player_CustomBoomerang_PlayCatchAnimation);

        PlayerAnimation_PlayOnce(
            play,
            &player->skelAnimeUpper,
            (PlayerAnimationHeader*)&gPlayerAnim_link_boom_catch
        );

        Player_PlaySfx(player, NA_SE_PL_CATCH_BOOMERANG);
        Player_AnimSfx_PlayVoice(player, NA_SE_VO_LI_SWORD_N);

        return true;
    }

    return false;
}

/* Original OoT address name: func_80835C08 */
s32 Player_CustomBoomerang_PlayCatchAnimation(Player* player, PlayState* play) {
    if (!Player_InitItemAction_CustomBoomerang(player, play) &&
        PlayerAnimation_Update(play, &player->skelAnimeUpper)) {
        Player_SetUpperAction(play, player, Player_InitItemAction_CustomBoomerang);
    }

    return true;
}

/* Original OoT address name: func_80834EB8 */
s32 Player_CheckCustomBoomerangAimReady(Player* player, PlayState* play) {
    if ((player->unk_AA5 == 0) || (player->unk_AA5 == 3)) {
        if (Player_IsZTargeting(player, play) || (PLAYER_FOCUS_ACTOR(player) != NULL) ||
            (Camera_CheckValidMode(Play_GetCamera(play, CAM_ID_MAIN), CAM_MODE_BOWARROW) == 0)) {
            return true;
            }

        player->unk_AA5 = 3;
    }

    return false;
}

/* ============================================================================
 * Custom Wait Animation Helpers
 * ========================================================================== */

/* Original OoT address name: func_808332E4 */
s32 Player_IsCustomBoomerangUpperActionActive(Player* player) {
    return player->stateFlags1 & PLAYER_STATE1_MM_1000000;
}

/* Original OoT address name: func_808334B4 */
s32 Player_ShouldUseCustomBoomerangWaitAnim(Player* player) {
    return Player_IsCustomBoomerangUpperActionActive(player) && (player->unk_ACC != 0);
}

/* Original OoT address name: func_808334E4 */
PlayerAnimationHeader* Player_GetCustomBoomerangWaitAnimRight(Player* player) {
    if (Player_ShouldUseCustomBoomerangWaitAnim(player)) {
        return (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_waitR;
    }

    return GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_wait, player->modelAnimType);
}

/* Original OoT address name: func_80833528 */
PlayerAnimationHeader* Player_GetCustomBoomerangWaitAnimLeft(Player* player) {
    if (Player_ShouldUseCustomBoomerangWaitAnim(player)) {
        return (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_waitL;
    }

    return GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_wait, player->modelAnimType);
}

/* Original OoT address name: func_808335B0 */
PlayerAnimationHeader* Player_GetCustomBoomerangSideWalkAnimRight(Player* player) {
    if (Player_ShouldUseCustomBoomerangWaitAnim(player)) {
        return (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_side_walkR;
    }

    return GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_wait, player->modelAnimType);
}

/* Original OoT address name: func_808335F4 */
PlayerAnimationHeader* Player_GetCustomBoomerangSideWalkAnimLeft(Player* player) {
    if (Player_ShouldUseCustomBoomerangWaitAnim(player)) {
        return (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_side_walkL;
    }

    return GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_wait, player->modelAnimType);
}

/* ============================================================================
 * Boomerang Throw
 * ========================================================================== */

/* Original OoT address name: func_808359FC */
s32 Player_CustomBoomerang_ThrowOnRelease(Player* player, PlayState* play) {
    if (PlayerAnimation_Update(play, &player->skelAnimeUpper)) {
        Player_SetUpperAction(play, player, Player_CustomBoomerang_WaitForReturn);

        player->unk_ACC = 0;
        player->stateFlags1 &= ~PLAYER_STATE1_MM_1000000;
    } else if (PlayerAnimation_OnFrame(&player->skelAnimeUpper, 6.0f)) {
        f32 posX;
        f32 posZ;
        s32 yaw;
        Actor_CustomBoomerang* boomerang;

        posX = (Math_SinS(player->actor.shape.rot.y) * 10.0f) + player->actor.world.pos.x;
        posZ = (Math_CosS(player->actor.shape.rot.y) * 10.0f) + player->actor.world.pos.z;

        yaw = (PLAYER_FOCUS_ACTOR(player) != NULL)
            ? player->actor.shape.rot.y + 14000
            : player->actor.shape.rot.y;

        player->stateFlags1 |= PLAYER_STATE1_MM_2000000;

        boomerang = (Actor_CustomBoomerang*)Actor_Spawn(
            &play->actorCtx,
            play,
            ACTOR_CUSTOM_BOOMERANG,
            posX,
            player->actor.world.pos.y + 30.0f,
            posZ,
            player->actor.focus.rot.x,
            yaw,
            0,
            0
        );

        if (boomerang != NULL) {
            boomerang->moveTo = PLAYER_FOCUS_ACTOR(player);
            boomerang->returnTimer = 20;

            PLAYER_ZORA_BOOMERANG_ACTOR(player) = &boomerang->actor;

            if (!Player_CheckHostileLockOn(player)) {
                Player_SetParallel(player);
            }

            player->unk_D57 = 4;

            Player_PlaySfx(player, NA_SE_IT_BOOMERANG_THROW);
            Player_AnimSfx_PlayVoice(player, NA_SE_VO_LI_SWORD_N);
        } else {
            player->stateFlags1 &= ~PLAYER_STATE1_MM_2000000;
        }
    }

    return true;
}

/* Original OoT address name: func_808358F0 */
s32 Player_CustomBoomerang_HoldReadyUntilRelease(Player* player, PlayState* play) {
    PlayerAnimationHeader* animSeg = player->skelAnime.animation;

    if ((Player_GetCustomBoomerangWaitAnimRight(player) == animSeg) ||
        (Player_GetCustomBoomerangWaitAnimLeft(player) == animSeg) ||
        (Player_GetCustomBoomerangSideWalkAnimRight(player) == animSeg) ||
        (Player_GetCustomBoomerangSideWalkAnimLeft(player) == animSeg)) {
        AnimTaskQueue_AddCopy(
            play,
            player->skelAnime.limbCount,
            player->skelAnimeUpper.jointTable,
            player->skelAnime.jointTable
        );
    } else {
        PlayerAnimation_Update(play, &player->skelAnimeUpper);
    }

    Player_CheckCustomBoomerangAimReady(player, play);

    if (!sPlayerHeldItemButtonIsHeldDown) {
        Player_SetUpperAction(play, player, Player_CustomBoomerang_ThrowOnRelease);

        PlayerAnimation_PlayOnce(
            play,
            &player->skelAnimeUpper,
            (player->unk_B08 < 0.5f)
                ? (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throwR
                : (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throwL
        );
    }

    return true;
}

/* Original OoT address name: func_80835884 */
s32 Player_CustomBoomerang_StartReadyAnimation(Player* player, PlayState* play) {
    if (PlayerAnimation_Update(play, &player->skelAnimeUpper)) {
        Player_SetUpperAction(play, player, Player_CustomBoomerang_HoldReadyUntilRelease);

        PlayerAnimation_PlayLoop_MM(
            play,
            &player->skelAnimeUpper,
            (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_waitR
        );
    }

    Player_CheckCustomBoomerangAimReady(player, play);

    return true;
}

/* Original OoT address name: func_80834D2C */
s32 Player_SetupRangedOrCustomBoomerangAction(Player* player, PlayState* play) {
    PlayerAnimationHeader* anim;

    if (player->heldItemAction != PLAYER_CUSTOM_IA_BOOMERANG) {
        if (!Player_DrawBowOrHookshot(player, play)) {
            return false;
        }

        if (Player_IsHoldingHookshot(player)) {
            anim = (PlayerAnimationHeader*)&gPlayerAnim_link_hook_shot_ready;
        } else {
            anim = (PlayerAnimationHeader*)&gPlayerAnim_link_bow_bow_ready;
        }

        PlayerAnimation_PlayOnce(play, &player->skelAnimeUpper, anim);
    } else {
        Player_SetUpperAction(play, player, Player_CustomBoomerang_StartReadyAnimation);

        player->stateFlags1 |= PLAYER_STATE1_MM_1000000;
        player->unk_ACC = 10;

        PlayerAnimation_PlayOnce(
            play,
            &player->skelAnimeUpper,
            (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_wait2waitR
        );
    }

    if (player->stateFlags1 & PLAYER_STATE1_MM_800000) {
        PlayerAnimation_PlayLoop_MM(
            play,
            &player->skelAnime,
            (PlayerAnimationHeader*)&gPlayerAnim_link_uma_anim_walk
        );
    } else if ((player->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
               !Player_UpdateHostileLockOn(player)) {
        PlayerAnimation_PlayLoop_MM(
            play,
            &player->skelAnime,
            GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_wait, player->modelAnimType)
        );
    }

    return true;
}