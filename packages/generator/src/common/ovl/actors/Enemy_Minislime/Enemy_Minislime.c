#include <combo/enemy_minislime.h>
#include <combo/enemy_actor_layout.h>
#include <combo/matrix.h>
#include <combo/effect.h>
#if defined(GAME_OOT)
#include <combo/enemy_keep.h>
#endif

#define MINISLIME_HOOK_PULL (1u << 9)
#define MINISLIME_HOOK_ATTACHED (1u << 13)

#if defined(GAME_MM)
#define MinislimeObject 0x128
#define MinislimeToMtx ((Mtx* (*)(Mtx*))0x80181a18)
#define MinislimeSfx Actor_PlaySfx
#define MinislimeIceBreak (DMG_EXPLOSIVES | DMG_GORON_PUNCH | DMG_GORON_POUND | DMG_GORON_SPIKES | DMG_POWDER_KEG)
#else
#define MinislimeObject 0x1128
#define MinislimeToMtx ((Mtx* (*)(Mtx*))0x800ab8d8)
void FOREIGN_ENEMY_API_SFX_ACTOR(Actor*, u16);
#define MinislimeSfx FOREIGN_ENEMY_API_SFX_ACTOR
#define MinislimeIceBreak (DMG_EXPLOSIVE | DMG_HAMMER)
#endif

typedef EnemyNativeHookshot MinislimeHookshot;

static void Minislime_Detach(Actor* actor, PlayState* play)
{
    actor->flags &= ~MINISLIME_HOOK_ATTACHED;
    for (Actor* item = play->actorCtx.actors[ACTORCAT_ITEMACTION].first; item; item = item->next)
    {
        if (item->id != ACTOR_ARMS_HOOK)
            continue;
        MinislimeHookshot* hook = (MinislimeHookshot*)item;
        if (hook->attached == actor)
            hook->attached = NULL;
        if (hook->collider.base.at == actor)
        {
            hook->collider.base.at = NULL;
            hook->collider.base.atFlags &= ~AT_HIT;
        }
    }
}

static void Minislime_Init(Actor* actor, PlayState* play)
{
    static ColliderCylinderInit body = {
        { COL_MATERIAL_NONE, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1,
          COLSHAPE_CYLINDER },
        { ELEM_MATERIAL_UNK0,
          { 0xf7cfffff, 0, 4 },
          { 0xf7cfffff, 0, 0 },
          ATELEM_ON | ATELEM_SFX_HARD,
          ACELEM_ON | ACELEM_HOOKABLE | ACELEM_NO_DAMAGE,
          OCELEM_ON },
        { 54, 60, -30, { 0, 0, 0 } },
    };
    EnemyMinislime* slime = (EnemyMinislime*)actor;
    Collider_InitCylinder(play, &slime->body);
    Collider_SetCylinder(play, &slime->body, actor, &body);
    actor->colChkInfo.mass = 30;
    actor->shape.rot.x = (s16)(Rand_Next() >> 16);
    actor->shape.rot.z = (s16)(Rand_Next() >> 16);
    slime->spin = 0x800 + (s16)(Rand_ZeroOne() * 0x800);
    slime->shattering = 1;
    slime->frozenAlpha = 200;
    slime->hookSafePos = actor->world.pos;
    slime->motionScale = (Vec3f){ .15f, .075f, .15f };
}

static void Minislime_Destroy(Actor* actor, PlayState* play)
{
    EnemyMinislime* slime = (EnemyMinislime*)actor;
    Minislime_Detach(actor, play);
    if (slime->ownerSlot)
        *slime->ownerSlot = NULL;
    Collider_DestroyCylinder(play, &slime->body);
}

