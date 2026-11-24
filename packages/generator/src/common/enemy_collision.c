#include <combo.h>
#include <combo/enemy.h>
#include <combo/dma.h>

static void enemyForgetMask(ColliderElement* element);

#if defined(GAME_OOT)
typedef struct
{
    Collider base;
    ColliderJntSphElement element;
} EnemySourceSphere;
typedef struct
{
    ColliderInit base;
    ColliderJntSphElementInit element;
} EnemySourceSphereInit;
_Static_assert(sizeof(EnemySourceSphere) == 0x58, "MM sphere ABI");
_Static_assert(sizeof(EnemySourceSphereInit) == 0x2c, "MM sphere initializer ABI");
_Static_assert(sizeof(ColliderJntSphElement) == 0x40, "Shared sphere element ABI");

s32 Collider_InitJntSphElement(PlayState*, ColliderJntSphElement*);

typedef struct EnemySphereView {
    struct EnemySphereView* next;
    EnemySourceSphere* source;
    Actor* owner;
    ColliderJntSph native;
    u8 sourceShape;
} EnemySphereView;
static EnemySphereView* sSphereViews;

static void enemySphereToNative(EnemySphereView* view)
{
    view->sourceShape = view->source->base.shape;
    view->native.base = view->source->base;
    view->native.base.shape = COLSHAPE_JNTSPH;
    view->native.count = 1;
    view->native.elements = &view->source->element;
}

static void enemySphereToSource(EnemySphereView* view)
{
    view->source->base = view->native.base;
    view->source->base.shape = view->sourceShape;
}

static EnemySphereView* enemySphereView(EnemySourceSphere* source, int create)
{
    EnemySphereView* view;
    if (!source)
        return NULL;
    for (view = sSphereViews; view; view = view->next)
        if (view->source == source)
            return view;
    if (!create)
        return NULL;
    view = malloc(sizeof(*view));
    if (!view)
    {
        Actor* owner = comboEnemyCurrentActor();
        g.roomEnemyLackSoul = 1;
        if (owner)
            Actor_Kill(owner);
        return NULL;
    }
    memset(view, 0, sizeof(*view));
    view->source = source;
    view->owner = comboEnemyCurrentActor();
    view->next = sSphereViews;
    sSphereViews = view;
    return view;
}

s32 comboEnemyInitSphere(PlayState* play, EnemySourceSphere* source)
{
    EnemySphereView* view = enemySphereView(source, 1);
    if (!view)
        return 0;
    Collider_InitJntSph(play, &view->native);
    Collider_InitJntSphElement(play, &source->element);
    view->sourceShape = 5;
    enemySphereToSource(view);
    return 1;
}

s32 comboEnemySetSphere(PlayState* play, EnemySourceSphere* source, Actor* actor, EnemySourceSphereInit* init)
{
    ColliderJntSphInit nativeInit;
    EnemySphereView* view;
    if (!init || !source)
        return 0;
    view = enemySphereView(source, 1);
    if (!view)
        return 0;
    nativeInit.base = init->base;
    nativeInit.base.shape = COLSHAPE_JNTSPH;
    nativeInit.count = 1;
    nativeInit.elements = &init->element;
    view->owner = actor;
    view->sourceShape = init->base.shape;
    Collider_SetJntSph(play, &view->native, actor, &nativeInit, &source->element);
    enemySphereToSource(view);
    return 1;
}

s32 comboEnemyInitAndSetSphere(PlayState* play, EnemySourceSphere* source, Actor* actor, EnemySourceSphereInit* init)
{
    if (comboEnemyInitSphere(play, source))
        comboEnemySetSphere(play, source, actor, init);
    return 0;
}

s32 comboEnemyDestroySphere(PlayState* play, EnemySourceSphere* source)
{
    EnemySphereView** link = &sSphereViews;
    while (*link)
    {
        EnemySphereView* view = *link;
        if (view->source == source)
        {
            enemyForgetMask(&source->element.base);
            enemySphereToNative(view);
            Collider_DestroyJntSph(play, &view->native);
            enemySphereToSource(view);
            *link = view->next;
            free(view);
            break;
        }
        link = &view->next;
    }
    return 1;
}

void comboEnemyUpdateSphere(s32 limb, EnemySourceSphere* source)
{
    EnemySphereView* view = enemySphereView(source, 0);
    if (!view)
        return;
    enemySphereToNative(view);
    Collider_UpdateSpheres(limb, &view->native);
    enemySphereToSource(view);
}

#define ENEMY_SPHERE_RESET(kind) \
s32 comboEnemyResetSphere##kind(PlayState* play, EnemySourceSphere* source) \
{ \
    EnemySphereView* view = enemySphereView(source, 0); \
    if (!view) return 0; \
    enemySphereToNative(view); \
    Collider_ResetJntSph##kind(play, &view->native.base); \
    enemySphereToSource(view); \
    return 1; \
}

ENEMY_SPHERE_RESET(AT)
ENEMY_SPHERE_RESET(AC)
ENEMY_SPHERE_RESET(OC)

static void enemySphereEnter(Actor* actor)
{
    for (EnemySphereView* view = sSphereViews; view; view = view->next)
        if (view->owner == actor)
            enemySphereToSource(view);
}

static void enemySphereLeave(Actor* actor)
{
    for (EnemySphereView* view = sSphereViews; view; view = view->next)
        if (view->owner == actor)
            enemySphereToNative(view);
}

static void enemySphereRelease(Actor* actor)
{
    EnemySphereView** link = &sSphereViews;
    while (*link)
    {
        EnemySphereView* view = *link;
        if (view->owner == actor)
        {
            *link = view->next;
            free(view);
        }
        else
            link = &view->next;
    }
}

