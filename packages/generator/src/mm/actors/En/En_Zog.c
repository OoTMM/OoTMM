#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/mm/ocarina.h>
#include <combo/data/song_events.h>

struct EnZog;

typedef void (*EnZogFunc)(struct EnZog*);
typedef void (*EnZogActionFunc)(struct EnZog*, PlayState*);
typedef void (*EnZogSetCutscene)(struct EnZog*, s16);

#define ENZOG_GET_F(thisx) ((thisx)->params & 0xF)
#define ENZOG_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFC00) >> 0xA)

#define ENZOG_F_2 2
#define ENZOG_FC00_63 63

#define OBJECT_ZOG_LIMB_MAX 0x17

typedef struct EnZog {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ Vec3s jointTable[OBJECT_ZOG_LIMB_MAX];
    /* 0x25E */ Vec3s morphTable[OBJECT_ZOG_LIMB_MAX];
    /* 0x2E8 */ Path* path;
    /* 0x2EC */ s32 unk_2EC;
    /* 0x2F0 */ Vec3f unk_2F0;
    /* 0x2FC */ s16 unk_2FC;
    /* 0x2FE */ s16 unk_2FE;
    /* 0x300 */ s16 unk_300;
    /* 0x302 */ s16 unk_302;
    /* 0x304 */ s16 animIndex;
    /* 0x306 */ s16 cueId;
    /* 0x308 */ s16 unk_308;
    /* 0x30A */ u16 unk_30A;
    /* 0x30C */ s16 csIdList[7];
    /* 0x31A */ s16 csIdIndex;
    /* 0x31C */ s16 unk_31C;
    /* 0x31E */ s16 unk_31E;
    /* 0x320 */ s16 unk_320;
    /* 0x322 */ s16 unk_322;
    /* 0x324 */ s16 unk_324;
    /* 0x328 */ EnZogActionFunc actionFunc;
} EnZog; // size = 0x32C

void EnZog_GiveItem(Actor* this, PlayState* play)
{
    if (Actor_HasParentZ(this))
    {
        if (!gMmExtraFlags.maskZora && !(GET_PLAYER(play)->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM))
        {
            gMmExtraFlags.maskZora = 1;
            play->nextEntrance = ENTR_MM_COAST_FROM_MIKAU_CS;
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
        }
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_MASK_ZORA, NPC_MM_MASK_ZORA, 9999.f, 9999.f);
}

PATCH_FUNC(0x80b943c0, EnZog_GiveItem);

void EnZog_InitSetScaleHook(Actor* this, float scale)
{
    Actor_SetScale(this, scale);
    if (gMmExtraFlags.maskZora)
        Actor_Kill(this);
}

PATCH_CALL(0x80b935dc, EnZog_InitSetScaleHook);

Actor* EnZog_SpawnAsChild(ActorContext* actorCtx, Actor* parent, PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params)
{
    if (gMmExtraFlags.maskZora)
        return NULL;
    return Actor_SpawnAsChild(actorCtx, parent, play, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
}

PATCH_CALL(0x80b93874, EnZog_SpawnAsChild);

s32 EnZog_OfferGrab(Actor* actor, PlayState* play, s32 getItemId, f32 xzRange, f32 yRange)
{
    if (comboIsLinkAdult())
    {
        yRange = 96.0f;
    }
    return Actor_OfferGetItem(actor, play, getItemId, xzRange, yRange);
}

PATCH_CALL(0x80b95024, EnZog_OfferGrab);

void EnZog_WaitForPlayerAction(EnZog* this, PlayState* play) {
    if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        this->unk_300 = 2;
        this->actionFunc = actorAddr(ACTOR_EN_ZOG, 0x80b94470);
    } else if ((play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) && (this->actor.xzDistToPlayer < 120.0f)) {
        if (Ocarina_CheckSongEventSong(play->msgCtx.ocarinaSong, SONG_EVENT_HEALING_MIKAU) && (gSave.playerForm == MM_PLAYER_FORM_HUMAN)) {
            Ocarina_ClearLastPlayedSong(play);
            EnZogSetCutscene func_80B93BA8 = actorAddr(ACTOR_EN_ZOG, 0x80B93BA8);
            func_80B93BA8(this, 2);
            this->actor.update = actorAddr(ACTOR_EN_ZOG, 0x80b943c0);
            this->actor.shape.shadowDraw = NULL;
        }
    } else if (this->actor.xzDistToPlayer < 120.0f) {
        Actor_OfferTalk(&this->actor, play, 150.0f);
    }
    EnZogActionFunc func_80B943EC = actorAddr(ACTOR_EN_ZOG, 0x80b943ec);
    func_80B943EC(this, play);
}

PATCH_FUNC(0x80b9451c, EnZog_WaitForPlayerAction)

void EnZog_WaitForPlayerAction2(EnZog* this, PlayState* play) {
    if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        this->unk_300 = 2;
        this->actionFunc = actorAddr(ACTOR_EN_ZOG, 0x80b946fc);
    } else if ((play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) && (this->actor.xzDistToPlayer < 120.0f)) {
        if (Ocarina_CheckSongEventSong(play->msgCtx.ocarinaSong, SONG_EVENT_HEALING_MIKAU) && (gSave.playerForm == MM_PLAYER_FORM_HUMAN)) {
            Ocarina_ClearLastPlayedSong(play);
            EnZogSetCutscene func_80B93BA8 = actorAddr(ACTOR_EN_ZOG, 0x80B93BA8);
            func_80B93BA8(this, 2);
            this->actionFunc = actorAddr(ACTOR_EN_ZOG, 0x80b943c0);
            this->actor.shape.shadowDraw = NULL;
        }
        return;
    } else {
        if ((this->unk_302 == 2) && (this->unk_2FC == 2)) {
            this->unk_300 = 0;
            this->unk_2FE = 3;
            this->unk_302 = this->unk_300;
            this->unk_2FC = this->unk_2FE;
            EnZogFunc func_80B93B44 = actorAddr(ACTOR_EN_ZOG, 0x80b93b44);
            func_80B93B44(this);
        }

        if ((this->unk_302 == 0) && (this->actor.xzDistToPlayer < 120.0f)) {
            Actor_OfferTalk(&this->actor, play, 150.0f);
        }
    }

    EnZogActionFunc func_80B93A48 = actorAddr(ACTOR_EN_ZOG, 0x80b93a48);
    func_80B93A48(this, play);

    EnZogActionFunc func_80B943EC = actorAddr(ACTOR_EN_ZOG, 0x80b943ec);
    func_80B943EC(this, play);
}
PATCH_FUNC(0x80b948a8, EnZog_WaitForPlayerAction2)
