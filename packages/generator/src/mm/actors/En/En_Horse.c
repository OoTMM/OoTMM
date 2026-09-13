#include <combo.h>
#include <combo/custom.h>
#include <combo/mm/actors/En_Horse.h>

#define ENHORSE_ACTION_INACTIVE      1
#define ENHORSE_ACTION_IDLE          2
#define ENHORSE_ACTION_MOUNTED_IDLE  7

#define ENHORSE_ANIM_IDLE 0

#define ENHORSE_JUMPING            (1u << 2)
#define ENHORSE_CALC_RIDER_POS     (1u << 3)
#define ENHORSE_INACTIVE           (1u << 13)
#define ENHORSE_OBSTACLE           (1u << 14)
#define ENHORSE_TURNING_TO_PLAYER  (1u << 15)
#define ENHORSE_DRAW               (1u << 27)
#define ENHORSE_FLAG_30            (1u << 30)

#define ENHORSE_REINIT_CLEAR_FLAGS \
(ENHORSE_JUMPING | ENHORSE_CALC_RIDER_POS | ENHORSE_OBSTACLE | \
ENHORSE_TURNING_TO_PLAYER | ENHORSE_DRAW | ENHORSE_FLAG_30)

typedef void (*EnHorseWaitForObjectFunc)(Actor* thisx, PlayState* play);

extern void* func_8012F73C(ObjectContext* objectCtx, s32 objectSlot, s16 objectId);

typedef struct {
    u16 offset;
    u16 adultValue;
    u16 childValue;
} EponaOverlayPatch;

#define EPONA_PATCH(offset, value) { (offset), (value), 0 }

static EponaOverlayPatch sAdultEponaPatch[] = {
    EPONA_PATCH(0x865A, 0x0600),
    EPONA_PATCH(0x865E, 0x1E2C),
    EPONA_PATCH(0x867A, 0x0600),
    EPONA_PATCH(0x868E, 0x3CEC),
    EPONA_PATCH(0x8706, 0x0600),
    EPONA_PATCH(0x870A, 0x3CEC),
    EPONA_PATCH(0x872A, 0x0600),
    EPONA_PATCH(0x8736, 0x3CEC),
    EPONA_PATCH(0x879A, 0x0600),
    EPONA_PATCH(0x87A2, 0x75F0),
    EPONA_PATCH(0x87AE, 0x0600),
    EPONA_PATCH(0x87BE, 0x75F0),
    EPONA_PATCH(0x880E, 0x0600),
    EPONA_PATCH(0x8812, 0x6D50),
    EPONA_PATCH(0x8832, 0x0600),
    EPONA_PATCH(0x883E, 0x6D50),
    EPONA_PATCH(0x887E, 0x0600),
    EPONA_PATCH(0x8886, 0x6D50),
    EPONA_PATCH(0x8C22, 0x0600),
    EPONA_PATCH(0x8C2E, 0x3CEC),
    EPONA_PATCH(0x8C3A, 0x0600),
    EPONA_PATCH(0x8C4E, 0x3CEC),
    EPONA_PATCH(0x8D52, 0x0600),
    EPONA_PATCH(0x8D5E, 0x1E2C),
    EPONA_PATCH(0x8D6A, 0x0600),
    EPONA_PATCH(0x8D7E, 0x1E2C),
    EPONA_PATCH(0x8DDE, 0x0600),
    EPONA_PATCH(0x8DE6, 0x1E2C),
    EPONA_PATCH(0x8DF2, 0x0600),
    EPONA_PATCH(0x8E06, 0x1E2C),
    EPONA_PATCH(0x8E72, 0x0600),
    EPONA_PATCH(0x8E7E, 0x6D50),
    EPONA_PATCH(0x8E92, 0x0600),
    EPONA_PATCH(0x8E9E, 0x6D50),
    EPONA_PATCH(0x8F06, 0x0600),
    EPONA_PATCH(0x8F0E, 0x6D50),
    EPONA_PATCH(0x8F1A, 0x0600),
    EPONA_PATCH(0x8F2E, 0x6D50),
    EPONA_PATCH(0xBBDE, 0x3C23),
    EPONA_PATCH(0xC8AC, 0x1000),
    EPONA_PATCH(0xD792, 0x6D50),
    EPONA_PATCH(0xD796, 0x5584),
    EPONA_PATCH(0xD79A, 0x4DEC),
    EPONA_PATCH(0xD79E, 0x3CEC),
    EPONA_PATCH(0xD7A2, 0x75F0),
    EPONA_PATCH(0xD7A6, 0x32B0),
    EPONA_PATCH(0xD7AA, 0x1E2C),
    EPONA_PATCH(0xD7AE, 0x2470),
    EPONA_PATCH(0xD7B2, 0x2C38),
    EPONA_PATCH(0xD81A, 0x9D74),
    EPONA_PATCH(0xDAD6, 0x9F80),
    EPONA_PATCH(0xDADA, 0xA180),
    EPONA_PATCH(0xDADE, 0xA380),
};

