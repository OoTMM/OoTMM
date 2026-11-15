#include <combo.h>
#include <combo/enemy.h>
#include <combo/player.h>

_Static_assert(sizeof(DynaPolyActor) == 0x154, "OoT retail dynamic actor ABI");
_Static_assert(__builtin_offsetof(DynaPolyActor, bgId) == 0x13c, "OoT retail dynamic actor tail");
void comboEnemyDynaLoadMesh(PlayState* play, DynaPolyActor* actor, CollisionHeader* source)
{
    CollisionHeader* header = NULL;
    if (!play || !actor || !source)
        return;
    CollisionHeader_GetVirtual(source, &header);
    if (header)
        actor->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &actor->actor, header);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DYNA_LOAD_MESH, comboEnemyDynaLoadMesh);

u8 comboEnemyPlayerInCs(PlayState* play)
{
    return play && Player_InCsMode(play);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_PLAYER_IN_CS, comboEnemyPlayerInCs);

static int enemyCutsceneNone(s16 id)
{
    if (id < 0)
        return 1;
    g.roomEnemyLackSoul = 1;
    return 0;
}

s16 comboEnemyCutsceneIsNext(s16 id)
{
    return enemyCutsceneNone(id) ? -1 : 0;
}

void comboEnemyCutsceneQueue(s16 id)
{
    enemyCutsceneNone(id);
}

s16 comboEnemyCutsceneStartPlayer(s16 id, Actor* actor)
{
    return enemyCutsceneNone(id) ? id : -1;
}

s16 comboEnemyCutsceneStop(s16 id)
{
    return enemyCutsceneNone(id) ? id : -1;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_CUTSCENE_IS_NEXT_NONE, comboEnemyCutsceneIsNext);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_CUTSCENE_QUEUE_NONE, comboEnemyCutsceneQueue);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_CUTSCENE_START_PLAYER_NONE, comboEnemyCutsceneStartPlayer);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_CUTSCENE_STOP_NONE, comboEnemyCutsceneStop);

static s32 enemyUnownedMmSwitch(PlayState* play, s32 flag)
{
    (void)play;
    (void)flag;
    g.roomEnemyLackSoul = 1;
    return 1;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_UNOWNED_MM_SWITCH, enemyUnownedMmSwitch);
