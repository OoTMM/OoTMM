#include <combo.h>
#include <combo/mm/ocarina.h>
#include <combo/data/song_events.h>

struct DmChar08;

typedef void (*DmChar08ActionFunc)(struct DmChar08*, PlayState*);

typedef struct DmChar08 {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ SkelAnime skelAnime;
    /* 0x1A0 */ DmChar08ActionFunc actionFunc;
    /* 0x1A4 */ Actor* palmTree1;
    /* 0x1A8 */ Actor* palmTree2;
    /* 0x1AC */ Vec3f tree1Pos;
    /* 0x1B8 */ Vec3f tree2Pos;
    /* 0x1C4 */ Vec3f bubblePos;
    /* 0x1D0 */ Vec3f focusPos;
    /* 0x1DC */ s8 unk_1DC[8];
    /* 0x1E4 */ f32 targetYPos;
    /* 0x1E8 */ s8 unk_1E8[8];
    /* 0x1F0 */ f32 unk_1F0;
    /* 0x1F4 */ s16 unk_1F4;
    /* 0x1F6 */ s16 cueId;
    /* 0x1F6 */ s16 blinkTimer;
    /* 0x1FA */ s16 unk_1FA;
    /* 0x1FC */ u16 unk_1FC;
    /* 0x1FE */ u8 bubbleCount;
    /* 0x1FF */ u8 unk_1FF;
    /* 0x200 */ u8 eyeMode;
    /* 0x201 */ u8 eyeIndex;
    /* 0x202 */ u8 animIndex;
    /* 0x203 */ u8 prevAnimIndex;
    /* 0x204 */ s8 unk_204;
    /* 0x205 */ u8 alpha;
    /* 0x206 */ u8 unk_206;
    /* 0x207 */ u8 unk_207;
    /* 0x208 */ u8 unk_208;
    /* 0x209 */ u8 dynapolyInitialized;
} DmChar08; // size = 0x20C

void DmChar08_WarpToTemple(Actor* this, PlayState* play)
{
    play->nextEntrance = ((SCE_MM_TEMPLE_GREAT_BAY - 3) << 9) | (0 << 4);
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_FADE_BLACK;

    gIsEntranceOverride = 1;
}

PATCH_FUNC(0x80aafa18, DmChar08_WarpToTemple);

static void DmChar08_AfterWarpFromTemple(Actor* this, PlayState* play)
{
    gIsEntranceOverride = 1;
}

PATCH_FUNC(0x80aafb94, DmChar08_AfterWarpFromTemple);

void DmChar08_Emerge(PlayState* play, Actor* this)
{
    this->world.pos.x = -6480.f;
    this->world.pos.z = 1750.f;
    this->world.rot.x = 0;
    this->world.rot.y = -0x7234;
    this->world.rot.z = 0;
    this->flags |= 1;

    *(u16*)((char*)this + 0xbc) = 0;
    *(u16*)((char*)this + 0xbe) = 0x8dcc;
    *(u16*)((char*)this + 0xc0) = 0;

    *((char*)this + 0x1ff) = 2;
    *((char*)this + 0x202) = 2;
    *((char*)this + 0x203) = 99;
    *((char*)this + 0x200) = 1;
    *((char*)this + 0x207) = 0;
    *((char*)this + 0x208) = 0;
}

void func_80AAFA18(Actor* this, PlayState* play) {
    play->nextEntrance = ENTR_MM_TEMPLE_GREAT_BAY;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
    gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
    play->transitionTrigger = TRANS_TRIGGER_START;
    Audio_PlaySfx(NA_SE_SY_WHITE_OUT_T);
}

/* TODO this address is same as DmChar08_WarpToTemple above */
PATCH_FUNC(0x80aafa18, func_80AAFA18);

static s16 sSuccessSoundAlreadyPlayed = 0;

void DmChar08_WaitForSong(DmChar08* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Player* player2 = GET_PLAYER(play);

    if ((player2->stateFlags2 & PLAYER_STATE2_MM_USING_OCARINA) &&
        ((player2->actor.world.pos.x > -5780.0f) && (player2->actor.world.pos.x < -5385.0f) &&
         (player2->actor.world.pos.z > 1120.0f) && (player2->actor.world.pos.z < 2100.0f))) {
        if (!sSuccessSoundAlreadyPlayed) {
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
            sSuccessSoundAlreadyPlayed = true;
        }
    } else {
        sSuccessSoundAlreadyPlayed = false;
    }
    if ((player->transformation == MM_PLAYER_FORM_ZORA) && (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) &&
        Ocarina_CheckSongEventSong(play->msgCtx.ocarinaSong, SONG_EVENT_TEMPLE_GREATBAY)) {
        if ((player2->actor.world.pos.x > -5780.0f) && (player2->actor.world.pos.x < -5385.0f)) {
            if ((player2->actor.world.pos.z > 1120.0f) && (player2->actor.world.pos.z < 2100.0f)) {
                Ocarina_ClearLastPlayedSong(play);
                this->actionFunc = actorAddr(ACTOR_DM_CHAR08, 0x80aaf79c);
            }
        }
    }
}

PATCH_FUNC(0x80aaf610, DmChar08_WaitForSong);
