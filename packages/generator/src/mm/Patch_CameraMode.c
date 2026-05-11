/*#include <combo.h>

#define PLAYER_UNKAA5_3 3
#define PLAYER_STATE1_1000000 (1 << 24)

#define FUNC_800B7118_ADDR      0x800B7118
#define CAMERA_CHANGEMODE_ADDR  0x800DF840

typedef s32 (*Func800B7118Fn)(Player* player);
typedef s32 (*CameraChangeModeFn)(Camera* camera, s32 mode);

#define func_800B7118_addr(player) \
    ((Func800B7118Fn)FUNC_800B7118_ADDR)(player)

#define Camera_ChangeMode_addr(camera, mode) \
    ((CameraChangeModeFn)CAMERA_CHANGEMODE_ADDR)(camera, mode)

typedef struct EnTest3_Patch {
    Player player;
    s16 subCamId;
} EnTest3_Patch;

s32 Player_Patched_CameraModeForAA5(PlayState* play, Player* this) {
    s32 camMode;
    Camera* camera;

    if (this->unk_AA5 == PLAYER_UNKAA5_3) {
        if ((this->transformation == MM_PLAYER_FORM_HUMAN) &&
            (this->stateFlags1 & PLAYER_STATE1_1000000)) {
            camMode = CAM_MODE_BOOMERANG;
        } else if (func_800B7118_addr(this)) {
            if (this->transformation == MM_PLAYER_FORM_HUMAN) {
                camMode = CAM_MODE_SLINGSHOT;
            } else if (this->transformation == MM_PLAYER_FORM_DEKU) {
                camMode = CAM_MODE_DEKUSHOOT;
            } else {
                camMode = CAM_MODE_BOWARROW;
            }
        } else {
            camMode = CAM_MODE_ZORAFIN;
        }
    } else {
        camMode = CAM_MODE_FIRSTPERSON;
    }

    camera = (this->actor.id == ACTOR_PLAYER)
                 ? Play_GetCamera(play, CAM_ID_MAIN)
                 : Play_GetCamera(play, ((EnTest3_Patch*)this)->subCamId);

    return Camera_ChangeMode_addr(camera, camMode);
}

PATCH_FUNC(0x8083868C, Player_Patched_CameraModeForAA5);*/