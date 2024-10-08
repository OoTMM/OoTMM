#include <combo.h>
#include <combo/sfx.h>
#include "En_Nwc.h"

#define FLAGS (ACTOR_FLAG_MM_10 | ACTOR_FLAG_MM_80000000)

#if defined(GAME_MM)
# define SEGADDR_NIW_SKEL           ((void*)0x06002530)
# define SEGADDR_NIW_IDLE_ANIM      ((void*)0x060000e8)
# define SEGADDR_NWC_OPEN_EYES      ((void*)0x060007d0)
# define SEGADDR_NWC_CLOSED_EYES    ((void*)0x060009d0)
# define SEGADDR_NWC_BODY_DL        ((void*)0x060002e8)
#endif

void EnNwc_Init(Actor_EnNwc* this, GameState_Play* play);
void EnNwc_Destroy(Actor_EnNwc* this, GameState_Play* play);
void EnNwc_Update(Actor_EnNwc* this, GameState_Play* play);
void EnNwc_Draw(Actor_EnNwc* this, GameState_Play* play);

void EnNwc_LoadNiwSkeleton(Actor_EnNwc* this, GameState_Play* play);
void EnNwc_CrowAtTheEnd(Actor_EnNwc* this, GameState_Play* play);
void EnNwc_Follow(Actor_EnNwc* this, GameState_Play* play);
void EnNwc_HopForward(Actor_EnNwc* this, GameState_Play* play);
void EnNwc_RunAway(Actor_EnNwc* this, GameState_Play* play);
void EnNwc_Turn(Actor_EnNwc* this, GameState_Play* play);
void EnNwc_CheckForBreman(Actor_EnNwc* this, GameState_Play* play);

void EnNwc_DrawAdultBody(Actor_EnNwc* this, GameState_Play* play);
Actor_EnHs* EnNwc_FindGrog(GameState_Play* play);

typedef enum EnNwcState
{
    /* -1 */ NWC_STATE_NIW_LOADED = -1, // set after loading object_niw
    /*  0 */ NWC_STATE_CHECK_BREMAN,    // checking for breman mask
    /*  1 */ NWC_STATE_TURNING,         // turning to face a new direction to explore
    /*  2 */ NWC_STATE_HOPPING_FORWARD, // hopping to go explore
    /*  3 */ NWC_STATE_FOLLOWING,       // following the player
    /*  4 */ NWC_STATE_RUNNING          // running from the player after failed breman march
}
EnNwcState;

void EnNwc_Init(Actor_EnNwc* this, GameState_Play* play)
{
    s32 niwObjectSlot;

    niwObjectSlot = Object_GetSlot(&play->objectCtx, OBJECT_NIW);
    if (niwObjectSlot <= -1)
    {
        // niw object does not exist, we need it for tranformation, despawn
        Actor_Kill(&this->actor);
        return;
    }

    if (MM_GET_EVENT_WEEK(EV_MM_WEEK_GROG_GROWN_CHICKEN))
    {
        Actor_Spawn(&play->actorCtx, play, AC_EN_NIW, this->actor.world.pos.x, this->actor.world.pos.y,
                    this->actor.world.pos.z, 0, this->actor.world.rot.y, 0, 0);
        Actor_Kill(&this->actor);
        return;
    }

    this->niwObjectSlot = niwObjectSlot;
    this->nwcObjectIndex = this->actor.objectSlot;
    this->grog = EnNwc_FindGrog(play);

    this->footRotY = this->footRotZ = 0;
    Actor_SetScale(&this->actor, 0.01f);
    this->actionFunc = EnNwc_LoadNiwSkeleton;
    this->hasGrownUp = 0;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 6.0f);
    this->actor.velocity.y = 0.0f;
    this->actor.minVelocityY = -9.0f;
    this->actor.gravity = -1.0f;
}

void EnNwc_Destroy(Actor_EnNwc* this, GameState_Play* play) { }

