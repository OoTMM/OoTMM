#include <combo.h>
#include <combo/custom.h>

void Door_DrawRustyLock(PlayState* play)
{
    void* obj;
    s32 i;
    MtxF baseMtxF;
    f32 chainRotZ;
    f32 rotZStep;

    obj = comboGetObject(CUSTOM_OBJECT_ID_DOOR_LOCK);
    if (!obj)
        return;

    chainRotZ = 0.f;
    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x06, ((u32)obj & 0x00ffffff));
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 140, 64, 255);
    Matrix_Translate(0.0f, 5000.f, 500.0f, MTXMODE_APPLY);
    Matrix_Get(&baseMtxF);

    for (i = 0; i < 4; i++) {

        Matrix_Put(&baseMtxF);
        Matrix_RotateZ(chainRotZ, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_DOOR_LOCK_1);

        if (i % 2) {
            rotZStep = 2.0f *  0.54f;
        } else {
            rotZStep = M_PI - (2.0f *  0.54f);
        }

        chainRotZ += rotZStep;
    }

    Matrix_Put(&baseMtxF);
    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_DOOR_LOCK_0);

    CLOSE_DISPS();
}