static Vec3f Minislime_Scale(const EnemyMinislime* slime)
{
    f32 x = slime->motionScale.x, y = slime->motionScale.y, z = slime->motionScale.z;
    if (slime->throwTimer)
    {
        s16 phase = slime->throwTimer * (0x8000 / 5);
        x = .15f + .03f * Math_SinS(phase);
        y = .075f + .02f * Math_CosS(phase);
        z = .3f - x;
    }
    else if (slime->knockback)
    {
        s16 phase = slime->knockback * (0x8000 / 3);
        f32 amplitude = .05f * sqrtf(slime->knockback);
        x = z = .15f * (1.f + Math_CosS(phase) * amplitude);
        y = .075f * (1.f + Math_SinS(phase) * amplitude);
    }
    else if (slime->bounce)
    {
        s16 phase = slime->bounce * (0x8000 / 3);
        f32 amplitude = slime->bounce / 6 + 1;
        x = z = .15f + Math_CosS(phase) * amplitude / 150.f;
        y = .075f + Math_SinS(phase) * amplitude * .005f;
    }
    return (Vec3f){ x, y, z };
}

static void Minislime_BreakIce(EnemyMinislime* slime)
{
    Actor* actor = &slime->actor;
    Vec3f scale = Minislime_Scale(slime);
    s16 yaw = 0;
    for (int i = 0; i < ARRAY_COUNT(slime->shards); ++i, yaw += 0x1999)
    {
        MinislimeIceShard* shard = &slime->shards[i];
        s16 pitch = 0x1000 + (s16)(Rand_ZeroOne() * 0x3000);
        Vec3f direction = { Math_CosS(pitch) * Math_SinS(yaw), Math_SinS(pitch), Math_CosS(pitch) * Math_CosS(yaw) };
        shard->pos = (Vec3f){ actor->world.pos.x + 400.f * scale.x * direction.x,
                              actor->world.pos.y + 400.f * scale.y * (direction.y * 2.f - 1.f),
                              actor->world.pos.z + 400.f * scale.z * direction.z };
        f32 speed = 7.f + Rand_ZeroOne() * 3.f;
        shard->velocity = (Vec3f){ direction.x * speed, direction.y * speed, direction.z * speed };
        shard->rotation = (Vec3s){ Rand_Next() >> 16, Rand_Next() >> 16, Rand_Next() >> 16 };
        shard->scale = .002f + Rand_ZeroOne() * .006f;
    }
    slime->shardFloor = actor->floorPoly ? actor->floorHeight : actor->world.pos.y - 80.f;
    slime->freeze = slime->frozenAlpha = 0;
    MinislimeSfx(actor, NA_SE_EV_ICE_BROKEN);
}

static void Minislime_UpdateEffects(EnemyMinislime* slime)
{
    for (int i = 0; i < ARRAY_COUNT(slime->shards); ++i)
    {
        MinislimeIceShard* shard = &slime->shards[i];
        if (!shard->scale)
            continue;
        shard->velocity.y -= 1.f;
        shard->pos.x += shard->velocity.x;
        shard->pos.y += shard->velocity.y;
        shard->pos.z += shard->velocity.z;
        if (shard->pos.y < slime->shardFloor - 20.f)
        {
            shard->scale = 0.f;
            continue;
        }
        shard->rotation.x += (s16)((Rand_Next() >> 23) + 0x700);
        shard->rotation.y += (s16)((Rand_Next() >> 23) + 0x900);
        shard->rotation.z += (s16)((Rand_Next() >> 23) + 0xb00);
    }
#if defined(GAME_OOT)
    for (int i = 0; i < ARRAY_COUNT(slime->smoke); ++i)
    {
        if (!slime->smoke[i].life)
            continue;
        --slime->smoke[i].life;
        slime->smoke[i].pos.x += slime->smoke[i].velocity.x;
        slime->smoke[i].pos.y += slime->smoke[i].velocity.y;
        slime->smoke[i].pos.z += slime->smoke[i].velocity.z;
    }
#endif
}

