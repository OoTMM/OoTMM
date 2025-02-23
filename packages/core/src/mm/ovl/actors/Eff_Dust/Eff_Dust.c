#include "Eff_Dust.h"
#include <assets/mm/objects/gameplay_keep.h>

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_MM_20)

void EffDust_Init(EffDust* this, PlayState* play);
void EffDust_Destroy(EffDust* this, PlayState* play);
void EffDust_Update(EffDust* this, PlayState* play);
void EffDust_Draw(EffDust* this, PlayState* play);

void func_80918D64(EffDust* this, PlayState* play);
void func_80918FE4(EffDust* this, PlayState* play);
void func_80919230(EffDust* this, PlayState* play);

void func_80919768(EffDust* this, PlayState* play2);
void func_809199FC(EffDust* this, PlayState* play2);

void func_80918B40(EffDust* this) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(this->distanceTraveled); i++) {
        this->initialPositions[i].z = 0.0f;
        this->initialPositions[i].y = 0.0f;
        this->initialPositions[i].x = 0.0f;

        this->distanceTraveled[i] = 1.0f;
    }
    this->index = 0;
}

void EffDust_Init(EffDust* this, PlayState* play) {
    u32 type = this->actor.params;

    func_80918B40(this);
    switch (type) {
        case EFF_DUST_TYPE_0:
            this->actionFunc = func_80918D64;
            this->actor.draw = func_80919768;
            this->dx = 1.0f;
            this->dy = 1.0f;
            this->dz = 1.0f;
            this->scalingFactor = 0.2f;
            break;

        case EFF_DUST_TYPE_1:
            this->actionFunc = func_80918FE4;
            this->actor.draw = func_80919768;
            this->dy = 1.0f;
            this->dx = 0.8f;
            this->dz = 0.8f;
            this->scalingFactor = 0.5f;
            break;

        case EFF_DUST_TYPE_SPIN_ATTACK_CHARGE:
        case EFF_DUST_TYPE_3:
            this->actionFunc = func_80919230;
            this->actor.draw = func_809199FC;
            this->actor.room = -1;
            this->dx = 0.5f;
            this->scalingFactor = 15.0f;
            break;

        case EFF_DUST_TYPE_4:
            this->actionFunc = func_80919230;
            this->actor.draw = func_809199FC;
            this->actor.room = -1;
            this->dx = 0.5f;
            this->scalingFactor = 10.0f;
            break;

        case EFF_DUST_TYPE_5:
            this->actionFunc = func_80919230;
            this->actor.draw = func_809199FC;
            this->actor.room = -1;
            this->dx = 0.5f;
            this->scalingFactor = 20.0f;
            break;

        default:
            free(this);
            break;
    }
    this->life = 10;
}

void EffDust_Destroy(EffDust* this, PlayState* play) {
}

void func_80918D64(EffDust* this, PlayState* play) {
    s16 theta;
    s16 phi;
    s32 i;
    s32 j;
    f32* distanceTraveled = this->distanceTraveled;

    for (i = 0; i < ARRAY_COUNT(this->distanceTraveled); i++) {
        if (*distanceTraveled < 1.0f) {
            *distanceTraveled += 0.05f;
        }
        distanceTraveled++;
    }

    if (this->actor.home.rot.z != 0) {
        for (j = 0; j < 3; j++) {
            i = this->index & 0x3F;
            if (this->distanceTraveled[i] >= 1.0f) {
                // Spherical coordinate system.
                phi = Rand_CenteredFloat(0x10000);
                theta = Rand_ZeroFloat(0x1000);
                this->initialPositions[i].x = -(f32)this->actor.home.rot.z * Math_CosS(phi) * Math_CosS(theta);
                this->initialPositions[i].y = -(f32)this->actor.home.rot.z * Math_SinS(phi) * Math_CosS(theta);
                this->initialPositions[i].z = -(f32)this->actor.home.rot.z * Math_SinS(theta);
                this->distanceTraveled[i] = 0.0f;
                this->index++;
            }
        }
    }
}

