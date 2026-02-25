#include "Tg_Sw.h"

// #include "overlays/actors/ovl_En_Sw/z_en_sw.h"

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define THIS ((TGSw*)thisx)

void TGSw_Init(Actor* thisx, PlayState* play);
void TGSw_Destroy(Actor* thisx, PlayState* play);
void TGSw_Update(Actor* thisx, PlayState* play);

void TGSw_Die(struct TGSw* this, PlayState* play);

void TGSw_Idle(TGSw* this, PlayState* play) {
    f32 scaledAbsoluteRotY, scaledAbsoluteRotZ;
    PlayerImpactType playerImpactType;

    if (play->actorCtx.playerImpact.timer != 0) {
        scaledAbsoluteRotY = ABS_ALT(this->actor.world.rot.y) * 4.0f;
        scaledAbsoluteRotZ = ABS_ALT(this->actor.world.rot.z) * 4.0f;

        if ((scaledAbsoluteRotZ < this->actor.xzDistToPlayer) || (scaledAbsoluteRotY < this->actor.yDistanceFromLink)) {
            return;
        }
        playerImpactType = play->actorCtx.playerImpact.type;
        if ((playerImpactType == PLAYER_IMPACT_BONK) || (playerImpactType == PLAYER_IMPACT_GORON_GROUND_POUND)) {
            this->actionFunc = TGSw_Die;
        }
    }
}

void TGSw_Die(TGSw* this, PlayState* play) {
    Actor* actorIter = NULL;

    do {
        actorIter = SubS_FindActor(play, actorIter, ACTORCAT_ENEMY, ACTOR_EN_SW);
        if (actorIter == NULL) {
            break;
        }
        if (TGSW_GET_FC(&this->actor) == ENSW_GET_FC(actorIter)) {
            actorIter->parent = &this->actor;
            actorIter->speed = ABS_ALT(this->actor.world.rot.x);
            actorIter->csId = CS_ID_NONE;
            break;
        }
        actorIter = actorIter->next;
    } while (actorIter != NULL);

    actorIter = NULL;

    do {
        actorIter = SubS_FindActor(play, actorIter, ACTORCAT_NPC, ACTOR_EN_SW);
        if (actorIter == NULL) {
            break;
        }
        if (TGSW_GET_FC(&this->actor) == ENSW_GET_FC(actorIter)) {
            actorIter->parent = &this->actor;
            actorIter->speed = ABS_ALT(this->actor.world.rot.x);
            actorIter->csId = CS_ID_NONE;
            break;
        }
        actorIter = actorIter->next;
    } while (actorIter != NULL);

    Actor_Kill(&this->actor);
}

void TGSw_Init(Actor* thisx, PlayState* play) {
    TGSw* this = THIS;

    this->actor.csId = this->actor.world.rot.z;
    this->actionFunc = TGSw_Idle;
}

void TGSw_Destroy(Actor* thisx, PlayState* play) {
}

void TGSw_Update(Actor* thisx, PlayState* play) {
    TGSw* this = THIS;

    this->actionFunc(this, play);
}

ActorProfile TG_Sw_Profile = {
    ACTOR_TG_SW,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(TGSw),
    (ActorFunc)TGSw_Init,
    (ActorFunc)TGSw_Destroy,
    (ActorFunc)TGSw_Update,
    NULL, // Draw
};

OVL_INFO_ACTOR(ACTOR_TG_SW, TG_Sw_Profile);
