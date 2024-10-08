#include <combo.h>
#include <combo/play/collision_context.h>
#include <combo/misc.h>
#include "Bg_Ingate.h"

#define FLAGS (ACTOR_FLAG_MM_10 | ACTOR_FLAG_MM_20)

#if defined(GAME_MM)
#define SEGADDR_BG_INGATE_DL    ((void*)(0x06000000+0x6b0))
#define SEGADDR_BG_INGATE_COL   ((void*)(0x06000000+0x16dc))
#endif

void BgIngate_Init(Actor_BgIngate* this, GameState_Play* play);
void BgIngate_Destroy(Actor_BgIngate* this, GameState_Play* play);
void BgIngate_Update(Actor_BgIngate* this, GameState_Play* play);
void BgIngate_Draw(Actor_BgIngate* this, GameState_Play* play);

Actor* BgIngate_FindActor(Actor_BgIngate* this, GameState_Play* play, u8 actorCategory, s16 actorId);
s32 func_80953BEC(Actor_BgIngate* this);
void func_80953B40(Actor_BgIngate* this);
void func_80953F8C(Actor_BgIngate* this, GameState_Play* play);
void BgIngate_EndCruise(Actor_BgIngate* this, GameState_Play* play);
void func_809541B8(Actor_BgIngate* this, GameState_Play* play);
void func_809542A0(Actor_BgIngate* this, GameState_Play* play);
void func_80954340(Actor_BgIngate* this, GameState_Play* play);
void func_809543D4(Actor_BgIngate* this, GameState_Play* play);

/**
 * @brief Searches for an actor based on the parameters given to the function. Returns Actor* of actor found or NULL
 *
 * @param this
 * @param play
 * @param actorCategory - Category of Actor
 * @param actorId - ID of actor to search for
 * @return Actor*
 */
Actor* BgIngate_FindActor(Actor_BgIngate* this, GameState_Play* play, u8 actorCategory, s16 actorId) {
    Actor* actorIter = NULL;

    while (1) {
        actorIter = SubS_FindActor(play, actorIter, actorCategory, actorId);

        if (actorIter == NULL) {
            break;
        }

        if ((this != (Actor_BgIngate*)actorIter) && (actorIter->update != NULL)) {
            break;
        }

        if (actorIter->next == NULL) {
            actorIter = NULL;
            break;
        }
        actorIter = actorIter->next;
    }

    return actorIter;
}

void func_80953B40(Actor_BgIngate* this) {
    s32 temp;

    if (!MM_CHECK_EVENT_INF(EVENTINF_35)) {
        this->timePathTotalTime = 4 * 1000;
        this->timePathTimeSpeed = 8;
    } else {
        this->timePathTotalTime = 1 * 2000;
        this->timePathTimeSpeed = 1;
    }
    temp = this->timePath->count - (SUBS_TIME_PATHING_ORDER - 1);
    this->timePathWaypointTime = this->timePathTotalTime / temp;
    this->timePathWaypoint = SUBS_TIME_PATHING_ORDER - 1;
    this->timePathElapsedTime = 0;
    this->flags &= ~0x1;
    this->flags &= ~0x2;
}

s32 func_80953BEC(Actor_BgIngate* this) {
    f32 knots[265];
    Vec3f sp68;
    Vec3f sp5C;
    Vec3f timePathTargetPos;
    s16 yaw;

    SubS_TimePathing_FillKnots(knots, SUBS_TIME_PATHING_ORDER, this->timePath->count + SUBS_TIME_PATHING_ORDER);
    if (!(this->flags & 1)) {
        timePathTargetPos = gZeroVec3f;
        SubS_TimePathing_Update(this->timePath, &this->timePathProgress, &this->timePathElapsedTime,
                                this->timePathWaypointTime, this->timePathTotalTime, &this->timePathWaypoint, knots,
                                &timePathTargetPos, this->timePathTimeSpeed);
        this->flags |= 1;
    } else {
        timePathTargetPos = this->timePathTargetPos;
    }
    this->dyna.actor.world.pos.x = timePathTargetPos.x;
    this->dyna.actor.world.pos.z = timePathTargetPos.z;
    this->timePathTargetPos = gZeroVec3f;
    if (SubS_TimePathing_Update(this->timePath, &this->timePathProgress, &this->timePathElapsedTime,
                                this->timePathWaypointTime, this->timePathTotalTime, &this->timePathWaypoint, knots,
                                &this->timePathTargetPos, this->timePathTimeSpeed)) {
        this->flags |= 2;
    } else {
        sp68 = this->dyna.actor.world.pos;
        sp5C = this->timePathTargetPos;
        yaw = Math_Vec3f_Yaw(&sp68, &sp5C);
        this->dyna.actor.world.rot.y = yaw;
        this->dyna.actor.shape.rot.y = yaw;
    }

    return 0;
}

