#include <combo.h>
#include <combo/item.h>
#include <combo/mm/bombers_notebook.h>

#include "En_Yb.h"

#define FLAGS (ACTOR_FLAG_MM_1 | ACTOR_FLAG_MM_8 | ACTOR_FLAG_MM_10 | ACTOR_FLAG_MM_2000000)

#if defined(GAME_MM)
# define SEGADDR_EN_PLAYER_ANIM_DANCE_LOOP      SEGADDR_FROM_OFFSET(4, 0xcf98)
# define SEGADDR_EN_PLAYER_ANIM_WAIT            SEGADDR_FROM_OFFSET(4, 0xdf28)

# define SEGADDR_EN_YB_UNUSED_ANIM              SEGADDR_FROM_OFFSET(6, 0x0200)
# define SEGADDR_EN_YB_SKEL                     SEGADDR_FROM_OFFSET(6, 0x5f48)
#endif

void EnYb_Init(Actor_EnYb* this, GameState_Play* play);
void EnYb_Destroy(Actor_EnYb* this, GameState_Play* play);
void EnYb_Update(Actor_EnYb* this, GameState_Play* play);
void EnYb_Draw(Actor* this, GameState_Play* play);

void EnYb_Idle(Actor_EnYb* this, GameState_Play* play);
void EnYb_TeachingDanceFinish(Actor_EnYb* this, GameState_Play* play);
void EnYb_SetupLeaving(Actor_EnYb* this, GameState_Play* play);

void EnYb_UpdateAnimation(Actor_EnYb* this, GameState_Play* play);
void EnYb_FinishTeachingCutscene(Actor_EnYb* this);
void EnYb_Disappear(Actor_EnYb* this, GameState_Play* play);
void EnYb_ReceiveMask(Actor_EnYb* this, GameState_Play* play);
void EnYb_Talk(Actor_EnYb* this, GameState_Play* play);
void EnYb_TeachingDance(Actor_EnYb* this, GameState_Play* play);
void EnYb_WaitForMidnight(Actor_EnYb* this, GameState_Play* play);

void EnYb_ActorShadowFunc(Actor* this, Lights* mapper, GameState_Play* play);
void EnYb_ChangeAnim(GameState_Play* play, Actor_EnYb* this, s16 animIndex, u8 animMode, f32 morphFrames);
s32 EnYb_CanTalk(Actor_EnYb* this, GameState_Play* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 40, 0, { 0, 0, 0 } },
};

// crashes if I try to mod it in to look at it
//  assumption: draw uses two different skeleton functions, might be incompatible
static AnimationHeader* gYbUnusedAnimations[] = { SEGADDR_EN_YB_UNUSED_ANIM };

static PlayerAnimationHeader* gPlayerAnimations[] = {
    SEGADDR_EN_PLAYER_ANIM_WAIT,
    SEGADDR_EN_PLAYER_ANIM_DANCE_LOOP,
};

static Vec3f D_80BFB2E8 = { 0.0f, 0.5f, 0.0f };

static Vec3f D_80BFB2F4 = { 500.0f, -500.0f, 0.0f };

static Vec3f D_80BFB300 = { 500.0f, -500.0f, 0.0f };

