#include <combo.h>
#include <combo/player.h>
#include <combo/mask.h>
#include <combo/oot/player_action.h>
#include "combo/oot/player.h"
#include <combo/common/animation.h>
#include <combo/custom_animations.h>

typedef s32 (*LinkAnimation_Update_t)(PlayState* play, SkelAnime* skelAnime);
typedef void (*LinkAnimation_PlayOnce_t)(PlayState* play, SkelAnime* skelAnime, LinkAnimationHeader* anim);
typedef void (*LinkAnimation_PlayLoopSetSpeed_t)(
    PlayState* play,
    SkelAnime* skelAnime,
    LinkAnimationHeader* anim,
    f32 speed
);

#define LinkAnimation_Update_ADDR           0x8008bca4
#define LinkAnimation_PlayOnce_ADDR         0x8008c178
#define LinkAnimation_PlayLoopSetSpeed_ADDR 0x8008c298

#define LinkAnimation_Update_OOT \
    ((LinkAnimation_Update_t)LinkAnimation_Update_ADDR)

#define LinkAnimation_PlayOnce_OOT \
    ((LinkAnimation_PlayOnce_t)LinkAnimation_PlayOnce_ADDR)

#define LinkAnimation_PlayLoopSetSpeed_OOT \
    ((LinkAnimation_PlayLoopSetSpeed_t)LinkAnimation_PlayLoopSetSpeed_ADDR)

#define KAMARO_DANCE_STATE_FRAME PLAYER_STATE2_5

#define PLAYER_CUSTOM_STATE2_KAMARO_DANCE (1 << 0)

#define KAMARO_DANCE_START_FRAME 0.0f
#define KAMARO_DANCE_END_FRAME   144.0f
#define KAMARO_DANCE_FRAME_COUNT 145.0f
#define KAMARO_DANCE_SPEED       1.0f

#define KAMARO_DANCE_BLOCK_START_BUTTONS \
    (A_BUTTON | R_TRIG | Z_TRIG | U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)

#define KAMARO_DANCE_START_SWALLOW_PRESS_BUTTONS \
    (B_BUTTON | U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)

#define KAMARO_DANCE_START_SWALLOW_CUR_BUTTONS \
    (B_BUTTON)

#define KAMARO_DANCE_ACTIVE_SWALLOW_PRESS_BUTTONS \
    (B_BUTTON | U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS)

static Player* sPlayerCustomStateOwner;
static u32 sPlayerCustomState2;
static s32 sKamaroMusicActive;
static f32 sKamaroDanceFrame;

static s8 sKamaroSavedHeldItemAction;
static s8 sKamaroSavedItemAction;
static s8 sKamaroSavedPrevHeldItemAction;
static s8 sKamaroSavedPrevItemAction;
static s32 sKamaroHasSavedItemAction;
static s32 sKamaroHadHeldItem;
static s32 sKamaroDanceStartGrace;

static void Player_KamaroDanceActionOoT(Player* link, PlayState* play);

void Player_SetModels(Player* this, s32 modelGroup);
s32 Player_ActionToModelGroup(Player* this, s32 itemAction);

static void Player_CustomState2_ResetIfStale(Player* link)
{
    if (sPlayerCustomStateOwner != link)
    {
        sPlayerCustomStateOwner = link;
        sPlayerCustomState2 = 0;
    }
}

static void Player_CustomState2_Set(Player* link, u32 flags)
{
    Player_CustomState2_ResetIfStale(link);
    sPlayerCustomState2 |= flags;
}

static void Player_CustomState2_Clear(Player* link, u32 flags)
{
    Player_CustomState2_ResetIfStale(link);
    sPlayerCustomState2 &= ~flags;
}

static s32 Player_CustomState2_Has(Player* link, u32 flags)
{
    Player_CustomState2_ResetIfStale(link);
    return (sPlayerCustomState2 & flags) != 0;
}

static void Player_CustomState2_ClearAll(Player* link)
{
    if (sPlayerCustomStateOwner == link)
    {
        sPlayerCustomState2 = 0;
    }
}

static s32 Player_KamaroGetCurrentItemActionOoT(Player* link)
{
    if (link->heldItemAction != PLAYER_IA_NONE)
    {
        return link->heldItemAction;
    }

    return link->itemAction;
}