typedef struct {
    s32 active;
    EnHorse* horse;
    ActorFunc update;
    ActorFunc draw;
    u32 stateFlags;
} EponaReloadContext;

typedef struct {
    s32 length;
    Actor* first;
    s32 categoryChanged;
} EponaActorListEntry;

_Static_assert(
    sizeof(EponaActorListEntry) == 0xC,
    "MM ActorListEntry size is wrong"
);

#define ACTORCTX_ACTOR_LISTS(actorCtx) \
    ((EponaActorListEntry*)((u8*)(actorCtx) + 0x10))

static EponaReloadContext sEponaReload;
static void* sHorseOverlayRam;

void EponaAge_ClearReload(void)
{
    memset(&sEponaReload, 0, sizeof(sEponaReload));
}

static s32 EponaAge_IsValidHorse(Actor* actor)
{
    return actor != NULL &&
           actor->id == ACTOR_EN_HORSE &&
           actor->init == NULL &&
           actor->update != NULL &&
           ((EnHorse*)actor)->type == HORSE_TYPE_2;
}

static void EponaAge_StopActor(Actor* actor)
{
    actor->speed = 0.0f;
    actor->velocity.x = 0.0f;
    actor->velocity.y = 0.0f;
    actor->velocity.z = 0.0f;
}

static void EponaAge_Reinit(
    Actor* thisx,
    PlayState* play
);

static void EponaAge_ApplyOverlayVariant(s32 adult)
{
    ActorOvl* ovl;
    s32 i;
    s32 size;

    if (sHorseOverlayRam == NULL)
        return;

    for (i = 0; i < ARRAY_COUNT(sAdultEponaPatch); i++) {
        EponaOverlayPatch* patch = &sAdultEponaPatch[i];

        *(u16*)((u8*)sHorseOverlayRam + patch->offset) =
            adult ? patch->adultValue : patch->childValue;
    }

    ovl = &gActorOvl[ACTOR_EN_HORSE];
    size = (u8*)ovl->vramEnd - (u8*)ovl->vramStart;

    osWritebackDCache(sHorseOverlayRam, size);
    osInvalICache(sHorseOverlayRam, size);
}

void EponaAge_OnHorseOverlayLoaded(void* loadedRam)
{
    s32 i;

    if (loadedRam == NULL)
        return;

    sHorseOverlayRam = loadedRam;

    for (i = 0; i < ARRAY_COUNT(sAdultEponaPatch); i++) {
        EponaOverlayPatch* patch = &sAdultEponaPatch[i];
        patch->childValue = *(u16*)((u8*)loadedRam + patch->offset);
    }

    EponaAge_ApplyOverlayVariant(comboIsLinkAdult());
}

static EnHorse* EponaAge_FindHorse(PlayState* play)
{
    EponaActorListEntry* lists;
    Actor* actor;

    actor = GET_PLAYER(play)->rideActor;
    if (EponaAge_IsValidHorse(actor))
        return (EnHorse*)actor;

    lists = ACTORCTX_ACTOR_LISTS(&play->actorCtx);

    for (actor = lists[ACTORCAT_BG].first; actor != NULL; actor = actor->next) {
        if (EponaAge_IsValidHorse(actor))
            return (EnHorse*)actor;
    }

    return NULL;
}

static void* EponaAge_HorseOverlayAddr(u32 vram)
{
    ActorOvl* ovl = &gActorOvl[ACTOR_EN_HORSE];

    if (ovl->data == NULL)
        return NULL;

    return (u8*)ovl->data +
           (vram - (u32)ovl->vramStart);
}