s32 func_80953DA8(Actor_BgIngate* this, GameState_Play* play) {
    Camera* mainCam = Play_GetCamera(play, CAM_ID_MAIN);

    if (MM_CHECK_EVENT_INF(EVENTINF_35)) {
        Player_SetCsActionWithHaltedActors(play, &this->dyna.actor, PLAYER_CSACTION_WAIT);
    } else {
        MM_SET_EVENT_INF(EVENTINF_41);
    }
    Camera_ChangeSetting(mainCam, CAM_SET_BOAT_CRUISE);
    play->bButtonAmmoPlusOne = 99;

    return 0;
}

void func_80953E38(GameState_Play* play) {
    Camera_ChangeSetting(Play_GetCamera(play, CAM_ID_MAIN), CAM_SET_NORMAL0);

    if (!MM_CHECK_EVENT_INF(EVENTINF_35)) {
        MM_CLEAR_EVENT_INF(EVENTINF_41);
    }

    play->bButtonAmmoPlusOne = -1;
}

void func_80953EA4(Actor_BgIngate* this, GameState_Play* play) {
    Actor_Player* player = GET_PLAYER(play);

    Player_SetCsActionWithHaltedActors(play, &this->dyna.actor, PLAYER_CSACTION_58);
    player->unk_3A0.x = this->dyna.actor.world.pos.x;
    player->unk_3A0.z = this->dyna.actor.world.pos.z;
    this->flags &= ~0x4;
    this->unk16A = 0x1E;
    this->actionFunc = func_80954340;
}

void func_80953F14(Actor_BgIngate* this, GameState_Play* play) {
    Actor_Player* player = GET_PLAYER(play);

    player->actor.shape.rot.y = this->dyna.actor.shape.rot.y;
    player->actor.world.rot.y = player->actor.shape.rot.y;
    player->currentYaw = player->actor.shape.rot.y;
    player->actor.focus.rot.y = player->actor.shape.rot.y;
    this->flags |= 0x10;
    func_80953DA8(this, play);
    if (this->timePath != NULL) {
        func_80953B40(this);
    }
    this->csId = CS_ID_NONE;
    this->actionFunc = BgIngate_EndCruise;
}

void func_80953F8C(Actor_BgIngate* this, GameState_Play* play) {
}

void BgIngate_EndCruise(Actor_BgIngate* this, GameState_Play* play) {
    Actor_Player* player = GET_PLAYER(play);
    Camera* mainCam = Play_GetCamera(play, CAM_ID_MAIN);

    if (!MM_CHECK_EVENT_INF(EVENTINF_40)) {

        if (!MM_CHECK_EVENT_INF(EVENTINF_35) && (this->flags & 0x10) && (this->unk16C == 0)) {
            this->dyna.actor.messageId = 0x9E3;
            PlayerDisplayTextBox(play, this->dyna.actor.messageId, NULL);
            this->flags &= ~0x10;
        }

        if ((this->flags & 2) || (MM_CHECK_EVENT_INF(EVENTINF_35) && (gSaveContext.minigameScore >= 20))) {

            if (this->timePath->additionalPathIndex != ADDITIONAL_PATH_INDEX_NONE) {
                func_80953E38(play);
                Player_SetCsActionWithHaltedActors(play, &this->dyna.actor, PLAYER_CSACTION_WAIT);
                this->dyna.actor.messageId = 0x9E4;
                PlayerDisplayTextBox(play, this->dyna.actor.messageId, NULL);
                this->unk16C += 1;
                MM_SET_EVENT_WEEK(EV_MM_WEEK_WEEKEVENTREG_90_6);
                this->actionFunc = func_809543D4;
            } else {
                if (!MM_CHECK_EVENT_INF(EVENTINF_35)) {
                    MM_CLEAR_EVENT_INF(EVENTINF_41);
                } else {
                    MM_SET_EVENT_INF(EVENTINF_40);
                }
                this->actionFunc = func_809542A0;
            }
        } else if ((CutsceneManager_GetCurrentCsId() == CS_ID_NONE) && (this->timePath != NULL)) {
            Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_CRUISER - SFX_FLAG);
            func_80953BEC(this);
        }
    }
    if (CutsceneManager_GetCurrentCsId() != this->csId) {
        if (CutsceneManager_GetCurrentCsId() != CS_ID_NONE) {
            Camera_ChangeSetting(mainCam, CAM_SET_NORMAL0);
            player->state |= (1 << 5); /* PLAYER_STATE1_20 */
            play->actorCtx.flags &= ~ACTORCTX_FLAG_PICTO_BOX_ON;
        } else {
            Camera_ChangeSetting(mainCam, CAM_SET_BOAT_CRUISE);
            player->state &= ~(1 << 5); /* PLAYER_STATE1_20 */
        }
    }
    this->csId = CutsceneManager_GetCurrentCsId();
}

