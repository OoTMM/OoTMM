#include <combo/enemy_soul_preview.h>
#include <combo.h>
#include <combo/dma.h>
#include <combo/actor_ovl.h>
#include <combo/enemy_gold.h>
#include <combo/enemy_native.h>
#include <combo/play.h>
#include <combo/time.h>

#define GOLD_CHOICE_AIR    1

typedef struct
{
    u16 scene, reward, id, params, flags, operation;
} GoldChoice;

_Static_assert(sizeof(GoldChoice) == 12, "Gold choice ABI");

typedef struct GoldMonitor
{
    Actor actor;
    struct GoldMonitor* next;
    EnemyFamily family;
    GoldChoice choice;
    u8 defeated, retry, preview;
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

int comboEnemyGoldIsTarget(Actor* actor)
{
    return comboEnemyFamilyIsGold(actor);
}

static void goldFini(Actor* actor, PlayState* play)
{
    GoldMonitor* m = (GoldMonitor*)actor;
    comboEnemyFamilyClear(&m->family);
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
    if (m->defeated)
    {
        if (!m->family.live)
            Actor_Kill(actor);
        return;
    }
    if (m->retry)
    {
        --m->retry;
        return;
    }
    if (comboEnemyFamilyRewardReady(&m->family))
    {
        Vec3f pos = GET_PLAYER(play)->actor.world.pos;
        Actor_EnItem00* item = EnItem00_DropGoldReward(play, &pos, m->choice.scene, m->choice.reward);
        if (item)
        {
            PlaySound(NA_SE_SY_KINSTA_MARK_APPEAR);
            m->defeated = 1;
            if (!m->family.live)
                Actor_Kill(actor);
        }
        else
            m->retry = 15;
        return;
    }
    if (m->family.started || m->preview)
        return;
#if defined(GAME_OOT)

    if (((u16)actor->params >> 13) == 2 && IS_DAY)
        return;
#endif
    Vec3f pos = actor->world.pos;
#if defined(GAME_MM)
    if (m->choice.scene == 0x28)
    {
        switch (m->choice.reward)
        {
        case 14: pos = (Vec3f){ -83.f, -98.f, -1690.f }; break;
        case 2: pos = (Vec3f){ -810.f, 276.f, -720.f }; break;
        case 3: pos = (Vec3f){ -780.f, 210.f, -1310.f }; break;
        }
    }
#endif
    if (!comboEnemyFindFloor(play, actor, &pos, (m->choice.flags & GOLD_CHOICE_AIR) ? 40.f : 0.f, 50, 0) &&
        !comboEnemyFindFloor(play, actor, &pos, (m->choice.flags & GOLD_CHOICE_AIR) ? 40.f : 0.f, 150, 0))
        pos = actor->world.pos;
    if (!(m->choice.flags & GOLD_CHOICE_AIR))
    {
        Actor probe = { 0 };
        probe.world.pos = probe.prevPos = pos;
        probe.scale = (Vec3f){ 1.f, 1.f, 1.f };
        Actor_UpdateBgCheckInfo(play, &probe, 40.f, 32.f, 0.f, UPDBGCHECKINFO_FLAG_WALL);
        pos.x = probe.world.pos.x;
        pos.z = probe.world.pos.z;
    }
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
    m->family.flags = ENEMY_FAMILY_GOLD | ENEMY_FAMILY_NO_CLEAR;
    Actor* target = comboEnemyFamilySpawn(&m->family, &play->actorCtx, &entry, play, m->choice.operation);

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
