#include "global.h"
#include "terminal.h"
#include "src/overlays/actors/ovl_En_Horse/z_en_horse.h"

s32 func_8006CFC0(s32 sceneId) {
    s32 validSceneIds[] = { SCENE_HYRULE_FIELD, SCENE_LAKE_HYLIA, SCENE_GERUDO_VALLEY, SCENE_GERUDOS_FORTRESS,
                            SCENE_LON_LON_RANCH };
    s32 i;

    for (i = 0; i < ARRAY_COUNT(validSceneIds); i++) {
        if (sceneId == validSceneIds[i]) {
            return 1;
        }
    }

    return 0;
}

void func_8006D074(PlayState* play) {
    gSaveContext.save.info.horseData.sceneId = SCENE_HYRULE_FIELD;
    gSaveContext.save.info.horseData.pos.x = -1840;
    gSaveContext.save.info.horseData.pos.y = 72;
    gSaveContext.save.info.horseData.pos.z = 5497;
    gSaveContext.save.info.horseData.angle = -27353;
}

void func_8006D0AC(PlayState* play) {
    if (gSaveContext.save.info.horseData.sceneId == SCENE_LAKE_HYLIA) {
        gSaveContext.save.info.horseData.sceneId = SCENE_LAKE_HYLIA;
        gSaveContext.save.info.horseData.pos.x = -2065;
        gSaveContext.save.info.horseData.pos.y = -863;
        gSaveContext.save.info.horseData.pos.z = 1839;
        gSaveContext.save.info.horseData.angle = 0;
    }
}

typedef struct HorseSpawn {
    /* 0x00 */ s16 sceneId;
    /* 0x02 */ Vec3s pos;
    /* 0x08 */ s16 angle;
    /* 0x0A */ s16 type;
} HorseSpawn;

void func_8006D0EC(PlayState* play, Player* player) {
    s32 i;
    HorseSpawn horseSpawns[] = {
        { SCENE_HYRULE_FIELD, -460, 100, 6640, 0, 2 },  { SCENE_LAKE_HYLIA, -1929, -1025, 768, 0, 2 },
        { SCENE_GERUDO_VALLEY, 2566, -259, 767, 0, 2 }, { SCENE_GERUDOS_FORTRESS, -328, 10, 953, 0, 2 },
        { SCENE_LON_LON_RANCH, 928, 0, -2280, 0, 2 },
    };

    if ((AREG(6) != 0) && (Flags_GetEventChkInf(EVENTCHKINF_EPONA_OBTAINED) || (DREG(1) != 0))) {
        player->rideActor = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, player->actor.world.pos.x,
                                        player->actor.world.pos.y, player->actor.world.pos.z, player->actor.shape.rot.x,
                                        player->actor.shape.rot.y, player->actor.shape.rot.z, 9);

        ASSERT(player->rideActor != NULL, "player->ride.actor != NULL", "../z_horse.c", 343);

        Actor_MountHorse(play, player, player->rideActor);
        func_8002DE74(play, player);
        gSaveContext.save.info.horseData.sceneId = play->sceneId;

        if (play->sceneId == SCENE_GERUDOS_FORTRESS) {
            player->rideActor->room = -1;
        }
    } else if ((play->sceneId == SCENE_GERUDOS_FORTRESS) && (gSaveContext.minigameState == 3)) {
        Actor* horseActor;
        gSaveContext.minigameState = 0;
        horseActor = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, 3586.0f, 1413.0f, -402.0f, 0, 0x4000, 0, 1);
        horseActor->room = -1;
    } else if ((gSaveContext.save.entranceIndex == ENTR_LON_LON_RANCH_7) &&
               GET_EVENTCHKINF(EVENTCHKINF_EPONA_OBTAINED)) {
        Actor* horseActor =
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, -25.0f, 0.0f, -1600.0f, 0, -0x4000, 0, 1);
        ASSERT(horseActor != NULL, "horse_actor != NULL", "../z_horse.c", 389);
    } else if ((play->sceneId == gSaveContext.save.info.horseData.sceneId) &&
               (Flags_GetEventChkInf(EVENTCHKINF_EPONA_OBTAINED) || DREG(1) != 0)) {
        PRINTF(T("馬存在によるセット %d %d %d\n", "Set by existence of horse %d %d %d\n"),
               gSaveContext.save.info.horseData.sceneId, Flags_GetEventChkInf(EVENTCHKINF_EPONA_OBTAINED), DREG(1));

        if (func_8006CFC0(gSaveContext.save.info.horseData.sceneId)) {
            Actor* horseActor =
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, gSaveContext.save.info.horseData.pos.x,
                            gSaveContext.save.info.horseData.pos.y, gSaveContext.save.info.horseData.pos.z, 0,
                            gSaveContext.save.info.horseData.angle, 0, 1);
            ASSERT(horseActor != NULL, "horse_actor != NULL", "../z_horse.c", 414);
            if (play->sceneId == SCENE_GERUDOS_FORTRESS) {
                horseActor->room = -1;
            }
        } else {
            PRINTF(VT_COL(RED, WHITE));
            PRINTF(
                T("Horse_SetNormal():%d セットスポットまずいです。\n", "Horse_SetNormal():%d set spot is no good.\n"),
                gSaveContext.save.info.horseData.sceneId);
            PRINTF(VT_RST);
            func_8006D074(play);
        }
    } else if ((play->sceneId == SCENE_LON_LON_RANCH) && !Flags_GetEventChkInf(EVENTCHKINF_EPONA_OBTAINED) &&
               (DREG(1) == 0)) {
        Actor* horseActor = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, 0.0f, 0.0f, -500.0f, 0, 0, 0, 1);
        ASSERT(horseActor != NULL, "horse_actor != NULL", "../z_horse.c", 443);
    } else if (Flags_GetEventChkInf(EVENTCHKINF_EPONA_OBTAINED) || (DREG(1) != 0)) {
        for (i = 0; i < ARRAY_COUNT(horseSpawns); i++) {
            HorseSpawn* horseSpawn = &horseSpawns[i];
            if (horseSpawn->sceneId == play->sceneId) {
                Actor* horseActor =
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, horseSpawn->pos.x, horseSpawn->pos.y,
                                horseSpawn->pos.z, 0, horseSpawn->angle, 0, horseSpawn->type);
                ASSERT(horseActor != NULL, "horse_actor != NULL", "../z_horse.c", 466);
                if (play->sceneId == SCENE_GERUDOS_FORTRESS) {
                    horseActor->room = -1;
                }

                break;
            }
        }
    } else if (!Flags_GetEventChkInf(EVENTCHKINF_EPONA_OBTAINED)) {
        if ((DREG(1) == 0) && (play->sceneId == SCENE_LON_LON_BUILDINGS) && !IS_DAY) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, 0.0f, 0.0f, -60.0f, 0, 0x7360, 0, 1);
        }
    }
}

