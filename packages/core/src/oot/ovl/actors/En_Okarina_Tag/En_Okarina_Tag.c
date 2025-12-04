#include "En_Okarina_Tag.h"
#include <combo/custom.h>
#include <combo/config.h>
#include <combo/data/song_events.h>

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_UPDATE_DURING_OCARINA)

void EnOkarinaTag_DrawCustom(Actor* thisx, PlayState* play);

void EnOkarinaTag_Init(Actor* thisx, PlayState* play);
void EnOkarinaTag_Destroy(Actor* thisx, PlayState* play);
void EnOkarinaTag_Update(Actor* thisx, PlayState* play);

void func_80ABEF2C(EnOkarinaTag* this, PlayState* play);
void func_80ABF708(EnOkarinaTag* this, PlayState* play);
void func_80ABF28C(EnOkarinaTag* this, PlayState* play);
void func_80ABF0CC(EnOkarinaTag* this, PlayState* play);
void func_80ABF4C8(EnOkarinaTag* this, PlayState* play);
void func_80ABF7CC(EnOkarinaTag* this, PlayState* play);

ActorProfile En_Okarina_Tag_Profile = {
    /**/ ACTOR_EN_OKARINA_TAG,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(EnOkarinaTag),
    /**/ EnOkarinaTag_Init,
    /**/ EnOkarinaTag_Destroy,
    /**/ EnOkarinaTag_Update,
    /**/ NULL,
};

OVL_INFO_ACTOR(ACTOR_EN_OKARINA_TAG, En_Okarina_Tag_Profile);

void EnOkarinaTag_Destroy(Actor* thisx, PlayState* play) {
}

static int EnOkarinaTag_GetShuffledSongId(Actor* thisx, PlayState* play)
{
    switch (play->sceneId)
    {
    case SCE_OOT_TEMPLE_OF_TIME:
        return SONG_EVENT_TEMPLE_OF_TIME;
    case SCE_OOT_TOMB_DAMPE_WINDMILL:
        return SONG_EVENT_WINDMILL;
    case SCE_OOT_GRAVEYARD:
        return SONG_EVENT_GRAVEYARD;
    case SCE_OOT_ZORA_RIVER:
        return SONG_EVENT_ZORA_RIVER;
    case SCE_OOT_GORON_CITY:
        return SONG_EVENT_GORON_CITY;
    case SCE_OOT_GREAT_FAIRY_FOUNTAIN_SPELLS:
        switch (play->spawn)
        {
        case 0:
            return SONG_EVENT_GREAT_FAIRY_SPELL_WIND;
        case 1:
            return SONG_EVENT_GREAT_FAIRY_SPELL_FIRE;
        case 2:
            return SONG_EVENT_GREAT_FAIRY_SPELL_LOVE;
        default:
            return -1;
        }
    case SCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES:
        switch (play->spawn)
        {
        case 0:
            return SONG_EVENT_GREAT_FAIRY_UPGRADE_MAGIC;
        case 1:
            return SONG_EVENT_GREAT_FAIRY_UPGRADE_MAGIC2;
        case 2:
            return SONG_EVENT_GREAT_FAIRY_UPGRADE_DEFENSE;
        default:
            return -1;
        }
    case SCE_OOT_TEMPLE_WATER:
        return SONG_EVENT_TEMPLE_WATER;
    case SCE_OOT_TEMPLE_SHADOW:
        return SONG_EVENT_TEMPLE_SHADOW;
    case SCE_OOT_TEMPLE_SPIRIT:
        switch (thisx->room)
        {
            case 5:
                return SONG_EVENT_TEMPLE_SPIRIT_1;
            case 14:
                return SONG_EVENT_TEMPLE_SPIRIT_2;
            case 24:
                return SONG_EVENT_TEMPLE_SPIRIT_3;
            default:
                return -1;
        }
    case SCE_OOT_BOTTOM_OF_THE_WELL:
        return SONG_EVENT_TEMPLE_BOTW;
    case SCE_OOT_INSIDE_GANON_CASTLE:
        return SONG_EVENT_TEMPLE_GANON;
    }

    return -1;
}

