#include <combo.h>
#include <combo/mm/ocarina.h>
#include <combo/data/song_events.h>

struct EnPoComposer;

typedef void (*EnPoComposerFunc)(struct EnPoComposer*);
typedef void (*EnPoComposerActionFunc)(struct EnPoComposer*, PlayState*);

#define POE_COMPOSER_LIMB_MAX 0xc

typedef enum {
    /* 0 */ SHARP_CS_ENCOUNTER_HEALED_TALK,
    /* 1 */ SHARP_CS_ENCOUNTER_AGRO_TALK,
    /* 2 */ SHARP_CS_SONG_HEALING,
    /* 3 */ SHARP_CS_SONG_STORMS,
    /* 4 */ SHARP_CS_MAX
} SharpCsIdIndex;

typedef struct EnPoComposer {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder bodyCollider;
    /* 0x190 */ ColliderJntSph lanternCollider;
    /* 0x1B0 */ ColliderJntSphElement lanternColliderElements[1];
    /* 0x1F0 */ SkelAnime skelAnime;
    /* 0x234 */ EnPoComposerActionFunc actionFunc;
    /* 0x238 */ MtxF lanternMtxF;
    /* 0x278 */ LightNode* lightNode;
    /* 0x27C */ LightInfo lightInfo;
    /* 0x28A */ s8 unk_28A[0x6];
    /* 0x290 */ s32 sharpCsNum;
    /* 0x294 */ Vec3s jointTable[POE_COMPOSER_LIMB_MAX];
    /* 0x2DC */ Vec3s morphTable[POE_COMPOSER_LIMB_MAX];
    /* 0x324 */ s16 csIds[SHARP_CS_MAX];
    /* 0x32C */ s16 actionTimer;
    /* 0x32E */ s16 csCueTimer;
    /* 0x330 */ s16 inCutscene;
    /* 0x332 */ s16 visible;
    /* 0x334 */ u16 cueId;
    /* 0x336 */ union {
                Color_RGBA8 mainAlpha;  // the alpha is used as general alpha for the whole body
                Color_RGBA8 eyeColor;   // also used for robe bottom
    };
    /* 0x33A */ Color_RGBA8 lightColor; // used by lantern light, and lantern glass tint
} EnPoComposer; // size = 0x340

static s32 sPlayerIsPlayingOcarina = false;

void EnPoComposer_PlayCurse(EnPoComposer* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    // Damage the player every second
    if (this->actionTimer == 20) {
        // Quarter heart damage (4)
        func_800B8D10(play, &this->actor, 0.0f, 0, 0.0f, 1, 4);
        this->actionTimer = 0;
    } else {
        this->actionTimer++;
    }

    // Ocarina check
    if (player->stateFlags2 & PLAYER_STATE2_MM_USING_OCARINA) {
        if (!sPlayerIsPlayingOcarina) {
            // Play sound whenever the player begins playing the Ocarina
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
        }
        sPlayerIsPlayingOcarina = true;
    } else {
        sPlayerIsPlayingOcarina = false;
    }

    // Song check, check if player just played song of storms or song of healing
    if (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) {
        if (play->msgCtx.ocarinaSong == OCARINA_SONG_HEALING)
        {
            this->sharpCsNum = SHARP_CS_SONG_HEALING;
            EnPoComposerFunc EnPoComposer_SetupStartCutscene = actorAddr(ACTOR_EN_PO_COMPOSER, 0x80bc52d4);
            EnPoComposer_SetupStartCutscene(this);
        }
        else if (Ocarina_CheckSongEventSong(play->msgCtx.ocarinaSong, SONG_EVENT_STORMS_COMPOSER))
        {
            Ocarina_ClearLastPlayedSong(play);
            this->sharpCsNum = SHARP_CS_SONG_STORMS;
            /* Mark the valley as purified */
            MM_SET_EVENT_WEEK(MM_EV(14, 2));
            Actor_Kill(&this->actor);

            /* Reload the room */
            play->nextEntrance = ((SCE_MM_IKANA_CANYON - 3) << 9) | (14 << 4);
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
        }
    }

    // Play sharp's curse bgm
    Audio_PlaySequenceAtPos(SEQ_PLAYER_BGM_SUB, &this->actor.projectedPos, NA_BGM_SHARPS_CURSE, 2000.0f);
}

PATCH_FUNC(0x80bc5404, EnPoComposer_PlayCurse)