static void Minislime_SpawnSmoke(EnemyMinislime* slime, PlayState* play)
{
    Actor* actor = &slime->actor;
    Vec3f scale = Minislime_Scale(slime);
    Vec3f pos = actor->world.pos;
    pos.x += (Rand_ZeroOne() - .5f) * 200.f * scale.x;
    pos.z += (Rand_ZeroOne() - .5f) * 200.f * scale.z;
    if (actor->floorPoly)
        pos.y = MAX(pos.y, actor->floorHeight + 30.f);
    Vec3f velocity = { (Rand_ZeroOne() - .5f) * 1.5f, 2.f, (Rand_ZeroOne() - .5f) * 1.5f };
#if defined(GAME_OOT)
    for (int i = 0; i < ARRAY_COUNT(slime->smoke); ++i)
    {
        if (slime->smoke[i].life)
            continue;
        slime->smoke[i].pos = pos;
        slime->smoke[i].velocity = velocity;
        slime->smoke[i].life = 50;
        break;
    }
#else
    Vec3f accel = { 0.f, 0.f, 0.f };
    EffectSsIceSmoke_Spawn(play, &pos, &velocity, &accel, 500);
#endif
}

static void Minislime_Hit(EnemyMinislime* slime, Player* player)
{
    ColliderCylinder* collider = &slime->body;
    Actor* actor = &slime->actor;
    if (!(collider->base.acFlags & AC_HIT))
        return;
    collider->base.acFlags &= ~AC_HIT;
    ColliderElement* hit = collider->elem.acHitElem;
    u32 flags = hit ? hit->atDmgInfo.dmgFlags : 0;
    if (!flags || (flags & DMG_HOOKSHOT))
        return;
    if (slime->freeze)
    {
        if (flags & MinislimeIceBreak)
        {
            Minislime_BreakIce(slime);
        }
        else if (flags & (1u << 11))
        {
            slime->freeze = 0;
            slime->melt = 40;
            actor->world.rot.x = 0;
            actor->velocity.x = actor->velocity.z = 0.f;
            actor->shape.rot = (Vec3s){ 0, 0, 0 };
        }
    }
    else if (flags & (1u << 12))
    {
        slime->freeze = 80;
        slime->freezeAnchor = actor->world.pos;
        actor->velocity.x = actor->velocity.z = 0.f;
    }
    else
    {
        Actor* attacker = collider->base.ac ? collider->base.ac : &player->actor;
#if defined(GAME_MM)
        const u32 ranged = 0x10000 | 0x2000 | 0x1000 | 0x800 | 0x20;
#else
        const u32 ranged = 0x2000 | 0x1000 | 0x800 | 0x20;
#endif
        s16 yaw = flags & ranged ? attacker->shape.rot.y : Math_Vec3f_Yaw(&attacker->world.pos, &actor->world.pos);
        actor->world.rot.y = yaw;
        actor->velocity.x = 20.f * Math_SinS(yaw);
        actor->velocity.z = 20.f * Math_CosS(yaw);
        slime->knockback = 30;
        slime->bounce = 0;
    }
}

static void Minislime_Land(EnemyMinislime* slime, PlayState* play, Vec3f* pos, CollisionPoly* floor, f32 floorHeight)
{
    Actor* actor = &slime->actor;
    int groundTouch = !(actor->bgCheckFlags & BGCHECKFLAG_GROUND);
    actor->floorPoly = floor;
    actor->floorHeight = floorHeight;
    actor->world.pos = *pos;
    actor->world.pos.y = floorHeight + 400.f * Minislime_Scale(slime).y;
    actor->bgCheckFlags |= BGCHECKFLAG_GROUND;
    actor->velocity.y = 0.f;
    slime->throwTimer = 0;
    actor->shape.rot.x = actor->shape.rot.z = 0;
    if (!slime->knockback)
        actor->velocity.x = actor->velocity.z = 0.f;
    if (slime->shattering)
    {
        slime->shattering = 0;
        slime->attackDelay = 40;
        Minislime_BreakIce(slime);
    }
    else if (slime->freeze)
    {
        Minislime_BreakIce(slime);
    }
    if (!slime->knockback && groundTouch)
    {
        actor->home.pos = actor->world.pos;
        slime->bounce = 42;
        slime->idleTimer = 20;
        slime->motionScale = (Vec3f){ .19f, .045f, .19f };
        actor->shape.rot.y = 0;
        actor->world.rot.y = 0;
        MinislimeSfx(actor, 0x38da);
    }
    f32 surface = actor->world.pos.y;
    WaterBox* water;
    if (groundTouch && WaterBox_GetSurface1_2(play, &play->colCtx, pos->x, pos->z, &surface, &water) &&
        surface >= floorHeight)
    {
        Vec3f ripple = { pos->x, surface, pos->z };
        EffectSsGRipple_Spawn(play, &ripple, 500, 720, 0);
    }
}

