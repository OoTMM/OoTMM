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

static u8 kOcarinaActions[] = {
    OCARINA_ACTION_CHECK_LULLABY,
    OCARINA_ACTION_CHECK_EPONA,
    OCARINA_ACTION_CHECK_SARIA,
    OCARINA_ACTION_CHECK_STORMS,
    OCARINA_ACTION_CHECK_SUNS,
    OCARINA_ACTION_CHECK_TIME,
};

static int EnOkarinaTag_OcarinaAction(EnOkarinaTag* this, int vanilla)
{
    if (this->shuffledSongId < 0)
        return vanilla;
    return kOcarinaActions[gComboConfig.songEvents[this->shuffledSongId]];
}

void EnOkarinaTag_Disable(EnOkarinaTag* this)
{
    if (Config_Flag(CFG_OOT_SONG_EVENTS_SHUFFLE)) {
        this->actionFunc = (void*)Actor_Noop;
    } else {
        Actor_Kill(&this->actor);
    }
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
        EnOkarinaTag_Disable(this);
    } else {
        switch (this->type) {
            case 7:
                this->actionFunc = func_80ABEF2C;
                break;
            case 2:
                if (gSave.age== AGE_ADULT) {
                    EnOkarinaTag_Disable(this);
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
                EnOkarinaTag_Disable(this);
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
                    Message_StartOcarina(play, EnOkarinaTag_OcarinaAction(this, ocarinaSong + OCARINA_ACTION_CHECK_SARIA));
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

static void EnOkarinaTag_InhibitSongEffect(PlayState* play)
{
    if (Config_Flag(CFG_OOT_SONG_EVENTS_SHUFFLE)) {
        play->msgCtx.ocarinaAction = OCARINA_ACTION_CHECK_NOWARP_DONE;
        play->msgCtx.disableSunsSong = 1;
    }
}

void func_80ABF0CC(EnOkarinaTag* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (play->msgCtx.ocarinaMode == OCARINA_MODE_04) {
        this->actionFunc = func_80ABEF2C;
    } else {
        if (play->msgCtx.ocarinaMode == OCARINA_MODE_03) {
            EnOkarinaTag_InhibitSongEffect(play);
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
                        Message_StartOcarina(play, EnOkarinaTag_OcarinaAction(this, OCARINA_ACTION_CHECK_LULLABY));
                        break;
                    case 2:
                        Message_StartOcarina(play, EnOkarinaTag_OcarinaAction(this, OCARINA_ACTION_CHECK_STORMS));
                        break;
                    case 4:
                        Message_StartOcarina(play, EnOkarinaTag_OcarinaAction(this, OCARINA_ACTION_CHECK_TIME));
                        break;
                    case 6:
                        Message_StartOcarina(play, EnOkarinaTag_OcarinaAction(this, OCARINA_ACTION_CHECK_LULLABY));
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
        EnOkarinaTag_InhibitSongEffect(play);
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

static Gfx kDlistLoadTextureLullaby[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0, G_TX_MIRROR, G_TX_NOMIRROR, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx kDlistLoadTextureEpona[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx kDlistLoadTextureSaria[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0, G_TX_MIRROR, G_TX_NOMIRROR, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx kDlistLoadTextureStorms[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx kDlistLoadTextureSun[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx kDlistLoadTextureTime[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx kDlistLoadTexturePad[] = {
    gsDPLoadTextureBlock(0, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPEndDisplayList(),
};

static Gfx* kDlistsLoadTextures[] = {
    kDlistLoadTextureLullaby,
    kDlistLoadTextureEpona,
    kDlistLoadTextureSaria,
    kDlistLoadTextureStorms,
    kDlistLoadTextureSun,
    kDlistLoadTextureTime,
};

static const u32 kSongTagColors[] = {
    0xffff00,
    0xff8800,
    0x22ff44,
    0x9000ff,
    0xee2222,
    0x0000ff,
};

static const u32 kSongTagTextureAddrs[] = {
    CUSTOM_SONG_TAG_LULLABY_ADDR,
    CUSTOM_SONG_TAG_DUMMY_ADDR,
    CUSTOM_SONG_TAG_SARIA_ADDR,
    CUSTOM_SONG_TAG_DUMMY_ADDR,
    CUSTOM_SONG_TAG_SUN_ADDR,
    CUSTOM_SONG_TAG_DUMMY_ADDR,
};

static const Gfx kDlistSongTag[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPDisplayList(0x06000000),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA_PRIM, G_CC_PASS2),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_DECAL2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(&kQuad, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSPEndDisplayList(),
};

Actor* EnOkarinaTag_FindShadowShip(PlayState* play)
{
    Actor* actor = play->actorCtx.actors[ACTORCAT_BG].first;

    while (actor != NULL)
    {
        if (actor->id == ACTOR_BG_HAKA_SHIP)
        {
            return actor;
        }
        actor = actor->next;
    }

    return NULL;
}

void EnOkarinaTag_DrawCustom(Actor* thisx, PlayState* play)
{
    EnOkarinaTag* this = (EnOkarinaTag*)thisx;
    int song;
    float padScale;
    Gfx* loader;
    void* tex;
    u32 color;
    u8 r;
    u8 g;
    u8 b;
    Vec3f pos;
    float rot;
    u8 vertical;
    Actor* actor;

    song = gComboConfig.songEvents[this->shuffledSongId];
    loader = kDlistsLoadTextures[song];
    color = kSongTagColors[song];
    r = (color >> 16) & 0xff;
    g = (color >>  8) & 0xff;
    b = (color >>  0) & 0xff;

    /* Compute the texture */
    tex = comboCacheGetFile(kSongTagTextureAddrs[song]);
    if (!tex)
        return;
    loader[0].words.w1 = (u32)tex;

    /* Prepare the Matrix */
    pos.x = thisx->world.pos.x;
    pos.y = thisx->world.pos.y + 0.1f;
    pos.z = thisx->world.pos.z;
    rot = 0.f;
    padScale = 0.f;
    vertical = 0;

    switch (play->sceneId)
    {
    case SCE_OOT_TEMPLE_OF_TIME:
        pos.z += 15.f;
        break;
    case SCE_OOT_GRAVEYARD:
        rot = M_PI_2 * 3;
        padScale = 2.3f;
        break;
    case SCE_OOT_TOMB_DAMPE_WINDMILL:
        pos.z += 250.f;
        break;
    case SCE_OOT_ZORA_RIVER:
        padScale = 1.2f;
        rot = M_PI_2;
        break;
    case SCE_OOT_GREAT_FAIRY_FOUNTAIN_SPELLS:
    case SCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES:
        padScale = 1.2f;
        break;
    case SCE_OOT_BOTTOM_OF_THE_WELL:
        pos.x = 0.f;
        pos.z = -1480.f;
        padScale = 3.f;
        break;
    case SCE_OOT_INSIDE_GANON_CASTLE:
        rot = M_PI_2;
        pos.y = -239.9f;
        padScale = 1.2f;
        break;
    case SCE_OOT_TEMPLE_WATER:
        vertical = 1;
        padScale = 1.2f;
        switch (thisx->room)
        {
        case 0:
            pos.y = 928.f;
            pos.z = 350.f;
            break;
        case 1:
            pos.y = 432.f;
            pos.z = 0.f;
            break;
        case 17:
            pos.y = 859.f;
            pos.z = 340.f;
            break;
        }
        break;
    case SCE_OOT_TEMPLE_SHADOW:
        /* Does't work perfectly, will need to find a solution */
        actor = EnOkarinaTag_FindShadowShip(play);
        this->actor.flags |= ACTOR_FLAG_DRAW_CULLING_DISABLED;
        if (!actor)
            return;
        padScale = 4.f;
        rot = M_PI_2;
        pos.x = actor->world.pos.x - 85.f;
        pos.y = actor->world.pos.y + 150.f;
        pos.z = actor->world.pos.z;
        break;
    case SCE_OOT_TEMPLE_SPIRIT:
        padScale = 1.2f;
        switch (thisx->room)
        {
        case 5:
            rot = M_PI;
            pos.y = 667.f;
            pos.z = -1250.f;
            break;
        case 24:
            pos.z = -800.f;
            break;
        }
        break;
    }

    if (padScale)
    {
        tex = comboCacheGetFile(CUSTOM_SONG_TAG_PAD_ADDR);
        if (!tex)
            return;
        kDlistLoadTexturePad[0].words.w1 = (u32)tex;
    }

    Matrix_Translate(pos.x, pos.y, pos.z, MTXMODE_NEW);
    if (vertical)
        Matrix_RotateX(M_PI_2, MTXMODE_APPLY);
    if (rot)
        Matrix_RotateY(rot, MTXMODE_APPLY);

    /* Draw the display list */
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    if (padScale)
    {
        Matrix_Push();
        Matrix_Translate(0.f, vertical ? 0.f : -0.05f, vertical ? -0.05f : 0.f, MTXMODE_APPLY);
        Matrix_Scale(padScale, 1.f, padScale, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(POLY_XLU_DISP++, 0x06, (u32)kDlistLoadTexturePad);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 200, 200, 200, 255);
        gSPDisplayList(POLY_XLU_DISP++, (u32)kDlistSongTag);
        Matrix_Pop();
    }
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 0x06, (u32)loader);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, r, g, b, 255);
    gSPDisplayList(POLY_XLU_DISP++, (u32)kDlistSongTag);
    CLOSE_DISPS();
}
