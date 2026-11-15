#include <combo/enemy_snapper_mount.h>
#include <combo/enemy_keep.h>
#include <combo/enemy_flight.h>
#include <combo/matrix.h>
#include <combo/quake.h>
#include <combo/effect.h>
#include <combo/rand.h>
#include <combo/common/actors/En_Item00.h>

#if defined(GAME_MM)
#define MountModelId 0x01a6
#define MountDebrisDL 0x0406ab30
#define MountToMtx ((Mtx* (*)(Mtx*))0x80181a18)
#define MountModelFree ((void (*)(void*))0x80102ce0)
#define MountSfx Actor_PlaySfx
#define MountFlipDamage (DMG_EXPLOSIVES | DMG_GORON_POUND | DMG_DEKU_LAUNCH | DMG_POWDER_KEG)
#else
#define MountModelId 0x11a6
#define MountDebrisDL 0x0400c0d0
#define MountToMtx ((Mtx* (*)(Mtx*))0x800ab8d8)
#define MountModelFree ((void (*)(void*))0x80066c90)
void FOREIGN_ENEMY_API_SFX_ACTOR(Actor*, u16);
#define MountSfx FOREIGN_ENEMY_API_SFX_ACTOR
#define MountFlipDamage (DMG_EXPLOSIVE | DMG_HAMMER)
#endif

void Actor_SpawnFloorDustRing(PlayState*, Actor*, Vec3f*, f32, s32, f32, s16, s16, u8);

static void Mount_Init(Actor* actor, PlayState* play)
{
    EnemySnapperMount* mount = (EnemySnapperMount*)actor;
    static ColliderCylinderInit body = {
        { COL_MATERIAL_HARD, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1,
          COLSHAPE_CYLINDER },
        { ELEM_MATERIAL_UNK0,
          { 0xf7cfffff, 4, 4 },
          { 0xf7cfffff, 0, 0 },
          ATELEM_ON | ATELEM_SFX_NORMAL,
          ACELEM_ON | ACELEM_HOOKABLE | ACELEM_NO_DAMAGE,
          OCELEM_ON },
        { 53, 50, 0, { 0, 0, 0 } },
    };
    Collider_InitCylinder(play, &mount->body);
    Collider_SetCylinder(play, &mount->body, actor, &body);
    actor->colChkInfo.mass = 250;
    u32 size = comboLoadObject(NULL, MountModelId);
    if (size)
        mount->model = comboEnemyArenaAlloc(size);
    if (!mount->model || comboLoadObject(mount->model, MountModelId) != size)
    {
        Actor_Kill(actor);
        return;
    }

    u32 saved = gSegments[6];
    gSegments[6] = (u32)mount->model - K0BASE;
    SkelAnime_InitFlex(play, &mount->normalSkel, (void*)0x06007c70, (void*)0x060031dc, mount->normalJoints,
                       mount->normalMorph, ARRAY_COUNT(mount->normalJoints));
    SkelAnime_InitFlex(play, &mount->shellSkel, (void*)0x06001a50, (void*)0x06000b30, mount->shellJoints,
                       mount->shellMorph, ARRAY_COUNT(mount->shellJoints));
    Animation_PlayOnce(&mount->normalSkel, (void*)0x060031dc);
    gSegments[6] = saved;
    Actor_SetScale(actor, .013f);
    actor->gravity = -1.f;
    mount->shellScale = 1.f;
}

static void Mount_Destroy(Actor* actor, PlayState* play)
{
    EnemySnapperMount* mount = (EnemySnapperMount*)actor;
    Collider_DestroyCylinder(play, &mount->body);
    if (mount->model)
        MountModelFree(mount->model);
}

static void Mount_WaterTrail(Actor* actor, PlayState* play)
{
    if (!(actor->bgCheckFlags & BGCHECKFLAG_WATER) || actor->depthInWater <= 0.f)
        return;
    Vec3f pos = actor->world.pos;
    pos.y += actor->depthInWater;
    EffectSsGRipple_Spawn(play, &pos, 500, 900, 0);
    s16 yaw = actor->world.rot.y + (s16)((Rand_ZeroOne() - .5f) * 32768.f);
    pos.x -= 55.f * Math_SinS(yaw);
    pos.z -= 55.f * Math_CosS(yaw);
    pos.y += 8.f;
    EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 1400 + (s16)(Rand_ZeroOne() * 200.f));
}