static void enemySphereReset(void)
{
    while (sSphereViews)
    {
        EnemySphereView* view = sSphereViews;
        sSphereViews = view->next;
        free(view);
    }
}

static Collider* enemyNativeCollider(Collider* source)
{
    if (source && source->shape == 4)
    {
        EnemySphereView* view = enemySphereView((EnemySourceSphere*)source, 0);
        if (!view)
            return NULL;
        enemySphereToNative(view);
        return &view->native.base;
    }
    return source;
}

static void enemyRegisteredCollider(Collider* source)
{
    if (source && source->shape == 4)
    {
        EnemySphereView* view = enemySphereView((EnemySourceSphere*)source, 0);
        if (view)
            enemySphereToSource(view);
    }
}

static void enemyHitYaw(Actor* actor, Collider* collider, ColliderElement* element)
{
    u32 flags;
    if (!actor || !collider || !collider->ac || !element || !element->acHitElem)
        return;
    flags = comboEnemyAttackFlags(element->acHitElem);
    actor->world.rot.y = (flags & 0x13820u) ? collider->ac->shape.rot.y : Math_Vec3f_Yaw(&collider->ac->world.pos, &actor->world.pos);
}

void comboEnemyHitYawCylinder(Actor* actor, ColliderCylinder* collider)
{
    if (collider)
        enemyHitYaw(actor, &collider->base, &collider->elem);
}

void comboEnemyHitYawSphere(Actor* actor, EnemySourceSphere* collider)
{
    if (collider)
        enemyHitYaw(actor, &collider->base, &collider->element.base);
}

void comboEnemyHitYawJoint(Actor* actor, ColliderJntSph* collider, s32 index)
{
    if (collider && index >= 0 && index < collider->count && collider->elements)
        enemyHitYaw(actor, &collider->base, &collider->elements[index].base);
}

#define EnemyNativeDropFlag ((void (*)(Actor*, ColliderElement*, s32))0x800283bc)
void comboEnemySetDropFlag(Actor* actor, ColliderElement* element)
{
    ColliderElement hit;
    ColliderElement view;
    if (!actor || !element)
        return;
    view = *element;
    if (element->acHitElem)
    {
        hit = *element->acHitElem;
        hit.atDmgInfo.dmgFlags = comboEnemyAttackFlags(element->acHitElem) & 0x3800u;
        view.acHitElem = &hit;
    }
    EnemyNativeDropFlag(actor, &view, 0);
}

void comboEnemySetDropFlagJoint(Actor* actor, ColliderJntSph* collider)
{
    u8 flags = 0;
    if (!actor || !collider || collider->count < 0 || (collider->count && !collider->elements))
        return;
    for (s32 i = collider->count; i > 0; --i)
    {
        comboEnemySetDropFlag(actor, &collider->elements[i - 1].base);
        flags |= actor->dropFlag;
    }
    actor->dropFlag = flags;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DROP_FLAG, comboEnemySetDropFlag);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DROP_FLAG_JOINT, comboEnemySetDropFlagJoint);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_HIT_YAW_CYLINDER, comboEnemyHitYawCylinder);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_HIT_YAW_SPHERE, comboEnemyHitYawSphere);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_HIT_YAW_JOINT, comboEnemyHitYawJoint);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPHERE_INIT, comboEnemyInitSphere);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPHERE_SET, comboEnemySetSphere);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPHERE_INIT_SET, comboEnemyInitAndSetSphere);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPHERE_DESTROY, comboEnemyDestroySphere);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPHERE_UPDATE, comboEnemyUpdateSphere);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPHERE_RESET_AT, comboEnemyResetSphereAT);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPHERE_RESET_AC, comboEnemyResetSphereAC);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPHERE_RESET_OC, comboEnemyResetSphereOC);
#else
static void enemySphereEnter(Actor* actor) {}
static void enemySphereLeave(Actor* actor) {}
static void enemySphereRelease(Actor* actor) {}
static void enemySphereReset(void) {}
static Collider* enemyNativeCollider(Collider* source)
{
    return source;
}

static void enemyRegisteredCollider(Collider* source) {}
#endif

typedef struct EnemyMaskView {
    struct EnemyMaskView* next;
    Actor* owner;
    ColliderElement* element;
    u32 sourceAt;
    u32 sourceAc;
    u8 native;
    u8 hitActive;
    ColliderElement* nativeHit;
    ColliderElement* capturedHit;
    u32 capturedFlags;
    u8 capturedPower;
    ColliderElement hit;
} EnemyMaskView;
static EnemyMaskView* sMaskViews;

u32 comboEnemyAttackFlags(ColliderElement* attack)
{
    if (!attack)
        return 0;
    for (EnemyMaskView* view = sMaskViews; view; view = view->next)
        if (view->hitActive && attack == &view->hit)
            return attack->atDmgInfo.dmgFlags;
        else if (attack == view->element)
            return view->native ? view->sourceAt : attack->atDmgInfo.dmgFlags;
#if defined(GAME_MM)
    return comboEnemyDamageFlagsForHit(attack->atDmgInfo.dmgFlags, comboEnemyAttackProvenance(attack));
#else
    return comboEnemyDamageFlags(attack->atDmgInfo.dmgFlags);
#endif
}