static void Minislime_Move(EnemyMinislime* slime, PlayState* play)
{
    Actor* actor = &slime->actor;
    Vec3f* pos = &actor->world.pos;
    Vec3f* velocity = &actor->velocity;
    velocity->y = MAX(-20.f, velocity->y - ((slime->shattering || slime->throwTimer) ? 1.f : 2.f));
    Vec3f next = { pos->x + velocity->x + CLAMP(actor->colChkInfo.displacement.x, -12.f, 12.f), pos->y + velocity->y,
                   pos->z + velocity->z + CLAMP(actor->colChkInfo.displacement.z, -12.f, 12.f) };
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    int blocked = BgCheck_EntityLineTest1(&play->colCtx, pos, &next, &hit, &poly, 1, 1, 1, 0, &bgId);
    if (velocity->y <= 0.f && (!blocked || poly->normal.y > 0x4000))
    {
        Vec3f probe = blocked ? hit : next;
        probe.y = pos->y + 4.f;
        CollisionPoly* floor = NULL;
        f32 floorHeight = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &probe);
        f32 extent = 400.f * Minislime_Scale(slime).y;
        if (floor && floorHeight <= pos->y + 1.f && next.y - extent <= floorHeight)
        {
            Minislime_Land(slime, play, &probe, floor, floorHeight);
            return;
        }
    }
    actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    if (!blocked)
    {
        *pos = next;
    }
    else if (poly->normal.y < -0x4000)
    {
        velocity->y = MIN(0.f, velocity->y);
    }
    else
    {
        f32 nx = poly->normal.x * (1.f / 32767.f);
        f32 nz = poly->normal.z * (1.f / 32767.f);
        f32 dot = velocity->x * nx + velocity->z * nz;
        velocity->x -= 2.f * dot * nx;
        velocity->z -= 2.f * dot * nz;
    }
}