static void Mount_WallDust(Actor* actor, PlayState* play)
{
#if defined(GAME_MM)
    Vec3f zero = { 0.f, 0.f, 0.f };
    s16 yaw = actor->wallYaw;
    Vec3f base = actor->world.pos;
    base.x -= 35.f * Math_SinS(yaw);
    base.z -= 35.f * Math_CosS(yaw);
    for (int i = 0; i < 4; ++i)
    {
        s16 angle = Rand_ZeroOne() * 32768.f;
        f32 distance = 50.f + Rand_ZeroOne() * 60.f;
        f32 horizontal = distance * Math_CosS(angle);
        Vec3f pos = { base.x + horizontal * Math_SinS(yaw + 0x4000), base.y + distance * Math_SinS(angle),
                      base.z + horizontal * Math_CosS(yaw + 0x4000) };
        func_800B12F0(play, &pos, &zero, &zero, 950 + (s16)(Rand_ZeroOne() * 100.f), 20 + (s16)(Rand_ZeroOne() * 10.f),
                      20);
    }
#else
    Actor_SpawnFloorDustRing(play, actor, &actor->world.pos, 36.f, 3, 8.f, 400, 15, 1);
#endif
}

static void Mount_StartDebris(EnemySnapperMount* mount)
{
    Actor* actor = &mount->actor;
    Vec3f base = actor->world.pos;
    base.x -= 50.f * Math_SinS(actor->wallYaw);
    base.z -= 50.f * Math_CosS(actor->wallYaw);
    mount->debrisTimer = 40;
    for (int i = 0; i < ARRAY_COUNT(mount->debris); ++i)
    {
        SnapperMountDebris* chip = &mount->debris[i];
        s16 yaw = actor->wallYaw + (s16)((Rand_ZeroOne() - .5f) * 0x5000);
        s16 pitch = 0x1000 + (s16)(Rand_ZeroOne() * 0x3000);
        f32 vertical = Math_SinS(pitch), horizontal = Math_CosS(pitch);
        f32 speed = 7.f + Rand_ZeroOne() * 5.f;
        chip->velocity =
            (Vec3f){ Math_SinS(yaw) * speed * horizontal, speed * vertical + 3.f, Math_CosS(yaw) * speed * horizontal };
        chip->rotation = (Vec3s){ Rand_ZeroOne() * 65536.f, Rand_ZeroOne() * 65536.f, Rand_ZeroOne() * 65536.f };
        s16 side = actor->wallYaw + ((s16)(yaw - actor->wallYaw) > 0 ? 0x4000 : -0x4000);
        chip->pos = (Vec3f){ base.x + Math_SinS(side) * 60.f * horizontal, base.y + 60.f * vertical,
                             base.z + Math_CosS(side) * 60.f * horizontal };
        chip->scale = .002f + Rand_ZeroOne() * .0025f;
    }
    MountSfx(actor, 0x393f);
}

static int Mount_ChargeHasFloor(Actor* actor, PlayState* play)
{
    Vec3f probe = actor->world.pos;
    probe.x += 22.f * Math_SinS(actor->world.rot.y);
    probe.z += 22.f * Math_CosS(actor->world.rot.y);
    probe.y += 35.f;
    CollisionPoly* floor = NULL;
    s32 bgId;
#if defined(GAME_MM)
    f32 height = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, actor, &probe);
#else
    f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &probe);
#endif
    EnemySnapperMount* mount = (EnemySnapperMount*)actor;
    f32 currentFloor =
        (mount->mode == SNAPPER_MOUNT_FLIPPING || mount->mode == SNAPPER_MOUNT_DEFEAT) && actor->floorPoly
            ? actor->floorHeight
            : actor->world.pos.y;
    return floor && fabsf(height - currentFloor) <= 24.f;
}

