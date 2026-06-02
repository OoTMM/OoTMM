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

void Matrix_MultVecZ(f32 z, Vec3f* dest) {
    Vec3f src = { 0, 0, z };
    Matrix_MultVec3f(&src, dest);
}

void Actor_GetSlopeDirection(CollisionPoly* floorPoly, Vec3f* slopeNormal, s16* downwardSlopeYaw) {
    slopeNormal->x = COLPOLY_GET_NORMAL(floorPoly->normal.x);
    slopeNormal->y = COLPOLY_GET_NORMAL(floorPoly->normal.y);
    slopeNormal->z = COLPOLY_GET_NORMAL(floorPoly->normal.z);
    *downwardSlopeYaw = Math_Atan2S(slopeNormal->z, slopeNormal->x);
}

void Math_Vec3s_ToVec3f(Vec3f* dest, Vec3s* src) {
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

void Math_Vec3f_ToVec3s(Vec3s* dest, Vec3f* src) {
    f32 x = src->x;
    f32 y = src->y;
    f32 z = src->z;

    dest->x = x;
    dest->y = y;
    dest->z = z;
}

s32 func_80123590(PlayState* play, Actor* actor) {
    Player* player = GET_PLAYER(play);

    if ((player->stateFlags1 & PLAYER_STATE1_ACTOR_CARRY) && (player->heldActor == actor)) {
        player->interactRangeActor = NULL;
        player->heldActor = NULL;
        player->actor.child = NULL;
        player->stateFlags1 &= ~PLAYER_STATE1_ACTOR_CARRY;
        return 1;
    }

    return 0;
}

void Audio_PlaySfx_AtPosWithChannelIO(Vec3f* pos, u16 sfxId, u8 ioData) {
    AudioSfx_SetChannelIO(pos, sfxId, ioData);
    Audio_PlaySfxGeneral(sfxId, pos, 4, &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
}