static void Player_SetKamaroItemActionOoT(Player* link, s32 heldItemAction, s32 itemAction)
{
    link->heldItemAction = heldItemAction;
    link->itemAction = itemAction;
}

static void Player_HideKamaroHeldItemModelOoT(Player* link)
{
    Player_SetModels(link, Player_ActionToModelGroup(link, PLAYER_IA_NONE));
}

static void Player_RestoreKamaroHeldItemModelOoT(Player* link)
{
    Player_SetModels(link, Player_ActionToModelGroup(link, Player_KamaroGetCurrentItemActionOoT(link)));
}

static void Player_SaveKamaroItemActionOoT(Player* link)
{
    sKamaroSavedHeldItemAction = link->heldItemAction;
    sKamaroSavedItemAction = link->itemAction;

#ifdef PLAYER_HAS_PREV_ITEM_ACTION_FIELDS
    sKamaroSavedPrevHeldItemAction = link->prevHeldItemAction;
    sKamaroSavedPrevItemAction = link->prevItemAction;
#else
    sKamaroSavedPrevHeldItemAction = PLAYER_IA_NONE;
    sKamaroSavedPrevItemAction = PLAYER_IA_NONE;
#endif

    sKamaroHadHeldItem = Player_KamaroGetCurrentItemActionOoT(link) != PLAYER_IA_NONE;
    sKamaroHasSavedItemAction = true;
}

static void Player_PreserveKamaroItemActionOoT(Player* link)
{
    if (!sKamaroHasSavedItemAction)
    {
        return;
    }

    Player_SetKamaroItemActionOoT(link, sKamaroSavedHeldItemAction, sKamaroSavedItemAction);

#ifdef PLAYER_HAS_PREV_ITEM_ACTION_FIELDS
    link->prevHeldItemAction = sKamaroSavedPrevHeldItemAction;
    link->prevItemAction = sKamaroSavedPrevItemAction;
#endif
}

static void Player_ClearKamaroSavedItemActionOoT(void)
{
    sKamaroSavedHeldItemAction = PLAYER_IA_NONE;
    sKamaroSavedItemAction = PLAYER_IA_NONE;
    sKamaroSavedPrevHeldItemAction = PLAYER_IA_NONE;
    sKamaroSavedPrevItemAction = PLAYER_IA_NONE;
    sKamaroHadHeldItem = false;
    sKamaroHasSavedItemAction = false;
}

static void Player_RestoreKamaroItemActionOoT(Player* link)
{
    Player_PreserveKamaroItemActionOoT(link);
    Player_ClearKamaroSavedItemActionOoT();
}

s32 Player_IsKamaroMaskActiveOoT(Player* link)
{
    return Player_CustomState2_Has(link, PLAYER_CUSTOM_STATE2_KAMARO_DANCE) ||
           (link->actionFunc == Player_KamaroDanceActionOoT);
}

static void Player_ClearKamaroStateOoT(Player* link)
{
    link->stateFlags2 &= ~KAMARO_DANCE_STATE_FRAME;
    Player_CustomState2_Clear(link, PLAYER_CUSTOM_STATE2_KAMARO_DANCE);
}

static void Player_StopKamaroMusicOoT(void)
{
    if (sKamaroMusicActive)
    {
        AudioSeq_QueueSeqCmd((1 << 28) | 0xff | (1 << 24) | (0 << 16));
        sKamaroMusicActive = false;
    }
}

static void Player_EndKamaroDanceOoT(PlayState* play, Player* link)
{
    s32 hadHeldItem = sKamaroHadHeldItem;

    Player_ClearKamaroStateOoT(link);
    Player_StopKamaroMusicOoT();

    sKamaroDanceStartGrace = false;

    Player_RestoreKamaroItemActionOoT(link);

    Player_SetupActionPreserveItemActionOoT(play, link, Player_GetIdleActionOoT(), 1);
    LinkAnimation_PlayOnce_OOT(play, &link->skelAnime, Player_GetIdleAnimOoT(link));

    if (hadHeldItem)
    {
        Player_RestoreKamaroHeldItemModelOoT(link);
    }

    link->yaw = link->actor.shape.rot.y;
}

void Player_StopKamaroMaskOoT(PlayState* play, Player* link)
{
    if (Player_IsKamaroMaskActiveOoT(link))
    {
        Player_EndKamaroDanceOoT(play, link);
    }
}