static int Minislime_FollowCommand(EnemyMinislime* slime, PlayState* play, Player* player)
{
    Actor* actor = &slime->actor;
    if (slime->mode == MINISLIME_ACTIVE)
        return 0;
    if (actor->flags & MINISLIME_HOOK_ATTACHED)
        Minislime_Detach(actor, play);
    slime->wasHooked = 0;
    slime->body.base.acFlags &= ~AC_HIT;
    if (slime->mode == MINISLIME_DEFEAT_IDLE)
    {
        actor->velocity = (Vec3f){ 0.f, 0.f, 0.f };
        slime->bounce = slime->bounceWindup = slime->throwTimer = 0;
        return 1;
    }
    Vec3f goal = slime->owner->world.pos;
    Vec3f next = actor->world.pos;
    if (slime->mode == MINISLIME_REGROUP)
    {
        if (slime->frozenAlpha > 20)
            Minislime_BreakIce(slime);
        slime->melt = slime->frozenAlpha = 0;
        goal.y += 20.f;
        slime->bounce = slime->bounceWindup = slime->throwTimer = slime->knockback = 0;
        slime->motionScale = (Vec3f){ .095f, .095f, .15f };
        actor->shape.rot.x = Math_Vec3f_Pitch(&actor->world.pos, &goal);
        actor->shape.rot.y = Math_Vec3f_Yaw(&actor->world.pos, &goal);
        actor->shape.rot.z = 0;
        Vec3f delta = { goal.x - next.x, goal.y - next.y, goal.z - next.z };
        f32 length = sqrtf(SQ(delta.x) + SQ(delta.y) + SQ(delta.z));
        if (length > 0.f)
        {
            f32 step = MIN(1.f, 15.f / length);
            next.x += delta.x * step;
            next.y += delta.y * step;
            next.z += delta.z * step;
        }
        enemyFlightSlide(play, &actor->world.pos, &next);
        delta = (Vec3f){ goal.x - actor->world.pos.x, goal.y - actor->world.pos.y, goal.z - actor->world.pos.z };
        if (SQ(delta.x) + SQ(delta.y) + SQ(delta.z) < SQ(12.f))
            Actor_Kill(actor);
    }
    else
    {
        slime->bounce = slime->bounceWindup = slime->idleTimer = 0;
        Math_StepToF(&slime->motionScale.x, .15f, .0075f);
        Math_StepToF(&slime->motionScale.y, .075f, .00375f);
        Math_StepToF(&slime->motionScale.z, .15f, .0075f);
        goal.y += 80.f;
        Math_StepToF(&next.x, goal.x, 10.f);
        Math_StepToF(&next.y, goal.y, 10.f);
        Math_StepToF(&next.z, goal.z, 10.f);
        if (enemyFlightClear(play, &actor->world.pos, &next))
            actor->world.pos = next;
        if (slime->launch)
        {
            slime->mode = MINISLIME_ACTIVE;
            slime->launch = slime->bounce = slime->bounceWindup = slime->idleTimer = 0;
            actor->velocity = (Vec3f){ 0.f, 0.f, 0.f };
            actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
            if (Math_Vec3f_DistXYZ(&actor->world.pos, &goal) < 24.f)
            {
                s16 yaw = Math_Vec3f_Yaw(&actor->world.pos, &player->actor.world.pos);
                f32 dx = player->actor.world.pos.x - actor->world.pos.x;
                f32 dz = player->actor.world.pos.z - actor->world.pos.z;
                f32 distance = MAX(200.f, sqrtf(dx * dx + dz * dz));
                actor->velocity =
                    (Vec3f){ 17.5f * Math_SinS(yaw), (distance - 200.f) * .01f + 3.f, 17.5f * Math_CosS(yaw) };
                slime->throwTimer = 10;
                actor->world.rot.y = yaw;
            }
            Minislime_Move(slime, play);
        }
    }
    return 1;
}

