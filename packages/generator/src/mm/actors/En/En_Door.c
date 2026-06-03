#include <combo.h>

void EnDoor_DrawLock(PlayState* play, Actor* this)
{
    u8 timer;
    s32 yaw;

    /* Fix rotation */
    yaw = Math_Vec3f_Yaw(&play->view.eye, &this->world.pos);
    if (ABS((s16)(this->shape.rot.y - yaw)) < 0x4000) {
        Matrix_RotateY(M_PI, MTXMODE_APPLY);
    }

    timer = *(u8*)((char*)this + 0x1a6);
    if (timer) {
        Actor_DrawDoorLock(play, timer, 0);
    } else {
        Door_DrawRustyLock(play);
    }
}