typedef struct struct_8011F9B8 {
    /* 0x00 */ s16 sceneId;
    /* 0x04 */ s32 cutsceneIndex;
    /* 0x08 */ Vec3s pos;
    /* 0x0E */ s16 angle;
    /* 0x10 */ s16 type;
} struct_8011F9B8;

void func_8006D684(PlayState* play, Player* player) {
    static struct_8011F9B8 D_8011F9B8[] = {
        { SCENE_GERUDOS_FORTRESS, 0xFFF0, { 3600, 1413, 360 }, 0x8001, 8 },
        { SCENE_LON_LON_RANCH, 0xFFF0, { -250, 1, -1580 }, 0x4000, 6 },
        { SCENE_LON_LON_RANCH, 0xFFF1, { 0, 0, 0 }, 0x0000, 5 },
        { SCENE_LON_LON_RANCH, 0xFFF5, { 0, 0, 0 }, 0x0000, 7 },
        { SCENE_HYRULE_FIELD, 0xFFF3, { -2961, 313, 7700 }, 0x0000, 7 },
        { SCENE_HYRULE_FIELD, 0xFFF4, { -1900, 313, 7015 }, 0x0000, 7 },
        { SCENE_HYRULE_FIELD, 0xFFF5, { -4043, 313, 6933 }, 0x0000, 7 },
        { SCENE_HYRULE_FIELD, 0xFFF6, { -4043, 313, 6933 }, 0x0000, 7 },
    };
    s32 pad;
    s32 i;
    Vec3s spawnPos;

    if ((gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_11 ||
         gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_12 ||
         gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_13 ||
         gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_15) &&
        (gSaveContext.respawnFlag == 0)) {
        Vec3s spawnPositions[] = {
            { -2961, 313, 7700 },
            { -1900, 313, 7015 },
            { -4043, 313, 6933 },
            { -2313, 313, 5990 },
        };

        if (gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_11) {
            spawnPos = spawnPositions[0];
        } else if (gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_12) {
            spawnPos = spawnPositions[1];
        } else if (gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_13) {
            spawnPos = spawnPositions[2];
        } else {
            spawnPos = spawnPositions[3];
        }

        player->rideActor = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, spawnPos.x, spawnPos.y, spawnPos.z, 0,
                                        player->actor.world.rot.y, 0, 7);
        ASSERT(player->rideActor != NULL, "player->ride.actor != NULL", "../z_horse.c", 561);

        Actor_MountHorse(play, player, player->rideActor);
        func_8002DE74(play, player);
        gSaveContext.save.info.horseData.sceneId = play->sceneId;
    } else if ((play->sceneId == SCENE_LON_LON_RANCH) && (GET_EVENTINF_HORSES_STATE() == EVENTINF_HORSES_STATE_6) &&
               !Flags_GetEventChkInf(EVENTCHKINF_EPONA_OBTAINED) && (DREG(1) == 0)) {
        player->rideActor =
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, 894.0f, 0.0f, -2084.0f, 0, -0x7FFF, 0, 5);
        ASSERT(player->rideActor != NULL, "player->ride.actor != NULL", "../z_horse.c", 582);

        Actor_MountHorse(play, player, player->rideActor);
        func_8002DE74(play, player);
        gSaveContext.save.info.horseData.sceneId = play->sceneId;

        if (play->sceneId == SCENE_GERUDOS_FORTRESS) {
            player->rideActor->room = -1;
        }
    } else {
        for (i = 0; i < ARRAY_COUNT(D_8011F9B8); i++) {
            if ((play->sceneId == D_8011F9B8[i].sceneId) &&
                (((void)0, gSaveContext.save.cutsceneIndex) == D_8011F9B8[i].cutsceneIndex)) {
                if (D_8011F9B8[i].type == 7) {
                    if ((play->sceneId == SCENE_LON_LON_RANCH) &&
                        (((void)0, gSaveContext.save.cutsceneIndex) == 0xFFF1)) {
                        D_8011F9B8[i].pos.x = player->actor.world.pos.x;
                        D_8011F9B8[i].pos.y = player->actor.world.pos.y;
                        D_8011F9B8[i].pos.z = player->actor.world.pos.z;
                    }

                    player->rideActor =
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, D_8011F9B8[i].pos.x, D_8011F9B8[i].pos.y,
                                    D_8011F9B8[i].pos.z, 0, player->actor.world.rot.y, 0, D_8011F9B8[i].type);
                    ASSERT(player->rideActor != NULL, "player->ride.actor != NULL", "../z_horse.c", 628);

                    Actor_MountHorse(play, player, player->rideActor);
                    func_8002DE74(play, player);
                } else if ((D_8011F9B8[i].type == 5) || (D_8011F9B8[i].type == 6) || (D_8011F9B8[i].type == 8)) {
                    Vec3f sp54;
                    s32 temp = 0;

                    if (GET_EVENTINF_HORSES_HORSETYPE() != HORSE_EPONA && D_8011F9B8[i].type == 6) {
                        temp = 0x8000;
                    }

                    player->rideActor =
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, D_8011F9B8[i].pos.x, D_8011F9B8[i].pos.y,
                                    D_8011F9B8[i].pos.z, 0, D_8011F9B8[i].angle, 0, D_8011F9B8[i].type | temp);
                    ASSERT(player->rideActor != NULL, "player->ride.actor != NULL", "../z_horse.c", 667);

                    player->actor.world.pos.x = D_8011F9B8[i].pos.x;
                    player->actor.world.pos.y = D_8011F9B8[i].pos.y;
                    player->actor.world.pos.z = D_8011F9B8[i].pos.z;
                    player->actor.shape.rot.x = player->actor.shape.rot.z = 0;
                    player->actor.shape.rot.y = D_8011F9B8[i].angle;

                    Actor_MountHorse(play, player, player->rideActor);
                    func_8002DE74(play, player);

                    sp54.x = player->actor.world.pos.x - 200.0f;
                    sp54.y = player->actor.world.pos.y + 100.0f;
                    sp54.z = player->actor.world.pos.z;

                    Play_SetCameraAtEye(play, play->activeCamId, &player->actor.world.pos, &sp54);
                } else {
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_HORSE, D_8011F9B8[i].pos.x, D_8011F9B8[i].pos.y,
                                D_8011F9B8[i].pos.z, 0, D_8011F9B8[i].angle, 0, D_8011F9B8[i].type);
                }
                break;
            }
        }
    }
}

