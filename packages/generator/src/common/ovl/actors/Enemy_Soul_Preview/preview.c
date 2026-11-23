#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/config.h>
#include <combo/dma.h>
#include <combo/souls.h>
#include <combo/enemy_keep.h>
#include <combo/enemy_room_models.h>
#include <combo/enemy_soul_preview.h>

#if defined(GAME_OOT)
#define PreviewFree ((void (*)(void*))0x80066c90)
#define PreviewToMtx ((Mtx* (*)(Mtx*))0x800ab8d8)
#else
#define PreviewFree ((void (*)(void*))0x80102ce0)
#define PreviewToMtx ((Mtx* (*)(Mtx*))0x80181a18)
#endif
typedef struct PreviewMesh {
    struct PreviewMesh* next;
    u32 vrom, size, refs, pinnedUntil;
    u32 padding[3];
    u8 data[];
} PreviewMesh;
typedef struct Preview {
    struct Preview* next;
    PreviewMesh* mesh;
    Vec3f position;
    s16 id, yaw, soul;
    u16 params;
    s8 room;
    u8 sourceGame, soulGame;
} Preview;
typedef struct {
    u16 game, id;
    u32 vrom, size;
} PreviewEntry;
static u32 sPreviewTable;
static Preview* sPreviews;
static PreviewMesh* sMeshes;
static u16 sCount;
static int previewEntry(int game, s16 id, PreviewEntry* entry)
{
    DmaEntry file;
    u32 header[2];
    if (!sPreviewTable || !comboDmaLookup(&file, sPreviewTable) || file.pend || file.vstart != sPreviewTable ||
        file.vend - file.vstart < 8)
        return 0;
    LoadFile(header, sPreviewTable, sizeof(header));
    if (header[0] != 0x53505431 || header[1] > (file.vend - file.vstart - 8) / sizeof(*entry))
        return 0;
    u32 lo = 0, hi = header[1];
    while (lo < hi)
    {
        u32 mid = lo + (hi - lo) / 2;

        LoadFile(entry, sPreviewTable + 8 + mid * sizeof(*entry), sizeof(*entry));
        if (entry->game == game && entry->id == id)
            return 1;
        if (entry->game < game || (entry->game == game && entry->id < id))
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

static int previewRead(void* dst, u32 vrom, u32 size)
{
    DmaEntry file;
    if (!comboDmaLookup(&file, vrom) || file.pend || size > file.vend - vrom)
        return 0;
    LoadFile(dst, vrom, size);
    return 1;
}

static int previewReadMesh(u8* dst, const PreviewEntry* entry)
{
    if (!(entry->vrom & 1))
        return previewRead(dst, entry->vrom, entry->size);

    u32 cursor = entry->vrom & ~1u, remaining = entry->size;
    while (remaining)
    {
        u32 part[2];
        if (!previewRead(part, cursor, sizeof(part)) || part[1] - 1 >= remaining)
            return 0;
#if defined(GAME_MM)
        if (part[0] < 0x08000000)
            part[0] ^= 0x04000000;
#endif
        if (!previewRead(dst, part[0], part[1]))
            return 0;
        cursor += sizeof(part);
        dst += part[1];
        remaining -= part[1];
    }
    return 1;
}

static PreviewMesh* previewMesh(const PreviewEntry* entry)
{
    for (PreviewMesh* mesh = sMeshes; mesh; mesh = mesh->next)
        if (mesh->vrom == entry->vrom)
            return mesh;
    if (entry->size < 104 || entry->size > 0x10000)
        return NULL;
    PreviewMesh* mesh = comboEnemyArenaAlloc(sizeof(*mesh) + entry->size);
    if (!mesh)
        return NULL;
    if (!previewReadMesh(mesh->data, entry) || *(u32*)mesh->data != 0x53504d32 ||
        ((u32*)mesh->data)[1] >= entry->size - 7 || !((u16*)mesh->data)[7] || ((u16*)mesh->data)[7] > 128 ||
        32 + ((u16*)mesh->data)[7] * 64 > ((u32*)mesh->data)[1] ||
        (((u32*)mesh->data)[7] & 0xffffff) >= entry->size - 7)
    {
        PreviewFree(mesh);
        return NULL;
    }
    mesh->vrom = entry->vrom;
    mesh->size = entry->size;
    mesh->refs = 0;
    mesh->pinnedUntil = 0;
    mesh->next = sMeshes;
    sMeshes = mesh;
    return mesh;
}

static u16 previewObjectId(PreviewMesh* mesh)
{
    u16 id = ((u16*)mesh->data)[6];
#if defined(GAME_OOT)
    const int host = 0;
#else
    const int host = 1;
#endif
    if (id && !(id & 0x3000) && ((u32*)mesh->data)[6] != host)
        id |= 0x1000;
    return id;
}

static int previewBlocked(EnemySoulPreviewRequest* request)
{
    for (Preview* p = sPreviews; p; p = p->next)
        if (p->room == request->play->roomCtx.curRoom.num && p->sourceGame == request->sourceGame &&
            p->id == request->id && p->params == request->params && p->position.x == request->x &&
            p->position.y == request->y && p->position.z == request->z)
            return 1;
    return 0;
}

static int previewRecord(EnemySoulPreviewRequest* request)
{
    PlayState* play = request->play;
    int game = request->sourceGame;
    s16 id = request->id, yaw = request->yaw;
    u16 params = request->params;
    f32 x = request->x, y = request->y, z = request->z;
    sPreviewTable = request->table;
    int room = play->roomCtx.curRoom.num;
    if (previewBlocked(request))
        return 1;
    if (sCount >= 255)
        return 0;
    PreviewEntry entry;
    if (!previewEntry(game, id, &entry))
        return 0;
    Preview* p = comboEnemyArenaAlloc(sizeof(*p));
    if (!p)
        return 0;
    PreviewMesh* mesh = previewMesh(&entry);
    if (!mesh)
    {
        PreviewFree(p);
        return 0;
    }
    u16 objectId = previewObjectId(mesh);
    if (objectId && !comboEnemyObjectPreload(objectId, room))
    {
        PreviewFree(p);
        return 0;
    }
    p->mesh = mesh;
    ++mesh->refs;
    p->room = room;
    p->id = id;
    p->params = params;
    p->sourceGame = game;
    p->soulGame = request->soulGame;
    p->soul = request->soul;
    p->yaw = yaw;
    p->position = (Vec3f){ x, y, z };
    p->next = sPreviews;
    sPreviews = p;
    ++sCount;
    return 1;
}

static void previewReset(void)
{
    while (sPreviews)
    {
        Preview* p = sPreviews;
        sPreviews = p->next;
        PreviewFree(p);
    }
    while (sMeshes)
    {
        PreviewMesh* p = sMeshes;
        sMeshes = p->next;
        PreviewFree(p);
    }
    sCount = 0;
}

static void previewDraw(PlayState* play)
{
    if (!Config_Flag(CFG_SOULS_ENEMY_LENS))
        return;
#if defined(GAME_OOT)
    int previous = play->roomCtx.prevRoom.num;
#else
    int previous = play->roomCtx.prefRoom.num;
#endif
    Preview** at = &sPreviews;
    while (*at)
    {
        Preview* p = *at;
        if (p->room >= 0 && p->room != play->roomCtx.curRoom.num && p->room != previous)
        {
            *at = p->next;
            --p->mesh->refs;
            PreviewFree(p);
            --sCount;
        }
        else
            at = &p->next;
    }

    PreviewMesh** meshAt = &sMeshes;
    while (*meshAt)
    {
        PreviewMesh* mesh = *meshAt;
        if (!mesh->refs && (s32)(play->gameplayFrames - mesh->pinnedUntil) >= 0)
        {
            *meshAt = mesh->next;
            PreviewFree(mesh);
        }
        else
            meshAt = &mesh->next;
    }
    if (Config_Flag(CFG_SOULS_ENEMY_LENS_REQUIRED) && !play->actorCtx.lensActive)
        return;
    OPEN_DISPS(play->state.gfxCtx);
    for (Preview* p = sPreviews; p; p = p->next)
    {
        if (p->soulGame ? comboHasSoulMm(p->soul) : comboHasSoulOot(p->soul))
            continue;

        f32 dx = p->position.x - play->view.eye.x, dy = p->position.y - play->view.eye.y,
            dz = p->position.z - play->view.eye.z;
        if (dx * dx + dy * dy + dz * dz > 25000000.f)
            continue;
        u16 objectId = previewObjectId(p->mesh);
        void* object = NULL;
        if (objectId)
        {
            int slot = Object_GetSlot(&play->objectCtx, objectId);
            object = comboObjectSlotSegmentAndPin(&play->objectCtx, slot);
            if (!object)
                continue;
        }
        u8* data = p->mesh->data;
        int count = ((u16*)data)[7];
        if ((u8*)play->state.gfxCtx->polyXlu.end - (u8*)play->state.gfxCtx->polyXlu.append < 4096)
            continue;
        if ((u8*)play->state.gfxCtx->polyOpa.end - (u8*)play->state.gfxCtx->polyOpa.append < count * sizeof(Mtx) + 4096)
            continue;
        Mtx* palette = (Mtx*)GRAPH_ALLOC(play->state.gfxCtx, count * sizeof(Mtx));
        if (!palette)
            continue;
        p->mesh->pinnedUntil = play->gameplayFrames + 3;
        f32 scale = ((f32*)data)[4];
        Matrix_Translate(p->position.x, p->position.y + 2.f - ((f32*)data)[5] * scale, p->position.z, MTXMODE_NEW);

        if (((u32*)data)[7] & 0x80000000u)
            Matrix_ReplaceRotation(&play->billboardMtxF);
        else
            Matrix_RotateY(p->yaw * (3.14159265358979323846f / 32768.f), MTXMODE_APPLY);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        for (int i = 0; i < count; ++i)
        {
            Matrix_Push();
            Matrix_Mult((MtxF*)(data + 32 + i * 64), MTXMODE_APPLY);
            PreviewToMtx(&palette[i]);
            Matrix_Pop();
        }
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
        gDPPipeSync(POLY_XLU_DISP++);
        gSPClearGeometryMode(POLY_XLU_DISP++, G_FOG);
        gDPSetCycleType(POLY_XLU_DISP++, G_CYC_2CYCLE);
        gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_AA_ZB_XLU_SURF2);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, 220);
        gDPSetCombineLERP(POLY_XLU_DISP++, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, 0, 0, 0, COMBINED, COMBINED, 0,
                          ENVIRONMENT, 0);
        gSPSegment(POLY_XLU_DISP++, 6, object);
        gSPSegment(POLY_XLU_DISP++, 11, data);
        gSPSegment(POLY_XLU_DISP++, 13, palette);
        u32 displayList = ((u32*)data)[1];

        if ((p->params & 0xff) == 1 && (((u32*)data)[7] & 0xffffff))
            displayList = ((u32*)data)[7] & 0xffffff;
        gSPDisplayList(POLY_XLU_DISP++, (Gfx*)(0x0b000000 | displayList));
    }
    CLOSE_DISPS();
}