static void Mount_Defeat(EnemySnapperMount* mount, PlayState* play)
{
    Actor* actor = &mount->actor;
    u32 segment = gSegments[6];
    gSegments[6] = (u32)mount->model - K0BASE;
    if (mount->previousMode != SNAPPER_MOUNT_DEFEAT)
    {
        mount->previousMode = SNAPPER_MOUNT_DEFEAT;
        Animation_PlayLoop(&mount->normalSkel, (void*)0x06000af4);
        actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | (1u << 10) | (1u << 13));
        actor->shape.rot.x = actor->shape.rot.z = 0;
        actor->velocity.y = 22.f;
        actor->gravity = -1.f;
        actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        MountSfx(actor, 0x39a3);
    }
    SkelAnime_Update(&mount->normalSkel);
    gSegments[6] = segment;
    int pastFloor = !actor->floorPoly && actor->world.pos.y < actor->home.pos.y - 80.f;
    if ((actor->bgCheckFlags & BGCHECKFLAG_GROUND) || pastFloor)
    {
        actor->speed = 0.f;
        if (pastFloor)
            actor->gravity = actor->velocity.y = 0.f;
        actor->shape.rot.x = (s32)Rand_Next() >> 22;
        actor->shape.rot.z = (s32)Rand_Next() >> 22;
        if (Math_StepToF(&actor->scale.x, 0.f, .00075f))
        {
            MountSfx(actor, 0x3a87);
            Actor_Kill(actor);
            return;
        }
        actor->scale.y = actor->scale.z = actor->scale.x;
    }
    else
    {
        actor->speed = !(actor->bgCheckFlags & BGCHECKFLAG_WALL) && Mount_ChargeHasFloor(actor, play) ? 5.f : 0.f;
        if (actor->velocity.y > 0.f)
        {
            Vec3f from = actor->world.pos;
            from.y += 20.f;
            Vec3f to = from;
            to.y += actor->velocity.y;
            if (!enemyFlightClear(play, &from, &to))
                actor->velocity.y = 0.f;
        }
    }
    Actor_MoveWithGravity(actor);
    Actor_UpdateBgCheckInfo(play, actor, 25.f, 34.f, 35.f, 5);
}