void func_8006DC68(PlayState* play, Player* player) {
    if (LINK_IS_ADULT) {
        if (!func_8006CFC0(gSaveContext.save.info.horseData.sceneId)) {
            PRINTF(VT_COL(RED, WHITE));
            PRINTF(
                T("Horse_Set_Check():%d セットスポットまずいです。\n", "Horse_Set_Check():%d set spot is no good.\n"),
                gSaveContext.save.info.horseData.sceneId);
            PRINTF(VT_RST);
            func_8006D074(play);
        }

        if (func_8006CFC0(play->sceneId)) {
            if (IS_CUTSCENE_LAYER ||
                ((gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_11 ||
                  gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_12 ||
                  gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_13 ||
                  gSaveContext.save.entranceIndex == ENTR_HYRULE_FIELD_15) &&
                 (gSaveContext.respawnFlag == 0)) ||
                ((play->sceneId == SCENE_LON_LON_RANCH) && (GET_EVENTINF_HORSES_STATE() == EVENTINF_HORSES_STATE_6) &&
                 !Flags_GetEventChkInf(EVENTCHKINF_EPONA_OBTAINED) && (DREG(1) == 0))) {
                func_8006D684(play, player);
            } else {
                func_8006D0EC(play, player);
            }
        }
    }
}

void func_8006DD9C(Actor* actor, Vec3f* arg1, s16 arg2) {
    s16 x = Math_Vec3f_Yaw(&actor->world.pos, arg1) - actor->world.rot.y;

    if (x > arg2) {
        actor->world.rot.y += arg2;
    } else if (x < -arg2) {
        actor->world.rot.y -= arg2;
    } else {
        actor->world.rot.y += x;
    }

    actor->shape.rot.y = actor->world.rot.y;
}
