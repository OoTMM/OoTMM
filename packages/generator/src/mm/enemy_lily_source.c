#include <combo.h>
#include <combo/dma.h>
#include <combo/enemy_gold.h>
#include <combo/enemy_native.h>
#include <combo/global.h>

typedef struct {
    u16 scene, params, id, targetParams, air, operation;
} LilyChoice;
_Static_assert(sizeof(LilyChoice) == 12, "Lily source metadata ABI");
typedef struct LilySource {
    struct LilySource* next;
    Actor* platform;
    Actor* target;
    ActorFunc init, update, fini;
    ActorFunc targetInit, targetUpdate, targetFini;
    LilyChoice choice;
    u8 defeated, retry;
} LilySource;
_Static_assert(sizeof(LilySource) + 16 <= 128, "Planner lily monitor allowance");
static LilySource* sSources;
static LilySource* sPrepared;
static int sSpawning;
static u32 sChoicesVrom __attribute__((section(".data"))) = 0;
EXPORT_SYMBOL(ENEMY_LILY_CHOICES_MM_V1, sChoicesVrom);
int comboEnemyLilySpawning(void)
{
    return sSpawning;
}

static int lilyChoice(PlayState* play, u16 params, LilyChoice* choice)
{
    DmaEntry file;
    u32 header[2];
    if ((params >> 12) != 0 || !sChoicesVrom || !comboDmaLookup(&file, sChoicesVrom) ||
        file.vstart != sChoicesVrom || file.pend || file.vend - file.vstart < 8 ||
        comboDmaLoadFilePartial(header, sChoicesVrom, 0, 8) != 8 || header[0] != 0x454c5331 ||
        header[1] > (file.vend - file.vstart - 8) / sizeof(*choice))
        return 0;
    u32 low = 0, high = header[1];
    while (low < high)
    {
        u32 mid = low + (high - low) / 2;
        if (comboDmaLoadFilePartial(choice, sChoicesVrom, 8 + mid * sizeof(*choice), sizeof(*choice)) != sizeof(*choice))
            return 0;
        if (choice->scene == play->sceneId && choice->params == params)
            return 1;
        if (choice->scene < play->sceneId || (choice->scene == play->sceneId && choice->params < params))
            low = mid + 1;
        else
            high = mid;
    }
    return 0;
}

u16 comboEnemyLilyPrepare(PlayState* play, u16 params)
{
    LilyChoice choice;
    if (!lilyChoice(play, params, &choice))
        return params;
    LilySource* source = malloc(sizeof(*source));
    if (!source)
        return params;
    memset(source, 0, sizeof(*source));
    source->choice = choice;
    sPrepared = source;
    return params | 0x1000;
}

static LilySource* lilyOwner(Actor* actor, int target)
{
    for (LilySource* s = sSources; s; s = s->next)
        if ((target ? s->target : s->platform) == actor)
            return s;
    return NULL;
}

static void lilyTargetInit(Actor*, PlayState*);
static void lilyTargetUpdate(Actor*, PlayState*);
static void lilyTargetFini(Actor*, PlayState*);
static void lilyBind(LilySource* s)
{
    Actor* a = s->target;
    if (a->init != lilyTargetInit)
        s->targetInit = a->init;
    if (a->update != lilyTargetUpdate)
        s->targetUpdate = a->update;
    if (a->fini != lilyTargetFini)
        s->targetFini = a->fini;
    a->init = s->targetInit ? lilyTargetInit : NULL;
    a->update = s->targetUpdate ? lilyTargetUpdate : NULL;
    a->fini = lilyTargetFini;
}

static void lilyTargetInit(Actor* actor, PlayState* play)
{
    LilySource* s = lilyOwner(actor, 1);
    if (!s)
    {
        Actor_Kill(actor);
        return;
    }
    actor->init = NULL;
    if (s->targetInit)
        s->targetInit(actor, play);
    s->targetInit = NULL;
    lilyBind(s);
}