static void Mount_Update(Actor* actor, PlayState* play)
{
    EnemySnapperMount* mount = (EnemySnapperMount*)actor;
    if (mount->mode == SNAPPER_MOUNT_DEFEAT)
    {
        Mount_Defeat(mount, play);
        return;
    }
    Actor* rider = actor->parent;
    if (!rider || rider->child != actor || !rider->update)
    {
        Actor_Kill(actor);
        return;
    }
    u32 hit = (mount->body.base.acFlags & AC_HIT) && mount->body.elem.acHitElem
                  ? mount->body.elem.acHitElem->atDmgInfo.dmgFlags
                  : 0;
    mount->body.base.acFlags &= ~AC_HIT;
    if (mount->carrying && mount->mode != SNAPPER_MOUNT_REAR && (hit & MountFlipDamage))
    {
        mount->carrying = 0;
        mount->walkToRider = 1;
        mount->mode = SNAPPER_MOUNT_FLIPPING;
        actor->velocity.y = 22.f;
        actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        if (mount->body.base.ac)
        {
            Actor* attacker = mount->body.base.ac;
            actor->world.rot.y = Math_Vec3f_Yaw(&actor->world.pos, &attacker->world.pos) + 0x8000;
        }
        actor->shape.rot.y = actor->world.rot.y;
        if (actor->floorPoly)
        {
            Vec3f drop = actor->world.pos;
            drop.y = MAX(drop.y, actor->floorHeight + 5.f);
            Actor* arrows = Item_DropCollectible(play, &drop, ITEM00_ARROWS_10);
            if (arrows)
            {
                arrows->velocity.y = 15.f;
                arrows->world.rot.y = Math_Vec3f_Yaw(&actor->world.pos, &actor->home.pos);
            }
        }
        MountSfx(actor, 0x39a3);
    }
    u32 saved = gSegments[6];
    gSegments[6] = (u32)mount->model - K0BASE;
    if (mount->previousMode != mount->mode)
    {
        mount->previousMode = mount->mode;
        if (mount->mode == SNAPPER_MOUNT_EMERGE)
            Animation_PlayOnce(&mount->normalSkel, (void*)0x060031dc);
        else if (mount->mode == SNAPPER_MOUNT_READY)
            Animation_PlayLoop(&mount->normalSkel, mount->walkToRider ? (void*)0x0600823c : (void*)0x06004210);
        else if (mount->mode == SNAPPER_MOUNT_REAR)
            Animation_MorphToPlayOnce(&mount->normalSkel, (void*)0x06000440, -2.f);
        else if (mount->mode == SNAPPER_MOUNT_RETREAT)
        {
            Animation_MorphToPlayOnce(&mount->normalSkel, (void*)0x06001c68, -3.f);
            MountSfx(actor, 0x39a2);
            if (actor->parent && actor->parent->update)
                MountSfx(actor->parent, 0x399f);
        }
        else if (mount->mode == SNAPPER_MOUNT_FLIPPING)
        {
            Animation_PlayLoop(&mount->normalSkel, (void*)0x06000af4);
        }
    }

    int finished = SkelAnime_Update(&mount->normalSkel);
    f32 frame = mount->normalSkel.curFrame;
    actor->speed = 0.f;
    switch (mount->mode)
    {
        case SNAPPER_MOUNT_EMERGE:
            if (finished)
                mount->mode = SNAPPER_MOUNT_READY;
            break;
        case SNAPPER_MOUNT_READY:
            if (mount->walkToRider)
            {
                s16 yaw = Math_Vec3f_Yaw(&actor->world.pos, &rider->world.pos);
                Math_ScaledStepToS(&actor->world.rot.y, yaw, 0x400);
                if (!(actor->bgCheckFlags & BGCHECKFLAG_WALL) && Mount_ChargeHasFloor(actor, play))
                    actor->speed = 1.f;
            }
            break;
        case SNAPPER_MOUNT_REAR:
            if (finished)
                mount->mode = SNAPPER_MOUNT_RETREAT;
            break;
        case SNAPPER_MOUNT_RETREAT:
            mount->shellScale = frame > 2.f ? 1.5f - (frame - 2.f) / 12.f : 1.f + .25f * frame;
            mount->limbStretch = frame > 2.f ? 1.5f - (frame - 2.f) * (7.f / 30.f) : mount->shellScale;
            if (finished)
            {
                mount->mode = SNAPPER_MOUNT_EXTEND;
                mount->spikes = .5f;
                MountSfx(actor, 0x392b);
            }
            break;
        case SNAPPER_MOUNT_EXTEND:
            mount->spikes += .2f;
            if (mount->spikes > 1.1f)
            {
                mount->mode = SNAPPER_MOUNT_SPINUP;
                mount->spikes = 1.f;
                mount->spinTimer = 15;
                actor->shape.rot.z = 0x680;
            }
            break;
        case SNAPPER_MOUNT_SPINUP:
            mount->spin += 0x3b00;
            MountSfx(actor, 0x399e - SFX_FLAG);
            if (!--mount->spinTimer)
            {
                mount->mode = SNAPPER_MOUNT_CHARGE;
                s16 displayYaw = actor->world.rot.y + mount->spin;
                actor->world.rot.y = actor->yawTowardsPlayer;
                if (actor->bgCheckFlags & BGCHECKFLAG_WALL)
                {
                    s16 angle = actor->yawTowardsPlayer - actor->wallYaw;
                    actor->world.rot.y = actor->wallYaw + CLAMP(angle, -0x3c00, 0x3c00);
                    actor->bgCheckFlags &= ~BGCHECKFLAG_WALL;
                }
                mount->spin = displayYaw - actor->world.rot.y;
                mount->chargeFrames = 0;
                if (Mount_ChargeHasFloor(actor, play))
                    actor->speed = 15.f;
            }
            break;
        case SNAPPER_MOUNT_CHARGE: {
            mount->spin += 0x3b00;
            MountSfx(actor, 0x399e - SFX_FLAG);
            if (mount->chargeFrames < 20)
                ++mount->chargeFrames;
            if (mount->body.base.atFlags & AT_HIT)
            {
                s16 angle = actor->yawTowardsPlayer - actor->world.rot.y;
                s16 turn = angle > 0 ? -0x300 : 0x300;
                actor->world.rot.y += turn;
                mount->spin -= turn;
            }
            if (!Mount_ChargeHasFloor(actor, play) || (actor->bgCheckFlags & BGCHECKFLAG_WALL))
            {
                actor->velocity.y = mount->chargeFrames * .375f;
                mount->wallImpact = !!(actor->bgCheckFlags & BGCHECKFLAG_WALL);
                if (mount->wallImpact)
                {
                    s16 quake = Quake_Request(Play_GetCamera(play, CAM_ID_MAIN), 3);
                    Quake_SetSpeed(quake, 20000);
                    Quake_SetPerturbations(quake, 15, 0, 0, 0);
                    Quake_SetDuration(quake, 10);
                    Rumble_Request(actor->xyzDistToPlayerSq, 180, 20, 100);
                    Mount_WallDust(actor, play);
                    Mount_StartDebris(mount);
                }
                actor->shape.rot.z = 0;
                actor->world.rot.y += mount->spin;
                mount->spin = 0;
                mount->mode = SNAPPER_MOUNT_RETRACT;
                MountSfx(actor, 0x392d);
            }
            else
            {
                actor->speed = 15.f;
                if (play->gameplayFrames & 1)
                    Mount_WaterTrail(actor, play);
            }
            break;
        }
        case SNAPPER_MOUNT_RETRACT:
            if (mount->wallImpact)
                Mount_WallDust(actor, play);
            mount->spikes -= .1f;
            if (mount->spikes < .5f)
            {
                mount->mode = SNAPPER_MOUNT_RECOVER;
                mount->shellScale = 1.f;
                mount->limbStretch = .1f;
                Animation_MorphToPlayOnce(&mount->normalSkel, (void*)0x060031dc, -3.f);
            }
            break;
        case SNAPPER_MOUNT_FLIPPING:
            if (actor->velocity.y <= 0.f && (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            {
                mount->mode = SNAPPER_MOUNT_OVERTURNED;
                MountSfx(actor, 0x38dd);
                Mount_WaterTrail(actor, play);
                mount->spikes = 0.f;
                actor->shape.rot.x = actor->shape.rot.z = 0;
            }
            else
            {
                actor->shape.rot.x += 0x800;
                actor->shape.rot.z += 0x1000;
                if (Mount_ChargeHasFloor(actor, play))
                    actor->speed = 5.f;
            }
            break;
        case SNAPPER_MOUNT_OVERTURNED:
            actor->shape.rot.x = (s32)Rand_Next() >> 22;
            actor->shape.rot.z = (s32)Rand_Next() >> 22;
            break;
        case SNAPPER_MOUNT_RECOVER:
            Math_ScaledStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 0x600);
            if (finished)
            {
                mount->mode = mount->carrying ? SNAPPER_MOUNT_RETREAT : SNAPPER_MOUNT_READY;
            }
            else
            {
                mount->shellScale = frame > 7.f ? 1.5f - (frame - 7.f) / 6.f : 1.f + frame / 14.f;
                mount->limbStretch = frame > 7.f ? mount->shellScale : .1f + .2f * frame;
            }
            break;
    }
    SkelAnime_Update(&mount->shellSkel);
    gSegments[6] = saved;

    if (mount->debrisTimer)
    {
        --mount->debrisTimer;
        for (int i = 0; i < ARRAY_COUNT(mount->debris); ++i)
        {
            SnapperMountDebris* chip = &mount->debris[i];
            chip->velocity.y -= 1.f;
            chip->pos.x += chip->velocity.x;
            chip->pos.y += chip->velocity.y;
            chip->pos.z += chip->velocity.z;
            chip->rotation.x += (s16)((Rand_Next() >> 23) + 0x700);
            chip->rotation.y += (s16)((Rand_Next() >> 23) + 0x900);
            chip->rotation.z += (s16)((Rand_Next() >> 23) + 0xb00);
        }
    }
    actor->shape.rot.y = actor->world.rot.y;
    Actor_MoveWithGravity(actor);
#if defined(GAME_MM)
    Actor_UpdateBgCheckInfo(play, actor, 25.f, 34.f, 35.f, 5 | UPDBGCHECKINFO_FLAG_MM_400);
#else
    Actor_UpdateBgCheckInfo(play, actor, 25.f, 34.f, 35.f, 5);
#endif
    enum { HOOKSHOT_PULLS_PLAYER = 1u << 10 };
    if (!mount->carrying)
        actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | HOOKSHOT_PULLS_PLAYER);
    mount->body.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    if (mount->mode == SNAPPER_MOUNT_SPINUP || mount->mode == SNAPPER_MOUNT_CHARGE)
    {
        mount->body.elem.acDmgInfo.dmgFlags = 0xf7cf7fff;
        Player* player = GET_PLAYER(play);
        Vec3f from = actor->world.pos;
        from.y += mount->body.dim.height * .5f;
        Vec3f target = player->actor.world.pos;
        target.y += player->cylinder.dim.yShift + player->cylinder.dim.height * .5f;
        f32 dx = target.x - from.x;
        f32 dz = target.z - from.z;
        f32 reach = mount->body.dim.radius + player->cylinder.dim.radius + 16.f;
        if (dx * dx + dz * dz <= reach * reach && enemyFlightClear(play, &from, &target))
            CollisionCheck_SetAT(play, &play->colChkCtx, &mount->body.base);
    }
    else
    {
        mount->body.elem.acDmgInfo.dmgFlags = 0xf7cfffff;
    }
    if (mount->carrying)
    {
        actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | HOOKSHOT_PULLS_PLAYER;
        actor->focus.pos = actor->world.pos;
        actor->focus.pos.y += 25.f;
        rider->world.pos = actor->world.pos;
        rider->world.pos.y += mount->bodyHeight + 46.f;
        rider->shape.yOffset = 0.f;
        rider->shape.rot.y = rider->world.rot.y = actor->world.rot.y;
        if (mount->riderLift > 0.f)
        {
            Vec3f top = rider->world.pos;
            top.y += mount->riderLift + 35.f;
            Vec3f hit;
            CollisionPoly* poly;
            s32 bgId;
            f32 lift = mount->riderLift;
            if (BgCheck_EntityLineTest1(&play->colCtx, &rider->world.pos, &top, &hit, &poly, 0, 0, 1, 0, &bgId))
                lift = CLAMP(hit.y - rider->world.pos.y - 35.f, 0.f, lift);
            rider->world.pos.y += lift;
            rider->shape.rot.x = rider->shape.rot.z = 0;
        }
        else
        {
            if (mount->mode == SNAPPER_MOUNT_SPINUP || mount->mode == SNAPPER_MOUNT_CHARGE)
                rider->shape.rot.y += mount->spin;
            s16 yaw = rider->shape.rot.y;
            s16 roll = actor->shape.rot.z;
            rider->shape.rot.x = 0;
            rider->shape.rot.z = roll;
            if (roll)
            {
                f32 lateral = -46.f * Math_SinS(roll);
                Vec3f seat = { rider->world.pos.x + lateral * Math_CosS(yaw),
                               rider->world.pos.y + 46.f * (Math_CosS(roll) - 1.f),
                               rider->world.pos.z - lateral * Math_SinS(yaw) };
                if (enemyFlightClear(play, &rider->world.pos, &seat))
                    rider->world.pos = seat;
            }
        }
        rider->velocity.y = 0.f;
        CollisionCheck_SetAC(play, &play->colChkCtx, &mount->body.base);
    }
    Collider_UpdateCylinder(actor, &mount->body);
    CollisionCheck_SetOC(play, &play->colChkCtx, &mount->body.base);
}