void EnOkarinaTag_Init(Actor* thisx, PlayState* play) {
    EnOkarinaTag* this = (EnOkarinaTag*)thisx;

    this->shuffledSongId = (s8)EnOkarinaTag_GetShuffledSongId(thisx, play);
    if (this->shuffledSongId >= 0 && Config_Flag(CFG_OOT_SONG_EVENTS_SHUFFLE)) {
        this->actor.draw = EnOkarinaTag_DrawCustom;
    }

    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->type = PARAMS_GET_U(this->actor.params, 10, 6);
    this->ocarinaSong = PARAMS_GET_U(this->actor.params, 6, 4);
    this->switchFlag = PARAMS_GET_U(this->actor.params, 0, 6);
    if (this->switchFlag == 0x3F) {
        this->switchFlag = -1;
    }
    if (this->ocarinaSong == 0xF) {
        this->ocarinaSong = 0;
        this->unk_158 = 1;
    }
    this->actor.attentionRangeType = ATTENTION_RANGE_1;
    if (this->actor.world.rot.z > 0) {
        this->interactRange = this->actor.world.rot.z * 40.0f;
    }

    if ((this->switchFlag >= 0) && (Flags_GetSwitch(play, this->switchFlag))) {
        Actor_Kill(&this->actor);
    } else {
        switch (this->type) {
            case 7:
                this->actionFunc = func_80ABEF2C;
                break;
            case 2:
                if (gSave.age== AGE_ADULT) {
                    Actor_Kill(&this->actor);
                    break;
                }
                /* FALLTHROUGH */
            case 1:
            case 4:
            case 6:
                this->actionFunc = func_80ABF28C;
                break;
            case 5:
                this->actor.messageId = 0x5021;
                this->actionFunc = func_80ABF708;
                break;
            default:
                Actor_Kill(&this->actor);
                break;
        }
    }
}

void func_80ABEF2C(EnOkarinaTag* this, PlayState* play) {
    Player* player;
    u16 ocarinaSong;

    player = GET_PLAYER(play);
    this->unk_15A++;
    if ((this->switchFlag >= 0) && (Flags_GetSwitch(play, this->switchFlag))) {
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    } else {
        if ((this->ocarinaSong != 6) || (gSaveContext.save.info.scarecrowSpawnSongSet)) {
            if ((this->actor.xzDistToPlayer < (90.0f + this->interactRange)) &&
                (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 80.0f)) {
                if (player->stateFlags2 & PLAYER_STATE2_25) {
                    ocarinaSong = this->ocarinaSong;
                    if (ocarinaSong == 6) {
                        ocarinaSong = 0xA;
                    }
                    player->stateFlags2 |= PLAYER_STATE2_23;
                    Message_StartOcarina(play, ocarinaSong + OCARINA_ACTION_CHECK_SARIA);
                    this->actionFunc = func_80ABF0CC;
                } else if ((this->actor.xzDistToPlayer < (50.0f + this->interactRange) &&
                            ((fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 40.0f)))) {
                    this->unk_15A = 0;
                    player->unk_6a8 = &this->actor;
                }
            }
        }
    }
}