u8 comboEnemyAttackDamage(ColliderElement* attack)
{
    if (!attack)
        return 0;
    for (EnemyMaskView* view = sMaskViews; view; view = view->next)
        if ((view->hitActive && attack == &view->hit) || attack == view->element)
            return attack->atDmgInfo.damage;
#if defined(GAME_OOT)
    const u32 melee = (1u << 1) | (1u << 6) | (7u << 8) | (63u << 22) | (1u << 30);
    u32 flags = attack->atDmgInfo.dmgFlags;
    if (flags && !(flags & (flags - 1)) && (flags & melee))
        return CollisionCheck_GetSwordDamageWrapper(flags);
#endif
    return attack->atDmgInfo.damage;
}

static u32 enemySourceAttackFlags(Actor* attacker, ColliderElement* attack)
{
#if defined(GAME_OOT)
    if (attacker && attacker->id == ACTOR_CUSTOM_EN_BOM &&
        attack->atDmgInfo.dmgFlags == (DMG_EXPLOSIVE | DMG_HAMMER_JUMP))
        return (1u << 31) | (1u << 3);
#else
    (void)attacker;
#endif
    return comboEnemyAttackFlags(attack);
}

#if defined(GAME_OOT)
static u8 enemyMmAttackPower(Actor* attacker, ColliderElement* attack)
{
    u8 power = comboEnemyAttackDamage(attack);
    u32 flags = attack->atDmgInfo.dmgFlags;
    if (flags == DMG_DEKU_NUT && power == 0)
        return 1;
    if (attacker && attacker->id == ACTOR_EN_ARROW && power == 1 &&
        (flags == DMG_ARROW_NORMAL || flags == DMG_ARROW_FIRE ||
         flags == DMG_ARROW_ICE || flags == DMG_ARROW_LIGHT))
        return 2;
    if (attacker && attacker->id == ACTOR_MAGIC_FIRE && flags == (1u << 17))
        return 1;
    if (!attacker || power < 8)
        return power;
    if (attacker->id == ACTOR_CUSTOM_EN_BOM && flags == (DMG_EXPLOSIVE | DMG_HAMMER_JUMP))
        return power - 4;
    if ((attacker->id == ACTOR_EN_BOM || attacker->id == ACTOR_CUSTOM_EN_BOM) && flags == DMG_EXPLOSIVE)
        return power - 6;
    return power;
}
#endif

static int enemySourceHitAllowed(Actor* attacker, ColliderElement* attack, ColliderElement* target)
{
    for (EnemyMaskView* view = sMaskViews; view; view = view->next)
        if (view->element == target)
            return !!(enemySourceAttackFlags(attacker, attack) & (view->native ? view->sourceAc : target->acDmgInfo.dmgFlags));
    return 1;
}

void comboEnemyCaptureHit(ColliderElement* target)
{
    if (!target || !(target->acElemFlags & ACELEM_HIT) || !target->acHitElem)
        return;
    for (EnemyMaskView* view = sMaskViews; view; view = view->next)
        if (view->element == target)
        {
            view->capturedHit = target->acHitElem;
            Actor* attacker = target->acHit ? target->acHit->actor : NULL;
            view->capturedFlags = enemySourceAttackFlags(attacker, target->acHitElem);
#if defined(GAME_OOT)
            view->capturedPower = enemyMmAttackPower(attacker, target->acHitElem);
#else
            view->capturedPower = comboEnemyAttackDamage(target->acHitElem);
#endif
            return;
        }
}

static void enemyRestoreHit(EnemyMaskView* view)
{
    if (!view->hitActive)
        return;
    if (view->element->acHitElem == &view->hit)
        view->element->acHitElem = view->nativeHit;
    view->nativeHit = NULL;
    view->hitActive = 0;
}

static void enemyProjectHit(EnemyMaskView* view)
{
    ColliderElement* attack = view->element->acHitElem;
    if (view->hitActive || !(view->element->acElemFlags & ACELEM_HIT) || !attack)
        return;
    view->nativeHit = attack;
    view->hit = *attack;
    view->hit.atDmgInfo.dmgFlags = view->capturedHit == attack ? view->capturedFlags : comboEnemyAttackFlags(attack);
    view->hit.atDmgInfo.damage = view->capturedHit == attack ? view->capturedPower : comboEnemyAttackDamage(attack);
    view->hitActive = 1;
    view->element->acHitElem = &view->hit;
}

static void enemyForgetMask(ColliderElement* element)
{
    EnemyMaskView** link = &sMaskViews;
    while (*link)
    {
        EnemyMaskView* view = *link;
        if (view->element == element)
        {
            enemyRestoreHit(view);
            if (view->native)
            {
                element->atDmgInfo.dmgFlags = view->sourceAt;
                element->acDmgInfo.dmgFlags = view->sourceAc;
            }
            *link = view->next;
            free(view);
        }
        else
            link = &view->next;
    }
}

static void enemyForgetJoint(ColliderJntSph* collider)
{
    if (!collider || collider->count < 0 || collider->count > 256 || !collider->elements)
        return;
    for (s32 i = 0; i < collider->count; ++i)
        enemyForgetMask(&collider->elements[i].base);
}

static void enemyForgetTris(ColliderTris* collider)
{
    if (!collider || collider->count < 0 || collider->count > 256 || !collider->elements)
        return;
    for (s32 i = 0; i < collider->count; ++i)
        enemyForgetMask(&collider->elements[i].base);
}

s32 comboEnemyDestroyJoint(PlayState* play, ColliderJntSph* collider)
{
    if (!collider)
        return 0;
    enemyForgetJoint(collider);
    return Collider_DestroyJntSph(play, collider);
}

s32 comboEnemyFreeJoint(PlayState* play, ColliderJntSph* collider)
{
    if (!collider)
        return 0;
    enemyForgetJoint(collider);
    return Collider_FreeJntSph(play, collider);
}

