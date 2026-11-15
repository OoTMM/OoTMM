#include <combo/enemy_soul_preview.h>
#include <combo.h>
#include <combo/dma.h>
#include <combo/actor_ovl.h>
#include <combo/enemy_gold.h>
#include <combo/enemy_native.h>
#include <combo/play.h>
#include <combo/time.h>

typedef struct {
    u16 scene, reward, id, params, air, operation;
} GoldChoice;
_Static_assert(sizeof(GoldChoice) == 12, "Gold choice ABI");
typedef struct GoldMonitor {
    Actor actor;
    struct GoldMonitor* next;
    Actor* target;
    ActorFunc targetInit, targetUpdate, targetFini;
    GoldChoice choice;
    Vec3f drop;
    u8 defeated, retry, armed, preview;
} GoldMonitor;
#if defined(GAME_OOT)
_Static_assert(sizeof(GoldMonitor) <= 0x4d0, "Fits retail OoT En_Sw allocation");
#else
_Static_assert(sizeof(GoldMonitor) <= 0x4e0, "Fits retail MM En_Sw allocation");
#endif
static GoldMonitor* sMonitors;
static u32 sChoicesVrom __attribute__((section(".data"))) = 0;
static int sSpawning;
#if defined(GAME_OOT)
EXPORT_SYMBOL(ENEMY_GOLD_CHOICES_OOT_V1, sChoicesVrom);
#else
EXPORT_SYMBOL(ENEMY_GOLD_CHOICES_MM_V1, sChoicesVrom);
#endif
int comboEnemyGoldSpawning(void)
{
    return sSpawning;
}

static int goldLookup(PlayState* play, u16 params, GoldChoice* result)
{
    u16 reward;
#if defined(GAME_OOT)
    if (!(params >> 13))
        return 0;
    u16 bits = params & 255, group = (params >> 8) & 31, bit = 0;
    if (!group || !bits || (bits & (bits - 1)))
        return 0;
    while (!(bits & (1 << bit)))
        ++bit;
    reward = (group - 1) * 8 + bit;
    if (Play_ExpandMQ(play, play->sceneId) != play->sceneId)
        reward += 0xb0;
#else
    if (!(params & 3))
        return 0;
    reward = (params & 0xfc) >> 2;
#endif
    DmaEntry file;
    u32 header[2];
    if (!sChoicesVrom || !comboDmaLookup(&file, sChoicesVrom) || file.vstart != sChoicesVrom || file.pend ||
        file.vend - file.vstart < 8 || comboDmaLoadFilePartial(header, sChoicesVrom, 0, 8) != 8 ||
        header[0] != 0x45475331 || header[1] > (file.vend - file.vstart - 8) / sizeof(*result))
        return 0;

    u32 low = 0, high = header[1];
    while (low < high)
    {
        u32 i = low + (high - low) / 2;
        if (comboDmaLoadFilePartial(result, sChoicesVrom, 8 + i * sizeof(*result), sizeof(*result)) != sizeof(*result))
            return 0;
        if (result->scene == play->sceneId && result->reward == reward)
            return 1;
        if (result->scene < play->sceneId || (result->scene == play->sceneId && result->reward < reward))
            low = i + 1;
        else
            high = i;
    }
    return 0;
}

static GoldMonitor* goldOwner(Actor* actor)
{
    for (GoldMonitor* m = sMonitors; m; m = m->next)
        if (m->target == actor)
            return m;
    return NULL;
}

static void goldTargetInit(Actor*, PlayState*);
static void goldTargetUpdate(Actor*, PlayState*);
static void goldTargetFini(Actor*, PlayState*);
static void goldTargetBind(GoldMonitor* m)
{
    Actor* a = m->target;
    if (!a)
        return;
    if (a->init != goldTargetInit)
        m->targetInit = a->init;
    if (a->update != goldTargetUpdate)
        m->targetUpdate = a->update;
    if (a->fini != goldTargetFini)
        m->targetFini = a->fini;
    a->init = m->targetInit ? goldTargetInit : NULL;
    a->update = m->targetUpdate ? goldTargetUpdate : NULL;
    a->fini = goldTargetFini;
}

static void goldTargetInit(Actor* a, PlayState* play)
{
    GoldMonitor* m = goldOwner(a);
    if (!m)
    {
        Actor_Kill(a);
        return;
    }
    ActorFunc init = m->targetInit;
    a->init = NULL;
    if (init)
        init(a, play);
    m->targetInit = NULL;
    goldTargetBind(m);
}

static void goldTargetUpdate(Actor* a, PlayState* play)
{
    GoldMonitor* m = goldOwner(a);
    if (!m)
    {
        Actor_Kill(a);
        return;
    }
    ActorFunc update = m->targetUpdate;
    a->update = update;
    if (update)
    {
        int hadHealth = a->colChkInfo.health != 0;
        update(a, play);
        if (m->armed != 2)
        {
            m->drop = a->world.pos;

            if ((m->armed || hadHealth) && !a->colChkInfo.health)
                m->armed = 2;
            else if (a->colChkInfo.health)
                m->armed = 1;
        }
        if (!a->update)
            m->defeated = 1;
    }

    if (a->update && (a->category == ACTORCAT_ENEMY || a->category == ACTORCAT_BOSS))
        Actor_ChangeCategory(play, &play->actorCtx, a, ACTORCAT_NPC);
    goldTargetBind(m);
}

