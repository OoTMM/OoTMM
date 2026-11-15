#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/enemy_placement.h>
#define NativeBubbleSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)

typedef struct {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[16], morphTable[16];
    s32 action, unused254;
    ActorFunc actionFunc;
    s32 moveMode, timer, unused264;
    s16 actionState, charge, actionVar1, actionVar2, flameScrollMod;
    f32 bobPhase, bobSize, maxSpeedXZ, flyHeightMod, bobSpeedMod, flameScaleY, flameScaleX;
    Vec3f waypointPos;
    u8 path, waypoint, flamePrimBlue, flamePrimAlpha;
    Color_RGB8 flameEnvColor;
    s32 blureIdx;
    s16 fireIceTimer;
    u8 dmgEffect;
    ColliderJntSph collider;
    ColliderJntSphElement elements[1];
} NativeBubblePrefix;
ASSERT_OFFSET(NativeBubblePrefix, action, 0x240);
ASSERT_OFFSET(NativeBubblePrefix, collider, 0x29c);
typedef struct {
    Path* paths;
    u32 segment;
    Path local;
    Vec3s points[4];
} BubblePathScope;
static s16 bubbleCoordinate(f32 coordinate)
{
    return coordinate < -32768.f ? -32768 : coordinate > 32767.f ? 32767
                                                                 : coordinate;
}

static void bubblePathBegin(PlayState* play, const Vec3f* home, BubblePathScope* scope)
{
    static const s16 offsets[4][2] = { { -40, 0 }, { 0, 40 }, { 40, 0 }, { 0, -40 } };
    scope->paths = play->pathList;
    scope->segment = gSegments[15];
    for (u32 i = 0; i < 4; ++i)
    {
        scope->points[i].x = bubbleCoordinate(home->x + offsets[i][0]);
        scope->points[i].y = bubbleCoordinate(home->y + 60.f);
        scope->points[i].z = bubbleCoordinate(home->z + offsets[i][1]);
    }
    scope->local.count = 4;
    scope->local.points = (Vec3s*)0x0f000000;
    gSegments[15] = (u32)scope->points & 0x1fffffff;
    play->pathList = &scope->local;
}

static void bubblePathEnd(PlayState* play, BubblePathScope* scope)
{
    play->pathList = scope->paths;
    gSegments[15] = scope->segment;
}

static void bubbleDraw(Actor* actor, PlayState* play)
{
    ActorFunc draw = actorAddr(0x69, 0x80917128);
    if (draw)
        draw(actor, play);
    if (!(actor->colorFilterParams & 0xff))
        actor->colorFilterTimer = 0;
}

static void bubbleUpdate(Actor* actor, PlayState* play)
{
    BubblePathScope scope;
    ActorFunc update = actorAddr(0x69, 0x80916e58);
    Vec3f origin = actor->home.pos;
    comboEnemyNativePlacementOrigin(actor, &origin);
    bubblePathBegin(play, &origin, &scope);
    if (update)
        update(actor, play);
    NativeBubblePrefix* bubble = (NativeBubblePrefix*)actor;
    if (actor->update && actor->params == -3 && bubble->action == 8 &&
        actor->colChkInfo.health && !(actor->flags & ACTOR_FLAG_ATTACHED_TO_ARROW))
    {

        Actor_UpdateBgCheckInfo(play, actor, 0.f, 25.f, 0.f, UPDBGCHECKINFO_FLAG_WALL);
        if (actor->bgCheckFlags & BGCHECKFLAG_WALL)
        {
            ((ActorFunc)actorAddr(0x69, 0x80915b2c))(actor, play);
            ((void (*)(Actor*))actorAddr(0x69, 0x80915af8))(actor);
        }
        actor->focus.pos = actor->world.pos;
        bubble->elements[0].dim.worldSphere.center.x = actor->world.pos.x;
        bubble->elements[0].dim.worldSphere.center.y = actor->world.pos.y + actor->shape.yOffset * actor->scale.y;
        bubble->elements[0].dim.worldSphere.center.z = actor->world.pos.z;
    }
    bubblePathEnd(play, &scope);
    if (actor->update)
        actor->update = bubbleUpdate;
}

static void bubbleInit(Actor* actor, PlayState* play)
{
    BubblePathScope scope;
    ActorFunc init = actorAddr(0x69, 0x80914128);
    actor->init = NULL;
    bubblePathBegin(play, &actor->home.pos, &scope);
    if (init)
        init(actor, play);
    bubblePathEnd(play, &scope);
    if (actor->update)
        actor->update = bubbleUpdate;
    if (actor->draw)
        actor->draw = bubbleDraw;
}

Actor* comboEnemyNativeBubbleSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    BubblePathScope scope;
    Vec3f home;
    Actor* actor;
    if (!ctx || !entry || !play || entry->id != 0x69 ||
        ((u16)entry->params != 0x00fd && (u16)entry->params != 0x02fc && (u16)entry->params != 0x02fb))
        return NULL;
    home.x = entry->pos.x;
    home.y = entry->pos.y;
    home.z = entry->pos.z;
    bubblePathBegin(play, &home, &scope);
    actor = NativeBubbleSpawnEntry(ctx, entry, play);
    bubblePathEnd(play, &scope);
    if (actor && actor->init)
        actor->init = bubbleInit;
    else if (actor)
    {
        if (actor->update)
            actor->update = bubbleUpdate;
        if (actor->draw)
            actor->draw = bubbleDraw;
    }
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_BUBBLE_PATROL_V1, comboEnemyNativeBubbleSpawn);