static s32 Mount_DrawShellLimb(PlayState* play, s32 limb, Gfx** dl, Vec3f* pos, Vec3s* rot, void* data)
{
    EnemySnapperMount* mount = data;
    if (limb == 1)
        pos->y -= 700.f;
    if (limb == 3)
        Matrix_Scale(1.f, mount->spikes, mount->spikes, MTXMODE_APPLY);
    return 0;
}

static s32 Mount_DrawNormalLimb(PlayState* play, s32 limb, Gfx** dl, Vec3f* pos, Vec3s* rot, void* data)
{
    EnemySnapperMount* mount = data;
    if (mount->mode == SNAPPER_MOUNT_RETREAT || mount->mode == SNAPPER_MOUNT_RECOVER)
    {
        if (limb == 2)
        {
            Matrix_Scale(mount->shellScale, mount->limbStretch, mount->shellScale, MTXMODE_APPLY);
        }
        else if (limb == 5 || limb == 7 || limb == 9 || limb == 11)
        {
            Matrix_Scale(mount->shellScale, mount->limbStretch, mount->limbStretch, MTXMODE_APPLY);
        }
    }
    return 0;
}

static void Mount_DrawBodyHeight(PlayState* play, s32 limb, Gfx** dl, Vec3s* rot, void* data)
{
    if (limb != 1)
        return;
    EnemySnapperMount* mount = data;
    Vec3f offset = { 0.f, 0.f, 0.f };
    Vec3f body;
    Matrix_MultVec3f(&offset, &body);
    mount->bodyHeight = body.y - mount->actor.world.pos.y;
}

