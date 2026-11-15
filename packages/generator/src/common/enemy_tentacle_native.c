#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/global.h>

int comboEnemyTentacleCeiling(PlayState* play, Vec3f* home)
{
    Vec3f start = *home;
    start.y += 90.f;
    Vec3f end = start;
    end.y += 20.f;
    Vec3f ceiling;
    CollisionPoly* poly = NULL;
    s32 bgId;
    f32 floor;
    if (!BgCheck_EntityLineTest1(&play->colCtx, &start, &end, &ceiling, &poly, 0, 0, 1, 1, &bgId) ||
        !poly || bgId != BGCHECK_SCENE || poly->normal.y > -30000 || ceiling.y < home->y + 96.f || ceiling.y > home->y + 104.f)
        goto rejected;
    poly = NULL;
#if defined(GAME_MM)
    floor = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &poly, &bgId, NULL, &start);
#else
    floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, NULL, &start);
#endif
    if (poly && bgId == BGCHECK_SCENE && floor > -30000.f && ceiling.y - floor >= 448.f)
        return 1;
rejected:
    g.roomEnemyLackSoul = 1;
    return 0;
}

#if defined(GAME_OOT)
static int sTentacleSelected;
s32 comboEnemyNativeTentacleGet(PlayState* play, s32 flag)
{
    return sTentacleSelected && flag == 0xff ? 0 : Flags_GetSwitch(play, flag);
}

void comboEnemyNativeTentacleSet(PlayState* play, s32 flag)
{
    if (!sTentacleSelected || flag != 0xff)
        Flags_SetSwitch(play, flag);
}

static void tentacleUpdate(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, 0xde, -1);
    int previous = sTentacleSelected;
    sTentacleSelected = 1;
    if (profile && profile->update)
        profile->update(actor, play);
    sTentacleSelected = previous;
}

static void tentacleInit(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, 0xde, -1);
    ActorFunc init = profile ? profile->init : NULL;
    int previous = sTentacleSelected;
    actor->init = NULL;
    sTentacleSelected = 1;
    if (init && init != tentacleInit)
        init(actor, play);
    sTentacleSelected = previous;
    if (actor->update)
        actor->update = tentacleUpdate;
}

Actor* comboEnemyNativeTentacleSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    Vec3f home;
    int previous;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != 0xde ||
        (u16)entry->params < 0xff00 || (u16)entry->params > 0xff02)
        return NULL;
    home.x = entry->pos.x;
    home.y = entry->pos.y;
    home.z = entry->pos.z;
    if (!comboEnemyTentacleCeiling(play, &home))
        return NULL;
    previous = sTentacleSelected;
    sTentacleSelected = 1;
    actor = ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)(ctx, entry, play);
    sTentacleSelected = previous;
    if (actor && actor->init)
        actor->init = tentacleInit;
    else if (actor && actor->update)
        actor->update = tentacleUpdate;
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_TENTACLE_V1, comboEnemyNativeTentacleSpawn);
EXPORT_SYMBOL(ENEMY_NATIVE_TENTACLE_GET_V1, comboEnemyNativeTentacleGet);
EXPORT_SYMBOL(ENEMY_NATIVE_TENTACLE_SET_V1, comboEnemyNativeTentacleSet);
#endif