static void lilyTargetUpdate(Actor* actor, PlayState* play)
{
    LilySource* s = lilyOwner(actor, 1);
    if (!s)
    {
        Actor_Kill(actor);
        return;
    }
    actor->update = s->targetUpdate;
    if (s->targetUpdate)
    {
        s->targetUpdate(actor, play);
        if (!actor->update)
        {
            s->defeated = 1;
            if ((s->choice.params & 0x7f) != 0x7f)
                Flags_SetSwitch(play, s->choice.params & 0x7f);
        }
    }
    if (actor->update && (actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS))
        Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_NPC);
    lilyBind(s);
}

static void lilyTargetFini(Actor* actor, PlayState* play)
{
    LilySource* s = lilyOwner(actor, 1);
    if (!s)
        return;
    ActorFunc fini = s->targetFini;
    s->target = NULL;
    if (fini)
        fini(actor, play);
}

static void lilySourceFini(Actor* actor, PlayState* play)
{
    LilySource* s = lilyOwner(actor, 0);
    if (!s)
        return;
    if (s->target)
    {
        s->target->init = s->targetInit;
        s->target->update = s->targetUpdate;
        s->target->fini = s->targetFini;
        Actor_Kill(s->target);
    }
    if (!s->init && s->fini)
        s->fini(actor, play);
    LilySource** link = &sSources;
    while (*link && *link != s)
        link = &(*link)->next;
    if (*link)
        *link = s->next;
    free(s);
}

static void lilySourceUpdate(Actor* actor, PlayState* play)
{
    LilySource* s = lilyOwner(actor, 0);
    if (!s)
    {
        Actor_Kill(actor);
        return;
    }
    if (s->update)
        s->update(actor, play);
    if (s->defeated || s->target || actor->room != play->roomCtx.curRoom.num)
        return;

    if ((gMmSave.info.weekEventReg[12] & 1) || ((s->choice.params & 0x7f) != 0x7f && Flags_GetSwitch(play, s->choice.params & 0x7f)))
        return;
    if (s->retry)
    {
        --s->retry;
        return;
    }
    Vec3f pos = actor->world.pos;
    CollisionPoly* floor = NULL;
    s32 bgId;
    pos.y += 80.f;
    f32 y = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, NULL, &pos);
    if (!floor || y < actor->world.pos.y - 20.f)
    {
        s->retry = 15;
        return;
    }
    pos.y = y + (s->choice.air ? 40.f : 1.f);
    ActorEntry entry = { .id = s->choice.id, .pos = { pos.x, pos.y, pos.z }, .rot = { 7, 0x7f, 0x7f }, .params = s->choice.targetParams };
    int previous = sSpawning;
    sSpawning = 1;
    Actor* target = comboEnemyNativeStandaloneSpawn(&play->actorCtx, &entry, play, s->choice.operation);
    sSpawning = previous;
    if (!target || (!target->init && !target->update))
    {
        s->retry = 30;
        return;
    }
    target->room = actor->room;
    Actor_ChangeCategory(play, &play->actorCtx, target, ACTORCAT_NPC);
    s->target = target;
    lilyBind(s);
}

static void lilySourceInit(Actor* actor, PlayState* play)
{
    LilySource* s = lilyOwner(actor, 0);
    if (!s)
    {
        Actor_Kill(actor);
        return;
    }
    actor->init = NULL;
    actor->update = s->update;
    actor->fini = s->fini;
    if (s->init)
        s->init(actor, play);
    s->init = NULL;
    s->update = actor->update;
    s->fini = actor->fini;
    if (actor->update)
        actor->update = lilySourceUpdate;
    actor->fini = lilySourceFini;
}

void comboEnemyLilyWrap(Actor* actor, PlayState* play, u16 params)
{
    LilySource* s = sPrepared;
    sPrepared = NULL;
    if (!s)
        return;
    if (!actor || (!actor->init && !actor->update))
    {
        free(s);
        return;
    }
    s->platform = actor;
    s->init = actor->init;
    s->update = actor->update;
    s->fini = actor->fini;
    s->next = sSources;
    sSources = s;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    if (actor->init)
        actor->init = lilySourceInit;
    if (actor->update)
        actor->update = lilySourceUpdate;
    actor->fini = lilySourceFini;
}