static void Minislime_UpdateFreeze(EnemyMinislime* slime, PlayState* play)
{
    Actor* actor = &slime->actor;
    if (slime->freeze == 80)
    {
        slime->frozenAlpha = MIN(200, slime->frozenAlpha + 10);
        MinislimeSfx(actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
        if (slime->frozenAlpha == 200)
            --slime->freeze;
    }
    else if (slime->freeze == 1)
    {
        Minislime_BreakIce(slime);
        slime->idleTimer = 20;
        slime->bounce = slime->bounceWindup = 0;
    }
    else
    {
        --slime->freeze;
        if (slime->freeze == 40)
        {
            slime->freezeAnchor = actor->world.pos;
        }
        else if (slime->freeze < 20 || (slime->freeze < 40 && (slime->freeze & 1)))
        {
            f32 inverse = 1.f / slime->freeze;
            Vec3f next = actor->world.pos;
            f32 offset = inverse + Rand_ZeroOne() * inverse;
            next.x = slime->freezeAnchor.x + (Rand_ZeroOne() < .5f ? -offset : offset);
            offset = inverse + Rand_ZeroOne() * inverse;
            next.z = slime->freezeAnchor.z + (Rand_ZeroOne() < .5f ? -offset : offset);
            if (enemyFlightClear(play, &actor->world.pos, &next))
                actor->world.pos = next;
        }
    }
    if (!slime->bounce)
        Minislime_Move(slime, play);
}

static void Minislime_UpdateGroundMotion(EnemyMinislime* slime, PlayState* play)
{
    Actor* actor = &slime->actor;
    if (slime->throwTimer)
    {
        Minislime_Move(slime, play);
        return;
    }
    if (slime->bounceWindup)
    {
        Math_StepToF(&slime->motionScale.x, .18f, .01f);
        Math_StepToF(&slime->motionScale.y, .05f, .01f);
        slime->motionScale.z = slime->motionScale.x;
        if (--slime->bounceWindup)
            return;
        s16 yaw = actor->yawTowardsPlayer;
        actor->velocity = (Vec3f){ Math_SinS(yaw), 12.f, Math_CosS(yaw) };
        actor->shape.rot.y = yaw;
        Minislime_Move(slime, play);
        return;
    }
    if (slime->bounce)
    {
        --slime->bounce;
        f32 amplitude = (slime->bounce / 6) + 1.f;
        s16 phase = slime->bounce * 0x8000 / 3;
        slime->motionScale.x = slime->motionScale.z = .15f + Math_CosS(phase) * amplitude / 150.f;
        slime->motionScale.y = .075f + Math_SinS(phase) * amplitude * .005f;
        if (actor->xzDistToPlayer < 150.f && slime->bounce < 38)
        {
            slime->bounce = 0;
            slime->bounceWindup = 1;
            MinislimeSfx(actor, 0x38d9);
        }
        return;
    }
    if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND))
    {
        int rising = actor->velocity.y > 0.f;
        Math_StepToF(&slime->motionScale.x, rising ? .095f : .18f, rising ? .02f : .003f);
        Math_StepToF(&slime->motionScale.y, rising ? .107f : .05f, rising ? .02f : .003f);
        slime->motionScale.z = slime->motionScale.x;
        Minislime_Move(slime, play);
        return;
    }
    if (actor->xzDistToPlayer < 150.f)
    {
        slime->bounceWindup = 4;
        slime->idleTimer = 0;
        MinislimeSfx(actor, 0x38d9);
        return;
    }
    if (!slime->idleTimer || !--slime->idleTimer)
    {
        slime->idleTimer = 20;
        if (actor->xzDistToPlayer < 300.f)
            actor->shape.rot.y = actor->yawTowardsPlayer;
        else if (SQ(actor->world.pos.x - actor->home.pos.x) + SQ(actor->world.pos.z - actor->home.pos.z) < SQ(200.f))
            actor->shape.rot.y = Math_Vec3f_Yaw(&actor->world.pos, &actor->home.pos);
        else
            actor->shape.rot.y += (s32)Rand_Next() >> 19;
    }
    s16 phase = slime->idleTimer * (0x8000 / 10);
    f32 wave = Math_SinS(phase);
    Math_StepToF(&slime->motionScale.x, .15f + .014f * wave, .01f);
    Math_StepToF(&slime->motionScale.y, .075f + .007f * Math_CosS(phase), .01f);
    Math_StepToF(&slime->motionScale.z, .3f - slime->motionScale.x, .01f);
    f32 speed = MAX(0.f, 1.5f * wave);
    actor->velocity.x = speed * Math_SinS(actor->shape.rot.y);
    actor->velocity.z = speed * Math_CosS(actor->shape.rot.y);
    Minislime_Move(slime, play);
}