s32 comboEnemyDestroyTris(PlayState* play, ColliderTris* collider)
{
    if (!collider)
        return 0;
    enemyForgetTris(collider);
    return Collider_DestroyTris(play, collider);
}

s32 comboEnemyFreeTris(PlayState* play, ColliderTris* collider)
{
    if (!collider)
        return 0;
    enemyForgetTris(collider);
    return Collider_FreeTris(play, collider);
}

s32 comboEnemyDestroyCylinder(PlayState* play, ColliderCylinder* collider)
{
    if (!collider)
        return 0;
    enemyForgetMask(&collider->elem);
    return Collider_DestroyCylinder(play, collider);
}

s32 comboEnemyDestroyQuad(PlayState* play, ColliderQuad* collider)
{
    if (!collider)
        return 0;
    enemyForgetMask(&collider->elem);
    return Collider_DestroyQuad(play, collider);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_JOINT_DESTROY, comboEnemyDestroyJoint);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_JOINT_FREE, comboEnemyFreeJoint);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_TRIS_DESTROY, comboEnemyDestroyTris);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_TRIS_FREE, comboEnemyFreeTris);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_CYLINDER_DESTROY, comboEnemyDestroyCylinder);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_QUAD_DESTROY, comboEnemyDestroyQuad);

static int enemyTrackMask(Actor* owner, ColliderElement* element)
{
    EnemyMaskView* view;
    for (view = sMaskViews; view; view = view->next)
        if (view->element == element)
            return view->owner == owner;
    view = malloc(sizeof(*view));
    if (!view)
        return 0;
    view->owner = owner;
    view->element = element;
    view->native = 0;
    view->hitActive = 0;
    view->nativeHit = NULL;
    view->capturedHit = NULL;
    view->capturedFlags = 0;
    view->capturedPower = 0;
    view->next = sMaskViews;
    sMaskViews = view;
    return 1;
}

static int enemyTrackCollider(Collider* collider)
{
    Actor* owner = comboEnemyCurrentActor();
    if (!owner || !comboEnemyUsesSourceDamage(owner))
        return 1;
    if (!collider || collider->actor != owner)
        return 0;
    switch (collider->shape)
    {
    case COLSHAPE_JNTSPH: {
        ColliderJntSph* spheres = (ColliderJntSph*)collider;
        if (spheres->count < 0 || spheres->count > 256 || (spheres->count && !spheres->elements))
            return 0;
        for (s32 i = 0; i < spheres->count; ++i)
            if (!enemyTrackMask(owner, &spheres->elements[i].base))
                return 0;
        return 1;
    }
    case COLSHAPE_CYLINDER:
        return enemyTrackMask(owner, &((ColliderCylinder*)collider)->elem);
    case COLSHAPE_TRIS: {
        ColliderTris* tris = (ColliderTris*)collider;
        if (tris->count < 0 || tris->count > 256 || (tris->count && !tris->elements))
            return 0;
        for (s32 i = 0; i < tris->count; ++i)
            if (!enemyTrackMask(owner, &tris->elements[i].base))
                return 0;
        return 1;
    }
    case COLSHAPE_QUAD:
        return enemyTrackMask(owner, &((ColliderQuad*)collider)->elem);
#if defined(GAME_OOT)
    case 4:
        return enemyTrackMask(owner, &((EnemySourceSphere*)collider)->element.base);
#endif
    default: return 0;
    }
}

static int enemyPrepareCollider(Collider* source)
{
    if (enemyTrackCollider(source))
        return 1;
    g.roomEnemyLackSoul = 1;
    Actor* owner = comboEnemyCurrentActor();
    if (owner)
        Actor_Kill(owner);
    return 0;
}

void comboEnemyCollisionEnter(Actor* actor)
{
    for (EnemyMaskView* view = sMaskViews; view; view = view->next)
        if (view->owner == actor)
        {
            if (view->native)
            {
                view->element->atDmgInfo.dmgFlags = view->sourceAt;
                view->element->acDmgInfo.dmgFlags = view->sourceAc;
                view->native = 0;
            }
            enemyProjectHit(view);
        }
    enemySphereEnter(actor);
}

void comboEnemyCollisionLeave(Actor* actor)
{
    for (EnemyMaskView* view = sMaskViews; view; view = view->next)
        if (view->owner == actor && !view->native)
        {
            enemyRestoreHit(view);
            view->sourceAt = view->element->atDmgInfo.dmgFlags;
            view->sourceAc = view->element->acDmgInfo.dmgFlags;
            view->element->atDmgInfo.dmgFlags = comboEnemyDamageMask(view->sourceAt);
            view->element->acDmgInfo.dmgFlags = comboEnemyDamageMask(view->sourceAc);
            view->native = 1;
        }
    enemySphereLeave(actor);
}

void comboEnemyCollisionRelease(Actor* actor)
{
    EnemyMaskView** link = &sMaskViews;
    while (*link)
    {
        EnemyMaskView* view = *link;
        if (view->owner == actor)
        {
            enemyRestoreHit(view);
            *link = view->next;
            free(view);
        }
        else
            link = &view->next;
    }
    enemySphereRelease(actor);
}

void comboEnemyCollisionReset(void)
{
#if defined(GAME_MM)
    comboEnemyAttackReset();
#endif
    while (sMaskViews)
    {
        EnemyMaskView* view = sMaskViews;
        sMaskViews = view->next;
        free(view);
    }
    enemySphereReset();
}