void func_80918FE4(EffDust* this, PlayState* play) {
    s16 theta;
    s16 phi;
    f32* distanceTraveled = this->distanceTraveled;
    s32 i;
    s32 j;

    for (i = 0; i < ARRAY_COUNT(this->distanceTraveled); i++) {
        if (*distanceTraveled < 1.0f) {
            *distanceTraveled += 0.03f;
        }
        distanceTraveled++;
    }

    for (j = 0; j < 2; j++) {
        i = this->index & 0x3F;
        if (this->distanceTraveled[i] >= 1.0f) {
            // Spherical coordinate system.
            phi = Rand_CenteredFloat(0x10000);
            theta = Rand_ZeroFloat(0x2000);
            this->initialPositions[i].x = 400.0f * Math_CosS(phi) * Math_CosS(theta);
            this->initialPositions[i].y = 400.0f * Math_SinS(theta);
            this->initialPositions[i].z = 400.0f * Math_SinS(phi) * Math_CosS(theta);
            this->distanceTraveled[i] = 0.0f;
            this->index++;
        }
    }
}

void func_80919230(EffDust* this, PlayState* play) {
    s16 theta;
    Player* player = GET_PLAYER(play);
    Actor* parent = this->actor.parent;
    f32* distanceTraveled = this->distanceTraveled;
    s32 i;
    s32 j;

    if ((parent == NULL) || (parent->update == NULL) || !(player->stateFlags1 & PLAYER_STATE1_MM_1000)) {
        if (this->life != 0) {
            this->life--;
        } else {
            Actor_Kill(&this->actor);
        }

        for (i = 0; i < ARRAY_COUNT(this->distanceTraveled); i++) {
            if (*distanceTraveled < 1.0f) {
                *distanceTraveled += 0.2f;
            }
            distanceTraveled++;
        }

        return;
    }

    for (i = 0; i < ARRAY_COUNT(this->distanceTraveled); i++) {
        if (*distanceTraveled < 1.0f) {
            *distanceTraveled += 0.1f;
        }
        distanceTraveled++;
    }

    this->actor.world.pos = parent->world.pos;

    for (j = 0; j < 3; j++) {
        i = this->index & 0x3F;

        if (this->distanceTraveled[i] >= 1.0f) {
            theta = Rand_CenteredFloat(0x10000);
            switch (this->actor.params) {
                case EFF_DUST_TYPE_SPIN_ATTACK_CHARGE:
                    this->initialPositions[i].x = (Rand_ZeroOne() * 4500.0f) + 700.0f;
                    if (this->initialPositions[i].x > 3000.0f) {
                        this->initialPositions[i].y = (3000.0f * Rand_ZeroOne()) * Math_SinS(theta);
                        this->initialPositions[i].z = (3000.0f * Rand_ZeroOne()) * Math_CosS(theta);
                    } else {
                        this->initialPositions[i].y = 3000.0f * Math_SinS(theta);
                        this->initialPositions[i].z = 3000.0f * Math_CosS(theta);
                    }
                    break;

                case EFF_DUST_TYPE_3:
                    this->initialPositions[i].x = (Rand_ZeroOne() * 2500.0f) + 700.0f;
                    if (this->initialPositions[i].x > 2000.0f) {
                        this->initialPositions[i].y = (2000.0f * Rand_ZeroOne()) * Math_SinS(theta);
                        this->initialPositions[i].z = (2000.0f * Rand_ZeroOne()) * Math_CosS(theta);
                    } else {
                        this->initialPositions[i].y = 2000.0f * Math_SinS(theta);
                        this->initialPositions[i].z = 2000.0f * Math_CosS(theta);
                    }
                    break;

                case EFF_DUST_TYPE_4:
                    this->initialPositions[i].x = (Rand_ZeroOne() * 8500.0f) + 1700.0f;
                    if (this->initialPositions[i].x > 5000.0f) {
                        this->initialPositions[i].y = (4000.0f * Rand_ZeroOne()) * Math_SinS(theta);
                        this->initialPositions[i].z = (4000.0f * Rand_ZeroOne()) * Math_CosS(theta);
                    } else {
                        this->initialPositions[i].y = 4000.0f * Math_SinS(theta);
                        this->initialPositions[i].z = 4000.0f * Math_CosS(theta);
                    }
                    break;

                default:
                    break;
            }

            this->distanceTraveled[i] = 0.0f;
            this->index++;
        }
    }
}

