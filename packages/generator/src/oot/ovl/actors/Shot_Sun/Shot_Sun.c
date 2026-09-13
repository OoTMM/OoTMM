#include "Shot_Sun.h"

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY)

void ShotSun_Init(Actor* thisx, PlayState* play);
void ShotSun_Destroy(Actor* thisx, PlayState* play);
void ShotSun_Update(Actor* thisx, PlayState* play);

void ShotSun_SpawnFairy(ShotSun* this, PlayState* play);
void ShotSun_TriggerFairy(ShotSun* this, PlayState* play);
void ShotSun_UpdateFairySpawner(ShotSun* this, PlayState* play);
void ShotSun_UpdateHyliaSun(ShotSun* this, PlayState* play);

ActorProfile Shot_Sun_Profile = {
    /**/ ACTOR_SHOT_SUN,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(ShotSun),
    /**/ ShotSun_Init,
    /**/ ShotSun_Destroy,
    /**/ ShotSun_Update,
    /**/ NULL,
};

OVL_INFO_ACTOR(ACTOR_SHOT_SUN, Shot_Sun_Profile);

typedef enum FairySpawnerState {
    /* 0 */ SPAWNER_OUT_OF_RANGE,
    /* 1 */ SPAWNER_OCARINA_START,
    /* 2 */ SPAWNER_OCARINA_PLAYING
} FairySpawnerState;

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0x00000020, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 30, 60, 0, { 0, 0, 0 } },
};

void ShotSun_Init(Actor* thisx, PlayState* play) {
    ShotSun* this = (ShotSun*)thisx;
    s32 params;

    this->xflag = Xflag_InitEx(&this->actor, play);
    if (!Xflag_IsValidEx(this->xflag))
        this->xflag = XFLAGID_NONE;

    params = PARAMS_GET_U(this->actor.params, 0, 8);
    if (params == 0x40 || params == 0x41) {
        this->fairySpawnerState = SPAWNER_OUT_OF_RANGE;
        this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        this->actor.flags |= ACTOR_FLAG_UPDATE_DURING_OCARINA;
        this->actionFunc = ShotSun_UpdateFairySpawner;
        this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    } else {
        Collider_InitCylinder(play, &this->collider);
        Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
        this->actionFunc = ShotSun_UpdateHyliaSun;
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    }
}

void ShotSun_Destroy(Actor* thisx, PlayState* play) {
    ShotSun* this = (ShotSun*)thisx;
    s32 params = PARAMS_GET_U(this->actor.params, 0, 8);

    if (params != 0x40 && params != 0x41) {
        Collider_DestroyCylinder(play, &this->collider);
    }
}

void ShotSun_SpawnFairy(ShotSun* this, PlayState* play) {
    g.xflagId = this->xflag;
    g.xflagOverride = TRUE;
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, this->actor.home.pos.x, this->actor.home.pos.y,
                this->actor.home.pos.z, 0, 0, 0, /* FAIRY_HEAL_BIG */ 0x07);
    g.xflagOverride = FALSE;
    Actor_Kill(&this->actor);
}

void ShotSun_TriggerFairy(ShotSun* this, PlayState* play) {
    this->actionFunc = ShotSun_SpawnFairy;
    this->timer = 50;

    Actor_Spawn(&play->actorCtx, play, ACTOR_DEMO_KANKYO, this->actor.home.pos.x, this->actor.home.pos.y,
                this->actor.home.pos.z, 0, 0, 0, /* DEMOKANKYO_SPARKLES */ 0x11);

    Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_EV_TRE_BOX_APPEAR);
}

void ShotSun_UpdateFairySpawner(ShotSun* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 params = PARAMS_GET_U(this->actor.params, 0, 8);

    if (Math3D_Vec3fDistSq(&this->actor.world.pos, &player->actor.world.pos) > SQ(150.0f)) {
        this->fairySpawnerState = SPAWNER_OUT_OF_RANGE;
    } else {
        if (this->fairySpawnerState == SPAWNER_OUT_OF_RANGE) {
            if (!(player->stateFlags2 & PLAYER_STATE2_24)) {
                player->stateFlags2 |= PLAYER_STATE2_23;
                return;
            } else {
                this->fairySpawnerState = SPAWNER_OCARINA_START;
            }
        }
        if (this->fairySpawnerState == SPAWNER_OCARINA_START) {
            Message_StartOcarina(play, OCARINA_ACTION_FREE_PLAY);
            this->fairySpawnerState = SPAWNER_OCARINA_PLAYING;
        } else if (this->fairySpawnerState == SPAWNER_OCARINA_PLAYING && play->msgCtx.ocarinaMode == OCARINA_MODE_04) {
            if ((params == 0x40 && play->msgCtx.lastPlayedSong == OCARINA_SONG_SUNS) ||
                (params == 0x41 && play->msgCtx.lastPlayedSong == OCARINA_SONG_STORMS)) {
                this->actionFunc = ShotSun_TriggerFairy;
                this->timer = 0;
            } else {
                this->fairySpawnerState = SPAWNER_OUT_OF_RANGE;
            }
            this->fairySpawnerState = SPAWNER_OUT_OF_RANGE;
        }
    }
}

void ShotSun_UpdateHyliaSun(ShotSun* this, PlayState* play) {
    Vec3s cylinderPos;
    Player* player = GET_PLAYER(play);
    Actor_EnItem00* collectible;

    if (this->collider.base.acFlags & AC_HIT) {
        Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
        if (!gOotExtraFlags.fireArrow) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_ITEM_ETCETERA, 700.0f, -800.0f, 7261.0f, 0, 0, 0, 7);
            //play->csCtx.script = SEGMENTED_TO_VIRTUAL(gLakeHyliaFireArrowsCS);
            //if (1) {}
            //gSaveContext.cutsceneTrigger = 1;
        } else {
            Vec3f spawnPos;

            spawnPos.x = 700.0f;
            spawnPos.y = -800.0f;
            spawnPos.z = 7261.0f;

            collectible = (Actor_EnItem00*)Item_DropCollectible(play, &spawnPos, ITEM00_MAGIC_LARGE);
            if (collectible != NULL) {
                collectible->timer = 6000;
                collectible->actor.speed = 0.0f;
            }
        }
        Actor_Kill(&this->actor);
    } else {
        if (!(this->actor.xzDistToPlayer > 120.0f) && gSaveContext.save.dayTime >= CLOCK_TIME(6, 30) &&
            gSaveContext.save.dayTime < CLOCK_TIME(7, 30)) {
            cylinderPos.x = player->bodyPartsPos[PLAYER_BODYPART_HEAD].x + play->envCtx.sunPos.x * (1.0f / 6.0f);
            cylinderPos.y =
                player->bodyPartsPos[PLAYER_BODYPART_HEAD].y - 30.0f + play->envCtx.sunPos.y * (1.0f / 6.0f);
            cylinderPos.z = player->bodyPartsPos[PLAYER_BODYPART_HEAD].z + play->envCtx.sunPos.z * (1.0f / 6.0f);

            this->hitboxPos = cylinderPos;

            Collider_SetCylinderPosition(&this->collider, &cylinderPos);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

void ShotSun_Update(Actor* thisx, PlayState* play) {
    ShotSun* this = (ShotSun*)thisx;
    this->actionFunc(this, play);
}