void func_80ABF0CC(EnOkarinaTag* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (play->msgCtx.ocarinaMode == OCARINA_MODE_04) {
        this->actionFunc = func_80ABEF2C;
    } else {
        if (play->msgCtx.ocarinaMode == OCARINA_MODE_03) {
            if (this->switchFlag >= 0) {
                Flags_SetSwitch(play, this->switchFlag);
            }
            if (play->sceneId == SCE_OOT_TEMPLE_WATER) {
                play->msgCtx.msgMode = MSGMODE_PAUSED;
            }
            play->msgCtx.ocarinaMode = OCARINA_MODE_04;
            Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
            this->actionFunc = func_80ABEF2C;
            return;
        }
        if (this->unk_158 != 0) {
            if ((play->msgCtx.ocarinaMode == OCARINA_MODE_05) || (play->msgCtx.ocarinaMode == OCARINA_MODE_06) ||
                (play->msgCtx.ocarinaMode == OCARINA_MODE_07) || (play->msgCtx.ocarinaMode == OCARINA_MODE_08) ||
                (play->msgCtx.ocarinaMode == OCARINA_MODE_09) || (play->msgCtx.ocarinaMode == OCARINA_MODE_0A) ||
                (play->msgCtx.ocarinaMode == OCARINA_MODE_0D)) {
                if (this->switchFlag >= 0) {
                    Flags_SetSwitch(play, this->switchFlag);
                }
                play->msgCtx.ocarinaMode = OCARINA_MODE_04;
                Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
                this->actionFunc = func_80ABEF2C;
                return;
            }
        }
        if ((play->msgCtx.ocarinaMode >= OCARINA_MODE_05) && (play->msgCtx.ocarinaMode < OCARINA_MODE_0E)) {
            play->msgCtx.ocarinaMode = OCARINA_MODE_04;
            this->actionFunc = func_80ABEF2C;
        } else if (play->msgCtx.ocarinaMode == OCARINA_MODE_01) {
            player->stateFlags2 |= PLAYER_STATE2_23;
        }
    }
}

void func_80ABF28C(EnOkarinaTag* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->unk_15A++;
    if ((this->ocarinaSong != 6) || (gSaveContext.save.info.scarecrowSpawnSongSet)) {
        if ((this->switchFlag >= 0) && Flags_GetSwitch(play, this->switchFlag)) {
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        } else if (((this->type != 4) || !GET_EVENTCHKINF(EV_OOT_CHK_DOOR_TIME)) &&
                   ((this->type != 6) || !GET_EVENTCHKINF(0x1d)) &&
                   (this->actor.xzDistToPlayer < (90.0f + this->interactRange)) &&
                   (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 80.0f)) {
            if (player->stateFlags2 & PLAYER_STATE2_24) {
                switch (this->type) {
                    case 1:
                        Message_StartOcarina(play, OCARINA_ACTION_CHECK_LULLABY);
                        break;
                    case 2:
                        Message_StartOcarina(play, OCARINA_ACTION_CHECK_STORMS);
                        break;
                    case 4:
                        Message_StartOcarina(play, OCARINA_ACTION_CHECK_TIME);
                        break;
                    case 6:
                        Message_StartOcarina(play, OCARINA_ACTION_CHECK_LULLABY);
                        break;
                    default:
                        Actor_Kill(&this->actor);
                        break;
                }
                player->stateFlags2 |= PLAYER_STATE2_23;
                this->actionFunc = func_80ABF4C8;
            } else if ((this->actor.xzDistToPlayer < (50.0f + this->interactRange)) &&
                       (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 40.0f)) {
                this->unk_15A = 0;
                player->stateFlags2 |= PLAYER_STATE2_23;
            }
        }
    }
}