static void Minislime_Update(Actor* actor, PlayState* play)
{
    EnemyMinislime* slime = (EnemyMinislime*)actor;
    if (!slime->owner || !slime->owner->update || slime->owner->room != actor->room)
    {
        Minislime_Detach(actor, play);
        Actor_Kill(actor);
        return;
    }
    Minislime_UpdateEffects(slime);
    Player* player = GET_PLAYER(play);
    if (Minislime_FollowCommand(slime, play, player))
        return;
    if (actor->flags & MINISLIME_HOOK_ATTACHED)
    {
        slime->wasHooked = 1;
        slime->body.base.acFlags &= ~AC_HIT;
        if (enemyFlightClear(play, &slime->hookSafePos, &actor->world.pos))
        {
            slime->hookSafePos = actor->world.pos;
        }
        else
        {
            actor->world.pos = slime->hookSafePos;
            Minislime_Detach(actor, play);
        }
        return;
    }
    if (slime->wasHooked)
    {
        slime->wasHooked = slime->bounce = slime->bounceWindup = slime->idleTimer = 0;
        actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        actor->velocity = (Vec3f){ 0.f, 0.f, 0.f };
    }
    if (slime->shattering)
        actor->shape.rot.x += slime->spin;
    if (slime->throwTimer && !--slime->throwTimer)
        slime->throwTimer = 10;
    if (slime->body.base.atFlags & AT_HIT)
    {
        slime->body.base.atFlags &= ~AT_HIT;
        slime->attackDelay = 10;
    }
    Minislime_Hit(slime, player);
    if (slime->melt)
    {
        --slime->melt;
        slime->frozenAlpha = MIN(200, slime->melt * 10);
        if (slime->melt % 25 == 0)
            Minislime_SpawnSmoke(slime, play);
        MinislimeSfx(actor, 0x2944 - SFX_FLAG);
        Minislime_Move(slime, play);
        if (!slime->melt)
        {
            slime->idleTimer = 20;
            slime->bounce = slime->bounceWindup = 0;
        }
    }
    else if (slime->freeze)
    {
        Minislime_UpdateFreeze(slime, play);
    }
    else if (slime->knockback)
    {
        if (!--slime->knockback)
        {
            slime->idleTimer = 20;
            slime->motionScale = (Vec3f){ .15f, .075f, .15f };
        }
        Vec3f* velocity = &actor->velocity;
        f32 speed = sqrtf(velocity->x * velocity->x + velocity->z * velocity->z);
        f32 factor = speed > 1.f ? (speed - 1.f) / speed : 0.f;
        velocity->x *= factor;
        velocity->z *= factor;
        Minislime_Move(slime, play);
    }
    else
    {
        Minislime_UpdateGroundMotion(slime, play);
    }

    Vec3f scale = Minislime_Scale(slime);
    if (actor->bgCheckFlags & BGCHECKFLAG_GROUND)
        actor->world.pos.y = actor->floorHeight + scale.y * 400.f;
    slime->hookSafePos = actor->world.pos;
    actor->focus.pos = actor->world.pos;
    Collider_UpdateCylinder(actor, &slime->body);
    slime->body.dim.radius = scale.x * 360.f;
    slime->body.dim.height = scale.y * 800.f;
    slime->body.dim.yShift = scale.y * -400.f;
    if (!slime->shattering || actor->velocity.y < 0.f)
        CollisionCheck_SetOC(play, &play->colChkCtx, &slime->body.base);
    if (!slime->shattering)
    {
        if (!slime->melt && slime->knockback <= 15)
            CollisionCheck_SetAC(play, &play->colChkCtx, &slime->body.base);
        if (!slime->freeze && !slime->attackDelay && actor->xzDistToPlayer < 160.f)
        {
            Vec3f target = player->actor.world.pos;
            target.y += 20.f;
            if (enemyFlightClear(play, &actor->world.pos, &target))
                CollisionCheck_SetAT(play, &play->colChkCtx, &slime->body.base);
        }
    }
    if (slime->attackDelay)
        --slime->attackDelay;
}