void Player_ResetKamaroMaskStateOoT(Player* link)
{
    link->stateFlags2 &= ~KAMARO_DANCE_STATE_FRAME;
    Player_CustomState2_ClearAll(link);

    sKamaroMusicActive = false;
    sKamaroDanceFrame = KAMARO_DANCE_START_FRAME;
    sKamaroDanceStartGrace = false;

    Player_ClearKamaroSavedItemActionOoT();
}

static s32 Player_KamaroItemActionIsCrouchStabMeleeOoT(s32 itemAction)
{
    return (itemAction == PLAYER_IA_SWORD_MASTER) ||
           (itemAction == PLAYER_IA_SWORD_KOKIRI) ||
           (itemAction == PLAYER_IA_SWORD_BIGGORON) ||
           (itemAction == PLAYER_IA_DEKU_STICK);
}

static s32 Player_KamaroCanInterruptCrouchStabOoT(Player* link)
{
    s32 itemAction = Player_KamaroGetCurrentItemActionOoT(link);

    return Player_KamaroItemActionIsCrouchStabMeleeOoT(itemAction) &&
           ((link->meleeWeaponAnimation == PLAYER_MWA_STAB_1H) ||
            (link->meleeWeaponAnimation == PLAYER_MWA_STAB_2H)) &&
           (link->stateFlags1 & PLAYER_STATE1_SHIELDING);
}

static s32 Player_KamaroStateBlocksStartOoT(Player* link)
{
    if (link->mask != MASK_KAMARO)
    {
        return true;
    }

    if (Player_IsKamaroMaskActiveOoT(link))
    {
        return true;
    }

    if (!(link->actor.bgCheckFlags & BGCHECKFLAG_GROUND))
    {
        return true;
    }

    if (link->stateFlags1 & (
        PLAYER_ACTOR_STATE_CUTSCENE_FROZEN |
        PLAYER_ACTOR_STATE_HOLD_ITEM |
        PLAYER_STATE1_DEAD |
        PLAYER_STATE1_TALKING
    ))
    {
        return true;
    }

    return false;
}

static s32 Player_KamaroInputBlocksStartOoT(Player* link, Input* input)
{
    if (input == NULL)
    {
        return true;
    }

    if (input->press.button & KAMARO_DANCE_BLOCK_START_BUTTONS)
    {
        return true;
    }

    if ((input->cur.button & R_TRIG) && !Player_KamaroCanInterruptCrouchStabOoT(link))
    {
        return true;
    }

    return false;
}

static s32 Player_KamaroShouldSwallowFailedStartOoT(Player* link, Input* input)
{
    return (input != NULL) &&
           (input->press.button & B_BUTTON) &&
           (link->mask == MASK_KAMARO) &&
           !Player_IsKamaroMaskActiveOoT(link) &&
           (input->cur.button & R_TRIG) &&
           !Player_KamaroCanInterruptCrouchStabOoT(link);
}

static s32 Player_CanStartKamaroDanceOoT(Player* link, Input* input)
{
    s32 canInterruptCrouchStab = Player_KamaroCanInterruptCrouchStabOoT(link);

    if (Player_KamaroStateBlocksStartOoT(link))
    {
        return false;
    }

    if (Player_KamaroInputBlocksStartOoT(link, input))
    {
        return false;
    }

    if (Player_UsingItem(link) && !canInterruptCrouchStab)
    {
        return false;
    }

    if ((link->actionFunc != Player_GetIdleActionOoT()) && !canInterruptCrouchStab)
    {
        return false;
    }

    return true;
}

static void Player_ApplyKamaroAnimRangeOoT(Player* link)
{
    link->skelAnime.startFrame = KAMARO_DANCE_START_FRAME;
    link->skelAnime.endFrame = KAMARO_DANCE_END_FRAME;
    link->skelAnime.animLength = KAMARO_DANCE_FRAME_COUNT;
    link->skelAnime.playSpeed = KAMARO_DANCE_SPEED;
}

static void Player_KamaroDanceActionOoT(Player* link, PlayState* play)
{
    link->stateFlags2 |= KAMARO_DANCE_STATE_FRAME;

    Player_DecelerateToZeroOoT(link);
    link->actor.speed = link->speedXZ;
}