void EnYb_Init(Actor_EnYb* this, GameState_Play* play) {
    s16 csId;
    s32 i;

    Actor_SetScale(&this->actor, 0.01f);
    ActorShape_Init(&this->actor.shape, 0.0f, EnYb_ActorShadowFunc, 20.0f);

    //! @bug this alignment is because of player animations, but should be using ALIGN16
    SkelAnime_InitFlex(play, &this->skelAnime, SEGADDR_EN_YB_SKEL, SEGADDR_EN_YB_UNUSED_ANIM,
                       (void*)((uintptr_t)this->jointTable & ~0xF), (void*)((uintptr_t)this->morphTable & ~0xF),
                       YB_LIMB_MAX);

    Animation_PlayLoop(&this->skelAnime, SEGADDR_EN_YB_UNUSED_ANIM);

    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actionFunc = EnYb_Idle;
    this->animIndex = 3; // gets overwritten to 2 in EnYb_ChangeAnim later
    this->actor.minVelocityY = -9.0f;
    this->actor.gravity = -1.0f;

    EnYb_ChangeAnim(play, this, 2, ANIMMODE_LOOP, 0.0f);

    csId = this->actor.csId;
    for (i = 0; i < ARRAY_SIZE(this->csIdList); i++) {
        this->csIdList[i] = csId;
        if (csId != CS_ID_NONE) {
            this->actor.csId = csId;
            csId = CutsceneManager_GetAdditionalCsId(this->actor.csId);
        }
    }

    this->csIdIndex = -1;
    this->actor.csId = this->csIdList[0];

    // between midnight and morning start spawned
    if (CURRENT_TIME < CLOCK_TIME(6, 0)) {
        this->alpha = 255;
    } else { // else (night 6pm to midnight): wait to appear
        this->alpha = 0;
        this->actionFunc = EnYb_WaitForMidnight;
        this->actor.flags &= ~ACTOR_FLAG_MM_1;
    }

    if (MM_GET_EVENT_WEEK(MM_EV(82,2))) {
        Actor_Kill(&this->actor);
    }
}

void EnYb_Destroy(Actor_EnYb* this, GameState_Play* play) {

    Collider_DestroyCylinder(play, &this->collider);
}

void func_80BFA2FC(GameState_Play* play) {
    if (gMmExtraFlags2.maskKamaro) {
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_KAMAROS_MASK);
    }
    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_KAMARO);
}

/**
 * Custom shadow draw function of type ActorShadowFunc.
 */
void EnYb_ActorShadowFunc(Actor* this, Lights* mapper, GameState_Play* play) {
    Vec3f oldPos;
    Actor_EnYb* thisx = (Actor_EnYb*)this;

    if (thisx->alpha > 0) {
        if (thisx->animIndex == 2) {
            f32 tempScale = (((27.0f - thisx->shadowPos.y) + thisx->actor.world.pos.y) * ((1 / 2.25f) * 0.001f)) + 0.01f;
            thisx->actor.scale.x = tempScale;
        }
        Math_Vec3f_Copy(&oldPos, &thisx->actor.world.pos);
        Math_Vec3f_Copy(&thisx->actor.world.pos, &thisx->shadowPos);
        func_800B4AEC(play, &thisx->actor, 50.0f);

        if (oldPos.y < thisx->actor.floorHeight) {
            thisx->actor.world.pos.y = thisx->actor.floorHeight;
        } else {
            thisx->actor.world.pos.y = oldPos.y;
        }

        ActorShadow_DrawCircle(&thisx->actor, mapper, play);
        Math_Vec3f_Copy(&thisx->actor.world.pos, &oldPos);
        thisx->actor.scale.x = 0.01f;
    }
}

void EnYb_ChangeAnim(GameState_Play* play, Actor_EnYb* this, s16 animIndex, u8 animMode, f32 morphFrames) {
    if ((animIndex < 0) || (animIndex > 2)) {
        return;
    }

    if ((animIndex == this->animIndex) && (animMode == ANIMMODE_LOOP)) {
        return;
    }

    if (animIndex > 0) {
        if (animMode == ANIMMODE_LOOP) {
            PlayerAnimation_Change(play, &this->skelAnime, gPlayerAnimations[animIndex - 1], 1.0f, 0.0f,
                                   Animation_GetLastFrame(gPlayerAnimations[animIndex - 1]), ANIMMODE_LOOP,
                                   morphFrames);
        } else {
            // unused case, (only called once with animMode = ANIMMODE_LOOP)
            PlayerAnimation_Change(play, &this->skelAnime, gPlayerAnimations[animIndex - 1], 1.0f, 0.0f,
                                   Animation_GetLastFrame(gPlayerAnimations[animIndex - 1]), ANIMMODE_LOOP,
                                   morphFrames);
        }
    } else {
        // unused case, (only called once with animIndex = 2)
        Animation_Change(&this->skelAnime, gYbUnusedAnimations[animIndex], 1.0f, 0.0f,
                         Animation_GetLastFrame(gYbUnusedAnimations[animIndex]), animMode, morphFrames);
    }
    this->animIndex = animIndex;
}