static void Minislime_Draw(Actor* actor, PlayState* play)
{
    EnemyMinislime* slime = (EnemyMinislime*)actor;
    GraphicsContext* gfx = play->state.gfxCtx;
    Vec3f* pos = &actor->world.pos;
    Vec3f scale = Minislime_Scale(slime);
    Gfx_SetupDL25_Xlu(gfx);
    OPEN_DISPS(gfx);
    Matrix_Push();
    Matrix_SetTranslateRotateYXZ(pos->x, pos->y, pos->z, &actor->shape.rot);
    Matrix_Scale(scale.x, scale.y, scale.z, MTXMODE_APPLY);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 255, 255, 255, 255);
    gSPMatrix(POLY_XLU_DISP++, MinislimeToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
              G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_XLU_DISP++, 0x060113b0);
    if (slime->frozenAlpha)
    {
        f32 iceScale = slime->melt ? slime->melt * .0025f : .1f;
        u8 alpha = slime->frozenAlpha;
        Matrix_Translate(0.f, (.1f - iceScale) * -4000.f, 0.f, MTXMODE_APPLY);
        Matrix_Scale(.1f, iceScale, .1f, MTXMODE_APPLY);
        gSPSegment(POLY_XLU_DISP++, 8, Gfx_TwoTexScroll(gfx, 0, 0, 0, 64, 64, 1, 0, 0u - play->gameplayFrames, 32, 32));
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 255, 255, 255, alpha);
        gSPMatrix(POLY_XLU_DISP++, MinislimeToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, 0x06010db0);
    }
    Matrix_Pop();
    int shardMaterial = 0;
    for (int i = 0; i < ARRAY_COUNT(slime->shards); ++i)
    {
        MinislimeIceShard* shard = &slime->shards[i];
        if (!shard->scale)
            continue;
        if (!shardMaterial)
        {
            gSPSegment(POLY_XLU_DISP++, 8,
                       Gfx_TwoTexScroll(gfx, 0, 0, 0, 64, 64, 1, 0, 0u - play->gameplayFrames, 32, 32));
            gSPDisplayList(POLY_XLU_DISP++, 0x06010f20);
            shardMaterial = 1;
        }
        Matrix_Push();
        Matrix_SetTranslateRotateYXZ(shard->pos.x, shard->pos.y, shard->pos.z, &shard->rotation);
        Matrix_Scale(shard->scale, shard->scale, shard->scale, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, MinislimeToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, 0x06010fe0);
        Matrix_Pop();
    }
#if defined(GAME_OOT)
    u32 smokeMaterial = 0, smokeModel = 0;
    for (int i = 0; i < ARRAY_COUNT(slime->smoke); ++i)
    {
        int life = slime->smoke[i].life;
        if (!life)
            continue;
        if (!smokeMaterial)
        {
            smokeMaterial = (u32)comboEnemyKeepAsset(0x1001, 0x51180) & 0x1fffffff;
            smokeModel = (u32)comboEnemyKeepAsset(0x1001, 0x51238) & 0x1fffffff;
            if (!smokeMaterial || !smokeModel)
                break;
            Gfx_SetupDL25_Xlu(gfx);
            gSPDisplayList(POLY_XLU_DISP++, smokeMaterial);
        }
        Vec3f* smoke = &slime->smoke[i].pos;
        u8 alpha = MIN(100, 10 * MIN(life, 50 - life));
        Matrix_Push();
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 195, 235, 235, alpha);
        gSPSegment(POLY_XLU_DISP++, 8, Gfx_TwoTexScroll(gfx, 0, life * 3, life * 15, 32, 64, 1, 0, 0, 32, 32));
        Matrix_Translate(smoke->x, smoke->y, smoke->z, MTXMODE_NEW);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_Scale(.05f, .05f, 1.f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, MinislimeToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, smokeModel);
        Matrix_Pop();
    }
#endif
    CLOSE_DISPS();
}

static ActorProfile sMinislimeProfile = {
    ACTOR_ENEMY_MINISLIME, ACTORCAT_PROP,          ACTOR_FLAG_UPDATE_CULLING_DISABLED | MINISLIME_HOOK_PULL,
    MinislimeObject,       sizeof(EnemyMinislime), Minislime_Init,
    Minislime_Destroy,     Minislime_Update,       Minislime_Draw,
};
OVL_INFO_ACTOR(ACTOR_ENEMY_MINISLIME, sMinislimeProfile);
