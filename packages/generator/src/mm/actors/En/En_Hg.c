#include <combo.h>
#include <combo/item.h>
#include <combo/mm/ocarina.h>
#include <combo/data/song_events.h>

struct EnHg;

typedef void (*EnHgActionFunc)(struct EnHg*, PlayState*);
typedef void (*EnHgFunc)(struct EnHg*);

#define PAMELAS_FATHER_GIBDO_LIMB_MAX 0x13

typedef struct EnHg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnHgActionFunc actionFunc;
    /* 0x1D8 */ MtxF mf;
    /* 0x218 */ s32 csIdIndex;
    /* 0x21C */ s32 animIndex;
    /* 0x220 */ Vec3s jointTable[PAMELAS_FATHER_GIBDO_LIMB_MAX];
    /* 0x292 */ Vec3s morphTable[PAMELAS_FATHER_GIBDO_LIMB_MAX];
    /* 0x304 */ s8 pad304[0xC];
    /* 0x310 */ u16 csIdList[4];
} EnHg; // size = 0x318

typedef enum {
    /* 0 */ HG_CS_FIRST_ENCOUNTER,
    /* 1 */ HG_CS_GET_MASK,
    /* 2 */ HG_CS_SUBSEQUENT_ENCOUNTER,
    /* 3 */ HG_CS_SONG_OF_HEALING
} HgCsIndex;

void EnHg_HandlerGiveMask(EnHg* this, PlayState* play)
{
    if (gMmExtraFlags.maskGibdo || Actor_HasParentZ(&this->actor))
    {
        gMmExtraFlags.maskGibdo = 1;
        MM_SET_EVENT_WEEK(EV_MM_WEEK_PAMELA_FATHER);
        Actor_Kill(&this->actor);
        return;
    }

    comboGiveItemNpc(&this->actor, play, GI_MM_MASK_GIBDO, NPC_MM_MASK_GIBDO, 9999.f, 9999.f);
}

static u32 sHasSoundPlayed = false;

void EnHg_WaitForPlayerAction(EnHg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    // If the player has gone upstairs this interaction will not trigger
    if ((this->actor.colChkInfo.health != 1) || (fabsf(this->actor.yDistanceFromLink) >= 80.0f)) {
        return;
    }

    if (player->stateFlags2 & PLAYER_STATE2_MM_USING_OCARINA) {
        if (!sHasSoundPlayed) {
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
        }
        sHasSoundPlayed = true;
    } else {
        sHasSoundPlayed = false;
    }

    if (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) {
        if (Ocarina_CheckSongEventSong(play->msgCtx.ocarinaSong, SONG_EVENT_HEALING_PAMELA_FATHER) && (gSaveContext.save.playerForm == MM_PLAYER_FORM_HUMAN)) {
            Ocarina_ClearLastPlayedSong(play);
            this->actionFunc = EnHg_HandlerGiveMask;
        }
    } else {
        if ((this->actor.xzDistToPlayer < 60.0f) && (fabsf(this->actor.yDistanceFromLink) < 40.0f)) {
            EnHgActionFunc EnHg_PlayCutscene = actorAddr(ACTOR_EN_HG, 0x80bcf8a0);
            EnHgActionFunc EnHg_HandleCutscene = actorAddr(ACTOR_EN_HG, 0x80bcf95c);
            if ((this->actionFunc != EnHg_PlayCutscene) && (this->actionFunc != EnHg_HandleCutscene)) {
                if (!MM_GET_EVENT_WEEK(MM_EV(61, 1))) {
                    MM_SET_EVENT_WEEK(MM_EV(61, 1));
                    this->csIdIndex = HG_CS_FIRST_ENCOUNTER;
                } else {
                    this->csIdIndex = HG_CS_SUBSEQUENT_ENCOUNTER;
                }

                EnHgFunc EnHg_SetupCutscene = actorAddr(ACTOR_EN_HG, 0x80bcf88c);
                EnHg_SetupCutscene(this);
                return;
            }
        }

        EnHgActionFunc EnHg_HandleTatlDialog = actorAddr(ACTOR_EN_HG, 0x80bcf710);
        EnHg_HandleTatlDialog(this, play);
    }
}

PATCH_FUNC(0x80bcfc0c, EnHg_WaitForPlayerAction);