s32 EnYb_CanTalk(Actor_EnYb* this, GameState_Play* play) {
    if ((this->actor.xzDistToPlayer < 100.0f) && Player_IsFacingActor(&this->actor, 0x3000, play) &&
        Actor_IsFacingPlayer(&this->actor, 0x3000)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void EnYb_UpdateAnimation(Actor_EnYb* this, GameState_Play* play) {
    if (this->animIndex <= 0) {
        SkelAnime_Update(&this->skelAnime);
    } else {
        PlayerAnimation_Update(play, &this->skelAnime);
    }
}

void EnYb_FinishTeachingCutscene(Actor_EnYb* this) {
    if (this->csIdIndex != -1) {
        if (CutsceneManager_GetCurrentCsId() == this->csIdList[this->csIdIndex]) {
            CutsceneManager_Stop(this->csIdList[this->csIdIndex]);
        }
        this->csIdIndex = -1;
    }
}

void EnYb_ChangeCutscene(Actor_EnYb* this, s16 csIdIndex) {
    EnYb_FinishTeachingCutscene(this);
    this->csIdIndex = csIdIndex;
}

/**
 * Sets a flag that enables the Kamaro dancing proximity music at night.
 */
void EnYb_EnableProximityMusic(Actor_EnYb* this) {
    Actor_PlaySeq_FlaggedKamaroDance(&this->actor);
}

void EnYb_Disappear(Actor_EnYb* this, GameState_Play* play) {
    Vec3f sp60;
    s32 i;

    EnYb_UpdateAnimation(this, play);
    for (i = 3; i >= 0; i--) {
        sp60.x = Rand_CenteredFloat(60.0f) + this->actor.world.pos.x;
        sp60.z = Rand_CenteredFloat(60.0f) + this->actor.world.pos.z;
        sp60.y = Rand_CenteredFloat(50.0f) + (this->actor.world.pos.y + 20.0f);
        func_800B3030(play, &sp60, &D_80BFB2E8, &D_80BFB2E8, 100, 0, 2);
    }

    SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EN_EXTINCT);
    if (this->alpha > 10) {
        this->alpha -= 10;
    } else {
        Actor_Kill(&this->actor);
    }
}

void EnYb_SetupLeaving(Actor_EnYb* this, GameState_Play* play) {
    EnYb_UpdateAnimation(this, play);
    if (Actor_TalkOfferAccepted(&this->actor, &play->gs)) {
        this->actor.flags &= ~ACTOR_FLAG_MM_10000;
        this->actionFunc = EnYb_Talk;
        // I am counting on you
        PlayerDisplayTextBox(play, 0x147D, &this->actor);
        func_80BFA2FC(play);
    } else {
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    }
    EnYb_EnableProximityMusic(this);
}

void EnYb_ReceiveMask(Actor_EnYb* this, GameState_Play* play) {
    EnYb_UpdateAnimation(this, play);
    // Player is parent: receiving the Kamaro mask
    if (Actor_HasParent(&this->actor, play))
    {
        gMmExtraFlags2.maskKamaro = 1;
        this->actor.parent = NULL;
        this->actionFunc = EnYb_SetupLeaving;
        this->actor.flags |= ACTOR_FLAG_MM_10000;
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    }
    else
        comboGiveItemNpc(&this->actor, play, GI_MM_MASK_KAMARO, NPC_MM_MASK_KAMARO, 10000.0f, 100.0f);

    EnYb_EnableProximityMusic(this);
}

void EnYb_Talk(Actor_EnYb* this, GameState_Play* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 2, 0x1000, 0x200);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnYb_UpdateAnimation(this, play);

    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        switch (play->msgCtx.currentTextId) {
            case 0x147D: // I am counting on you
                Message_Close(play);
                this->actionFunc = EnYb_Disappear;
                MM_SET_EVENT_WEEK(MM_EV(82, 2)); // WEEKEVENTREG_82_04
                break;

            case 0x147C: // Spread my dance across the world
                if (Player_GetMask(play) == PLAYER_MASK_KAMARO) {
                    Message_Close(play);
                    this->actionFunc = EnYb_Idle;

                } else if (gMmExtraFlags2.maskKamaro) {
                    DisplayTextBox2(play, 0x147D); // I am counting on you
                    func_80BFA2FC(play);

                } else {
                    Message_Close(play);
                    this->actionFunc = EnYb_ReceiveMask;
                    EnYb_ReceiveMask(this, play);
                }
                break;

            default:
                Message_Close(play);
                this->actionFunc = EnYb_Idle;
                break;
        }
    }
    EnYb_EnableProximityMusic(this);
}