void EnNwc_SpawnDust(Actor_EnNwc* this, GameState_Play* play)
{
    static Color_RGBA8 sPrimColor = { 255, 255, 255, 255 };
    static Color_RGBA8 sEnvColor = { 80, 80, 80, 255 };
    Vec3f pos;
    Vec3f vec5;
    Vec3f velocity;
    Vec3f accel;
    s16 yaw;
    s16 pitch;
    Vec3f eye = GET_ACTIVE_CAM(play)->eye;
    s32 i;

    yaw = Math_Vec3f_Yaw(&eye, &this->actor.world.pos);
    pitch = -Math_Vec3f_Pitch(&eye, &this->actor.world.pos);
    vec5.x = this->actor.world.pos.x - 5.0f * Math_SinS(yaw) * Math_CosS(pitch);
    vec5.y = this->actor.world.pos.y - 5.0f * Math_SinS(pitch);
    vec5.z = this->actor.world.pos.z - 5.0f * Math_CosS(yaw) * Math_CosS(pitch);

    for (i = 0; i < 5; i++)
    {
        velocity.x = Rand_CenteredFloat(4.0f);
        velocity.y = Rand_CenteredFloat(4.0f);
        velocity.z = Rand_CenteredFloat(4.0f);
        accel.x = -velocity.x * 0.1f;
        accel.y = -velocity.y * 0.1f;
        accel.z = -velocity.z * 0.1f;
        pos.x = vec5.x + velocity.x;
        pos.y = vec5.y + velocity.y;
        pos.z = vec5.z + velocity.z;

        EffectSsDust_Spawn_2_Normal(play, &pos, &velocity, &accel, &sPrimColor, &sEnvColor, 300, 30, 10);
    }
}

Actor_EnHs* EnNwc_FindGrog(GameState_Play* play)
{
    Actor* grogSearch = play->actorCtx.actors[ACTORCAT_NPC].first;

    while (grogSearch != NULL)
    {
        if (grogSearch->id == AC_EN_HS)
            return (Actor_EnHs*)grogSearch;
        grogSearch = grogSearch->next;
    }

    return NULL;
}

s32 EnNwc_PlayerReleasedBremanMarch(Actor_EnNwc* this, GameState_Play* play)
{
    Actor_Player* player = GET_PLAYER(play);

    // Weird: home.rot.x holds count of chicks having transformed into adult.
    // Weird: Its incremented by 1 unlike chicks following, so it should max at 10.
    if (this->grog->actor.home.rot.x >= 20)
        return 0;

    if (player->state3 & (1 << 29))
        return 0;

    return 1;
}

/**
 * Summary: Checks 1) if grog exists, 2) player is using breman mask, 3) and within range.
 *  Used to identify if the chick should be captured by breman mask.
 */
s32 EnNwc_IsFound(Actor_EnNwc* this, GameState_Play* play)
{
    Actor_Player* player = GET_PLAYER(play);

    if (this->grog == NULL)
        return 0;

    if ((player->state3 & (1 << 29)) && (this->actor.xzDistToPlayer < 100.0f))
        return 1;

    return 0;
}

void EnNwc_ChangeState(Actor_EnNwc* this, s16 newState)
{
    this->actor.speed = 0.0f;
    switch (newState)
    {
        case NWC_STATE_CHECK_BREMAN:
            this->stateTimer = 10;
            this->actionFunc = EnNwc_CheckForBreman;
            break;

        case NWC_STATE_TURNING:
            this->stateTimer = Rand_ZeroFloat(20.0f) + 15.0f;
            this->actionFunc = EnNwc_Turn;
            this->fallingRotY = TRUNCF_BINANG(Rand_CenteredFloat(0x10000));
            break;

        case NWC_STATE_HOPPING_FORWARD:
            this->stateTimer = Rand_ZeroFloat(20.0f) + 15.0f;
            this->actionFunc = EnNwc_HopForward;
            break;

        case NWC_STATE_FOLLOWING:
            this->actionFunc = EnNwc_Follow;
            this->transformTimer = 0;
            this->randomRot = TRUNCF_BINANG(Rand_CenteredFloat(0x2710));
            break;

        case NWC_STATE_RUNNING:
            //! FAKE: & 0xFFFF
            this->actor.world.rot.y = this->actor.home.rot.z * 0x3000 & 0xFFFF;
            this->actor.shape.rot.y = this->actor.world.rot.y;
            this->stateTimer = Rand_ZeroFloat(40.0f) + 120.0f;
            this->actionFunc = EnNwc_RunAway;
            break;

        default:
            break;
    }

    this->state = newState;
}

/**
 * Summary: Changes the current actor state
 *     If previously random behavior -> check if breman mask is active
 *     If previously checking for breman -> select random (NWC_STATE_TURNING, NWC_STATE_HOPPING_FORWARD)
 */
