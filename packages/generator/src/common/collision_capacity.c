#include <combo.h>

#define COLLIDER_CAPACITY 512

typedef s32 (*CollisionResetFunc)(PlayState*, Collider*);
typedef void (*CollisionApplyFunc)(PlayState*, CollisionCheckContext*, Collider*);
typedef void (*CollisionPairFunc)(PlayState*, CollisionCheckContext*, Collider*, Collider*);
typedef s32 (*CollisionLineFunc)(PlayState*, CollisionCheckContext*, Collider*, Vec3f*, Vec3f*);

extern CollisionResetFunc gCollisionResetAT[];
extern CollisionResetFunc gCollisionResetAC[];
extern CollisionResetFunc gCollisionResetOC[];
extern CollisionApplyFunc gCollisionHitEffects[];
extern CollisionApplyFunc gCollisionDamage[];
extern CollisionPairFunc gCollisionPairsAC[COLSHAPE_MAX][COLSHAPE_MAX];
extern CollisionPairFunc gCollisionPairsOC[COLSHAPE_MAX][COLSHAPE_MAX];
extern CollisionLineFunc gCollisionLinesOC[];
s32 FrameAdvance_IsEnabled(PlayState* play);

static Collider* sCollidersAT[COLLIDER_CAPACITY];
static Collider* sCollidersAC[COLLIDER_CAPACITY];
static Collider* sCollidersOC[COLLIDER_CAPACITY];

#if defined(GAME_MM)
static int sEnemyCombatActive;

static int enemyCombatActive(PlayState* play)
{
    for (int category = 0; category < ACTORCAT_MAX; ++category)
        for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
            if (actor->update && actor->room == play->roomCtx.curRoom.num &&
                (actor->flags & ACTOR_FLAG_HOSTILE) && actor->isTargeted)
                return 1;
    for (int i = 0; i < play->colChkCtx.colATCount && i < COLLIDER_CAPACITY; ++i) {
        Collider* col = sCollidersAT[i];
        Actor* actor = col ? col->actor : NULL;
        if (actor && actor->update && actor->room == play->roomCtx.curRoom.num &&
            (col->atFlags & AT_ON) && (col->atFlags & AT_TYPE_ENEMY) &&
            actor->xyzDistToPlayerSq < 160000.f)
            return 1;
    }
    return 0;
}

int comboEnemyCombatActive(PlayState* play)
{
    return sEnemyCombatActive || enemyCombatActive(play);
}
#endif

static Collider** collisionList(PlayState* play, CollisionCheckContext* ctx, Collider** native, Collider** extended)
{
    return ctx == &play->colChkCtx ? extended : native;
}

void comboCollisionClear(PlayState* play, CollisionCheckContext* ctx)
{
    if (ctx->sacFlags & SAC_ON)
        return;
#if defined(GAME_MM)
    if (ctx == &play->colChkCtx)
        sEnemyCombatActive = enemyCombatActive(play);
#endif
    ctx->colATCount = 0;
    ctx->colACCount = 0;
    ctx->colOCCount = 0;
    ctx->colLineCount = 0;
    bzero(ctx->colAT, sizeof(ctx->colAT));
    bzero(ctx->colAC, sizeof(ctx->colAC));
    bzero(ctx->colOC, sizeof(ctx->colOC));
    bzero(ctx->colLine, sizeof(ctx->colLine));
    if (ctx == &play->colChkCtx)
    {
        bzero(sCollidersAT, sizeof(sCollidersAT));
        bzero(sCollidersAC, sizeof(sCollidersAC));
        bzero(sCollidersOC, sizeof(sCollidersOC));
    }
}

static int collisionReset(PlayState* play, Collider* col, CollisionResetFunc* reset)
{
    if (FrameAdvance_IsEnabled(play) || col->shape >= COLSHAPE_MAX)
        return 0;
    reset[col->shape](play, col);
    return !col->actor || col->actor->update;
}