void EnYb_TeachingDanceFinish(Actor_EnYb* this, GameState_Play* play) {
    EnYb_UpdateAnimation(this, play);
    if (Actor_TalkOfferAccepted(&this->actor, &play->gs)) {
        this->actionFunc = EnYb_Talk;
        // Spread my dance across the world
        PlayerDisplayTextBox(play, 0x147C, &this->actor);
        this->actor.flags &= ~ACTOR_FLAG_MM_10000;
    } else {
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    }
    EnYb_EnableProximityMusic(this);
}

// dancing countdown
void EnYb_TeachingDance(Actor_EnYb* this, GameState_Play* play) {
    EnYb_UpdateAnimation(this, play);

    if (this->teachingCutsceneTimer > 0) {
        this->teachingCutsceneTimer--;
    } else {
        EnYb_FinishTeachingCutscene(this);
        this->actionFunc = EnYb_TeachingDanceFinish;
        this->actor.flags |= ACTOR_FLAG_MM_10000;
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    }
    EnYb_EnableProximityMusic(this);
}

void EnYb_Idle(Actor_EnYb* this, GameState_Play* play) {
    Actor_Player* player = GET_PLAYER(play);

    EnYb_UpdateAnimation(this, play);
    if ((this->actor.xzDistToPlayer < 180.0f) && (fabsf(this->actor.yDistanceFromLink) < 50.0f) &&
        (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) && (play->msgCtx.ocarinaSong == OCARINA_SONG_HEALING) &&
        (gSave.playerForm == MM_PLAYER_FORM_HUMAN)) {
        this->actionFunc = EnYb_TeachingDance;
        this->teachingCutsceneTimer = 0;
        EnYb_ChangeCutscene(this, 0);
    } else if (Actor_TalkOfferAccepted(&this->actor, &play->gs)) {
        func_80BFA2FC(play);
        this->actionFunc = EnYb_Talk;
        if (player->currentMask == PLAYER_MASK_KAMARO) {
            // I have taught you, go use it
            PlayerDisplayTextBox(play, 0x147C, &this->actor);
        } else {
            // regular talk to him first dialogue
            PlayerDisplayTextBox(play, 0x147B, &this->actor);
        }
    } else if (EnYb_CanTalk(this, play)) {
        Actor_OfferTalk(&this->actor, play, 120.0f);
    }

    if (this->playerOcarinaOut & 1) {
        if (!(player->state2 & (1 << 27) /* PLAYER_STATE2_8000000 */)) {
            this->playerOcarinaOut &= ~1;
        }
    } else if ((player->state2 & (1 << 27) /* PLAYER_STATE2_8000000 */) && (this->actor.xzDistToPlayer < 180.0f) &&
               (fabsf(this->actor.yDistanceFromLink) < 50.0f)) {
        this->playerOcarinaOut |= 1;
        Actor_PlaySfx(&this->actor, NA_SE_SY_TRE_BOX_APPEAR);
    }

    EnYb_EnableProximityMusic(this);
}