void EnNwc_ToggleState(Actor_EnNwc* this)
{
    this->actor.speed = 0.0f;
    if (this->state != NWC_STATE_CHECK_BREMAN)
        EnNwc_ChangeState(this, NWC_STATE_CHECK_BREMAN);
    else
        EnNwc_ChangeState(this, Rand_ZeroFloat(2.0f) + 1.0f);
}

void EnNwc_CheckFound(Actor_EnNwc* this, GameState_Play* play)
{
    if (EnNwc_IsFound(this, play))
    {
        u8 currentChickCount = (this->grog->actor.home.rot.z / 2);

        if (currentChickCount > 9)
            currentChickCount = 9;

        // save our current chick order
        this->actor.home.rot.z = this->grog->actor.home.rot.z + 1;

        // if < 10 chicks, increment grog's chick counter
        if (this->grog->actor.home.rot.z < 20)
            this->grog->actor.home.rot.z += 2;

        EnNwc_ChangeState(this, NWC_STATE_FOLLOWING);
        Audio_PlaySfx_AtPosWithAllChannelsIO(&gSfxDefaultPos, NA_SE_SY_CHICK_JOIN_CHIME, currentChickCount);
    }
}

void EnNwc_LoadNiwSkeleton(Actor_EnNwc* this, GameState_Play* play)
{
    if (Object_IsLoaded(&play->objectCtx, this->niwObjectSlot))
    {
        gSegments[0x06] = OS_K0_TO_PHYSICAL(play->objectCtx.slots[this->niwObjectSlot].segment);

        SkelAnime_InitFlex(play, &this->niwSkeleton, SEGADDR_NIW_SKEL, SEGADDR_NIW_IDLE_ANIM, this->jointTable, this->morphTable,
                           NIW_LIMB_MAX);
        Animation_Change(&this->niwSkeleton, SEGADDR_NIW_IDLE_ANIM, 1.0f, 0.0f, Animation_GetLastFrame(SEGADDR_NIW_IDLE_ANIM),
                         0, 0.0f);

        gSegments[0x06] = OS_K0_TO_PHYSICAL(play->objectCtx.slots[this->nwcObjectIndex].segment);
        this->state = NWC_STATE_NIW_LOADED;
        EnNwc_ToggleState(this);
    }
}

void EnNwc_CrowAtTheEnd(Actor_EnNwc* this, GameState_Play* play)
{
    // I guess grog handles the scene transit?
    Math_SmoothStepToS(&this->upperBodyRotY, 0x2710, 2, 0x1B58, 0x3E8);
    Math_SmoothStepToS(&this->footRotZ, 0, 2, 0x1B58, 0x3E8);
    Math_SmoothStepToS(&this->footRotY, 0x36B0, 2, 0x1B58, 0x3E8);
}

/**
 * Summary: Controls the Chick when following Breman Mask.
 *
 * ActionFunc for NWC Type: NWC_STATE_FOLLOWING
 */
