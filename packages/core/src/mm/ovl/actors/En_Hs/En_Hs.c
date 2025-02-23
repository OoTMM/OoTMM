#include <combo.h>
#include <combo/item.h>
#include <combo/global.h>
#include <combo/math.h>
#include <combo/mm/bombers_notebook.h>
#include <assets/mm/objects/object_hs.h>
#include "En_Hs.h"

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnHs_Init(Actor_EnHs* this, PlayState* play);
void EnHs_Destroy(Actor_EnHs* this, PlayState* play);
void EnHs_Update(Actor_EnHs* this, PlayState* play);
void EnHs_Draw(Actor_EnHs* this, PlayState* play);

void func_80952FE0(Actor_EnHs* this, PlayState* play);
void func_80953098(Actor_EnHs* this, PlayState* play);
void func_80953180(Actor_EnHs* this, PlayState* play);
void EnHs_DoNothing(Actor_EnHs* this, PlayState* play);
void EnHs_SceneTransitToBunnyHoodDialogue(Actor_EnHs* this, PlayState* play);
void func_80953354(Actor_EnHs* this, PlayState* play);
void func_8095345C(Actor_EnHs* this, PlayState* play);

static ColliderCylinderInit sCylinderInit =
{
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 40, 40, 0, { 0, 0, 0 } },
};

Vec3f D_8095393C = { 300.0f, 1000.0f, 0.0f };

void func_80952C50(Actor_EnHs* this, PlayState* play)
{
    Player* player = GET_PLAYER(play);
    s32 i;

    for (i = 0; i < ARRAY_COUNT(this->nwcPos); i++)
        Math_Vec3f_Copy(&this->nwcPos[i], &player->actor.world.pos);

    this->actor.home.rot.x = 0; // reset adult transformed count
    this->actor.home.rot.z = 0; // reset chick count
}

void EnHs_Init(Actor_EnHs* this, PlayState* play)
{
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, (void*)gHsSkel, (void*)gHsIdleAnim, this->jointTable, this->morphTable,
                       OBJECT_HS_LIMB_MAX);
    Animation_PlayLoop(&this->skelAnime, (void*)gHsIdleAnim);
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&this->actor, 0.01f);
    this->actionFunc = func_8095345C;

    if (play->curSpawn == 1)
        this->actor.flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;

    this->stateFlags = 0;
    this->actor.attentionRangeType = ATTENTION_RANGE_6;
    func_80952C50(this, play);
}

void EnHs_Destroy(Actor_EnHs* this, PlayState* play)
{
    Collider_DestroyCylinder(play, &this->collider);
}

void func_80952DFC(PlayState* play)
{
    if (gMmExtraFlags.maskBunny)
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_BUNNY_HOOD);
    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GROG);
}

void EnHs_UpdateChickPos(Vec3f* dst, Vec3f src, f32 offset)
{
    Vec3f diff;
    f32 distance;

    Math_Vec3f_Diff(&src, dst, &diff);

    distance = SQXZ(diff); // gets un-squared after we check if we are too close

    if (SQ(offset) > distance)
        return;

    distance = sqrtf(distance);
    diff.x *= (distance - offset) / distance;
    diff.z *= (distance - offset) / distance;

    dst->x += diff.x;
    dst->z += diff.z;
}

void func_80952F00(Actor_EnHs* this, PlayState* play)
{
    Player* player = GET_PLAYER(play);
    s32 i;
    f32 offset;

    if (this->actor.home.rot.z >= 20) // current chick count >= 10
        offset = 15.0f;
    else
        offset = 10.0f;

    EnHs_UpdateChickPos(&this->nwcPos[0], player->actor.world.pos, offset);

    for (i = 1; i < ARRAY_COUNT(this->nwcPos); i++)
        EnHs_UpdateChickPos(&this->nwcPos[i], this->nwcPos[i - 1], offset);
}

void func_80952FE0(Actor_EnHs* this, PlayState* play)
{
    if (this->stateTimer < 40)
        Math_SmoothStepToS(&this->headRot.y, 0x1F40, 6, 0x1838, 0x64);
    else if (this->stateTimer < 80)
        Math_SmoothStepToS(&this->headRot.y, -0x1F40, 6, 0x1838, 0x64);
    else
    {
        this->actionFunc = func_80953180;
        this->stateFlags &= ~4;
        DisplayTextBox2(play, 0x33F6);
        func_80952DFC(play);
    }
    this->stateTimer++;
}

void func_80953098(Actor_EnHs* this, PlayState* play)
{
    if (Actor_HasParent(&this->actor, play))
    {
        gMmExtraFlags.maskBunny = 1;
        this->actor.parent = NULL;
        this->actionFunc = func_8095345C;
        this->actor.flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
        this->stateFlags |= 0x10;
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    }
    else
    {
        this->stateFlags |= 8;
        if (gMmExtraFlags.maskBunny)
            comboGiveItemNpc(&this->actor, play, GI_MM_RUPEE_RED, -1, 10000.0f, 50.0f);
        else
            comboGiveItemNpc(&this->actor, play, GI_MM_MASK_BUNNY, NPC_MM_MASK_BUNNY, 10000.0f, 50.0f);
    }
}

void func_80953180(Actor_EnHs* this, PlayState* play)
{
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play))
    {
        switch (play->msgCtx.currentTextId)
        {
            case 0x33F4: // laughing that they are all roosters (!)
            case 0x33F6: // Grog regrets not being able to see his chicks reach adult hood
                Message_Close(play);
                this->actionFunc = func_8095345C;
                break;

            case 0x33F7: // notice his chicks are grown up, happy, wants to give you bunny hood
                this->actor.flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
                Message_Close(play);
                this->actionFunc = func_80953098;
                func_80953098(this, play);
                break;

            case 0x33F9: // laughing that they are all roosters (.)
                this->actor.flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
                Message_Close(play);
                this->actionFunc = func_8095345C;
                break;

            case 0x33F5: // He heard from his gramps (?) the moon is going to fall
                play->msgCtx.msgLength = 0;
                this->actionFunc = func_80952FE0;
                this->stateTimer = 0;
                this->headRot.z = 0;
                this->stateFlags |= 4;
                break;

            default:
                Message_Close(play);
                this->actionFunc = func_8095345C;
                break;
        }
    }
}

