#include <combo.h>

typedef s32 (*EnemyOverride)(PlayState*, s32, Gfx**, Vec3f*, Vec3s*, Actor*);
typedef void (*EnemyPost)(PlayState*, s32, Gfx**, Vec3s*, Actor*);
typedef void (*EnemyTransform)(PlayState*, s32, Actor*);
typedef struct {
    EnemyOverride override;
    EnemyPost post;
    EnemyTransform transform;
    Actor* actor;
    Vec3s savedRotation;
    int pushed;
} EnemyFlexDraw;

static s32 enemyFlexOverride(PlayState* play, s32 limb, Gfx** dl, Vec3f* pos, Vec3s* rot, void* arg)
{
    EnemyFlexDraw* draw = arg;
    draw->pushed = 0;
    if (draw->override && draw->override(play, limb, dl, pos, rot, draw->actor))
        return 1;
    Matrix_TranslateRotateZYX(pos, rot);
    Matrix_Push();
    draw->transform(play, limb, draw->actor);
    draw->pushed = 1;
    draw->savedRotation = *rot;
    *pos = (Vec3f){ 0 };
    *rot = (Vec3s){ 0 };
    return 0;
}

static void enemyFlexPost(PlayState* play, s32 limb, Gfx** dl, Vec3s* rot, void* arg)
{
    EnemyFlexDraw* draw = arg;
    if (draw->pushed)
    {
        Matrix_Pop();
        *rot = draw->savedRotation;
    }
    if (draw->post)
        draw->post(play, limb, dl, rot, draw->actor);
}

static void enemyTransformFlexOpa(PlayState* play, void** limbs, Vec3s* joints, s32 count,
                                  EnemyOverride override, EnemyPost post, EnemyTransform transform, Actor* actor)
{
    EnemyFlexDraw draw = { .override = override, .post = post, .transform = transform, .actor = actor };
    SkelAnime_DrawFlexOpa(play, limbs, joints, count, enemyFlexOverride, enemyFlexPost, &draw);
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_TRANSFORM_FLEX_OPA, enemyTransformFlexOpa);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_LILYPAD_COLLISION, CollisionHeader_GetVirtual);