static void Mount_Draw(Actor* actor, PlayState* play)
{
    EnemySnapperMount* mount = (EnemySnapperMount*)actor;
    int shell = (mount->mode >= SNAPPER_MOUNT_EXTEND && mount->mode <= SNAPPER_MOUNT_RETRACT) ||
                mount->mode == SNAPPER_MOUNT_OVERTURNED;
    int spinning = mount->mode == SNAPPER_MOUNT_SPINUP || mount->mode == SNAPPER_MOUNT_CHARGE;
    u32 saved = gSegments[6];
    gSegments[6] = (u32)mount->model - K0BASE;
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    OPEN_DISPS(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 6, mount->model);
    gSPSegment(POLY_OPA_DISP++, 8, (u8*)mount->model + 0x55a0);
    Matrix_Push();
    if (spinning)
        Matrix_RotateY(mount->spin * (M_PI / 32768.f), MTXMODE_APPLY);
    if (mount->mode == SNAPPER_MOUNT_OVERTURNED)
    {
        Matrix_Translate(0.f, 1460.f, 0.f, MTXMODE_APPLY);
        Matrix_RotateZ(M_PI, MTXMODE_APPLY);
    }
    SkelAnime* skel = shell ? &mount->shellSkel : &mount->normalSkel;
    SkelAnime_DrawFlexOpa(play, skel->skeleton, skel->jointTable, skel->dListCount,
                          shell ? Mount_DrawShellLimb : Mount_DrawNormalLimb, Mount_DrawBodyHeight, actor);
    Matrix_Pop();
    gSegments[6] = saved;
    gSPSegment(POLY_OPA_DISP++, 6, saved + K0BASE);
    if (mount->debrisTimer)
    {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
        for (int i = 0; i < ARRAY_COUNT(mount->debris); ++i)
        {
            SnapperMountDebris* chip = &mount->debris[i];
            Matrix_Push();
            Matrix_SetTranslateRotateYXZ(chip->pos.x, chip->pos.y, chip->pos.z, &chip->rotation);
            Matrix_Scale(chip->scale, chip->scale, chip->scale, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, MountToMtx(GRAPH_ALLOC(play->state.gfxCtx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_OPA_DISP++, MountDebrisDL);
            Matrix_Pop();
        }
    }
    CLOSE_DISPS();
}

static ActorProfile sMountProfile = {
    ACTOR_ENEMY_SNAPPER_MOUNT,
    ACTORCAT_PROP,
    ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnemySnapperMount),
    Mount_Init,
    Mount_Destroy,
    Mount_Update,
    Mount_Draw,
};
OVL_INFO_ACTOR(ACTOR_ENEMY_SNAPPER_MOUNT, sMountProfile);