#define COLLISION_REGISTER(kind, reset) \
s32 comboCollisionSet##kind(PlayState* play, CollisionCheckContext* ctx, Collider* col) \
{ \
    int capacity = ctx == &play->colChkCtx ? COLLIDER_CAPACITY : ARRAY_COUNT(ctx->col##kind); \
    if (!collisionReset(play, col, reset) || ctx->col##kind##Count >= capacity || (ctx->sacFlags & SAC_ON)) \
        return -1; \
    int index = ctx->col##kind##Count++; \
    collisionList(play, ctx, ctx->col##kind, sColliders##kind)[index] = col; \
    return index; \
} \
s32 comboCollisionSet##kind##SAC(PlayState* play, CollisionCheckContext* ctx, Collider* col, s32 index) \
{ \
    if (!(ctx->sacFlags & SAC_ON)) \
        return comboCollisionSet##kind(play, ctx, col); \
    if (!collisionReset(play, col, reset) || index < 0 || index >= ctx->col##kind##Count) \
        return -1; \
    collisionList(play, ctx, ctx->col##kind, sColliders##kind)[index] = col; \
    return index; \
}

COLLISION_REGISTER(AT, gCollisionResetAT)
COLLISION_REGISTER(AC, gCollisionResetAC)
COLLISION_REGISTER(OC, gCollisionResetOC)

void comboCollisionHitEffects(PlayState* play, CollisionCheckContext* ctx)
{
    Collider** colliders = collisionList(play, ctx, ctx->colAC, sCollidersAC);
    for (int i = 0; i < ctx->colACCount; ++i)
    {
        Collider* col = colliders[i];
        if (col && (col->acFlags & AC_ON) && (!col->actor || col->actor->update))
            gCollisionHitEffects[col->shape](play, ctx, col);
    }
}

void comboCollisionAC(PlayState* play, CollisionCheckContext* ctx, Collider* at)
{
    Collider** colliders = collisionList(play, ctx, ctx->colAC, sCollidersAC);
    for (int i = 0; i < ctx->colACCount; ++i)
    {
        Collider* ac = colliders[i];
        if (!ac || !(ac->acFlags & AC_ON) || (ac->actor && !ac->actor->update))
            continue;
        if (!(ac->acFlags & at->atFlags & AC_TYPE_ALL) || at == ac)
            continue;
        if (!(at->atFlags & AT_SELF) && at->actor && ac->actor == at->actor)
            continue;
        gCollisionPairsAC[at->shape][ac->shape](play, ctx, at, ac);
    }
}

void comboCollisionAT(PlayState* play, CollisionCheckContext* ctx)
{
    if (!ctx->colATCount || !ctx->colACCount)
        return;
    Collider** colliders = collisionList(play, ctx, ctx->colAT, sCollidersAT);
    for (int i = 0; i < ctx->colATCount; ++i)
    {
        Collider* col = colliders[i];
        if (col && (col->atFlags & AT_ON) && (!col->actor || col->actor->update))
            comboCollisionAC(play, ctx, col);
    }
    comboCollisionHitEffects(play, ctx);
}

void comboCollisionOC(PlayState* play, CollisionCheckContext* ctx)
{
    Collider** colliders = collisionList(play, ctx, ctx->colOC, sCollidersOC);
    for (int i = 0; i < ctx->colOCCount; ++i)
    {
        Collider* left = colliders[i];
        if (!left || !(left->ocFlags1 & OC1_ON))
            continue;
        for (int j = i + 1; j < ctx->colOCCount; ++j)
        {
            Collider* right = colliders[j];
            if (!right || !(right->ocFlags1 & OC1_ON) || left->actor == right->actor)
                continue;
            if (!(left->ocFlags1 & right->ocFlags2 & OC1_TYPE_ALL) ||
                !(left->ocFlags2 & right->ocFlags1 & OC1_TYPE_ALL) ||
                ((left->ocFlags2 & OC2_UNK1) && (right->ocFlags2 & OC2_UNK2)) ||
                ((right->ocFlags2 & OC2_UNK1) && (left->ocFlags2 & OC2_UNK2)))
                continue;
            CollisionPairFunc collide = gCollisionPairsOC[left->shape][right->shape];
            if (collide)
                collide(play, ctx, left, right);
        }
    }
}

void comboCollisionDamage(PlayState* play, CollisionCheckContext* ctx)
{
    Collider** colliders = collisionList(play, ctx, ctx->colAC, sCollidersAC);
    for (int i = 0; i < ctx->colACCount; ++i)
    {
        Collider* col = colliders[i];
        if (col && !(col->acFlags & AC_NO_DAMAGE))
            gCollisionDamage[col->shape](play, ctx, col);
    }
}

s32 comboCollisionLineOC(PlayState* play, CollisionCheckContext* ctx, Vec3f* a, Vec3f* b,
                         Actor** exclusions, s32 numExclusions)
{
    Collider** colliders = collisionList(play, ctx, ctx->colOC, sCollidersOC);
    for (int i = 0; i < ctx->colOCCount; ++i)
    {
        Collider* col = colliders[i];
        if (!col || !(col->ocFlags1 & OC1_ON))
            continue;
        int excluded = 0;
        for (int j = 0; j < numExclusions; ++j)
        {
            if (col->actor == exclusions[j])
            {
                excluded = 1;
                break;
            }
        }
        if (excluded)
            continue;
        CollisionLineFunc check = gCollisionLinesOC[col->shape];
        if (check && check(play, ctx, col, a, b))
            return 1;
    }
    return 0;
}

#if defined(GAME_OOT)
PATCH_FUNC(0x8004bc78, comboCollisionClear);
PATCH_FUNC(0x8004bd50, comboCollisionSetAT);
PATCH_FUNC(0x8004be30, comboCollisionSetATSAC);
PATCH_FUNC(0x8004bf40, comboCollisionSetAC);
PATCH_FUNC(0x8004c020, comboCollisionSetACSAC);
PATCH_FUNC(0x8004c130, comboCollisionSetOC);
PATCH_FUNC(0x8004c210, comboCollisionSetOCSAC);
PATCH_FUNC(0x8004f610, comboCollisionHitEffects);
PATCH_FUNC(0x8004f6e4, comboCollisionAC);
PATCH_FUNC(0x8004f80c, comboCollisionAT);
PATCH_FUNC(0x80050118, comboCollisionOC);
PATCH_FUNC(0x80050718, comboCollisionDamage);
PATCH_FUNC(0x80050968, comboCollisionLineOC);
#else
PATCH_FUNC(0x800e2480, comboCollisionClear);
PATCH_FUNC(0x800e2558, comboCollisionSetAT);
PATCH_FUNC(0x800e2634, comboCollisionSetATSAC);
PATCH_FUNC(0x800e2740, comboCollisionSetAC);
PATCH_FUNC(0x800e281c, comboCollisionSetACSAC);
PATCH_FUNC(0x800e2928, comboCollisionSetOC);
PATCH_FUNC(0x800e2a04, comboCollisionSetOCSAC);
PATCH_FUNC(0x800e6450, comboCollisionHitEffects);
PATCH_FUNC(0x800e6524, comboCollisionAC);
PATCH_FUNC(0x800e6654, comboCollisionAT);
PATCH_FUNC(0x800e7308, comboCollisionOC);
PATCH_FUNC(0x800e7988, comboCollisionDamage);
PATCH_FUNC(0x800e7c64, comboCollisionLineOC);
#endif