#define ENEMY_COLLISION_REGISTER(kind) \
s32 comboEnemyCollisionSet##kind(PlayState* play, CollisionCheckContext* context, Collider* source) \
{ \
    Collider* native; \
    s32 result; \
    if (!enemyPrepareCollider(source)) return -1; \
    native = enemyNativeCollider(source); \
    if (!native) return -1; \
    result = CollisionCheck_Set##kind(play, context, native); \
    enemyRegisteredCollider(source); \
    return result; \
} \
s32 comboEnemyCollisionSet##kind##SAC(PlayState* play, CollisionCheckContext* context, Collider* source, s32 index) \
{ \
    Collider* native; \
    s32 result; \
    if (!enemyPrepareCollider(source)) return -1; \
    native = enemyNativeCollider(source); \
    if (!native) return -1; \
    result = CollisionCheck_Set##kind##_SAC(play, context, native, index); \
    enemyRegisteredCollider(source); \
    return result; \
}

ENEMY_COLLISION_REGISTER(AT)
ENEMY_COLLISION_REGISTER(AC)
ENEMY_COLLISION_REGISTER(OC)
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_COLLISION_AT, comboEnemyCollisionSetAT);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_COLLISION_AC, comboEnemyCollisionSetAC);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_COLLISION_OC, comboEnemyCollisionSetOC);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_COLLISION_AT_SAC, comboEnemyCollisionSetATSAC);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_COLLISION_AC_SAC, comboEnemyCollisionSetACSAC);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_COLLISION_OC_SAC, comboEnemyCollisionSetOCSAC);

#if defined(GAME_OOT)
s32 comboEnemyInitSetCylinder(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInit* init)
{
    Collider_InitCylinder(play, dest);
    Collider_SetCylinder(play, dest, actor, init);
    return 1;
}

s32 comboEnemyInitSetJoint(PlayState* play, ColliderJntSph* dest, Actor* actor, ColliderJntSphInit* init, ColliderJntSphElement* elements)
{
    Collider_InitJntSph(play, dest);
    Collider_SetJntSph(play, dest, actor, init, elements);
    return 0;
}

s32 comboEnemyInitSetTris(PlayState* play, ColliderTris* dest, Actor* actor, ColliderTrisInit* init, ColliderTrisElement* elements)
{
    Collider_InitTris(play, dest);
    Collider_SetTris(play, dest, actor, init, elements);
    return 0;
}

s32 comboEnemyInitSetQuad(PlayState* play, ColliderQuad* dest, Actor* actor, ColliderQuadInit* init)
{
    Collider_InitQuad(play, dest);
    Collider_SetQuad(play, dest, actor, init);
    return 0;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_CYLINDER_INIT_SET, comboEnemyInitSetCylinder);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_JOINT_INIT_SET, comboEnemyInitSetJoint);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_TRIS_INIT_SET, comboEnemyInitSetTris);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_QUAD_INIT_SET, comboEnemyInitSetQuad);
#endif

#if defined(GAME_OOT)
# define EnemyNativeHit ((s32 (*)(PlayState*, Collider*, ColliderElement*, Vec3f*, Collider*, ColliderElement*, Vec3f*, Vec3f*))0x8004ccc8)
#else
# define EnemyNativeHit ((s32 (*)(PlayState*, Collider*, ColliderElement*, Vec3f*, Collider*, ColliderElement*, Vec3f*, Vec3f*))0x800e3324)
#endif

s32 comboEnemyCollisionHit(PlayState* play, Collider* at, ColliderElement* atElem, Vec3f* atPos,
                           Collider* ac, ColliderElement* acElem, Vec3f* acPos, Vec3f* hitPos)
{
    s32 result;
    int sourceAt;
    int sourceAc;
    if (!at || !atElem || !ac || !acElem || !hitPos)
        return 0;
    sourceAc = comboEnemyUsesSourceDamage(ac->actor);
    if (sourceAc && !enemySourceHitAllowed(at->actor, atElem, acElem))
        return 0;
    sourceAt = comboEnemyUsesSourceDamage(at->actor);
#if defined(GAME_OOT)
    if (sourceAc && enemyMmAttackPower(at->actor, atElem))
    {
        u32 effect;
        float damage = comboEnemySourceMmDamage(ac, at->actor, atElem, &effect);
        if (!effect && (damage < 1.0f || damage - acElem->acDmgInfo.defense < 1.0f))
            return 0;
    }
    if (sourceAt)
    {
        u8 oldAcFlags = ac->acFlags;
        u8 oldElemFlags = acElem->acElemFlags;
        Actor* oldAcActor = ac->ac;
        Collider* oldAcHit = acElem->acHit;
        ColliderElement* oldAcHitElem = acElem->acHitElem;
        Vec3s oldAcPos = acElem->acDmgInfo.hitPos;
        u8 oldEffect = ac->actor ? ac->actor->colChkInfo.acHitEffect : 0;
        result = EnemyNativeHit(play, at, atElem, atPos, ac, acElem, acPos, hitPos);
        if (result && !(acElem->acElemFlags & ACELEM_NO_AT_INFO) && !(atElem->acElemFlags & ACELEM_HIT))
        {
            atElem->acDmgInfo.hitPos.x = hitPos->x;
            atElem->acDmgInfo.hitPos.y = hitPos->y;
            atElem->acDmgInfo.hitPos.z = hitPos->z;
        }
        if (atElem->ocElemFlags & OCELEM_UNK2)
        {
            ac->acFlags = (ac->acFlags & ~AC_HIT) | (oldAcFlags & AC_HIT);
            acElem->acElemFlags = (acElem->acElemFlags & ~ACELEM_HIT) | (oldElemFlags & ACELEM_HIT);
            ac->ac = oldAcActor;
            acElem->acHit = oldAcHit;
            acElem->acHitElem = oldAcHitElem;
            acElem->acDmgInfo.hitPos = oldAcPos;
            if (ac->actor)
                ac->actor->colChkInfo.acHitEffect = oldEffect;
        }
        if (sourceAc && result && !(atElem->ocElemFlags & OCELEM_UNK2) && acElem->acHitElem == atElem)
            comboEnemyCaptureHit(acElem);
        return result;
    }
#else
    if (sourceAc &&
        ac->actor->id == FOREIGN_ENEMY_ACTOR_BASE + 0x002d &&
        (enemySourceAttackFlags(at->actor, atElem) & 1u))
    {
        u8 oldOcFlags = atElem->ocElemFlags;
        atElem->ocElemFlags &= ~OCELEM_UNK2;
        result = EnemyNativeHit(play, at, atElem, atPos,
                                ac, acElem, acPos, hitPos);
        atElem->ocElemFlags = oldOcFlags;

        if (result && acElem->acHitElem == atElem)
            comboEnemyCaptureHit(acElem);

        return result;
    }

    if (sourceAt)
    {
        u8 oldOcFlags = atElem->ocElemFlags;
        Vec3s oldAtPos = atElem->acDmgInfo.hitPos;
        atElem->ocElemFlags &= ~OCELEM_UNK2;
        result = EnemyNativeHit(play, at, atElem, atPos, ac, acElem, acPos, hitPos);
        atElem->ocElemFlags = oldOcFlags;
        atElem->acDmgInfo.hitPos = oldAtPos;
        if (sourceAc && result && acElem->acHitElem == atElem)
            comboEnemyCaptureHit(acElem);
        return result;
    }
#endif
    result = EnemyNativeHit(play, at, atElem, atPos, ac, acElem, acPos, hitPos);
#if defined(GAME_MM)
    if (atElem->ocElemFlags & OCELEM_UNK2)
        return result;
#endif
    if (sourceAc && result && acElem->acHitElem == atElem)
        comboEnemyCaptureHit(acElem);
    return result;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_COLLISION_HIT, comboEnemyCollisionHit);

