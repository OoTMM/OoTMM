#include <combo/enemy_eyegore_native.h>
#include <combo/enemy_native.h>
#include <combo/global.h>

typedef struct {
    u8 count, additional;
    s16 value;
    void* points;
} NativeEyegorePath;
typedef struct {
    Actor actor;
    SkelAnime skeleton;
    Vec3s joints[24], morph[24];
    ActorFunc actionFunc;
    NativeEyegorePath* path;
    s16 waypoint, action, waitTimer, eyeTimer, headRot, switchFlag, dmgTimer;
    Vec3f parts[12];
    s16 partIndex, pathIndex;
    s32 animation;
    u8 retreating;
    f32 animationEnd, wakeupRange, minLaserRange;
    u8 pad368[0x378 - 0x368];
    s16 laserImpactTimer;
    u8 pad37a[0x3a8 - 0x37a];
    Vec3f laserBase, laserScale, laserScaleTarget;
    u8 pad3cc[0x408 - 0x3cc];
    Vec3f waypointPos;
    Vec3s pupilRot, laserRot;
    u8 pad420[0x42c - 0x420];
    s16 laserState;
} NativeEyegore;
_Static_assert(__builtin_offsetof(NativeEyegore, path) == 0x2ac, "Eyegore path");
_Static_assert(__builtin_offsetof(NativeEyegore, waypointPos) == 0x408, "Eyegore waypoint");
_Static_assert(__builtin_offsetof(NativeEyegore, laserBase) == 0x3a8, "Eyegore laser origin");
_Static_assert(__builtin_offsetof(NativeEyegore, laserState) == 0x42c, "Eyegore laser state");
enum
{
    EYEGORE_WAIT,
       EYEGORE_STAND,
       EYEGORE_WALK,
       EYEGORE_RETREAT,
       EYEGORE_LASER,
       EYEGORE_STOP = 12 };
enum
{
    EYEGORE_FIRE = 3,
       EYEGORE_BEAM_ON = 7 };

static NativeEyegorePath sEyegorePath = { 2, 0, 0, NULL };
static Actor* sEyegoreActor;
static PlayState* sEyegorePlay;
static int sEyegoreConstructor;
static void eyegoreGoal(NativeEyegore* egol, PlayState* play, Vec3f* goal)
{
    Actor* player = &GET_PLAYER(play)->actor;
    *goal = player->world.pos;
    if (egol->retreating)
    {
        f32 dx = egol->actor.world.pos.x - goal->x, dz = egol->actor.world.pos.z - goal->z;
        f32 distance = sqrtf(dx * dx + dz * dz);
        if (distance > 1.f)
        {
            dx /= distance;
            dz /= distance;
        }
        else
        {
            dx = -Math_SinS(egol->actor.yawTowardsPlayer);
            dz = -Math_CosS(egol->actor.yawTowardsPlayer);
        }
        goal->x = egol->actor.world.pos.x + dx * 100.f;
        goal->y = egol->actor.world.pos.y;
        goal->z = egol->actor.world.pos.z + dz * 100.f;
    }
}

void* comboEnemyNativeEyegorePath(PlayState* play, s16 index, s16 none)
{
    if ((sEyegoreConstructor || sEyegoreActor) && index == 0)
        return &sEyegorePath;
    return ((void* (*)(PlayState*, s16, s16))0x8013d648)(play, index, none);
}

s32 comboEnemyNativeEyegorePoint(void* path, s32 index, Vec3f* point)
{
    if (path == &sEyegorePath && sEyegoreActor && sEyegorePlay)
    {
        eyegoreGoal((NativeEyegore*)sEyegoreActor, sEyegorePlay, point);
        return 1;
    }
    if (path == &sEyegorePath)
    {
        g.roomEnemyLackSoul = 1;
        return 0;
    }
    return ((s32(*)(void*, s32, Vec3f*))0x8013d68c)(path, index, point);
}