static void goldTargetFini(Actor* a, PlayState* play)
{
    GoldMonitor* m = goldOwner(a);
    if (!m)
        return;
    ActorFunc fini = m->targetFini;
    m->target = NULL;
    if (fini)
        fini(a, play);
}

static void goldFini(Actor* actor, PlayState* play)
{
    GoldMonitor* m = (GoldMonitor*)actor;
    if (m->target)
    {
        Actor* a = m->target;
        a->init = m->targetInit;
        a->update = m->targetUpdate;
        a->fini = m->targetFini;
        Actor_Kill(a);
        m->target = NULL;
    }
    GoldMonitor** at = &sMonitors;
    while (*at && *at != m)
        at = &(*at)->next;
    if (*at)
        *at = m->next;
}

static void goldUpdate(Actor* actor, PlayState* play)
{
    GoldMonitor* m = (GoldMonitor*)actor;
    if (actor->room != play->roomCtx.curRoom.num)
        return;
    if (m->retry)
    {
        --m->retry;
        return;
    }
    if (m->defeated)
    {
        Vec3f pos = m->drop;
        Actor* token = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_SI, pos.x, pos.y, pos.z, 0, 0, 0, actor->params);
        if (token)
        {
            token->room = actor->room;
            PlaySound(NA_SE_SY_KINSTA_MARK_APPEAR);
            Actor_Kill(actor);
        }
        else
            m->retry = 15;
        return;
    }
    if (m->target || m->preview)
        return;
#if defined(GAME_OOT)

    if (((u16)actor->params >> 13) == 2 && IS_DAY)
        return;
#endif
    Vec3f pos = actor->world.pos;
    if (!comboEnemyFindFloor(play, actor, &pos, m->choice.air ? 40.f : 0.f, 50, 0) &&
        !comboEnemyFindFloor(play, actor, &pos, m->choice.air ? 40.f : 0.f, 150, 0))
        pos = actor->world.pos;
    int previous = sSpawning;
    sSpawning = 1;
    ActorEntry entry = { .id = m->choice.id, .pos = { pos.x, pos.y, pos.z }, .params = m->choice.params };
#if defined(GAME_MM)
    entry.rot = (Vec3s){ 7, 0x7f, 0x7f };
#else
    entry.rot.y = actor->shape.rot.y;
#endif
    int previousLackSoul = g.roomEnemyLackSoul;
    u32 previewSerial = gEnemySoulPreviewSerial;
    Actor* target = comboEnemyNativeStandaloneSpawn(&play->actorCtx, &entry, play, m->choice.operation);

    g.roomEnemyLackSoul = previousLackSoul;
    sSpawning = previous;
    if (!target && gEnemySoulPreviewSerial != previewSerial)
    {

        m->preview = 1;
        return;
    }
    if (!target || (!target->init && !target->update))
    {
        m->retry = 30;
        return;
    }
    target->room = actor->room;
    Actor_ChangeCategory(play, &play->actorCtx, target, ACTORCAT_NPC);
    m->target = target;
    m->drop = pos;
    goldTargetBind(m);
}

static void goldConvert(Actor* actor, PlayState* play, const GoldChoice* choice)
{
    if (!actor->update)
        return;
    if (actor->fini)
        ((ActorFunc)actor->fini)(actor, play);
    GoldMonitor* m = (GoldMonitor*)actor;
    memset((u8*)m + sizeof(Actor), 0, sizeof(*m) - sizeof(Actor));
    m->choice = *choice;
    m->drop = actor->world.pos;
    m->next = sMonitors;
    sMonitors = m;
    actor->init = NULL;
    actor->update = goldUpdate;
    actor->fini = goldFini;
    actor->draw = NULL;
    actor->flags = ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    actor->shape.shadowDraw = NULL;
    Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_NPC);
}

static void goldInit(Actor* actor, PlayState* play)
{
    GoldChoice choice;
    int selected = goldLookup(play, actor->params, &choice);
    ActorOvl* ovl = actor->ovl;
    ActorProfile* profile = (ActorProfile*)((u8*)ovl->data + (u32)ovl->dataInit - ovl->vramStart);
    actor->init = NULL;
    profile->init(actor, play);
    if (selected)
        goldConvert(actor, play, &choice);
}

void comboEnemyGoldWrap(Actor* actor, PlayState* play, u16 originalParams)
{
    GoldChoice choice;
    if (!actor || !goldLookup(play, originalParams, &choice))
        return;
    if (actor->init)
        actor->init = goldInit;
    else
        goldConvert(actor, play, &choice);
}
