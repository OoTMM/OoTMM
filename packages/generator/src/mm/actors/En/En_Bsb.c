#include <combo.h>
#include <combo/mm/ocarina.h>
#include <combo/data/song_events.h>

struct EnBsb;

typedef void (*EnBsbActionFunc)(struct EnBsb*, PlayState*);

#define OBJECT_BSB_LIMB_MAX 0x15
#define ENBSB_BODYPART_MAX 17
#define ENBSB_EFFECT_COUNT 50

typedef struct EnBsbEffect {
    /* 0x00 */ u8 isEnabled;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ Vec3f accel;
    /* 0x28 */ Vec3s rot;
    /* 0x30 */ f32 scale;
    /* 0x34 */ s16 life;
} EnBsbEffect; // size = 0x38

typedef struct EnBsb {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ SkelAnime skelAnime;
    /* 0x0188 */ Vec3s jointTable[OBJECT_BSB_LIMB_MAX];
    /* 0x0206 */ Vec3s morphTable[OBJECT_BSB_LIMB_MAX];
    /* 0x0284 */ EnBsbActionFunc actionFunc;
    /* 0x0288 */ Path* path;
    /* 0x028C */ s32 waypoint;
    /* 0x0290 */ u8 playedSfx;
    /* 0x0292 */ s16 unk_0292;
    /* 0x0294 */ s16 unk_0294;
    /* 0x0296 */ s16 cueId;
    /* 0x0298 */ Vec3f unk_0298;
    /* 0x02A4 */ s32 unk_02A4;
    /* 0x02A8 */ s32 unk_02A8;
    /* 0x02AC */ s16 unk_02AC;
    /* 0x02AE */ u8 unk_02AE;
    /* 0x02AF */ u8 unk_02AF;
    /* 0x02B0 */ s32 unk_02B0;
    /* 0x02B4 */ s16 unk_02B4;
    /* 0x02B6 */ s16 pathIndex;
    /* 0x02B8 */ s16 switchFlag1;
    /* 0x02BA */ s16 switchFlag2;
    /* 0x02BC */ s16 unk_02BC;
    /* 0x02C0 */ f32 unk_02C0;
    /* 0x02C4 */ f32 animEndFrame;
    /* 0x02C8 */ s8 unk_02C8[0x2];
    /* 0x02CA */ s16 alpha;
    /* 0x02CC */ s16 csIdList[5];
    /* 0x02D8 */ s32 animIndex;
    /* 0x02DC */ s32 unk_02DC;
    /* 0x02E0 */ Vec3f unk_02E0;
    /* 0x02EC */ Vec3f unk_02EC;
    /* 0x02F8 */ Vec3f unk_02F8;
    /* 0x0304 */ Vec3f unk_0304;
    /* 0x0310 */ Vec3s unk_0310;
    /* 0x0316 */ Vec3s unk_0316;
    /* 0x031C */ Vec3s unk_031C;
    /* 0x0322 */ s16 drawDmgEffTimer;
    /* 0x0324 */ s16 drawDmgEffType;
    /* 0x0328 */ f32 drawDmgEffScale;
    /* 0x032C */ f32 drawDmgEffFrozenSteamScale;
    /* 0x0330 */ Vec3f bodyPartsPos[ENBSB_BODYPART_MAX];
    /* 0x03FC */ s32 unk_03FC[ENBSB_BODYPART_MAX];
    /* 0x0440 */ s16 bodyPartIndex;
    /* 0x0444 */ EnBsbEffect effects[ENBSB_EFFECT_COUNT];
    /* 0x0F34 */ ColliderJntSph collider;
    /* 0x0F54 */ ColliderJntSphElement colliderElements[7];
    /* 0x1114 */ s8 unk_1114[0x4];
    /* 0x1118 */ s16 unk_1118;
    /* 0x111A */ s16 subCamId;
    /* 0x111C */ s16 unk_111C;
    /* 0x1120 */ f32 subCamFov;
    /* 0x1124 */ f32 subCamFovNext;
    /* 0x1128 */ Vec3f subCamEye;
    /* 0x1134 */ Vec3f subCamAt;
    /* 0x1140 */ Vec3f subCamEyeNext;
    /* 0x114C */ Vec3f subCamAtNext;
} EnBsb; // size = 0x1158

static void EnBsb_OnDeath(Actor* this, PlayState* play)
{
    /* Disable the fire ring */
    Flags_SetSwitch(play, 0xb);

    /* Mark Keeta as dead */
    MM_SET_EVENT_WEEK(EV_MM_WEEK_KEETA_DEAD);

    /* Warp on top of the bridge */
    play->nextEntrance = ((SCE_MM_IKANA_GRAVEYARD - 3) << 9) | (5 << 4);
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
}

PATCH_FUNC(0x80c0d9b4, EnBsb_OnDeath);

void EnBsb_WaitForSong(EnBsb* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 yaw = ABS_ALT((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y));

    if ((yaw < 0x4300) && !(this->actor.xzDistToPlayer > 300.0f)) {
        if (player->stateFlags2 & PLAYER_STATE2_MM_USING_OCARINA) {
            if (!this->playedSfx) {
                Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
                this->playedSfx = true;
            }
        } else {
            this->playedSfx = false;
        }

        if ((play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) && Ocarina_CheckSongEventSong(play->msgCtx.ocarinaSong, SONG_EVENT_AWAKENING_KEETA)) {
            Ocarina_ClearLastPlayedSong(play);
            play->msgCtx.ocarinaMode = OCARINA_MODE_END;
            Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_BOSS);
            EnBsbActionFunc func_80C0C0F4 = actorAddr(ACTOR_EN_BSB, 0x80c0c0f4);
            func_80C0C0F4(this, play);
        }
    }
}

PATCH_FUNC(0x80c0bfe8, EnBsb_WaitForSong)