void func_80ABF4C8(EnOkarinaTag* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (play->msgCtx.ocarinaMode == OCARINA_MODE_04) {
        this->actionFunc = func_80ABF28C;
    } else if (play->msgCtx.ocarinaMode == OCARINA_MODE_03) {
        Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
        if (this->switchFlag >= 0) {
            Flags_SetSwitch(play, this->switchFlag);
        }
        switch (this->type) {
            case 1:
                Flags_SetSwitch(play, this->switchFlag);
                SET_EVENTCHKINF(0x39);
                break;
            case 2:
                SetEventChk(EV_OOT_CHK_WINDMILL_FAST);
                SetEventChk(EV_OOT_CHK_WELL_DRAINED);
                Audio_SetMainBgmTempoFreqAfterFanfare(1.18921f, 90);
                break;
            case 4:
                SetEventChk(EV_OOT_CHK_DOOR_TIME);
                break;
            case 6:
                play->csCtx.script = (gSave.age == AGE_ADULT) ? SEGMENTED_TO_VIRTUAL(0x02003c80) : SEGMENTED_TO_VIRTUAL(0x02005020);
                SET_EVENTCHKINF(0x1d);
                Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
                break;
            default:
                break;
        }
        play->msgCtx.ocarinaMode = OCARINA_MODE_04;
        this->actionFunc = func_80ABF28C;
    } else {
        if (play->msgCtx.ocarinaMode >= OCARINA_MODE_05) {
            if (play->msgCtx.ocarinaMode < OCARINA_MODE_0E) {
                play->msgCtx.ocarinaMode = OCARINA_MODE_04;
                this->actionFunc = func_80ABF28C;
                return;
            }
        }
        if (play->msgCtx.ocarinaMode == OCARINA_MODE_01) {
            player->stateFlags2 |= PLAYER_STATE2_23;
        }
    }
}

void EnOkarinaTag_GiveSunSong(EnOkarinaTag* this, PlayState* play)
{
    if (Actor_HasParentZ(&this->actor))
    {
        SetEventChk(EV_OOT_CHK_SONG_SUN);
        Time_SwapDayNight();
        Actor_Kill(&this->actor);
        return;
    }
    comboGiveItemNpc(&this->actor, play, GI_OOT_SONG_SUN, NPC_OOT_ROYAL_TOMB_SONG, 10000.f, 500.f);
}

void func_80ABF708(EnOkarinaTag* this, PlayState* play) {
    if (GetEventChk(EV_OOT_CHK_SONG_SUN))
        Actor_Kill(&this->actor);

    this->actor.messageId = 0x5021;
    if (ActorTalkedTo(&this->actor))
    {
        Message_Close(play);
        this->actionFunc = EnOkarinaTag_GiveSunSong;
        EnOkarinaTag_GiveSunSong(this, play);
    }
    else
    {
        ActorEnableTalk(&this->actor, play, 70.f);
    }
}

void EnOkarinaTag_Update(Actor* thisx, PlayState* play) {
    EnOkarinaTag* this = (EnOkarinaTag*)thisx;
    this->actionFunc(this, play);
}

static Vtx kQuad[] = {
    {{ { -26, 0, -26 }, 0, { 2048, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ { -26, 0, 26 }, 0, { 2048, 2048 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ { 26, 0, 26 }, 0, { 0, 2048 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ { 26, 0, -26 }, 0, { 0, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
};

static const Gfx kDlistSongTag[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(0x06000000, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0, G_TX_MIRROR, G_TX_NOMIRROR, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA_PRIM, G_CC_MODULATEIDECALA_PRIM),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(&kQuad, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSPEndDisplayList(),
};

void EnOkarinaTag_DrawCustom(Actor* actor, PlayState* play)
{
    void* tex;
    u32 color;
    u8 r;
    u8 g;
    u8 b;

    /* Get color */
    color = 0xffff00;
    r = (color >> 16) & 0xff;
    g = (color >>  8) & 0xff;
    b = (color >>  0) & 0xff;

    /* Compute the texture */
    tex = comboCacheGetFile(CUSTOM_SONG_TAG_LULLABY_ADDR);
    if (!tex)
        return;

    /* Prepare the Matrix */
    Matrix_Translate(actor->world.pos.x, actor->world.pos.y + 0.1f, actor->world.pos.z, MTXMODE_NEW);

    /* Draw the display list */
    OPEN_DISPS(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 0x06, (u32)tex);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, 255);
    gSPDisplayList(POLY_OPA_DISP++, (u32)kDlistSongTag);
    CLOSE_DISPS();
}
