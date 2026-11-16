#include <combo.h>
#include <combo/enemy_native.h>

#define NativeIronKnuckleSpawnEntry ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)

u16 comboEnemyNativeIronKnuckleParams(u16 encoded)
{
    return encoded == 0xff81 || encoded == 0xff82 || encoded == 0xff83 ? encoded & ~0x80 : encoded;
}

static void nativeIronKnuckleUpdate(Actor* actor, PlayState* play)
{
    ActorFunc update = actorAddr(0x113, 0x80a68be0);
    if (update)
        update(actor, play);
    actor->shape.rot.x = 0;
    actor->shape.rot.z = 0;
}

static void nativeIronKnuckleInit(Actor* actor, PlayState* play)
{
    ActorOvl* overlay = actor->ovl;
    ActorProfile* profile;
    ActorFunc init;
    u16 encoded = (u16)actor->params | 0x80;
    if (actor->id != 0x113 || (encoded != 0xff81 && encoded != 0xff82 && encoded != 0xff83) || !overlay || !overlay->data ||
        overlay->vramEnd < overlay->vramStart || (u32)overlay->dataInit < overlay->vramStart ||
        (u32)overlay->dataInit > overlay->vramEnd ||
        overlay->vramEnd - (u32)overlay->dataInit < sizeof(*profile))
    {
        actor->init = NULL;
        Actor_Kill(actor);
        return;
    }
    profile = (ActorProfile*)((u8*)overlay->data + (u32)overlay->dataInit - overlay->vramStart);
    init = profile->init;
    if (profile->id != 0x113 || profile->objectId != 0x106 || !init || init == nativeIronKnuckleInit)
    {
        actor->init = NULL;
        Actor_Kill(actor);
        return;
    }
    actor->init = init;
    init(actor, play);
    actor->init = NULL;
    comboEnemyNativeIronKnuckleActivate(actor, play, encoded);
}

void comboEnemyNativeIronKnuckleActivate(Actor* actor, PlayState* play, u16 encoded)
{
    void (*start)(Actor*);
    if (!actor || actor->id != 0x113 || (encoded != 0xff81 && encoded != 0xff82 && encoded != 0xff83))
        return;
    if (actor->init)
    {
        if ((u16)actor->params == comboEnemyNativeIronKnuckleParams(encoded))
            actor->init = nativeIronKnuckleInit;
        return;
    }
    if (!actor->update || actor->params != (encoded & 0x7f))
        return;
    start = actorAddr(0x113, 0x80a676e4);
    if (!start)
        return;
    actor->shape.rot.x = 0;
    actor->shape.rot.z = 0;
    actor->update = nativeIronKnuckleUpdate;
    actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE;
    u32 previousSegment = gSegments[6];
    Actor_SetObjectDependency(play, actor);
    start(actor);
    gSegments[6] = previousSegment;
}

Actor* comboEnemyNativeIronKnuckleSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    ActorEntry native;
    Actor* actor;
    if (!ctx || !entry || !play || entry->id != 0x113)
        return NULL;

    u16 encoded = (u16)entry->params | 0x80;
    if (encoded != 0xff81 && encoded != 0xff82 && encoded != 0xff83)
        return NULL;
    native = *entry;
    native.params = comboEnemyNativeIronKnuckleParams(encoded);
    actor = NativeIronKnuckleSpawnEntry(ctx, &native, play);
    comboEnemyNativeIronKnuckleActivate(actor, play, encoded);
    return actor;
}

EXPORT_SYMBOL(ENEMY_NATIVE_IRON_KNUCKLE_V1, comboEnemyNativeIronKnuckleSpawn);