#if defined(GAME_OOT)
PATCH_CALL(0x8004d08c, comboEnemyCollisionHit);
PATCH_CALL(0x8004d310, comboEnemyCollisionHit);
PATCH_CALL(0x8004d570, comboEnemyCollisionHit);
PATCH_CALL(0x8004d77c, comboEnemyCollisionHit);
PATCH_CALL(0x8004d97c, comboEnemyCollisionHit);
PATCH_CALL(0x8004dbd0, comboEnemyCollisionHit);
PATCH_CALL(0x8004de28, comboEnemyCollisionHit);
PATCH_CALL(0x8004e04c, comboEnemyCollisionHit);
PATCH_CALL(0x8004e1d0, comboEnemyCollisionHit);
PATCH_CALL(0x8004e390, comboEnemyCollisionHit);
PATCH_CALL(0x8004e56c, comboEnemyCollisionHit);
PATCH_CALL(0x8004e644, comboEnemyCollisionHit);
PATCH_CALL(0x8004e7f8, comboEnemyCollisionHit);
PATCH_CALL(0x8004e8f0, comboEnemyCollisionHit);
PATCH_CALL(0x8004eadc, comboEnemyCollisionHit);
PATCH_CALL(0x8004ed80, comboEnemyCollisionHit);
PATCH_CALL(0x8004f024, comboEnemyCollisionHit);
PATCH_CALL(0x8004f2c0, comboEnemyCollisionHit);
#else
PATCH_CALL(0x800e3868, comboEnemyCollisionHit);
PATCH_CALL(0x800e3ac0, comboEnemyCollisionHit);
PATCH_CALL(0x800e3c40, comboEnemyCollisionHit);
PATCH_CALL(0x800e3e10, comboEnemyCollisionHit);
PATCH_CALL(0x800e3ffc, comboEnemyCollisionHit);
PATCH_CALL(0x800e4228, comboEnemyCollisionHit);
PATCH_CALL(0x800e44a4, comboEnemyCollisionHit);
PATCH_CALL(0x800e45d0, comboEnemyCollisionHit);
PATCH_CALL(0x800e4730, comboEnemyCollisionHit);
PATCH_CALL(0x800e479c, comboEnemyCollisionHit);
PATCH_CALL(0x800e4930, comboEnemyCollisionHit);
PATCH_CALL(0x800e4a78, comboEnemyCollisionHit);
PATCH_CALL(0x800e4c18, comboEnemyCollisionHit);
PATCH_CALL(0x800e4da4, comboEnemyCollisionHit);
PATCH_CALL(0x800e4f80, comboEnemyCollisionHit);
PATCH_CALL(0x800e50ec, comboEnemyCollisionHit);
PATCH_CALL(0x800e52b4, comboEnemyCollisionHit);
PATCH_CALL(0x800e5438, comboEnemyCollisionHit);
PATCH_CALL(0x800e54c0, comboEnemyCollisionHit);
PATCH_CALL(0x800e5654, comboEnemyCollisionHit);
PATCH_CALL(0x800e5820, comboEnemyCollisionHit);
PATCH_CALL(0x800e5988, comboEnemyCollisionHit);
PATCH_CALL(0x800e5b38, comboEnemyCollisionHit);
PATCH_CALL(0x800e5cf4, comboEnemyCollisionHit);
PATCH_CALL(0x800e5df8, comboEnemyCollisionHit);
PATCH_CALL(0x800e5f50, comboEnemyCollisionHit);
PATCH_CALL(0x800e60a4, comboEnemyCollisionHit);
#endif