static void eyegoreUpdate(Actor* actor, PlayState* play)
{
    NativeEyegore* egol = (NativeEyegore*)actor;
    ActorFunc update = actorAddr(0x184, 0x80a7f354);
    Actor* previous = sEyegoreActor;
    PlayState* previousPlay = sEyegorePlay;
    sEyegoreActor = actor;
    sEyegorePlay = play;
    s16 action = egol->action;
    s16 homeYaw = actor->home.rot.y;
    Vec3f before = actor->world.pos;
    if (actor->colChkInfo.health)
    {
        if (action == EYEGORE_STOP)
            actor->home.rot.y = actor->yawTowardsPlayer;
        egol->waypoint = 0;
        if (action == EYEGORE_WALK)
            eyegoreGoal(egol, play, &egol->waypointPos);
        if (action == EYEGORE_WAIT || action == EYEGORE_WALK || action == EYEGORE_STAND ||
            egol->actionFunc == actorAddr(0x184, 0x80a7d168) ||
            (action == EYEGORE_LASER && egol->laserState < EYEGORE_FIRE))
        {
            Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 1, 0x7d0, 0);
            actor->shape.rot.y = actor->world.rot.y;
        }
        if (action == EYEGORE_LASER && egol->laserState == EYEGORE_BEAM_ON &&
            !egol->waitTimer && !egol->laserImpactTimer)
            egol->laserImpactTimer = 1;
    }
    update(actor, play);
    actor->home.rot.y = homeYaw;
    if (egol->action == EYEGORE_WALK)
        actor->shape.rot.y = actor->world.rot.y;
    if ((action == EYEGORE_WALK || action == EYEGORE_RETREAT) &&
        !Eyegore_CanMove(play, actor, before, actor->world.pos))
    {
        actor->world.pos.x = before.x;
        actor->world.pos.z = before.z;
        if (action == EYEGORE_RETREAT && egol->action == EYEGORE_RETREAT)
            ((void (*)(NativeEyegore*))actorAddr(0x184, 0x80a7e2e8))(egol);
    }
    sEyegoreActor = previous;
    sEyegorePlay = previousPlay;
}

static void eyegoreDraw(Actor* actor, PlayState* play)
{
    NativeEyegore* egol = (NativeEyegore*)actor;
    s16 state = egol->laserState;
    if (state >= EYEGORE_BEAM_ON && egol->laserScale.z > 0.f)
    {
        s16 yaw = actor->world.rot.y;
        s16 pitch = egol->laserRot.x;
        f32 length = MIN(egol->laserScale.z, 1.4f) * 700.f;
        f32 horizontal = length * Math_CosS(pitch);
        Vec3f beam = { Math_SinS(yaw) * horizontal, -Math_SinS(pitch) * length, Math_CosS(yaw) * horizontal };
        f32 fraction = Eyegore_LaserWallFraction(play, egol->laserBase, beam);
        egol->laserScale.z = length * fraction / 700.f;
        if (fraction < 1.f && !egol->laserImpactTimer)
            egol->laserImpactTimer = 1;
        else if (egol->waitTimer <= 8 && !Eyegore_LaserHitsSurface(play, egol->laserBase, beam))
        {
            egol->laserScale.x *= .65f;
            egol->laserScale.y *= .65f;
            egol->laserScaleTarget.x = egol->laserScaleTarget.y = 0.f;
        }
    }
    ((ActorFunc)actorAddr(0x184, 0x80a7ffb8))(actor, play);
    egol->laserScaleTarget.z = MIN(egol->laserScaleTarget.z, 1.4f);
    if (state == EYEGORE_FIRE && egol->laserState == EYEGORE_BEAM_ON)
        egol->waitTimer = 35;
}

static void eyegoreActivate(Actor* actor, PlayState* play)
{
    NativeEyegore* egol = (NativeEyegore*)actor;
    void (*stand)(NativeEyegore*) = actorAddr(0x184, 0x80a7d1e4);
    if (!actor->update || !stand)
        return;
    u32 previousSegment = gSegments[6];
    Actor_SetObjectDependency(play, actor);
    if (!gSegments[6])
    {
        gSegments[6] = previousSegment;
        return;
    }
    egol->path = &sEyegorePath;
    egol->pathIndex = 0;
    egol->waypoint = 0;
    egol->switchFlag = -1;
    egol->wakeupRange = 300.f;
    egol->minLaserRange = 150.f;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    stand(egol);
    gSegments[6] = previousSegment;
    actor->update = eyegoreUpdate;
    actor->draw = eyegoreDraw;
}

static void eyegoreInit(Actor* actor, PlayState* play)
{
    ActorFunc init = actorAddr(0x184, 0x80a7cd60);
    int previous = sEyegoreConstructor;
    actor->init = NULL;
    sEyegoreConstructor = 1;
    init(actor, play);
    sEyegoreConstructor = previous;
    eyegoreActivate(actor, play);
}

Actor* comboEnemyNativeEyegoreSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    int previous;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != 0x184 || (u16)entry->params != 0x1fc0)
        return NULL;
    previous = sEyegoreConstructor;
    sEyegoreConstructor = 1;
    actor = ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)(ctx, entry, play);
    sEyegoreConstructor = previous;
    if (actor && actor->init)
        actor->init = eyegoreInit;
    else if (actor)
        eyegoreActivate(actor, play);
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_EYEGORE_V1, comboEnemyNativeEyegoreSpawn);
EXPORT_SYMBOL(ENEMY_NATIVE_EYEGORE_PATH_V1, comboEnemyNativeEyegorePath);
EXPORT_SYMBOL(ENEMY_NATIVE_EYEGORE_POINT_V1, comboEnemyNativeEyegorePoint);
