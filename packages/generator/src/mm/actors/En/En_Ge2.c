#include <combo.h>
#include <combo/entrance.h>

typedef struct {
    Actor actor;
} PictoActor;

typedef struct {
    PictoActor picto;
} EnGe2;

typedef enum {
    GERUDO_PURPLE_DETECTION_UNDETECTED,
    GERUDO_PURPLE_DETECTION_HEARD,
    GERUDO_PURPLE_DETECTION_PROXIMITY,
} GerudoPurpleDetection;

#define ACTOR_PLAYER_HEIGHT_REL(actor) (*(f32*)((u8*)(actor) + 0x09c))

static void EnGe2_ThrowPlayerOut(Actor* this, PlayState* play)
{
    s32 override;
    u32 entrance;
    u8* timer;

    /* Handle timer */
    timer = ((u8*)this) + 0x300;
    if (*timer)
    {
        *timer -= 1;
        return;
    }

    /* Transition */
    if (play->sceneId == SCE_MM_PIRATE_FORTRESS_ENTRANCE)
        entrance = ENTR_MM_GREAT_BAY_FROM_PIRATE_FORTRESS;
    else
        entrance = ENTR_MM_PIRATE_FORTRESS_EXTERIOR_FROM_INTERIOR;
    override = comboEntranceOverride(entrance);

    if (override == -1 || (u32)override == entrance)
    {
        play->nextEntrance = play->setupExitList[this->params & 0x1f];
        play->transitionTrigger = TRANS_TRIGGER_START;
        play->transitionType = 0x26;
    }
    else
    {
        comboTransition(play, (u32)override);
    }
}

PATCH_FUNC(0x80b8bc78, EnGe2_ThrowPlayerOut);

static s32 Player_IsWearingCustomGerudoMask(PlayState* play)
{
    Player* player = GET_PLAYER(play);

    return player->transformation == MM_PLAYER_FORM_HUMAN &&
           gCustomSave.customMask == PLAYER_CUSTOM_MASK_GERUDO;
}

GerudoPurpleDetection EnGe2_DetectPlayer_CustomGerudoMask(PlayState* play, EnGe2* this)
{
    if (this->picto.actor.xzDistToPlayer > 250.0f)
    {
        return GERUDO_PURPLE_DETECTION_UNDETECTED;
    }

    if (
        Player_GetMask(play) != PLAYER_MASK_STONE &&
        !Player_IsWearingCustomGerudoMask(play) &&
        this->picto.actor.xzDistToPlayer < 50.0f
    )
    {
        return GERUDO_PURPLE_DETECTION_PROXIMITY;
    }

    if (func_800B715C(play))
    {
        return GERUDO_PURPLE_DETECTION_HEARD;
    }

    return GERUDO_PURPLE_DETECTION_UNDETECTED;
}

PATCH_FUNC(0x80b8b514, EnGe2_DetectPlayer_CustomGerudoMask);

s32 EnGe2_LookForPlayer_CustomGerudoMask(
    PlayState* play,
    Actor* actor,
    Vec3f* pos,
    s16 yaw,
    s16 yawRange,
    f32 xzRange,
    f32 yRange
)
{
    s16 yawToPlayer;
    Vec3f posResult;
    CollisionPoly* outPoly;
    Player* player = GET_PLAYER(play);

    if (Player_GetMask(play) == PLAYER_MASK_STONE || Player_IsWearingCustomGerudoMask(play))
    {
        return false;
    }

    if (actor->xzDistToPlayer > xzRange)
    {
        return false;
    }

    if (fabsf(ACTOR_PLAYER_HEIGHT_REL(actor)) > yRange)
    {
        return false;
    }

    yawToPlayer = actor->yawTowardsPlayer - yaw;
    if (yawRange > 0 && yawRange < ABS_ALT(yawToPlayer))
    {
        return false;
    }

    if (BgCheck_AnyLineTest1(
            &play->colCtx,
            pos,
            &player->bodyPartsPos[PLAYER_BODYPART_HEAD],
            &posResult,
            &outPoly,
            false
        ))
    {
        return false;
    }

    return true;
}

PATCH_FUNC(0x80b8b5ac, EnGe2_LookForPlayer_CustomGerudoMask);