#include <combo.h>
#include <combo/item.h>
#include <combo/mm/ocarina.h>
#include <combo/data/song_events.h>

struct EnGg;
struct EnGgStruct;

typedef void (*EnGgActionFunc)(struct EnGg*, PlayState*);
typedef void (*EnGgUnkFunc)(struct EnGgStruct*, PlayState*);

#define OBJECT_GG_LIMB_MAX 0x14

typedef struct EnGgStruct {
    /* 0x00 */ Vec3f unk_00;
    /* 0x0C */ Vec3f unk_0C;
    /* 0x18 */ Vec3f unk_18;
    /* 0x24 */ Vec3f unk_24;
    /* 0x30 */ u8 animIndex;
    /* 0x34 */ EnGgUnkFunc unk_34;
    /* 0x38 */ EnGgUnkFunc unk_38;
    /* 0x3C */ s32 unk_3C;
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ u8 unk_48;
} EnGgStruct; // size = 0x4C

typedef struct EnGg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnGgActionFunc actionFunc;
    /* 0x1D8 */ Vec3s headRot;
    /* 0x1DE */ Vec3s torsoRot;
    /* 0x1E4 */ Vec3s jointTable[OBJECT_GG_LIMB_MAX];
    /* 0x25C */ Vec3s morphTable[OBJECT_GG_LIMB_MAX];
    /* 0x2D4 */ s8 unk_2D4[0x6];
    /* 0x2DA */ u8 csAnimIndex;
    /* 0x2DB */ u8 cueId;
    /* 0x2DC */ s16 csId;
    /* 0x2DE */ s8 unk2DE[4];
    /* 0x2E2 */ s16 unk_2E2;
    /* 0x2E4 */ s16 unk_2E4;
    /* 0x2E6 */ s16 animIndex;
    /* 0x2E8 */ s16 unk_2E8;
    /* 0x2EA */ s8 unk2EA[0x1A];
    /* 0x304 */ s16 unk_304;
    /* 0x306 */ u8 unk_306;
    /* 0x307 */ u8 unk_307;
    /* 0x308 */ u8 unk_308;
    /* 0x309 */ u8 unk_309;
    /* 0x30A */ s16 unk_30A;
    /* 0x30C */ s8 unk30C[4];
    /* 0x310 */ f32 unk_310;
    /* 0x314 */ s8 unk314[0xC];
    /* 0x320 */ Vec3f unk_320;
    /* 0x32C */ Vec3f unk_32C;
    /* 0x338 */ Vec3f unk_338;
    /* 0x344 */ EnGgStruct unk_344;
} EnGg; // size = 0x390

void EnGg_HandlerGiveItem(EnGg* this, PlayState* play)
{
    if (Actor_HasParentZ(&this->actor))
    {
        gMmExtraFlags.maskGoron = 1;
        Actor_Kill(&this->actor);
        return;
    }
    comboGiveItemNpc(&this->actor, play, GI_MM_MASK_GORON, NPC_MM_MASK_GORON, 9999.f, 9999.f);
}

void EnGg_MaybeDestroy(Actor* actor)
{
    if (gMmExtraFlags.maskGoron)
        Actor_Kill(actor);
}

void EnGg_WaitForSong(EnGg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->actor.xzDistToPlayer < 200.0f) {
        if (this->unk_306 == 0) {
            if (player->stateFlags2 & PLAYER_STATE2_MM_USING_OCARINA) {
                this->unk_306 = 1;
                Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
            }
        } else if (!(player->stateFlags2 & PLAYER_STATE2_MM_USING_OCARINA)) {
            this->unk_306 = 0;
        }

        if ((player->transformation == MM_PLAYER_FORM_HUMAN) && (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) &&
            Ocarina_CheckSongEventSong(play->msgCtx.ocarinaSong, SONG_EVENT_HEALING_DARMANI)) {
            Ocarina_ClearLastPlayedSong(play);
            if (!MM_GET_EVENT_WEEK(MM_EV(19, 7))) {
                MM_SET_EVENT_WEEK(MM_EV(19, 7));
            }
            this->unk_307 = true;
        }

        if (CutsceneManager_IsNext(this->csId)) {
            CutsceneManager_Start(this->csId, &this->actor);
            this->unk_307 = false;
        } else {
            if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
                CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
            }

            if (this->unk_307) {
                this->actionFunc = EnGg_HandlerGiveItem;
                this->unk_307 = false;
            }
        }
    } else {
        this->unk_307 = false;
    }
}

PATCH_FUNC(0x80b359dc, EnGg_WaitForSong)