static int goldFloorAt(PlayState* play, Actor* actor, Vec3f* position, f32 height)
{
    Vec3f probe = *position;
    CollisionPoly* poly = NULL;
    s32 bgId;
    probe.y += 80.f;
#if defined(GAME_MM)
    f32 y = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &poly, &bgId, actor, &probe);
#else
    f32 y = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &probe);
#endif
    if (!poly || !(y > -30000.f) || poly->normal.y < 0x6000)
        return 0;
    CollisionHeader* header = play->colCtx.colHeader;
    if (bgId != BGCHECK_SCENE)
    {
        if (bgId < 0 || bgId >= BG_ACTOR_MAX || !(play->colCtx.dyna.bgActorFlags[bgId] & 1))
            return 0;
        header = play->colCtx.dyna.bgActors[bgId].colHeader;
    }
    if (!header)
        return 0;
    u32 surface = header->surfaceTypeList[poly->type].data[0];
    u32 property = (surface >> 26) & 15;
    if (((surface >> 8) & 31) || property == 5 || property == 12 ||
        SurfaceType_GetFloorType(&play->colCtx, poly, bgId) == 9)
        return 0;
    Vec3f feet = { position->x, y + 2.f, position->z };
    Vec3f head = { position->x, y + height + 80.f, position->z }, hit;
    if (BgCheck_EntityLineTest1(&play->colCtx, &feet, &head, &hit, &poly, 1, 0, 1, 0, &bgId))
        return 0;
    position->y = y + height;
    return 1;
}

