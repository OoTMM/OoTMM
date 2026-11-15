#include <combo.h>
#include <combo/collision.h>
#include <combo/enemy_actor_layout.h>

#if defined(GAME_OOT)
#define AnubisLineTestAddress 0x80030940
#else
#define AnubisLineTestAddress 0x800c5650
#endif

typedef s32 (*AnubisLineTest)(CollisionContext*, Vec3f*, Vec3f*, Vec3f*,
                              CollisionPoly**, s32, s32, s32, s32, s32*, Actor*);

int comboEnemyClampWall(PlayState* play, Actor* actor, Vec3f from)
{
    if (!play)
        return 0;

    Vec3f to = actor->world.pos;
    if (from.x == to.x && from.z == to.z)
        return 0;

    Vec3f hit;
    CollisionPoly* poly = NULL;
    s32 bgId;
    from.y += 18.f;
    to.y += 18.f;
    if (!((AnubisLineTest)AnubisLineTestAddress)(&play->colCtx, &from, &to, &hit, &poly,
                                                 1, 0, 0, 0, &bgId, actor))
        return 0;

    actor->world.pos.x = from.x;
    actor->world.pos.z = from.z;
    actor->velocity.x = 0.f;
    actor->velocity.z = 0.f;
    actor->speed = 0.f;
    return 1;
}

void comboAnubisUpdateBg(PlayState* play, Actor* actor, float height, float radius, float ceiling, s32 flags)
{
#if defined(GAME_OOT)
    if (((EnemyNativeAnubis*)actor)->isPlayerOutOfRange && actor->shape.yOffset < -4220.f)
    {
        actor->world.pos = actor->prevPos = ((EnemyNativeAnubis*)actor)->home;
        actor->velocity.y = 0.f;
    }
#endif
    int crossed = (actor->flags & ACTOR_FLAG_ATTENTION_ENABLED) &&
                  comboEnemyClampWall(play, actor, actor->prevPos);
    if (actor->flags & ACTOR_FLAG_ATTENTION_ENABLED)
    {
        flags |= UPDBGCHECKINFO_FLAG_WALL;
        if (radius < 14.f)
            radius = 14.f;
    }
    Actor_UpdateBgCheckInfo(play, actor, height, radius, ceiling, flags);
    if (crossed)
        actor->bgCheckFlags |= BGCHECKFLAG_WALL;
}

static void anubisMirrorStep(Actor* actor, float* coordinate, float target, float fraction, float step)
{
    Vec3f from = actor->world.pos;
    Math_ApproachF(coordinate, target, fraction, step);
    if (actor->flags & ACTOR_FLAG_ATTENTION_ENABLED)
        comboEnemyClampWall(gPlay, actor, from);
}

void comboAnubisMirrorX(float* coordinate, float target, float fraction, float step)
{
    Actor* actor = (Actor*)((u8*)coordinate - __builtin_offsetof(Actor, world.pos.x));
    anubisMirrorStep(actor, coordinate, target, fraction, step);
}

void comboAnubisMirrorZ(float* coordinate, float target, float fraction, float step)
{
    Actor* actor = (Actor*)((u8*)coordinate - __builtin_offsetof(Actor, world.pos.z));
    anubisMirrorStep(actor, coordinate, target, fraction, step);
}

#if defined(GAME_OOT)
PATCH_CALL(0x809ec9d8, comboAnubisUpdateBg);
PATCH_CALL(0x809eca04, comboAnubisUpdateBg);
PATCH_CALL(0x80a30148, comboAnubisMirrorX);
PATCH_CALL(0x80a30174, comboAnubisMirrorZ);
#endif
EXPORT_SYMBOL(ENEMY_ANUBIS_WALL_V1, comboAnubisUpdateBg);