void EffDust_Update(EffDust* this, PlayState* play) {
    this->actionFunc(this, play);
}

Gfx D_80919DB0[] = {
    gsSPEndDisplayList(),
};

void func_80919768(EffDust* this, PlayState* play) {
    f32* distanceTraveled;
    Vec3f* initialPositions;
    s32 i;
    f32 aux;
    s16 sp92;
    Vec3f activeCamEye;

    activeCamEye = GET_ACTIVE_CAM(play)->eye;
    sp92 = Math_Vec3f_Yaw(&activeCamEye, &this->actor.world.pos);

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gDPPipeSync(POLY_XLU_DISP++);

    initialPositions = this->initialPositions;
    distanceTraveled = this->distanceTraveled;

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 255, 0);

    gSPSegment(POLY_XLU_DISP++, 0x08, D_80919DB0);

    for (i = 0; i < ARRAY_COUNT(this->distanceTraveled); i++) {
        if (*distanceTraveled < 1.0f) {
            aux = 1.0f - SQ(*distanceTraveled);
            Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
            Matrix_RotateYS(sp92, MTXMODE_APPLY);
            Matrix_Translate(initialPositions->x * ((this->dx * aux) + (1.0f - this->dx)),
                             initialPositions->y * ((this->dy * aux) + (1.0f - this->dy)),
                             initialPositions->z * ((this->dz * aux) + (1.0f - this->dz)), MTXMODE_APPLY);
            Matrix_Scale(this->scalingFactor, this->scalingFactor, this->scalingFactor, MTXMODE_APPLY);

            Matrix_ReplaceRotation(&play->billboardMtxF);

            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);

            gSPClearGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);

            gSPDisplayList(POLY_XLU_DISP++, gEffSparklesDL);

            gSPSetGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);
        }
        initialPositions++;
        distanceTraveled++;
    }

    CLOSE_DISPS();
}

void func_809199FC(EffDust* this, PlayState* play) {
    f32* distanceTraveled;
    Vec3f* initialPositions;
    s32 i;
    f32 aux;
    Player* player = GET_PLAYER(play);

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gDPPipeSync(POLY_XLU_DISP++);

    initialPositions = this->initialPositions;
    distanceTraveled = this->distanceTraveled;

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
    if (player->unk_B08 >= 0.85f) {
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
    } else {
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 0);
    }

    gSPSegment(POLY_XLU_DISP++, 0x08, D_80919DB0);

    for (i = 0; i < ARRAY_COUNT(this->distanceTraveled); i++) {
        if (*distanceTraveled < 1.0f) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (*distanceTraveled * 255.0f));

            aux = 1.0f - SQ(*distanceTraveled);
            Matrix_Mult(&player->leftHandMf, MTXMODE_NEW);
            Matrix_Translate(initialPositions->x * ((this->dx * aux) + (1.0f - this->dx)),
                             (initialPositions->y * (1.0f - *distanceTraveled)) + 320.0f,
                             (initialPositions->z * (1.0f - *distanceTraveled)) + -20.0f, MTXMODE_APPLY);

            Matrix_Scale(*distanceTraveled * this->scalingFactor, *distanceTraveled * this->scalingFactor,
                         *distanceTraveled * this->scalingFactor, MTXMODE_APPLY);

            Matrix_ReplaceRotation(&play->billboardMtxF);

            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
            gSPClearGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);

            gSPDisplayList(POLY_XLU_DISP++, gEffSparklesDL);
            gSPSetGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);
        }

        initialPositions++;
        distanceTraveled++;
    }

    CLOSE_DISPS();
}

void EffDust_Draw(EffDust* this, PlayState* play) {
    this->drawFunc(this, play);
}

ActorProfile EffDust_InitVars = {
    ACTOR_EFF_DUST,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EffDust),
    (ActorFunc)EffDust_Init,
    (ActorFunc)EffDust_Destroy,
    (ActorFunc)EffDust_Update,
    (ActorFunc)EffDust_Draw,
};

OVL_INFO_ACTOR(ACTOR_EFF_DUST, EffDust_InitVars);

