#include <combo.h>
#include <combo/inventory.h>
#include <combo/item.h>
#include <combo/enemy_placement.h>

#define NativeSkullKidSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)

static void nativeSkullKidInit(Actor* actor, PlayState* play)
{
    ActorOvl* overlay = actor->ovl;
    ActorProfile* profile;
    ActorFunc init;
    u8 original;
    if (actor->id != 0x115 || (u16)actor->params != 0xffff || !overlay || !overlay->data ||
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
    if (profile->id != 0x115 || profile->objectId != 0x10a || !init || init == nativeSkullKidInit)
    {
        actor->init = NULL;
        Actor_Kill(actor);
        return;
    }
    original = gSave.info.inventory.items[ITS_OOT_TRADE_ADULT];
    if (original < ITEM_OOT_POACHER_SAW)
        gSave.info.inventory.items[ITS_OOT_TRADE_ADULT] = ITEM_OOT_POACHER_SAW;
    actor->init = init;
    init(actor, play);
    actor->init = NULL;
    gSave.info.inventory.items[ITS_OOT_TRADE_ADULT] = original;
}

Actor* comboEnemyNativeSkullKidSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    u8 original;
    if (!ctx || !entry || !play || entry->id != 0x115 || (u16)entry->params != 0xffff)
        return NULL;
    original = gSave.info.inventory.items[ITS_OOT_TRADE_ADULT];
    if (original < ITEM_OOT_POACHER_SAW)
        gSave.info.inventory.items[ITS_OOT_TRADE_ADULT] = ITEM_OOT_POACHER_SAW;
    actor = NativeSkullKidSpawnEntry(ctx, entry, play);
    gSave.info.inventory.items[ITS_OOT_TRADE_ADULT] = original;
    if (actor && actor->init)
        actor->init = nativeSkullKidInit;
    return actor;
}

EXPORT_SYMBOL(ENEMY_NATIVE_SKULL_KID_V1, comboEnemyNativeSkullKidSpawn);