static int goldFloor(PlayState* play, Actor* actor, Vec3f* position, f32 height, u16 maxRadius, int grounded)
{
    Vec3f origin = *position;
    f32 searchY = origin.y;
    if (grounded)
    {
        /* An authored hanging actor can sit exactly on the ceiling. Start
         * below that surface, without lowering already-grounded entries. */
        Vec3f below = { origin.x, origin.y - 16.f, origin.z };
        Vec3f above = { origin.x, origin.y + 2.f, origin.z }, hit;
        CollisionPoly* ceiling;
        s32 bgId;
        if (BgCheck_EntityLineTest1(&play->colCtx, &below, &above, &hit, &ceiling, 0, 0, 1, 0, &bgId))
            searchY -= 16.f;
    }
    for (int radius = 0; radius <= maxRadius; radius += maxRadius <= 150 ? 5 : 64)
    {
        for (int direction = 0; direction < (radius ? 16 : 1); ++direction)
        {
            s16 yaw = direction * 0x1000;
            Vec3f candidate = { origin.x + radius * Math_SinS(yaw), searchY, origin.z + radius * Math_CosS(yaw) };
            if (grounded)
                candidate.y -= 78.f;
            if (!goldFloorAt(play, actor, &candidate, height))
                continue;
            if (grounded)
            {
                WaterBox* water;
                f32 waterY = candidate.y;
                if (WaterBox_GetSurface1(play, &play->colCtx, candidate.x, candidate.z, &waterY, &water) &&
                    waterY > candidate.y + 5.f)
                    continue;
            }
            int safe = 1;

            for (Actor* tree = play->actorCtx.actors[ACTORCAT_PROP].first; tree; tree = tree->next)
            {
                if (tree->id != ACTOR_EN_WOOD02 || (u16)tree->params > 10)
                    continue;
                f32 dx = candidate.x - tree->world.pos.x, dz = candidate.z - tree->world.pos.z;
                if (candidate.y < tree->world.pos.y + 100.f && candidate.y + 80.f > tree->world.pos.y &&
                    dx * dx + dz * dz < 2500.f)
                {
                    safe = 0;
                    break;
                }
            }
            if (!safe)
                continue;

            f32 routeY = MAX(searchY + 2.f, candidate.y + 40.f);
            Vec3f from = { origin.x, routeY, origin.z };
            Vec3f to = { candidate.x, routeY, candidate.z }, hit;
            Vec3f feet = { candidate.x, candidate.y + 2.f, candidate.z };
            CollisionPoly* wall;
            s32 bgId;
            if (grounded && radius && (BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &wall, 1, 0, 0, 0, &bgId) ||
                BgCheck_EntityLineTest1(&play->colCtx, &to, &feet, &hit, &wall, 1, 0, 1, 0, &bgId)))
                continue;

            for (int side = 0; side < 8; ++side)
            {
                Vec3f edge = candidate;
                f32 clearance = grounded ? 32.f : 40.f;
                edge.x += clearance * Math_SinS(side * 0x2000);
                edge.z += clearance * Math_CosS(side * 0x2000);
                for (int level = 0; level < 3; ++level)
                {
                    from = (Vec3f){ candidate.x, candidate.y + 5.f + level * 35.f, candidate.z };
                    to = (Vec3f){ edge.x, from.y, edge.z };
                    if (BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &wall, 1, 0, 0, 0, &bgId))
                    {
                        safe = 0;
                        break;
                    }
                }
                if (!safe || !goldFloorAt(play, actor, &edge, height) || edge.y < candidate.y - 30.f ||
                    edge.y > candidate.y + 30.f)
                {
                    safe = 0;
                    break;
                }
            }
            if (safe)
            {
                *position = candidate;
                return 1;
            }
        }
    }

    if (actor && (origin.x != actor->world.pos.x || origin.y != actor->world.pos.y || origin.z != actor->world.pos.z))
    {
        Vec3f fallback = actor->world.pos;
        if (goldFloor(play, actor, &fallback, height, maxRadius, grounded))
        {
            *position = fallback;
            return 1;
        }
    }
    return 0;
}

static int previewDispatch(int operation, EnemySoulPreviewRequest* request)
{
    if (operation == 0)
        return previewRecord(request);
    else if (operation == 1)
        previewDraw(request->play);
    else if (operation == 2)
        previewReset();
    else if (operation == 4)
        return previewBlocked(request);
    else
        return goldFloor(request->play, request->actor, request->position, request->height, request->radius,
                         operation == 5);
    return 0;
}

static void unusedUpdate(Actor* actor, PlayState* play)
{
    (void)actor;
    (void)play;
}

ActorProfile SoulPreviewProfile = { 0x3ff, ACTORCAT_MISC, 0,   1, sizeof(Actor), (ActorFunc)previewDispatch,
                                    NULL,  unusedUpdate,  NULL };
__attribute__((section(".meta"), used)) static OvlInfoMetaActor __meta = { { 5 }, 0x3ff, (void*)&SoulPreviewProfile };
