#include <combo.h>
#include <combo/entrance.h>

#if defined(GAME_OOT)
# define HAS_BRONZE_SCALE() (!!(gSharedCustomSave.bronzeScaleOot))
# define BOOTS              2
#endif

#if defined(GAME_MM)
# define HAS_BRONZE_SCALE() (!!(gSharedCustomSave.bronzeScaleMm))
# define BOOTS              6
#endif

static Vec3f sBronzeScaleGroundPos;
static s16 sBronzeScaleGroundRot;
static u8 sBronzeScaleGroundRoom;

void Player_HandleBronzeScale(Player* this, PlayState* play)
{
    if (HAS_BRONZE_SCALE())
        return;

    if (!(this->stateFlags1 & (PLAYER_ACTOR_STATE_WATER | PLAYER_ACTOR_STATE_CLIMB | PLAYER_ACTOR_STATE_CLIMB2 | PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_JUMPING) || (this->actor.bgCheckFlags & BGCHECKFLAG_WATER) || !(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)))
    {
        sBronzeScaleGroundPos = this->actor.world.pos;
        sBronzeScaleGroundRot = this->actor.world.rot.y;
        sBronzeScaleGroundRoom = play->roomCtx.curRoom.num;
        g.bronzeScaleSolidGround = 1;
        return;
    }

#if defined(GAME_MM)
    if (this->transformation != MM_PLAYER_FORM_HUMAN && this->transformation != MM_PLAYER_FORM_FIERCE_DEITY)
        return;
#endif

    if (this->currentBoots == BOOTS)
        return;

    if (!(this->stateFlags1 & PLAYER_ACTOR_STATE_WATER))
        return;

    if (!g.bronzeScaleSolidGround)
        return;

    if (play->transitionTrigger)
        return;

    /* Player is in deep water with no bronze scale */
    PlaySound(NA_SE_EV_WATER_CONVECTION);
    Play_SetRespawnData(play, RESPAWN_MODE_DOWN, gSave.entrance, sBronzeScaleGroundRoom, 0xdff, &sBronzeScaleGroundPos, sBronzeScaleGroundRot);
    gSaveContext.respawnFlag = 1;
    gSaveContext.nextCutscene = 0;
    comboTransition(gPlay, gSave.entrance);
}