void func_809541B8(Actor_BgIngate* this, GameState_Play* play) {
    Actor_Player* player = GET_PLAYER(play);

    if (this->flags & 0x4) {
        if ((player->transformation == MM_PLAYER_FORM_HUMAN) && (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
            (this->dyna.actor.xzDistToPlayer < 40.0f)) {
            if (this->dyna.actor.yDistanceFromLink > 15.0f) {
                Player_SetCsActionWithHaltedActors(play, &this->dyna.actor, PLAYER_CSACTION_WAIT);
                this->dyna.actor.messageId = 0x9E6;
                PlayerDisplayTextBox(play, this->dyna.actor.messageId, NULL);
                this->actionFunc = func_809543D4;
            }
        }
    } else if (!DynaPolyActor_IsPlayerOnTop(&this->dyna)) {
        this->flags |= 4;
    }
}

void func_809542A0(Actor_BgIngate* this, GameState_Play* play) {
    if (MM_CHECK_EVENT_INF(EVENTINF_50)) {
        play->nextEntrance = 0xa820;
        MM_CLEAR_EVENT_INF(EVENTINF_50);
    } else {
        play->nextEntrance = 0xa810;
    }
    gSaveContext.nextCutscene = 0;
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_FADE_WHITE;
    gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
    this->actionFunc = func_80953F8C;
    MM_CLEAR_EVENT_WEEK(EV_MM_WEEK_WEEKEVENTREG_90_6);
    Environment_StartTime();
}

void func_80954340(Actor_BgIngate* this, GameState_Play* play) {
    if (DECR(this->unk16A) == 0) {
        if (this->timePath != NULL) {
            Player_SetCsActionWithHaltedActors(play, &this->dyna.actor, PLAYER_CSACTION_END);
            this->timePath = &play->setupPathList[this->timePath->additionalPathIndex];
            func_80953F14(this, play);
            Environment_StopTime();
        }
    }
}

void func_809543D4(Actor_BgIngate* this, GameState_Play* play) {
    u8 talkState = Message_GetState(&play->msgCtx);

    if (((talkState == TEXT_STATE_CHOICE) || (talkState == TEXT_STATE_EVENT)) && Message_ShouldAdvance(play)) {
        switch (this->dyna.actor.messageId) {
            case 0x9E4:
                this->dyna.actor.messageId = 0x9E5;
                DisplayTextBox2(play, this->dyna.actor.messageId);
                break;
            case 0x9E5:
                if (play->msgCtx.choiceIndex == 0) {
                    Player_SetCsActionWithHaltedActors(play, &this->dyna.actor, PLAYER_CSACTION_END);
                    this->flags &= ~0x4;
                    this->actionFunc = func_809541B8;
                    Environment_StartTime();
                    Audio_PlaySfx_MessageDecide();
                } else {
                    if (this->timePath != NULL) {
                        this->timePath = &play->setupPathList[this->timePath->additionalPathIndex];
                    }
                    func_80953F14(this, play);
                    MM_CLEAR_EVENT_WEEK(EV_MM_WEEK_WEEKEVENTREG_90_6);
                    Audio_PlaySfx_MessageCancel();
                }
                Message_Close(play);
                break;
            case 0x9E6:
                if (play->msgCtx.choiceIndex == 0) {
                    func_80953EA4(this, play);
                    MM_CLEAR_EVENT_WEEK(EV_MM_WEEK_WEEKEVENTREG_90_6);
                    Audio_PlaySfx_MessageDecide();
                } else {
                    Player_SetCsActionWithHaltedActors(play, &this->dyna.actor, PLAYER_CSACTION_END);
                    this->flags &= ~0x4;
                    this->actionFunc = func_809541B8;
                    Environment_StartTime();
                    Audio_PlaySfx_MessageCancel();
                }
                Message_Close(play);
                break;
        }
    }
}

void BgIngate_Init(Actor_BgIngate* this, GameState_Play* play) {
    s32 phi_a2;
    Vec3s* sp38;
    Vec3f sp2C;
    Vec3f sp20;

    if (BgIngate_FindActor(this, play, ACTORCAT_BG, AC_BG_INGATE) == NULL) {
        DynaPolyActor_Init(&this->dyna, DYNA_TRANSFORM_POS | DYNA_TRANSFORM_ROT_Y);
        DynaPolyActor_LoadMesh(play, &this->dyna, SEGADDR_BG_INGATE_COL);
        this->flags = 0;
        this->flags |= 0x8;
        this->flags |= 0x10;
        Actor_SetScale(&this->dyna.actor, 1.0f);
        this->timePath = SubS_GetAdditionalPath(play, BGINGATE_GET_PATH_INDEX(&this->dyna.actor), 0);
        this->dyna.actor.room = -1;
        if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF)) {
            MM_CLEAR_EVENT_WEEK(EV_MM_WEEK_WEEKEVENTREG_90_6);
        }
        if (!MM_CHECK_EVENT_INF(EVENTINF_35) && MM_GET_EVENT_WEEK(EV_MM_WEEK_WEEKEVENTREG_90_6)) {
            phi_a2 = 1;
            this->unk16C = 1;
            this->actionFunc = func_809541B8;
        } else {
            phi_a2 = 0;
            if (play->curSpawn == 6) {
                func_80953F14(this, play);
                if (MM_CHECK_EVENT_INF(EVENTINF_35)) {
                    Interface_InitMinigame(play);
                } else {
                    MM_SET_EVENT_INF(EVENTINF_41);
                }
            } else {
                this->actionFunc = func_80953F8C;
            }
        }
        this->timePath = SubS_GetAdditionalPath(play, BGINGATE_GET_PATH_INDEX(&this->dyna.actor), phi_a2);
        if (this->timePath != NULL) {
            sp38 = Lib_SegmentedToVirtual(this->timePath->points);
            Math_Vec3s_ToVec3f(&sp2C, &sp38[0]);
            Math_Vec3s_ToVec3f(&sp20, &sp38[1]);
            this->dyna.actor.world.rot.y = Math_Vec3f_Yaw(&sp2C, &sp20);
            this->dyna.actor.shape.rot.y = this->dyna.actor.world.rot.y;
            this->dyna.actor.world.pos.x = sp2C.x;
            this->dyna.actor.world.pos.y = -15.0f;
            this->dyna.actor.world.pos.z = sp2C.z;
        }
        this->timePath = SubS_GetAdditionalPath(play, BGINGATE_GET_PATH_INDEX(&this->dyna.actor), 0);
    } else {
        Actor_Kill(&this->dyna.actor);
    }
}

void BgIngate_Destroy(Actor_BgIngate* this, GameState_Play* play) {

    if (this->flags & 8) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    }
}

void BgIngate_Update(Actor_BgIngate* this, GameState_Play* play) {
    this->actionFunc(this, play);
}

void BgIngate_Draw(Actor_BgIngate* this, GameState_Play* play) {
    OPEN_DISPS(play->gs.gfx);

    Gfx_SetupDL25_Opa(play->gs.gfx);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, SEGADDR_BG_INGATE_DL);

    CLOSE_DISPS();
}

ActorProfile Actor_Bg_Ingate_InitVars =
{
    AC_BG_INGATE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SICHITAI_OBJ,
    sizeof(Actor_BgIngate),
    (ActorFunc)BgIngate_Init,
    (ActorFunc)BgIngate_Destroy,
    (ActorFunc)BgIngate_Update,
    (ActorFunc)BgIngate_Draw,
};

OVL_ACTOR_INFO(AC_BG_INGATE, Actor_Bg_Ingate_InitVars);
