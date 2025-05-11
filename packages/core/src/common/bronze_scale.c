#include <combo.h>
#include <combo/entrance.h>
#include <combo/config.h>

#if defined(GAME_OOT)
# define HAS_BRONZE_SCALE() (!!(gSharedCustomSave.bronzeScaleOot))
#endif

#if defined(GAME_MM)
# define HAS_BRONZE_SCALE() (!!(gSharedCustomSave.bronzeScaleMm))
#endif

static Vec3f sBronzeScaleGroundPos;
static s16 sBronzeScaleGroundRot;
static u8 sBronzeScaleGroundRoom;

static void BronzeScale_CopyPos(Player* this, PlayState* play)
{
    sBronzeScaleGroundPos = this->actor.world.pos;
    sBronzeScaleGroundRot = this->actor.world.rot.y;
    sBronzeScaleGroundRoom = play->roomCtx.curRoom.num;
}

void Player_HandleBronzeScale(Player* this, PlayState* play)
{
#if defined(GAME_MM)
    if (!Config_Flag(CFG_MM_SCALES))
        return;
#endif

    if (HAS_BRONZE_SCALE())
        return;

    if (g.bronzeScaleTimer == 0)
    {
        BronzeScale_CopyPos(this, play);
        g.bronzeScaleTimer = 1;
    }

    if (play->transitionTrigger)
        return;

    if (this->stateFlags1 & (PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN | PLAYER_ACTOR_STATE_DEATH | PLAYER_ACTOR_STATE_FROZEN))
        return;

    if (!(this->stateFlags1 & (PLAYER_ACTOR_STATE_WATER | PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_CLIMB2 | PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_JUMPING) || (this->actor.bgCheckFlags & BGCHECKFLAG_WATER) || !(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)))
    {
        BronzeScale_CopyPos(this, play);
        g.bronzeScaleSolidGround = 1;
        return;
    }

#if defined(GAME_MM)
    if (this->transformation != MM_PLAYER_FORM_HUMAN && this->transformation != MM_PLAYER_FORM_FIERCE_DEITY)
        return;
#endif

#if defined(GAME_OOT)
    if (this->currentBoots == 2)
        return;
#endif

#if defined(GAME_MM)
    if (this->currentBoots == 6 || this->currentBoots == 3)
        return;
#endif

    if (!(this->stateFlags1 & PLAYER_ACTOR_STATE_WATER))
        return;

    if (!g.bronzeScaleSolidGround)
    {
        g.bronzeScaleTimer++;
        if (g.bronzeScaleTimer < 20)
            return;
    }

    /* Player is in deep water with no bronze scale */
    PlaySound(NA_SE_EV_WATER_CONVECTION);
    Play_SetRespawnData(play, RESPAWN_MODE_DOWN, gSave.entrance, sBronzeScaleGroundRoom, 0xdff, &sBronzeScaleGroundPos, sBronzeScaleGroundRot);
    gSaveContext.respawnFlag = 1;
    gSaveContext.nextCutscene = 0;
    comboTransition(gPlay, gSave.entrance);
}
