#include <combo.h>

/* MM compatibility */
Vec3f gZeroVec3f = { 0.0f, 0.0f, 0.0f };

void DynaPolyActor_LoadMesh(PlayState* play, DynaPolyActor* dynaActor, CollisionHeader* meshHeader)
{
    CollisionHeader* header = NULL;

    CollisionHeader_GetVirtual(meshHeader, &header);
    dynaActor->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &dynaActor->actor, header);
}