void EnYb_WaitForMidnight(Actor_EnYb* this, GameState_Play* play) {
    if (CURRENT_TIME < CLOCK_TIME(6, 0)) {
        EnYb_UpdateAnimation(this, play);
        this->alpha += 5;
        if (this->alpha > 250) {
            this->alpha = 255;
            this->actor.flags |= ACTOR_FLAG_MM_1;
            this->actionFunc = EnYb_Idle;
        }
        EnYb_EnableProximityMusic(this);
    }
}

void EnYb_Update(Actor_EnYb* this, GameState_Play* play) {
    if (CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_MM_1)) {
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
    if (CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_MM_1)) {
        Actor_MoveWithGravity(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 40.0f, 25.0f, 40.0f, UPDBGCHECKINFO_FLAG_MM_1 | UPDBGCHECKINFO_FLAG_MM_4);
    }

    this->actionFunc(this, play);

    if ((this->csIdIndex != -1) && (CutsceneManager_GetCurrentCsId() != this->csIdList[this->csIdIndex])) {
        if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
            CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
            CutsceneManager_Queue(this->csIdList[this->csIdIndex]);
        } else if (CutsceneManager_IsNext(this->csIdList[this->csIdIndex])) {
            if (this->csIdIndex == 0) {
                CutsceneManager_StartWithPlayerCs(this->csIdList[this->csIdIndex], &this->actor);
            }
        } else {
            CutsceneManager_Queue(this->csIdList[this->csIdIndex]);
        }
    }
}

void EnYb_PostLimbDrawOpa(GameState_Play* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor_EnYb* this) {

    if (limbIndex == YB_LIMB_HEAD) {
        Matrix_MultVec3f(&D_80BFB2F4, &this->actor.focus.pos);
    }
    if (limbIndex == YB_LIMB_LEGS_ROOT) {
        Matrix_MultVec3f(&gZeroVec3f, &this->shadowPos);
    }
}

void EnYb_PostLimbDrawXlu(GameState_Play* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* this, Gfx** gfx) {
    Actor_EnYb* thisx = (Actor_EnYb*)this;
    if (limbIndex == YB_LIMB_HEAD) {
        Matrix_MultVec3f(&D_80BFB300, &thisx->actor.focus.pos);
    }
    if (limbIndex == YB_LIMB_LEGS_ROOT) {
        Matrix_MultVec3f(&gZeroVec3f, &thisx->shadowPos);
    }
}

void EnYb_Draw(Actor* this, GameState_Play* play) {
    Actor_EnYb* thisx = (Actor_EnYb*)this;
    OPEN_DISPS(play->gs.gfx);

    if (thisx->alpha != 0) {
        if (thisx->alpha < 255) {
            if (thisx->alpha > 128) {
                Gfx_SetupDL71(POLY_XLU_DISP++);
                Scene_SetRenderModeXlu(play, 2, 2);
            } else {
                Gfx_SetupDL72(POLY_XLU_DISP++);
                Scene_SetRenderModeXlu(play, 1, 2);
            }
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, thisx->alpha);

            POLY_XLU_DISP =
                SkelAnime_DrawFlex(play, thisx->skelAnime.skeleton, thisx->skelAnime.jointTable,
                                   thisx->skelAnime.dListCount, NULL, EnYb_PostLimbDrawXlu, &thisx->actor, POLY_XLU_DISP);

        } else {
            Gfx_SetupDL25_Opa(play->gs.gfx);
            Scene_SetRenderModeXlu(play, 0, 1);
            SkelAnime_DrawFlexOpa(play, thisx->skelAnime.skeleton, thisx->skelAnime.jointTable,
                                  thisx->skelAnime.dListCount, NULL, EnYb_PostLimbDrawOpa, &thisx->actor);
        }
    }

    CLOSE_DISPS();
}

ActorProfile En_Yb_InitVars = {
    AC_EN_YB,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_YB,
    sizeof(Actor_EnYb),
    (ActorFunc)EnYb_Init,
    (ActorFunc)EnYb_Destroy,
    (ActorFunc)EnYb_Update,
    (ActorFunc)EnYb_Draw,
};

OVL_ACTOR_INFO(AC_EN_YB, En_Yb_InitVars);
