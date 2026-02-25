#include <combo.h>

/* MM compatibility */
Vec3f gZeroVec3f = { 0.0f, 0.0f, 0.0f };

void DynaPolyActor_LoadMesh(PlayState* play, DynaPolyActor* dynaActor, CollisionHeader* meshHeader)
{
    CollisionHeader* header = NULL;

    CollisionHeader_GetVirtual(meshHeader, &header);
    dynaActor->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &dynaActor->actor, header);
}

void Matrix_RotateXS(s16 angle, MatrixMode mode)
{
    Matrix_RotateXF(angle * (M_PI / 32768.f), mode);
}

void Matrix_RotateYS(s16 angle, MatrixMode mode)
{
    Matrix_RotateYF(angle * (M_PI / 32768.f), mode);
}

void Matrix_RotateZS(s16 angle, MatrixMode mode)
{
    Matrix_RotateZF(angle * (M_PI / 32768.f), mode);
}