void EnNwc_Follow(Actor_EnNwc* this, GameState_Play* play)
{
    Vec3f* chickCoords = this->grog->nwcPos;
    Vec3f targetVector;
    s16 newRotY;

    this->stateTimer++;
    if (this->hasGrownUp & 1)
    {
        s16 targetUpperBodyRot = 0;
        s16 targetFootRot = 0;

        if (this->actor.speed > 0.0f)
        {
            if (this->stateTimer & 4)
            {
                targetFootRot = 0x1B58;
                targetUpperBodyRot = -0x2710;
            }
            if ((this->stateTimer & 3) == 3)
                this->actor.velocity.y = 2.0f; // hop up and down
        }
        Math_SmoothStepToS(&this->footRotZ, targetFootRot, 2, 0x1B58, 0x3E8);
        Math_SmoothStepToS(&this->upperBodyRotY, targetUpperBodyRot, 2, 0x1B58, 0x3E8);

    }
    else
    { // NOT grown up
        if (((this->stateTimer & 3) == 3) && ((this->stateTimer & 0x14) != 0))
            this->actor.velocity.y = 2.0f; // hop up and down
        if ((this->stateTimer & 0x1B) == 24)
            Actor_PlaySfx(&this->actor, NA_SE_EV_CHICK_SONG);
    }

    if ((this->grog->actor.home.rot.z >= 20) && // all 10 chicks have been found
        !(this->hasGrownUp & 1))
        {
        this->transformTimer += 2;
        if (this->transformTimer >= (s16)((this->actor.home.rot.z * 0x1E) + 0x1E))
        {
            // it is our turn to transform
            this->hasGrownUp |= 1;
            this->grog->actor.home.rot.x += 2; // increment grog's adult tranformation counter
            EnNwc_SpawnDust(this, play);
            Actor_SetScale(&this->actor, 0.002f);
            Actor_PlaySfx(&this->actor, NA_SE_EV_CHICK_TO_CHICKEN);
        }
    }

    Math_Vec3f_Diff(&chickCoords[this->actor.home.rot.z], &this->actor.world.pos, &targetVector);
    if (SQXZ(targetVector) < SQ(5.0f)) // too close to keep moving, stop
    {
        this->actor.speed = 0.0f;

        // first nwc in the line follows player, the rest follow the previous one
        if (this->actor.home.rot.z == 1)
            newRotY = this->actor.yawTowardsPlayer;
        else
            // for some reason the array is 10 * 2, incremented by 2, so this is "index - 1"
            newRotY = Math_Vec3f_Yaw(&this->actor.world.pos, &chickCoords[this->actor.home.rot.z - 2]);

    }
    else
    { // not too close: keep moving
        this->randomRot += TRUNCF_BINANG(Rand_CenteredFloat(0x5DC));
        if (this->randomRot > 0x1388)
            this->randomRot = 0x1388;
        else if (this->randomRot < -0x1388)
            this->randomRot = -0x1388;
        this->actor.speed = 2.0f;
        newRotY = Math_Vec3f_Yaw(&this->actor.world.pos, &chickCoords[this->actor.home.rot.z]) + this->randomRot;
    }

    this->actor.world.rot.y = newRotY;
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.world.rot.y, 2, 0xBB8, 0xC8);

    if (EnNwc_PlayerReleasedBremanMarch(this, play))
    {
        this->grog->actor.home.rot.x = 0; // reset adult count
        this->grog->actor.home.rot.z = 0; // reset chick follow count

        EnNwc_ChangeState(this, NWC_STATE_RUNNING);

        if (this->hasGrownUp & 1)
            EnNwc_SpawnDust(this, play);

        this->hasGrownUp &= ~1;
        Actor_SetScale(&this->actor, 0.01f);
    }

    if (this->actor.scale.x < 0.01f)
    {
        this->actor.scale.x += 0.002f;
        Actor_SetScale(&this->actor, this->actor.scale.x);
    }

    if (this->grog->actor.home.rot.x >= 20)
    { // all chicks have turned into adult cucco, stop and crow
        Actor_PlaySfx(&this->actor, NA_SE_EV_CHICKEN_CRY_M);
        this->actionFunc = EnNwc_CrowAtTheEnd;
        this->actor.speed = 0.0f;
        Actor_SetScale(&this->actor, 0.01f);
    }
}

/**
 * Summary: Chick is Walking (Hopping) in a straight line.
 *
 * ActionFunc for NWC Type: NWC_STATE_HOPPING_FORWARD
 */
void EnNwc_HopForward(Actor_EnNwc* this, GameState_Play* play)
{
    if (DECR(this->stateTimer) == 0)
    {
        EnNwc_ToggleState(this);
        return;
    }

    if ((this->stateTimer & 3) == 3)
        this->actor.velocity.y = 2.0f; // hop up and down
    if ((this->stateTimer & 0xB) == 8)
        Actor_PlaySfx(&this->actor, NA_SE_EV_CHICK_SONG);

    // they only move forward while off the ground, which gives the visual of them hopping to move
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)
        this->actor.speed = 0.0f;
    else
        this->actor.speed = 2.0f;
}

/**
 * Summary: Chick is Running Away from player (Breman Mask was dropped).
 *
 * ActionFunc for NWC Type: NWC_STATE_RUNNING
 */
void EnNwc_RunAway(Actor_EnNwc* this, GameState_Play* play)
{
    if (DECR(this->stateTimer) == 0)
    {
        EnNwc_ToggleState(this);
        return;
    }

    if ((this->stateTimer & 3) == 3)
        this->actor.velocity.y = 2.0f; // hop up and down
    if ((this->stateTimer & 0xB) == 8)
        Actor_PlaySfx(&this->actor, NA_SE_EV_CHICK_SONG);

    this->actor.speed = 2.0f;
    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL)
        EnNwc_ToggleState(this);
}

