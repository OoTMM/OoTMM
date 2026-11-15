static int DeadHand_PlaceHands(Actor* body, PlayState* play)
{
    enum { HAND_COUNT = 4, ATTEMPTS = 96 };
    Vec3f placed[HAND_COUNT];
    int count = 0;

    for (int attempt = 0; attempt < ATTEMPTS && count < HAND_COUNT; ++attempt)
    {
        s16 yaw = (s16)(Rand_ZeroOne() * 65536.f);
        f32 radius = sqrtf(80.f * 80.f + Rand_ZeroOne() * (280.f * 280.f - 80.f * 80.f));
        Vec3f spot = body->home.pos;
        spot.x += radius * Math_SinS(yaw);
        spot.z += radius * Math_CosS(yaw);
        spot.y += 50.f;

        CollisionPoly* poly = NULL;
        s32 bgId;
        f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, body, &spot);
        if (!poly || poly->normal.y < 0x6800 || fabsf(floor - body->home.pos.y) > 35.f)
            continue;
        spot.y = floor;
        int clear = 1;
        for (int i = 0; i < count; ++i)
        {
            f32 dx = spot.x - placed[i].x;
            f32 dz = spot.z - placed[i].z;
            if (dx * dx + dz * dz < 65.f * 65.f)
            {
                clear = 0;
                break;
            }
        }
        if (!clear)
            continue;

        Vec3f from = body->home.pos;
        Vec3f to = spot;
        Vec3f hit;
        from.y += 24.f;
        to.y += 24.f;
        if (BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &poly, 1, 0, 0, 0, &bgId))
            continue;
        for (int direction = 0; direction < 4; ++direction)
        {
            Vec3f edge = to;
            edge.x += direction == 0 ? 25.f : direction == 1 ? -25.f : 0.f;
            edge.z += direction == 2 ? 25.f : direction == 3 ? -25.f : 0.f;
            if (BgCheck_EntityLineTest1(&play->colCtx, &to, &edge, &hit, &poly, 1, 0, 0, 0, &bgId))
            {
                clear = 0;
                break;
            }
            f32 edgeFloor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, body, &edge);
            if (!poly || fabsf(edgeFloor - floor) > 20.f)
            {
                clear = 0;
                break;
            }
        }
        if (!clear)
            continue;
        Actor* hand =
            DEAD_HAND_SPAWN_ARM(&play->actorCtx, body, play, 0xa5, spot.x, spot.y, spot.z, 0, yaw + 0x8000, 0, 0);
        if (!hand)
            break;
        placed[count++] = spot;
    }
    return count;
}