#if defined(GAME_OOT)
static const float sEnemyMmDamageMultipliers[16] = { 0, 1, 2, .5f, .25f, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

float comboEnemySourceMmDamage(Collider* ac, Actor* attacker, ColliderElement* attack, u32* effect)
{
    u32 flags;
    u32 index = 0;
    u8 packed;
    DamageTable* table;
    *effect = 0;
    if (!ac || !ac->actor || !attack)
        return 0;
    table = ac->actor->colChkInfo.damageTable;
    if (!table)
        return enemyMmAttackPower(attacker, attack);
    flags = enemySourceAttackFlags(attacker, attack);
    if (!flags)
        return 0;
    while (flags > 1)
    {
        flags >>= 1;
        ++index;
    }
    packed = table->attack[index];
    *effect = packed >> 4;
    return enemyMmAttackPower(attacker, attack) * sEnemyMmDamageMultipliers[packed & 15];
}
#endif

int comboEnemyApplySourceDamage(Collider* col, ColliderElement* elem)
{
    DamageTable* table;
    ColliderElement* attack;
    u32 index;
    u32 sourceFlags;
    u8 packed = 0;
    float damage;
#if defined(GAME_OOT)
    float finalDamage = 0.0f;
#endif
    if (!col || !comboEnemyUsesSourceDamage(col->actor))
        return 0;
    if (!elem || !(col->acFlags & AC_HIT) || !(elem->acElemFlags & ACELEM_HIT) ||
        (elem->acElemFlags & ACELEM_NO_DAMAGE) || !elem->acHitElem)
        return 1;
#if defined(GAME_OOT)
    if (!elem->acHit)
        return 1;
#endif
    attack = elem->acHitElem;
    table = col->actor->colChkInfo.damageTable;
#if defined(GAME_OOT)
    Actor* attacker = elem->acHit->actor;
    u8 power = enemyMmAttackPower(attacker, attack);
    damage = power;
    sourceFlags = enemySourceAttackFlags(attacker, attack);
#else
    damage = comboEnemyAttackDamage(attack);
    sourceFlags = comboEnemyAttackFlags(attack);
#endif
    if (table)
    {
        if (!sourceFlags)
            return 1;
        index = 0;
        for (u32 bits = sourceFlags; bits > 1; bits >>= 1)
            ++index;
        packed = table->attack[index];
#if defined(GAME_OOT)
        damage *= sEnemyMmDamageMultipliers[packed & 15];
#else
        if ((index == 8 || index == 22 || index == 25) && (table->attack[10] & 15) == 4 &&
            comboEnemyAttackDamage(attack) > (index == 25 ? 2 : 1))
        {
            if ((table->attack[8] & 0xf0) != (table->attack[10] & 0xf0))
                packed = table->attack[10];
            packed = (packed & 0xf0) | (comboEnemyAttackDamage(attack) > 15 ? 15 : comboEnemyAttackDamage(attack));
        }
        damage = packed & 15;
#endif
    }
#if defined(GAME_OOT)
    if (power != 0)
    {
        if (damage < 1.0f)
        {
            if (!(packed >> 4))
                return 1;
        }
        else
        {
            finalDamage = damage - elem->acDmgInfo.defense;
            if (finalDamage < 1.0f && !(packed >> 4))
                return 1;
        }
    }
    if (table)
        col->actor->colChkInfo.damageEffect = packed >> 4;
    if (!(col->acFlags & AC_HARD) || sourceFlags == 0x20000000u)
        if (col->actor->colChkInfo.damage < finalDamage)
            col->actor->colChkInfo.damage = finalDamage;
#else
    if (table)
        col->actor->colChkInfo.damageEffect = packed >> 4;
    else
    {
        damage -= elem->acDmgInfo.defense;
        if (damage < 0)
            damage = 0;
    }
    if (!(col->acFlags & AC_HARD))
        col->actor->colChkInfo.damage += damage;
#endif
    return 1;
}

#if defined(GAME_OOT)
# define NativeDamageApply ((void (*)(PlayState*, CollisionCheckContext*, Collider*, ColliderElement*))0x800503dc)
#else
# define NativeDamageApply ((void (*)(PlayState*, CollisionCheckContext*, Collider*, ColliderElement*))0x800e75c8)
#endif

static void enemyDamageApply(PlayState* play, CollisionCheckContext* ctx, Collider* col, ColliderElement* elem)
{
    if (!comboEnemyApplySourceDamage(col, elem))
        NativeDamageApply(play, ctx, col, elem);
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_APPLY_DAMAGE, enemyDamageApply);

#if defined(GAME_MM)
# define NativeDamageValue ((float (*)(Collider*, ColliderElement*, Collider*, ColliderElement*, u32*))0x800e0410)

static float enemyDamageAdmission(Collider* at, ColliderElement* atElem, Collider* ac, ColliderElement* acElem, u32* effect)
{
    if (ac && acElem && comboEnemyUsesSourceDamage(ac->actor))
    {
        *effect = 0;
        return 1.0f + acElem->acDmgInfo.defense;
    }
    return NativeDamageValue(at, atElem, ac, acElem, effect);
}

PATCH_CALL(0x800e3380, enemyDamageAdmission);
PATCH_CALL(0x800e7850, enemyDamageApply);
PATCH_CALL(0x800e789c, enemyDamageApply);
PATCH_CALL(0x800e7904, enemyDamageApply);
PATCH_CALL(0x800e7950, enemyDamageApply);
PATCH_CALL(0x800e7970, enemyDamageApply);

#endif

u32 gForeignEnemyDamageTablesVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
# define DAMAGE_NATIVE_GAME 0
EXPORT_SYMBOL(FOREIGN_ENEMY_DAMAGE_TABLES_OOT, gForeignEnemyDamageTablesVrom);
#else
# define DAMAGE_NATIVE_GAME 1
EXPORT_SYMBOL(FOREIGN_ENEMY_DAMAGE_TABLES_MM, gForeignEnemyDamageTablesVrom);
#endif
static DamageTable sSourcePresets[23];
static u8 sHostAttacks[32];
static u32 sHostCandidates[32];
static u32 sLoadedVrom;

static int enemyDamageTablesLoad(void)
{
    DmaEntry file;
    u32 header[4];
    u32 offset;
    u32 expectedSize;
    if (sLoadedVrom != gForeignEnemyDamageTablesVrom || !sLoadedVrom)
    {
        if (!gForeignEnemyDamageTablesVrom || gForeignEnemyDamageTablesVrom < 0x08000000 ||
            !comboDmaLookup(&file, gForeignEnemyDamageTablesVrom) || file.vstart != gForeignEnemyDamageTablesVrom || file.pend ||
            file.vend < file.vstart || file.vend - file.vstart < sizeof(header) ||
            comboDmaLoadFilePartial(header, gForeignEnemyDamageTablesVrom, 0, sizeof(header)) != sizeof(header) ||
            header[0] != 0x45444d47 || (header[1] != 2 && header[1] != 3) || header[2] != DAMAGE_NATIVE_GAME ||
            header[3] != ARRAY_COUNT(sSourcePresets))
            return 0;
        expectedSize = sizeof(header) + sizeof(sHostAttacks) + sizeof(sSourcePresets);
        if (header[1] == 3)
            expectedSize += sizeof(sHostCandidates);
        if (file.vend - file.vstart != expectedSize ||
            comboDmaLoadFilePartial(sHostAttacks, gForeignEnemyDamageTablesVrom, sizeof(header), sizeof(sHostAttacks)) != sizeof(sHostAttacks))
            return 0;
        offset = sizeof(header) + sizeof(sHostAttacks);
        if (header[1] == 3)
        {
            if (comboDmaLoadFilePartial(sHostCandidates, gForeignEnemyDamageTablesVrom, offset, sizeof(sHostCandidates)) != sizeof(sHostCandidates))
                return 0;
            offset += sizeof(sHostCandidates);
        }
        for (u32 i = 0; i < ARRAY_COUNT(sHostAttacks); ++i)
        {
            u32 entry = sHostAttacks[i];
            int dynamic = entry == 0xfe || (entry >= 0x80 && entry < 0xa0);
            if (entry >= 32 && entry != 0xff && !(header[1] == 3 && dynamic))
                return 0;
            if (header[1] == 2)
                sHostCandidates[i] = entry == 0xff ? 0 : 1u << entry;
            else if (dynamic)
            {
                u32 candidates = sHostCandidates[i];
                if (!candidates || !(candidates & (candidates - 1)) ||
                    (entry != 0xfe && !(candidates & (1u << (entry & 31)))))
                    return 0;
            }
            else if (sHostCandidates[i] != (entry == 0xff ? 0 : 1u << entry))
                return 0;
        }
        if (comboDmaLoadFilePartial(sSourcePresets, gForeignEnemyDamageTablesVrom, offset, sizeof(sSourcePresets)) != sizeof(sSourcePresets))
            return 0;
        sLoadedVrom = gForeignEnemyDamageTablesVrom;
    }
    return 1;
}

u32 comboEnemyDamageFlagsForHit(u32 nativeFlags, u32 provenance)
{
    u32 result = 0;
    if (!enemyDamageTablesLoad())
        return 0;
    for (u32 bit = 0; nativeFlags; ++bit, nativeFlags >>= 1)
        if (nativeFlags & 1)
        {
            if (sHostAttacks[bit] < 32)
                result |= 1u << sHostAttacks[bit];
            else if (sHostAttacks[bit] != 0xff)
            {
                result |= provenance & sHostCandidates[bit];
                if (!provenance && sHostAttacks[bit] != 0xfe)
                    result |= 1u << (sHostAttacks[bit] & 31);
            }
        }
    return result;
}
EXPORT_SYMBOL(FOREIGN_ENEMY_DAMAGE_POLICY_V3, comboEnemyDamageFlagsForHit);

u32 comboEnemyDamageFlags(u32 nativeFlags)
{
    return comboEnemyDamageFlagsForHit(nativeFlags, 0);
}

u32 comboEnemyDamageMask(u32 sourceMask)
{
    u32 result = 0;
    if (!sourceMask || !enemyDamageTablesLoad())
        return 0;
    for (u32 bit = 0; bit < ARRAY_COUNT(sHostAttacks); ++bit)
        if (sourceMask & sHostCandidates[bit])
            result |= 1u << bit;
    return result;
}

static DamageTable* enemyDamageTableGet(s32 index)
{
    if (index < 0 || index >= ARRAY_COUNT(sSourcePresets) || !enemyDamageTablesLoad())
        return NULL;
    return &sSourcePresets[index];
}

static void enemyCollisionSetInfo(CollisionCheckInfo* info, s32 index, CollisionCheckInfoInit2* init)
{
    CollisionCheck_SetInfo2(info, enemyDamageTableGet(index), init);
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DAMAGE_TABLE, enemyDamageTableGet);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DAMAGE_INFO, enemyCollisionSetInfo);