void EnHs_DoNothing(Actor_EnHs* this, PlayState* play) { }

void EnHs_SceneTransitToBunnyHoodDialogue(Actor_EnHs* this, PlayState* play)
{
    if (DECR(this->stateTimer) == 0)
    {
        play->nextEntrance = play->setupExitList[HS_GET_EXIT_INDEX(&this->actor)];
        play->transitionTrigger = TRANS_TRIGGER_START;
        MM_SET_EVENT_WEEK(EV_MM_WEEK_GROG_GROWN_CHICKEN);
        this->actionFunc = EnHs_DoNothing;
    }
}

void func_80953354(Actor_EnHs* this, PlayState* play)
{
    if (!Play_InCsMode(play))
    {
        Player_SetCsActionWithHaltedActors(play, &this->actor, 0x07);
        this->actionFunc = EnHs_SceneTransitToBunnyHoodDialogue;
    }
}

void func_809533A0(Actor_EnHs* this, PlayState* play)
{
    u16 sp1E;

    if ((play->curSpawn == 1) && !(this->stateFlags & 0x20))
    {
        sp1E = 0x33F7;
        this->stateFlags |= 0x20;
    }
    else if (this->stateFlags & 0x10)
    {
        sp1E = 0x33F9;
        this->stateFlags &= ~0x10;
    }
    else if (MM_GET_EVENT_WEEK(EV_MM_WEEK_GROG_GROWN_CHICKEN))
        sp1E = 0x33F4;
    else
        sp1E = 0x33F5;

    PlayerDisplayTextBox(play, sp1E, &this->actor);

    if (sp1E == 0x33F4)
        func_80952DFC(play);
}

void func_8095345C(Actor_EnHs* this, PlayState* play)
{
    if (Actor_TalkOfferAccepted(&this->actor, &play->state))
    {
        this->actionFunc = func_80953180;
        func_809533A0(this, play);
        if (this->stateFlags & 8)
        {
            func_80952DFC(play);
            this->stateFlags &= ~8;
        }
    }
    else if (this->actor.home.rot.x >= 20) // chicks turned adult >= 10
    {
        this->actionFunc = func_80953354;
        this->stateTimer = 40;
    }
    else if (this->actor.flags & ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED)
    {
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, -1);
        this->stateFlags |= 1;
    }
    else if ((this->actor.xzDistToPlayer < 120.0f) && Player_IsFacingActor(&this->actor, 0x2000, play))
    {
        Actor_OfferTalk(&this->actor, play, 130.0f);
        this->stateFlags |= 1;
    }
    else
        this->stateFlags &= ~1;
}

void EnHs_Update(Actor_EnHs* this, PlayState* play)
{
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, UPDBGCHECKINFO_FLAG_FLOOR);

    if (SkelAnime_Update(&this->skelAnime))
        this->skelAnime.curFrame = 0.0f;

    this->actionFunc(this, play);

    func_80952F00(this, play);

    if (this->stateFlags & 4)
    {
        Math_SmoothStepToS(&this->headRot.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&this->torsoRot.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&this->torsoRot.y, 0, 6, 0x1838, 0x64);
    }
    else if (this->stateFlags & 1)
        Actor_TrackPlayer(play, &this->actor, &this->headRot, &this->torsoRot, this->actor.focus.pos);
    else
    {
        Math_SmoothStepToS(&this->headRot.x, 0x3200, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&this->headRot.y, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&this->torsoRot.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&this->torsoRot.y, 0, 6, 0x1838, 0x64);
    }
}

s32 EnHs_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor_EnHs* this)
{
    switch (limbIndex)
    {
        case 0x9:
            rot->x += this->headRot.y;
            rot->z += this->headRot.x;
            break;

        case 0xa:
            rot->x += this->headRot.y;
            rot->z += this->headRot.x;
            break;

        case 0xb:
            // for some reason this hair is always removed here in the Override limb
            // if you do re-enable, make sure you add the head rot like above
            *dList = NULL;
            return 0;

        // these two limbs both have empty enddisplaylist, they do nothing
        // at the same time (params == HS_TYPE_UNK1) is always false, because vanilla params is 0xFE01
        case 0xc:
            if (this->actor.params == HS_TYPE_UNK1)
            {
                *dList = NULL;
                return 0;
            }
            break;

        case 0xd:
            if (this->actor.params == HS_TYPE_UNK1)
            {
                *dList = NULL;
                return 0;
            }
            break;

        default:
            break;
    }
    return 0;
}

void EnHs_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor_EnHs* this)
{
    if (limbIndex == 0x9)
        Matrix_MultVec3f(&D_8095393C, &this->actor.focus.pos);
}

void EnHs_Draw(Actor_EnHs* this, PlayState* play)
{
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, EnHs_OverrideLimbDraw, EnHs_PostLimbDraw, &this->actor);
}

ActorProfile EnHs_InitVars =
{
    ACTOR_EN_HS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_HS,
    sizeof(Actor_EnHs),
    (ActorFunc)EnHs_Init,
    (ActorFunc)EnHs_Destroy,
    (ActorFunc)EnHs_Update,
    (ActorFunc)EnHs_Draw,
};

OVL_INFO_ACTOR(ACTOR_EN_HS, EnHs_InitVars);