static void EponaAge_Restore(EnHorse* horse, PlayState* play)
{
    Actor* actor = &horse->actor;
    Player* player = GET_PLAYER(play);
    s32 mounted = player->rideActor == actor;
    horse->stateFlags = sEponaReload.stateFlags & ~ENHORSE_REINIT_CLEAR_FLAGS;
    horse->postDrawFunc = NULL;
    horse->animIndex = ENHORSE_ANIM_IDLE;
    horse->curFrame = horse->skin.skelAnime.curFrame;
    EponaAge_StopActor(actor);
    actor->gravity = -3.5f;
    horse->playerControlled = mounted;
    if (mounted) {
        horse->stateFlags &= ~ENHORSE_INACTIVE;
        horse->action = ENHORSE_ACTION_MOUNTED_IDLE;
        player->rideActor = actor;
    } else if (sEponaReload.stateFlags & ENHORSE_INACTIVE) {
        horse->stateFlags |= ENHORSE_INACTIVE;
        horse->action = ENHORSE_ACTION_INACTIVE;
    } else {
        horse->stateFlags &= ~ENHORSE_INACTIVE;
        horse->action = ENHORSE_ACTION_IDLE;
    }

    actor->update = sEponaReload.update;
    actor->draw = sEponaReload.draw;
}

static void EponaAge_Reinit(Actor* thisx, PlayState* play)
{
    EnHorse* horse = (EnHorse*)thisx;
    EnHorseWaitForObjectFunc waitForObject;
    if (!sEponaReload.active || sEponaReload.horse != horse) {
        EponaAge_ClearReload();
        return;
    }
    waitForObject = (EnHorseWaitForObjectFunc)EponaAge_HorseOverlayAddr(0x8087D540);
    if (waitForObject != NULL) {
        waitForObject(thisx, play);
        EponaAge_Restore(horse, play);
    }
    EponaAge_ClearReload();
}

void EponaAge_Request(PlayState* play, s32 targetAdult)
{
    EnHorse* horse;
    Actor* actor;
    s32 objectSlot;
    s16 objectId;
    s32 adult = targetAdult != 0;
    if (sEponaReload.active || sHorseOverlayRam == NULL)
        return;
    horse = EponaAge_FindHorse(play);
    if (horse == NULL || (horse->unk_1EC & 1))
        return;
    actor = &horse->actor;
    objectSlot = actor->objectSlot;
    if (objectSlot <= OBJECT_SLOT_NONE)
        objectSlot = horse->objectSlot;
    if (objectSlot <= OBJECT_SLOT_NONE ||
        !Object_IsLoaded(&play->objectCtx, objectSlot)) {
        return;
        }
    objectId = adult
        ? CUSTOM_OBJECT_ID_MM_ADULT_EPONA
        : OBJECT_HORSE_LINK_CHILD;
    sEponaReload.active = true;
    sEponaReload.horse = horse;
    sEponaReload.update = actor->update;
    sEponaReload.draw = actor->draw;
    sEponaReload.stateFlags = horse->stateFlags;
    EponaAge_StopActor(actor);
    actor->draw = NULL;
    actor->init = EponaAge_Reinit;
    horse->postDrawFunc = NULL;
    horse->stateFlags &= ~ENHORSE_DRAW;
    Skin_Free(&play->state, &horse->skin);
    Skin_Setup(&horse->skin);
    EponaAge_ApplyOverlayVariant(adult);
    func_8012F73C(&play->objectCtx, objectSlot, objectId);
    actor->objectSlot = objectSlot;
    horse->objectSlot = objectSlot;
    horse->animIndex = ENHORSE_ANIM_IDLE;
    horse->curFrame = 0.0f;
}

s32 EponaAge_IsBusy(void)
{
    return sEponaReload.active;
}

static s16 EponaAge_GetObjectId(s16 childObjectId)
{
    return comboIsLinkAdult()
        ? CUSTOM_OBJECT_ID_MM_ADULT_EPONA
        : childObjectId;
}

s32 EnHorse_GetLinkHorseSlot(ObjectContext* objectCtx, s16 id)
{
    return Object_GetSlot(objectCtx, EponaAge_GetObjectId(id));
}
PATCH_CALL(0x8087cb6c, EnHorse_GetLinkHorseSlot);

s32 EnHorse_SpawnPersistentLinkHorse(ObjectContext* objectCtx, s16 id)
{
    return Object_SpawnPersistent(objectCtx, EponaAge_GetObjectId(id));
}
PATCH_CALL(0x8087cb88, EnHorse_SpawnPersistentLinkHorse);
