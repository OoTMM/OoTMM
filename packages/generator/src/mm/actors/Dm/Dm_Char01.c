#include <combo.h>
#include <combo/mm/ocarina.h>
#include <combo/data/song_events.h>

struct DmChar01;

typedef void (*DmChar01ActionFunc)(struct DmChar01*, PlayState*);

typedef struct DmChar01 {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ s8 unk15C[0x44];
    /* 0x1A0 */ DmChar01ActionFunc actionFunc;
    /* 0x1A4 */ s8 unk1A4[0x8];
    /* 0x1AC */ s16 unk_1AC[205];
    /* 0x346 */ s16 unk_346;
    /* 0x348 */ f32 unk_348;
    /* 0x34C */ u8 unk_34C;
    /* 0x34D */ u8 unk_34D;
} DmChar01; // size = 0x350

void DmChar01_WaitForSong(DmChar01* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Player* player2 = GET_PLAYER(play);

    if (MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE)) {
        return;
    }

    s16* D_80AAAAB4 = actorAddr(ACTOR_DM_CHAR01, 0x80aaaab4);

    if ((player->stateFlags2 & PLAYER_STATE2_MM_USING_OCARINA) && (player2->actor.world.pos.x > -40.0f) &&
        (player2->actor.world.pos.x < 40.0f) && (player2->actor.world.pos.z > 1000.0f) &&
        (player2->actor.world.pos.z < 1078.0f)) {
        if (!*D_80AAAAB4) {
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
            *D_80AAAAB4 = true;
        }
    } else {
        *D_80AAAAB4 = false;
    }

    if ((player->transformation == MM_PLAYER_FORM_DEKU) && (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) &&
        Ocarina_CheckSongEventSong(play->msgCtx.ocarinaSong, SONG_EVENT_TEMPLE_WOODFALL)) {

        if ((player2->actor.world.pos.x > -40.0f) && (player2->actor.world.pos.x < 40.0f) &&
            (player2->actor.world.pos.z > 1000.0f) && (player2->actor.world.pos.z < 1078.0f)) {
            Ocarina_ClearLastPlayedSong(play);
            MM_SET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE);
            play->nextEntrance = ((SCE_MM_WOODFALL - 3) << 9) | (4 << 4);
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
        }
    }
}

PATCH_FUNC(0x80aa8698, DmChar01_WaitForSong);
