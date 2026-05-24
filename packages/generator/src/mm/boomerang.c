#include <combo.h>
#include <combo/mm/boomerang.h>
#include <combo/mm/player.h>
#include <combo/custom_animations.h>
#include <assets/mm/objects/gameplay_keep.h>
#include <combo/common/animation.h>
#include <combo/controller.h>
#include "actors/Custom_Boomerang/Custom_Boomerang.h"

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

#define PLAYER_ANIMGROUP_WAIT   0
#define PLAYER_ANIMTYPE_MAX     6

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

#define Player_TryShield                    ((MmPlayerTryShieldFunc)OverlayAddr(MM_PLAYER_TRY_SHIELD))
#define Player_SetUpperAction               ((MmPlayerSetUpperActionFunc)OverlayAddr(MM_PLAYER_SET_UPPER_ACTION))
#define Player_DrawBowOrHookshot            ((MmPlayerDrawBowOrHookshotFunc)OverlayAddr(MM_PLAYER_DRAW_BOW_OR_HOOKSHOT))
#define Player_IsZTargeting                 ((MmPlayerIsZTargetingFunc)OverlayAddr(MM_PLAYER_IS_Z_TARGETING))
#define Player_AnimSfx_PlayVoice            ((MmPlayerAnimSfxPlayVoiceFunc)OverlayAddr(MM_PLAYER_ANIM_SFX_PLAY_VOICE))
#define Player_SetParallel                  ((MmPlayerSetParallelFunc)OverlayAddr(MM_PLAYER_SET_PARALLEL))
#define Player_UpdateHostileLockOn          ((MmPlayerUpdateHostileLockOnFunc)OverlayAddr(MM_PLAYER_UPDATE_HOSTILE_LOCK_ON))
#define sPlayerUseHeldItem                  (*(s32*)OverlayAddr(MM_PLAYER_USE_HELD_ITEM))
#define sPlayerHeldItemButtonIsHeldDown     (*(s32*)OverlayAddr(MM_PLAYER_HELD_ITEM_BUTTON_IS_HELD_DOWN))
#define GET_PLAYER_ANIM_CUSTOM(group, type) (((PlayerAnimationHeader**)OverlayAddr(MM_PLAYER_ANIM_TABLE))[(group) * PLAYER_ANIMTYPE_MAX + (type)])

#define PLAYER_FOCUS_ACTOR(player)              (*(Actor**)((u8*)(player) + 0x730))
#define PLAYER_ZORA_BOOMERANG_ACTOR(player)     (*(Actor**)((u8*)(player) + 0xA7C))

s32 Player_InitItemAction_CustomBoomerang(Player* player, PlayState* play);
s32 Player_InitItemAction_CustomBoomerang_Upper(Player* player, PlayState* play);
s32 Player_SetupRangedOrCustomBoomerangAction(Player* player, PlayState* play);
s32 Player_CheckBButtonPressedWithAmmo(PlayState* play);
s32 Player_CheckCustomBoomerangAimReady(Player* player, PlayState* play);
s32 Player_TryStartHeldItemAction(Player* player, PlayState* play);
s32 Player_CustomBoomerang_WaitForReturn(Player* player, PlayState* play);
s32 Player_CustomBoomerang_PlayCatchAnimation(Player* player, PlayState* play);
s32 Player_CustomBoomerang_StartReadyAnimation(Player* player, PlayState* play);
s32 Player_CustomBoomerang_HoldReadyUntilRelease(Player* player, PlayState* play);
s32 Player_CustomBoomerang_ThrowOnRelease(Player* player, PlayState* play);
s32 Player_IsCustomBoomerangUpperActionActive(Player* player);
s32 Player_ShouldUseCustomBoomerangWaitAnim(Player* player);
PlayerAnimationHeader* Player_GetCustomBoomerangWaitAnimRight(Player* player);
PlayerAnimationHeader* Player_GetCustomBoomerangWaitAnimLeft(Player* player);
PlayerAnimationHeader* Player_GetCustomBoomerangSideWalkAnimRight(Player* player);
PlayerAnimationHeader* Player_GetCustomBoomerangSideWalkAnimLeft(Player* player);

s32 Player_CheckBButtonPressedWithAmmo(PlayState* play) {
    return (play->bButtonAmmoPlusOne > 0) &&
           CHECK_BTN_ALL(play->state.input[0].press.button, BTN_B);
}

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

s32 Player_InitItemAction_CustomBoomerang_Upper(Player* player, PlayState* play) {
    player->stateFlags1 |= PLAYER_STATE1_MM_1000000;
    return true;
}

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

s32 Player_CustomBoomerang_PlayCatchAnimation(Player* player, PlayState* play) {
    if (!Player_InitItemAction_CustomBoomerang(player, play) &&
        PlayerAnimation_Update(play, &player->skelAnimeUpper)) {
        Player_SetUpperAction(play, player, Player_InitItemAction_CustomBoomerang);
    }

    return true;
}

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

s32 Player_IsCustomBoomerangUpperActionActive(Player* player) {
    return player->stateFlags1 & PLAYER_STATE1_MM_1000000;
}

s32 Player_ShouldUseCustomBoomerangWaitAnim(Player* player) {
    return Player_IsCustomBoomerangUpperActionActive(player) && (player->unk_ACC != 0);
}

PlayerAnimationHeader* Player_GetCustomBoomerangWaitAnimRight(Player* player) {
    if (Player_ShouldUseCustomBoomerangWaitAnim(player)) {
        return (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_waitR;
    }

    return GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_WAIT, player->modelAnimType);
}

PlayerAnimationHeader* Player_GetCustomBoomerangWaitAnimLeft(Player* player) {
    if (Player_ShouldUseCustomBoomerangWaitAnim(player)) {
        return (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_waitL;
    }

    return GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_WAIT, player->modelAnimType);
}

PlayerAnimationHeader* Player_GetCustomBoomerangSideWalkAnimRight(Player* player) {
    if (Player_ShouldUseCustomBoomerangWaitAnim(player)) {
        return (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_side_walkR;
    }

    return GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_WAIT, player->modelAnimType);
}

PlayerAnimationHeader* Player_GetCustomBoomerangSideWalkAnimLeft(Player* player) {
    if (Player_ShouldUseCustomBoomerangWaitAnim(player)) {
        return (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_side_walkL;
    }

    return GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_WAIT, player->modelAnimType);
}

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

s32 Player_CustomBoomerang_StartReadyAnimation(Player* player, PlayState* play) {
    if (PlayerAnimation_Update(play, &player->skelAnimeUpper)) {
        Player_SetUpperAction(play, player, Player_CustomBoomerang_HoldReadyUntilRelease);

        PlayerAnimation_PlayLoop(
            play,
            &player->skelAnimeUpper,
            (PlayerAnimationHeader*)&gPlayerAnim_link_boom_throw_waitR
        );
    }

    Player_CheckCustomBoomerangAimReady(player, play);

    return true;
}

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
        PlayerAnimation_PlayLoop(
            play,
            &player->skelAnime,
            (PlayerAnimationHeader*)&gPlayerAnim_link_uma_anim_walk
        );
    } else if ((player->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
               !Player_UpdateHostileLockOn(player)) {
        PlayerAnimation_PlayLoop(
            play,
            &player->skelAnime,
            GET_PLAYER_ANIM_CUSTOM(PLAYER_ANIMGROUP_WAIT, player->modelAnimType)
        );
    }

    return true;
}