static void Player_UpdateKamaroDanceAnimOoT(PlayState* play, Player* link)
{
    f32 oldPlaySpeed;

    link->stateFlags2 |= KAMARO_DANCE_STATE_FRAME;

    Player_ApplyKamaroAnimRangeOoT(link);

    sKamaroDanceFrame += KAMARO_DANCE_SPEED;

    while (sKamaroDanceFrame >= KAMARO_DANCE_FRAME_COUNT)
    {
        sKamaroDanceFrame -= KAMARO_DANCE_FRAME_COUNT;
    }

    while (sKamaroDanceFrame < KAMARO_DANCE_START_FRAME)
    {
        sKamaroDanceFrame += KAMARO_DANCE_FRAME_COUNT;
    }

    oldPlaySpeed = link->skelAnime.playSpeed;

    link->skelAnime.curFrame = sKamaroDanceFrame;
    link->skelAnime.playSpeed = 0.0f;

    LinkAnimation_Update_OOT(play, &link->skelAnime);

    link->skelAnime.curFrame = sKamaroDanceFrame;
    link->skelAnime.playSpeed = oldPlaySpeed;

    Player_DecelerateToZeroOoT(link);
    link->actor.speed = link->speedXZ;
}

static void Player_StartKamaroDanceOoT(PlayState* play, Player* link, Input* input)
{
    if (!Player_CanStartKamaroDanceOoT(link, input))
    {
        return;
    }

    Player_SaveKamaroItemActionOoT(link);

    Player_SetupActionPreserveItemActionOoT(play, link, Player_KamaroDanceActionOoT, 1);
    Player_HideKamaroHeldItemModelOoT(link);

    LinkAnimation_PlayLoopSetSpeed_OOT(
        play,
        &link->skelAnime,
        (LinkAnimationHeader*)&gPlayerAnim_alink_dance_loop,
        KAMARO_DANCE_SPEED
    );

    Player_ApplyKamaroAnimRangeOoT(link);

    sKamaroDanceFrame = KAMARO_DANCE_START_FRAME;
    link->skelAnime.curFrame = sKamaroDanceFrame;

    Player_CustomState2_Set(link, PLAYER_CUSTOM_STATE2_KAMARO_DANCE);
    sKamaroDanceStartGrace = true;

    sKamaroMusicActive = true;
    Audio_PlayFanfare(FANFARE_KAMARO);
}

void Player_UpdateKamaroMaskOoT(PlayState* play, Player* link, Input* input)
{
    if (input == NULL)
    {
        return;
    }

    if (Player_IsKamaroMaskActiveOoT(link) && link->mask != MASK_KAMARO)
    {
        Player_EndKamaroDanceOoT(play, link);
        return;
    }

    if (Player_CustomState2_Has(link, PLAYER_CUSTOM_STATE2_KAMARO_DANCE))
    {
        if (sKamaroDanceStartGrace)
        {
            sKamaroDanceStartGrace = false;
        }
        else if (!(input->cur.button & B_BUTTON))
        {
            Player_EndKamaroDanceOoT(play, link);
            return;
        }

        if (link->actionFunc != Player_KamaroDanceActionOoT)
        {
            Player_EndKamaroDanceOoT(play, link);
            return;
        }

        input->press.button &= ~KAMARO_DANCE_ACTIVE_SWALLOW_PRESS_BUTTONS;

        Player_PreserveKamaroItemActionOoT(link);
        Player_HideKamaroHeldItemModelOoT(link);
        Player_UpdateKamaroDanceAnimOoT(play, link);
        return;
    }

    if (input->press.button & B_BUTTON)
    {
        if (Player_CanStartKamaroDanceOoT(link, input))
        {
            Player_StartKamaroDanceOoT(play, link, input);

            if (Player_CustomState2_Has(link, PLAYER_CUSTOM_STATE2_KAMARO_DANCE))
            {
                input->press.button &= ~KAMARO_DANCE_START_SWALLOW_PRESS_BUTTONS;
                input->cur.button &= ~KAMARO_DANCE_START_SWALLOW_CUR_BUTTONS;

                Player_PreserveKamaroItemActionOoT(link);
                Player_HideKamaroHeldItemModelOoT(link);
                Player_UpdateKamaroDanceAnimOoT(play, link);
            }
        }
        else if (Player_KamaroShouldSwallowFailedStartOoT(link, input))
        {
            input->press.button &= ~B_BUTTON;
        }
    }
}