/**
 * Summary: Chick is turning to face a new direction.
 *
 * ActionFunc for NWC Type: NWC_STATE_TURNING
 */
void EnNwc_Turn(Actor_EnNwc* this, GameState_Play* play)
{
    if (DECR(this->stateTimer) == 0)
    {
        EnNwc_ToggleState(this);
        return;
    }

    if ((this->stateTimer & 7) == 7)
        this->actor.velocity.y = 2.0f; // vertical hop

    if ((this->stateTimer & 0xB) == 8)
        Actor_PlaySfx(&this->actor, NA_SE_EV_CHICK_SONG);

    // they only rotate when off the ground, giving the visual that they turn by hopping
    if (!(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND))
    {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->fallingRotY, 0xA, 0x1000, 0x800);
        this->actor.world.rot.y = this->actor.shape.rot.y;
    }
}

/**
 * Summary: Chick is standing still.
 *          Looking around, can see the player with the Breman Mask
 *
 * ActionFunc for NWC Type: NWC_STATE_CHECK_BREMAN
 */
void EnNwc_CheckForBreman(Actor_EnNwc* this, GameState_Play* play)
{
    if (DECR(this->stateTimer) == 0)
        EnNwc_ToggleState(this);

    EnNwc_CheckFound(this, play);
}

void EnNwc_Update(Actor_EnNwc* this, GameState_Play* play)
{
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 10.0f, 10.0f, 10.0f, 0x5);
    this->actionFunc(this, play);

    if (this->hasGrownUp & 1)
    {
        this->actor.objectSlot = this->niwObjectSlot;
        this->actor.draw = EnNwc_DrawAdultBody;
        this->actor.shape.shadowScale = 15.0f;
    }
    else
    {
        this->actor.objectSlot = this->nwcObjectIndex;
        this->actor.draw = EnNwc_Draw;
        this->actor.shape.shadowScale = 6.0f;
    }

    if (DECR(this->blinkTimer) == 0)
        this->blinkTimer = Rand_S16Offset(0x3C, 0x3C);

    if ((this->blinkTimer == 1) || (this->blinkTimer == 3))
        this->blinkState = 1;
    else
        this->blinkState = 0;
}

void EnNwc_Draw(Actor_EnNwc* this, GameState_Play* play)
{
    void* eyeTextures[] = { SEGADDR_NWC_OPEN_EYES, SEGADDR_NWC_CLOSED_EYES };
    Gfx* gfx;

    OPEN_DISPS(play->gs.gfx);

    Gfx_SetupDL25_Opa(play->gs.gfx);
    gfx = POLY_OPA_DISP;
    gSPSegment(&gfx[0], 0x08, Lib_SegmentedToVirtual(eyeTextures[this->blinkState]));
    gSPMatrix(&gfx[1], GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[2], SEGADDR_NWC_BODY_DL);
    POLY_OPA_DISP = &gfx[3];

    CLOSE_DISPS();
}

s32 EnNwc_OverrideLimbDraw(GameState_Play* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor_EnNwc* this)
{
    if (limbIndex == 0x0d)
        rot->y += this->upperBodyRotY;
    if ((limbIndex == 0x0b) || (limbIndex == 0x07))
    {
        rot->y += this->footRotY;
        rot->z += this->footRotZ;
    }

    return 0;
}

void EnNwc_DrawAdultBody(Actor_EnNwc* this, GameState_Play* play)
{
    Gfx_SetupDL25_Opa(play->gs.gfx);
    SkelAnime_DrawFlexOpa(play, this->niwSkeleton.skeleton, this->niwSkeleton.jointTable, this->niwSkeleton.dListCount,
                          EnNwc_OverrideLimbDraw, NULL, &this->actor);
}

ActorProfile EnNwc_InitVars =
{
    AC_EN_NWC,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_NWC,
    sizeof(Actor_EnNwc),
    (ActorFunc)EnNwc_Init,
    (ActorFunc)EnNwc_Destroy,
    (ActorFunc)EnNwc_Update,
    (ActorFunc)EnNwc_Draw,
};

OVL_ACTOR_INFO(AC_EN_NWC, EnNwc_InitVars);
